#pragma once
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "Ast/Node.h"

class AstPass {
public:
    virtual void run(
        Program * program,
        llvm::LLVMContext & context,
        llvm::Module & module,
        llvm::IRBuilder<> & builder) = 0;
};
