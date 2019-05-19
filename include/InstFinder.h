/////////////////////////////////////////////////
// 15-745 S19 Project
// Group: Rajnish Aggarwal <rajnisha@andrew.cmu.edu>
//        Peter Oostema <poostema@andrew,.cmu.edu>
/////////////////////////////////////////////////


#ifndef INSTFINDER_H
#define INSTFINDER_H

#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "Tensor.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"

using namespace llvm;

// Util API's
bool isReductionOp(StoreInst *&SI, Instruction *&MI);
bool extractOperands(StoreInst *&SI, Instruction *&MI,
        Tensor *&C, Tensor *&A, Tensor *&B);
bool verifyMatMulOperands(Tensor *C, Tensor *A, Tensor *B);
BasicBlock *deleteSetAndAddBB(std::vector<BasicBlock *> &loopBB, Loop *outerLoop);
void callHpcKernel(BasicBlock *parent, Tensor *&C, Tensor *&A, Tensor *&B);


/************************************
 * This is a primary class for going
 * over all the modules, it is called
 * the InstrFinder class because it will
 * help us extract all the matrix
 * multiplication instructions out of
 * the given program
 * ********************************/
class InstFinder : public ModulePass
{
public:
    static char ID;
    InstFinder() : ModulePass(ID) {}

    virtual bool runOnModule(Module &M);

    virtual void getAnalysisUsage(AnalysisUsage &AU) const override
    {
        ModulePass::getAnalysisUsage(AU);
        AU.addRequired<LoopInfoWrapperPass>();
        AU.addPreserved<LoopInfoWrapperPass>();
    }

    void findBasicBlocks(std::vector<BasicBlock*> &loopBB,
            Tensor *&C, Loop *&outerLoop);
};

#endif
