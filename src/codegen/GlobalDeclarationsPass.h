#pragma once
#include "AstPass.h"


class GlobalDeclarationsPass : AstPass {
public:
    void run(
        Program * program,
        llvm::LLVMContext & context,
        llvm::Module & module,
        llvm::IRBuilder<> & builder) override;
};

