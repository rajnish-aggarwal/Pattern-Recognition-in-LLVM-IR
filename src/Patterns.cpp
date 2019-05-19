#include "../include/Patterns.h"
#include "llvm/Support/raw_ostream.h"

/************************************************
 * This API deep prints everything inside the
 * matrix multiply instruction.
 * *********************************************/
void MatMulInstr::print()
{
    outs() << "<<<<<<< PRINTING FOR INSTRUCTION >>>>>>>>\n";
    outs() << *storeInstr << "\n";
    outs() << "<<<<<< PRINTING LHS Matrix  >>>>>>>>\n";
    lhs->print();
    outs() << "<<<<<< PRINTING RHS 1 Matrix  >>>>>>>>\n";
    rhs.first->print();
    outs() << "<<<<<< PRINTING RHS 2 Matrix  >>>>>>>>\n";
    rhs.second->print();
}
