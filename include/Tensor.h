/////////////////////////////////////////////////                                       ////// 15-745 S19 Project
//// Group: Rajnish Aggarwal <rajnisha@andrew.cmu.edu>
////        Peter Oostema <poostema@andrew,.cmu.edu>
/////////////////////////////////////////////////////

#ifndef TENSOR_H
#define TENSOR_H

#include "llvm/IR/Instructions.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Value.h"


using namespace llvm;

// This class stores all the information about a variable that is
// used to index into a tensor. Each tensor will have a list of these
// variable.

class IndexVar
{
private:
    Value *start;
    Value *end;
    Value *stride;
    Instruction *name;
    uint32_t size;

public:
    IndexVar(Instruction *I) : name(I)
    {}

    void setStartAndEnd(Value *s, Value *e)
    {
        start = s;
        end = e;
    }
    void setStride(Value *s) { stride = s; }
    void setSize(uint64_t size_) { size = size_; }
    uint64_t getSize() const { return (size); }
    Value *getStart() { return (start); }
    Value *getEnd() { return (end); }
    Value *getStride() { return (stride); }
    Instruction *getName() { return (name); }

    void print();
};

/********************************************************
 * This is the tensor class which is used to store
 * information about the tensors we are interested
 * in using.
 *
 * Use getIndices and isMatrix to determine if the
 * tensor at hand is a matrix object.
 *
 * Also, the indice order is innermost to outermost
 ********************************************************/
class Tensor
{
private:
    std::vector<IndexVar *> indexVariables;
    Instruction *refInstr;        // stores the instruction from where we start analysing
                                  // this object
    Instruction *defInstr;        // stores the instruction where this object is defined

public:
    Tensor(Instruction *I) : refInstr(I)
    {}

    Tensor() {}

    Instruction *getRefInstr() { return (refInstr); }
    void setRefInstr(Instruction *I) { refInstr = I; }
    void setDefInstr(Instruction *I) { defInstr = I; }
    std::vector<IndexVar *> &getIndexVariables()
    {
        return (indexVariables);
    }

    // UTIL API's
public:
    void print();
    void findIndexAndRange();

private:
    void extractRangeAndSize(Instruction *rInstr,
            Instruction *I, IndexVar *&var);
    uint64_t extractConstants(Instruction *I, Instruction *&rootVar);
};

#endif
