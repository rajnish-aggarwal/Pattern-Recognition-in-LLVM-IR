//////////////////////////////////
//  15-745 S19 PROJECT
//  Group: Rajnish Aggarwal <rajnisha@andrew.cmu.edu>
//         Peter Oostema <poostema@andrew.cmu.edu>
//////////////////////////////////

#ifndef PATTERNS_H
#define PATTERNS_H

#include "Tensor.h"
#include "llvm/IR/Function.h"

#include <utility>

class MatMulInstr
{
private:
    Instruction *storeInstr;   // instruction on which analysis is based
    Tensor *lhs;                     // lhs matrix for matmul
    std::pair<Tensor*, Tensor*> rhs;  // rhs matrix/vectors of the instruction

public:
    ~MatMulInstr()
    {
        delete (lhs);
        delete (rhs.first);
        delete (rhs.second);
    }
    // Get/set routines for instruction on which the analysis is
    // based.
    void setInstruction(Instruction *I)
    {
        storeInstr = I;
    }
    Instruction *getInstruction()
    {
        return (storeInstr);
    }
    // Routine to set the LHS matrix
    void setLhsMatrix(Tensor *t)
    {
        lhs = t;
    }
    // Routine to set the rhs pair
    void setRhsMatrices(Tensor *t1, Tensor *t2)
    {
        rhs = std::make_pair(t1, t2);
    }
    // Routine to get the lhs matrix
    Tensor *getLhsMatrix()
    {
        return (lhs);
    }
    // Routine to get the rhs matrix pair
    // Use (object)->first & (object)->second to get the actual
    // data.
    std::pair<Tensor *, Tensor *> *getRhsMatrixPair()
    {
        return (&rhs);
    }
    // Get the parent basic block of the instruction in picture
    // currently
    BasicBlock *getParent()
    {
        return (storeInstr->getParent());
    }
    void print();
};

#endif
