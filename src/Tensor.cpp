/*****************************************************
 * 15745 Project
 * Authors: Rajnish Aggarwal <rajnisha>
 *          Peter Oostema <poostema>
 *
 * Description:
 * ***************************************************/

#include "../include/Tensor.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Argument.h"
#include <algorithm>


struct less_than_key
{
    inline bool operator() (const IndexVar *i1, const IndexVar *i2)
    {
        return (i1->getSize() > i2->getSize());
    }
};


/*********************************************
 * Function: FindIndexAndRange
 *
 * Description: This function finds all the variables
 * used to index into the tensor and then populates
 * the indexVariables. Each tensor will have a list of
 * variables used to index into it.
 * ******************************************/
void Tensor::findIndexAndRange()
{
    // From the reference instruction, we get a variable
    // and the next reference instruction for this particular
    // tensor object.
    //
    // For each ref instruction, find the range of it's variable
    // and the stride. Do for all variables.
    //

    Instruction *rInstr = refInstr;

    while (isa<GetElementPtrInst> (rInstr)) {
//        outs() << "===========================================================\n";
//        outs() << "Analyzing " << *rInstr << "\n";

        uint32_t numOperands = rInstr->getNumOperands();
        Value *op1;
        Value *op2;
        if (numOperands == 2) {
            op1 = rInstr->getOperand(0);
            op2 = rInstr->getOperand(1);
        }
        else {
            op1 = rInstr->getOperand(0);
            op2 = rInstr->getOperand(2);
        }

//        outs() << "Operand 1 " << *op1 << "\n";
//        outs() << "Operand 2 " << *op2 << "\n";

        Instruction *I1 = dyn_cast<Instruction> (op1);
        Instruction *I2 = dyn_cast<Instruction> (op2);

        if (!I1 && !I2) {
            outs() << "Can't convert indices to instructions\n";
        }

        IndexVar *idx = NULL;
        if (isa<GetElementPtrInst> (I1) || isa<LoadInst> (I1)||
                isa<AllocaInst> (I1) || isa<Argument> (I1)) {
//            if (isa<Argument> (I1)) {
//                outs() << "Value passed in a function as argument\n";
//            }
            // Implies that I2 is the index variable
            extractRangeAndSize(rInstr, I2, idx);
            indexVariables.push_back(idx);
            if (isa<GetElementPtrInst> (I1)) {
                rInstr = I1;
            }
            else if (isa<AllocaInst> (I1)) {
                setDefInstr(I1);
                break;
            }
            else {
                LoadInst *LI = dyn_cast<LoadInst> (I1);
//                outs() << "Pointer operand " << *(LI->getPointerOperand()) << "\n";
                setDefInstr(dyn_cast<Instruction> (LI->getPointerOperand()));
                break;
            }
        }
        else if (isa<GetElementPtrInst> (I2) || isa<LoadInst> (I2) ||
                isa<AllocaInst> (I2) || isa<Argument> (I2)) {
            extractRangeAndSize(rInstr, I1, idx);
            indexVariables.push_back(idx);
            if (isa<GetElementPtrInst> (I2)) {
                rInstr = I1;
            }
            else if (isa<AllocaInst> (I2)) {
                setDefInstr(I1);
                break;
            }
            else {
                LoadInst *LI = dyn_cast<LoadInst> (I2);
//                outs() << "Pointer operand " << *(LI->getPointerOperand()) << "\n";
                setDefInstr(dyn_cast<Instruction> (LI->getPointerOperand()));
                break;
            }
        }
        else {
            break;
        }

//        outs() << "===========================================================\n";
    }

    std::sort(indexVariables.begin(), indexVariables.end(), less_than_key());
}

/************************************************
 * Function: extractRangeAndSize
 *
 * Description: This function uses the index variable
 * to find a PHI instruction and a compare instruction
 * which in-turn help us find the range and size of a
 * particular variable.
 *
 * For variables whose values are not available statically,
 * or variables passed as function arguments, we need to
 * have a runtime value propagation to the appropriate
 * variables because the size information is needed to
 * sort the indexes (for determining the order in which
 * they address the matrix).
 *
 * Handling these cases is an implementation roadblock
 * and has not been addressed as a part of this project.
 ***********************************************/
