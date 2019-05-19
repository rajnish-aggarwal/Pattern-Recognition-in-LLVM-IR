/*****************************************
 * 15745 Project
 * Authors: Rajnish Aggarwal <rajnisha>
 *          Peter Oostema <poostema>
 *
 * Description: This file consists of basic
 * utilities that are used to extract information from
 * a given instruction.
 * ***************************************/

#include "../include/InstFinder.h"
#include "../include/Tensor.h"
#include "../include/Patterns.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/DataLayout.h"
#include "utility"

/***************************************
 * Function: isReductionOp
 *
 * Description: This function analyses a store
 * instruction and checks if it is of the form
 *
 *              C += A * B
 *
 * For this, the store instruction must have the
 * operand as an add instruction which in-turn
 * should have one of its operands as a multiplication
 * instruction. This function also returns the found
 * multiplication instruction
 * **************************************/
bool isReductionOp(StoreInst *&SI, Instruction *&MI)
{
    Value *v = SI->getValueOperand();
    // This value should be an addition
    if (!isa<BinaryOperator> (v)) {
        outs() << "Instruction :" << *v << " not a Binary Operator\n";
        return (false);
    }

    Instruction *I = dyn_cast<Instruction> (v);
    unsigned opcode = I->getOpcode();
    if (opcode != Instruction::Add) {
        outs() << "Instruction :" << *v << " not a an addition\n";
        return (false);
    }

    Value *lhs = I->getOperand(0); // op 0 of addition should be the c matrix
    Value *rhs = I->getOperand(1); // op 1 of addition should be a mul instruction
                                   // or vice-versa

    if (isa<BinaryOperator> (lhs)) {
        MI = dyn_cast<Instruction> (lhs);
        if (MI == NULL) {
            outs() << "Dynamic cast of lhs failed\n";
            return (false);
        }
        opcode = MI->getOpcode();
        // This should be a mul operator
        if (opcode != Instruction::Mul) {
            outs() << "Lhs is a binary operator, but not a mul instruction " <<
                   *lhs << "\n";
            return (false);
        }
    }
    else if (isa<BinaryOperator> (rhs)) {
        MI = dyn_cast<Instruction> (rhs);
        if (MI == NULL) {
            outs() << "Dynamic cast of lhs failed\n";
            return (false);
        }

        opcode = MI->getOpcode();
        if (opcode != Instruction::Mul) {
            outs() << "Rhs is a binary operator, but not a mul instruction " <<
                *rhs << "\n";
            return (false);
        }
    }
    else {
        outs() << "Cannot decompile this instruction\n";
        return (false);
    }

//    outs() << "This instruction represents a reduction operation\n";
    return (true);
}


/**************************************************
 * Function: extractOperands
 *
 * Description: This function takes in a store
 * instruction and extracts the reduction instruction's
 * operands i.e from C += A * B it extracts
 * C, A and B. These are taken in as references from the
 * caller.
 *
 * NOTE:: This function assumes we have already verified
 * that we are analysing a reduction instruction.
 **************************************************/
