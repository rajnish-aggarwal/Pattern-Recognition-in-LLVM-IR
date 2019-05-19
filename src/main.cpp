/**************************************
 * 15745 Project
 * File: main.cpp
 * Authors: Rajnish Aggarwal <rajnisha>
 *          Peter Oostema <poostema>
 *
 * Description: This file gives a high level
 * implementation of identifying matrix multiplications
 * of the form C[i][j] += A[i][k] * B[k][j] and
 * *(C + i * N + j) += *(A + i * L + k) *
 *                     *(B + k * N + j) where
 * Matrix C has dimensions M x N,
 * Matrix A has dimensions M x L and
 * Matrix B has dimensions L x N.
 *
 * This is a proof of concept of how identifying matrix
 * multiply operations (more generalized algorithm
 * presented in report) can be performed within LLVM IR.
 * ************************************/

#include "../include/InstFinder.h"
#include "../include/Tensor.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"

/***************************************
 * Function : runOnModule
 * Description: This is the orchestrating function
 * of the entire implementation of the matrix multiply
 * identification algorithm.
 * ************************************/
bool InstFinder::runOnModule(Module &M)
{
    for (Function &F : M) {
//        outs() << "Analyzing function " << F.getName() << "\n";
        for (BasicBlock &BB : F) {
            for (Instruction &I : BB) {
                // Look for all the store instructions in a given
                // function.
                if (StoreInst *SI = dyn_cast<StoreInst> (&I)) {
                    // Now check if this store instruction corresponds
                    // to a c += a * b, kind of reduction operation
                    Instruction *MI = NULL; // Pointer to mult. instruction
                    bool isReduction = isReductionOp(SI, MI);
                    if (!isReduction) {
//                        outs() << "The instruction " << *SI << " is not a store instruction for a reduction operation\n";
                        continue;
                    }
                    // We have verified that the instruction at
                    // hand is reduction. Now we need to verify the integrity of
                    // the operands, i.e.
                    //     1. Get all the indexes that are used to address these
                    //        variables (tensors) and get their ranges
                    //     2. Once we have this information for all C, A and B
                    //        tensors, we go ahead and use our algorithm (CITE)
                    //        to see if they actually correspond to a matrix
                    //        multiply operation.
                    Tensor *a, *b, *c;
                    bool extracted = extractOperands(SI, MI, c, a, b);
                    if (!extracted) {
//                        outs() << "This instruction is not mat mul\n";
                        continue;
                    }
//                    outs() << "Extracted operands\n";
                    // Find all the indexes used to address this tensor
                    // and it's corresponding range as well
                    c->findIndexAndRange();
                    a->findIndexAndRange();
                    b->findIndexAndRange();

                    bool isMatMul = verifyMatMulOperands(c, a, b);
                    if (!isMatMul) {
                        outs() << "=================================";
                        outs() << "The matrices C, A and B do not qualify for being matrices\n";
                        outs() << "Printing matrix C\n";
                        c->print();
                        outs() << "Print matrix B\n";
                        b->print();
                        outs() << "Print matrix A\n";
                        a->print();
                        outs() << "=================================";
                        delete(c);
                        delete(a);
                        delete(b);
                        continue;
                    }
                    else {
                        outs() << "=================================\n";
                        outs() << "Found matrix multiply instruction\n";
                        outs() << "Matrix C\n";
                        c->print();
                        outs() << "Matrix A\n";
                        a->print();
                        outs() << "Matrix B\n";
                        b->print();
                        outs() << "=================================\n";
                        // NOTE:
                        // This part has not been implemented and falls
                        // out of the scope of this project
                        // We find the maximal set of basic blocks that
                        // comprise of this matMul operation.
                        //
                        // Ensure that they represent a valid Scop i.e.
                        // control flow with a single entry and single
                        // exit.
                        //
                        // Remove this Scop and insert a function call to
                        // the high performance kernel.
                        //
                        // __MAT_MUL__(C, sizesof(c), A, sizesof(A),
                        //             B, sizesof(b), isTranspose);
                        //
                        //             refer to hpc_matmul.c in datadir
                        //             for seeing an implementation of this
                        //             function.
                        //
                        // For proof of concept, we have show a matrix
                        // multiply kernel (mat_mul_kernel.c) which is approx.
                        // 7x faster than the state of the art Pollyhedral
                        // compilations in GCC and the Polly compiler.
                        std::vector<BasicBlock*> loopBB;
                        Loop *outerLoop = NULL;
                        // Find the maximal set of basic blocks that comprise
                        // this operation
                        findBasicBlocks(loopBB, c, outerLoop);
                        if (!outerLoop) {
                            outs() << "This loop structure is not a SCoP\n";
                            delete(a);
                            delete(b);
                            delete(c);
                        }
                        // This function changes the exit condition of the
                        // pre-header basic block to point to the exit of the
                        // loops
                        BasicBlock *bbForFunc = deleteSetAndAddBB(loopBB, outerLoop);
                        // Now in the basic block for function, insert a function
                        // call to the newly defined API. THIS PART HAS NOT BEEN
                        // IMPLEMENTED
                        callHpcKernel(bbForFunc, c, a, b);
                    }
                }
            }
        }
    }

    return (true);
}


char InstFinder::ID = 0;
RegisterPass<InstFinder> X("inst-finder", "15745 Instruction Finder Pass");