void Tensor::extractRangeAndSize(Instruction *rInstr,
        Instruction *I, IndexVar *&var)
{
    // rInstr is a getElementPtr type instruction. Use it to check
    // if inbounds has a dimension
    GetElementPtrInst *GEPI = dyn_cast<GetElementPtrInst> (rInstr);
    Type *T = cast<PointerType>(GEPI->getPointerOperandType())->getElementType();
    Module *m = rInstr->getParent()->getModule();
    DataLayout DL = m->getDataLayout();
    uint64_t size = DL.getTypeAllocSize(T);

//    outs() << "Size extracted from DataLayout " << size << "\n";
//    outs() << "For instruction " << *I << "\n";
    Instruction *rootVar = I; // This is the actual variable which
                              // indexes into the array. We may have
                              // complex pointer arithmetic hence cannot
                              // surely say that it will correspond to the
                              // tensor variable.
    if (size == 4) {
//        outs() << "We have used pointer arithematic in this test case\n";
        // For tc's using pointer arithematic, we need to find the order of
        // index variable using the constants multiplied with the instruction
        if (isa<BinaryOperator> (I)) {
            size = extractConstants(I, rootVar) * 4; // we're handling floats here
            var = new IndexVar(rootVar);
//            outs() << "Final size " << size << "\n";
            var->setSize(size);
        }
        else {
            //
            var = new IndexVar(I);
            var->setSize(size);
        }
    }
    else {
        // This is a simple implementation for proof of concept. The actual
        // implementation will need to handle various other cases.
        var = new IndexVar(I);
        var->setSize(size);
    }

    // Now, this root variable, should correspond to a PHI instruction, in
    // general cases.
    if (!isa<PHINode> (rootVar)) {
        outs() << "Sorry, could not find a PHI node, can't proceed with analysis\n";
    }

    // Using the PHI node, we want to find the range of this Tensor variable
    // and it's stride.
    PHINode *phiInst = dyn_cast<PHINode> (rootVar);
    Value *start = NULL;
    Value *end = NULL;
    Value *stride = NULL;

    for (uint32_t i = 0; i < phiInst->getNumOperands(); ++i) {
        Value *v = phiInst->getOperand(i);
        if (isa<ConstantInt> (v)) {
             // The value coming from the loop-preheader should be a
             // constant
             start = v;
             break;
        }
    }

    Value *v = dyn_cast<Value> (rootVar);
    for (User *u : v->users()) {
        if (!end && isa<CmpInst> (u)) {
            for (uint32_t i = 0; i < u->getNumOperands(); ++i) {
                Value *operand = u->getOperand(i);
                // This is the end value of this variable
                end = operand;
                break;
            }
        }

        else if (!stride && isa<BinaryOperator> (u)) {
            for (uint32_t i = 0; i < u->getNumOperands(); ++i) {
                Value *operand = u->getOperand(i);
                if (isa<ConstantInt> (operand)) {
                    // This value represents the stride.
                    stride = operand;
                    break;
                }
            }
        }
    }

    var->setStartAndEnd(start, end);
    var->setStride(stride);
}


/*****************************************************
 * Function: ExtractConstants
 *
 * Description: This function takes in an instruction
 * and recursively extract constant values it is multiplied
 * with till it reaches an instruction which is not a Binary
 * Operator.
 *
 * This function should only be called when the values are
 * contants available at compile time!
 * ***************************************************/
uint64_t Tensor::extractConstants(Instruction *I, Instruction *&rootVar)
{
    // Now I should be a binary operator, which has multiplication
    if (!isa<BinaryOperator> (I)) {
        rootVar = I;
        return (1);
    }

    BinaryOperator *BI = dyn_cast<BinaryOperator> (I);
    if (BI->getOpcode() != Instruction::Mul) {
        return (1);
    }

    Value *nextInstr = BI->getOperand(0);
    Value *constant = BI->getOperand(1);
    uint64_t val = dyn_cast<ConstantInt> (constant)->getZExtValue();

    I = dyn_cast<Instruction> (nextInstr);
    return (val * extractConstants(I, rootVar));
}



void Tensor::print()
{
    outs() << "=================================================\n";
    for (uint32_t i = 0; i < indexVariables.size(); ++i) {
        indexVariables[i]->print();
    }
    outs() << "=================================================\n";
}

void IndexVar::print()
{
    outs() << "Name: " << *name << "\n";
    outs() << "Start: " << *start << "\n";
    outs() << "End: " << *end << "\n";
    outs() << "Stride: " << *stride << "\n";
    outs() << "Size: " << size << "\n";
}