bool extractOperands(StoreInst     *&SI,
                     Instruction   *&MI,
                     Tensor        *&C,
                     Tensor        *&A,
                     Tensor        *&B)
{
    // For matrix C, get the operand pointer from the store
    // instruction, which should point to matrix C.
    Value *v = SI->getPointerOperand();
    Instruction *I = dyn_cast<Instruction> (v);
    C = new Tensor(I);

    Value *val = SI->getValueOperand();
    Instruction *valInstr = dyn_cast<Instruction> (val);
    if (!val) {
        delete(C);
        return (false);
    }

    Value *lhs = valInstr->getOperand(0);
    Value *rhs = valInstr->getOperand(1);

    // One of these instructions should be a mul instruction and the other
    // a load instruction
    Instruction *i_lhs = dyn_cast<Instruction> (lhs);
    Instruction *i_rhs = dyn_cast<Instruction> (rhs);

    BinaryOperator *BI = NULL;
    if (isa<BinaryOperator> (i_lhs)) {
        BI = dyn_cast<BinaryOperator>(i_lhs);
        if (BI->getOpcode() != Instruction::Mul) {
            delete(C);
            return (false);
        }
        // In this case, the RHS should be a load instruction.
        if (!isa<LoadInst> (i_rhs)) {
            delete(C);
            return (false);
        }
    }
    else if (isa<BinaryOperator> (i_rhs)) {
        BI = dyn_cast<BinaryOperator> (i_rhs);
        if (BI->getOpcode() != Instruction::Mul) {
            delete (C);
            return (false);
        }

        if (!isa<LoadInst> (i_lhs)) {
            delete (C);
            return (false);
        }
    }

    // Both the operands of the MUL binary instruction, should in turn be
    // load instructions. Verify that.
    lhs = BI->getOperand(0);
    rhs = BI->getOperand(1);

    i_lhs = dyn_cast<Instruction> (lhs);
    i_rhs = dyn_cast<Instruction> (rhs);

    if (!isa<LoadInst> (i_lhs) && !isa<LoadInst> (i_rhs)) {
        delete (C);
        return (false);
    }

    // Now we have finally confirmed that we are reading from two memory
    // locations to write to one. Hence, we can go ahead and extract the indexes
    // used to address these tensors.
    LoadInst *loadLhs = dyn_cast<LoadInst> (i_lhs);
    LoadInst *loadRhs = dyn_cast<LoadInst> (i_rhs);

    lhs = loadLhs->getPointerOperand();
    rhs = loadRhs->getPointerOperand();

    i_lhs = dyn_cast<Instruction> (lhs);
    i_rhs = dyn_cast<Instruction> (rhs);

    A = new Tensor(i_lhs);
    B = new Tensor(i_rhs);

    return (true);
}


/***************************************************
 * Function: verifyMatMulOperands
 *
 * Description: This is a part implementation of the
 * algorithm described in the report for verifying
 * that a particular operation represents a matrix
 * multiplication by going over the ranges of the tensors
 * *************************************************/
bool verifyMatMulOperands(Tensor *C, Tensor *A, Tensor *B)
{
    std::vector<IndexVar *> &cIdxVars = C->getIndexVariables();
    std::vector<IndexVar *> &aIdxVars = A->getIndexVariables();
    std::vector<IndexVar *> &bIdxVars = B->getIndexVariables();

    // All C variables must be a part of the inputs A and B
    // Based on the user provided operation [+, *] in our case
    // and this information of what input elements are getting
    // added to the output, we can determine the HPC kernel that we want
    // to use for this implementation
    //
    // For example, for C[i][j] += A[i][k] * B[k][j]
    // We eliminate i and j in the output from A and B in the
    // inputs, now we know that a row of A and a column of B
    // are getting multiplied to be added to the output element
    // C[i][j]. Based on this information, we can tell that this is a
    // mat_mul kernel with a 2D input
    std::vector<IndexVar *> commonToC;
    for (uint32_t i = 0; i < cIdxVars.size(); ++i) {
        IndexVar *v = cIdxVars[i];
        bool found = false;
        for (uint32_t j = 0; j < aIdxVars.size() && !found; ++j) {
            if (aIdxVars[j] == v) {
                commonToC.push_back(v);
                found = true;
            }
        }

        for (uint32_t j = 0; j < bIdxVars.size() && !found; ++j) {
            if (bIdxVars[j] == v) {
                commonToC.push_back(v);
                found = true;
            }
        }

        // If nothing matched, return false
        if (!found)
            return (false);
    }

    // Now the remaining variables of A and B which are not common
    // to C must be common to each other.

    std::vector<IndexVar *> commonToBoth;
    for (uint32_t i = 0; i < aIdxVars.size(); ++i) {
        IndexVar *v = aIdxVars[i];
        bool isCommonToC = false;
        for (uint32_t j = 0; j < commonToC.size(); ++j) {
            if (commonToC[j] == v) {
                isCommonToC = true;
            }
        }

        if (isCommonToC) {
            continue;
        }

        // Otherwise this variable should be comming to B
        bool commonToB = false;
        for (uint32_t j = 0; j < bIdxVars.size(); ++j) {
            if (bIdxVars[j] == v) {
                commonToB = true;
                commonToBoth.push_back(v);
                break;
            }
        }

        // This is a unique index in A, we do not guarentee
        // pattern recognition in this case.
        if (!commonToB) {
            return (false);
        }
    }


    // Now if there is any index left in B which is neither common
    // to C, nor to A, then we cannot match this pattern for now.


    for (uint32_t i = 0; i < bIdxVars.size(); ++i) {
        bool found = false;
        IndexVar *v = bIdxVars[i];
        for (uint32_t j = 0; j < commonToBoth.size(); ++j) {
            if (commonToBoth[j] == v) {
                found = true;
                break;
            }
        }

        for (uint32_t j = 0; j < commonToC.size() && !found; ++j) {
            if (commonToC[j] == v) {
                found = true;
                break;
            }
        }

        if (!found) {
            return (false);
        }
    }


    return (true);
}

