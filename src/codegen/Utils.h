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

    static llvm::Type * getLLVMTypeFromString(const std::string & str, llvm::LLVMContext & context) {
        if (str == "int") {
            return llvm::Type::getInt64Ty(context);
        }
        return llvm::Type::getVoidTy(context);
    }
}
