#pragma once

#include "llvm/IR/Type.h"
#include "Token/Token.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/DerivedTypes.h"
namespace Utils {
    static llvm::Type * getLLVMType(FlowToken::Type t, llvm::LLVMContext & context) {
        switch (t) {
            case FlowToken::Type::INT:
                return llvm::Type::getInt64Ty(context);
            default:
                return llvm::Type::getVoidTy(context);
        }
    }

    static llvm::Type * getLLVMType(const ptr<FlowType> & type, llvm::LLVMContext & context) {
        if (auto t = dynamic_cast<IntType *>(type.get())) {
            return llvm::Type::getInt64Ty(context);
        }
        return llvm::Type::getVoidTy(context);
    }
}