/*********************************
 * Function: findBasicBlocks
 *
 * Description: This function is used to find all the
 * basic blocks associated with a particular reduction instruction
 * ***********************************/
void InstFinder::findBasicBlocks(std::vector<BasicBlock*> &loopBB,
                                 Tensor                   *&C,
                                 Loop                     *&outerLoop)
{
    std::vector<IndexVar *> &indexVars = C->getIndexVariables();
    IndexVar* outerIdx = indexVars[0];
    Instruction *outerIdxInst = outerIdx->getName();

    BasicBlock *outerBB = outerIdxInst->getParent();
    Function *F = outerBB->getParent();

    auto &LIWP = getAnalysis<LoopInfoWrapperPass>(*F);
    LoopInfo *LI = &LIWP.getLoopInfo();
    Loop *l = LI->getLoopFor(outerBB);

    if (l == NULL) {
 //       outs() << "Found no loop associated with this basic block\n";
    }

    unsigned loopDepth = 100;
    while (loopDepth != 1) {
        loopDepth = l->getLoopDepth();
        if (loopDepth != 1) {
            l = l->getParentLoop();
        }
    }

    loopBB = l->getBlocks();
    outerLoop = l;

    // Check that this region is a SCoP
    // Check only exit blocks assuming we have run loop
    // simplify to insert a single preheader at the entry of
    // the SCoP
    SmallVector<BasicBlock*, 100> exitBlks;
    l->getExitingBlocks(exitBlks);

    if (exitBlks.size() > 1) {
        outerLoop = NULL;
    }
}


/************************************************
 * Function: deleteSetAndAddBB
 *
 * Description: This function sets the exit of the
 * loop pre-header to point to the loop exit. The
 * instrumented function call will be inserted in the
 * pre-header basic block
 * *********************************************/
BasicBlock *deleteSetAndAddBB(std::vector<BasicBlock *> &loopBB,
                              Loop *                    outerLoop)
{
    // We assume that this set is a SCOP
    BasicBlock *entry = outerLoop->getLoopPreheader();
    SmallVector<BasicBlock *, 1> exitBlks;
    outerLoop->getExitBlocks(exitBlks);
    BasicBlock *exit = exitBlks[0];

    // Now remove all the blocks that enclose this matrix multiply
    // from the code
    TerminatorInst *ti = entry->getTerminator();

    // Redirect the branch instruction of the entry to point directly to the
    // exit now. Since we are assuming that we have a SCOP here, we can say that
    // our implementation will always be fine.
    for (auto it = succ_begin(entry); it != succ_end(entry); ++it) {
        BasicBlock *bb = *it;
        if (!outerLoop->contains(bb)) {
            continue;
        }
        for (Instruction &I : *bb) {
            if (isa<PHINode> (&I)) {
                PHINode *phi = dyn_cast<PHINode> (&I);
                unsigned inCount = phi->getNumIncomingValues();
                for (uint32_t i = 0; i < inCount; ++i) {
                    BasicBlock *inBlk = phi->getIncomingBlock(i);
                    if (inBlk == entry) {
                        phi->removeIncomingValue(i);
                        break;
                    }
                }
            }
        }
    }

    if (isa<BranchInst> (ti)) {
        BranchInst *bi = dyn_cast<BranchInst> (ti);
        bi->setSuccessor(0, exit);
    }

    return (entry);
}



/****************************************
 * This is the function for instrumenting the
 * HPC kernel function call.
 *
 * There are various complexities involved in this
 * implementation and falls out of scope of this
 * project.
 * **************************************/
void callHpcKernel(BasicBlock *parent,
                   Tensor     *&C,
                   Tensor     *&A,
                   Tensor     *&B)
{
//    Function *F = parent->getParent();
//    Module *M = F->getParent();
//    outs() << "Parent Module " << M->getName() << "\n";
}

