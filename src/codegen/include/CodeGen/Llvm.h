#pragma once

// #include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include <memory>
#include <stdexcept>
#include "Common/Types.h"
#include "Common/Map.h"
#include "Token/Token.h"

struct Program;
struct FunctionDefinition;
struct ReturnStatement;
struct LetStatement;
struct Identifier;
struct BinaryOp;
struct Lambda;
struct IdentifierWithType;
struct PrefixExpression;
struct StreamOut;
struct IntegerLiteral;
struct FunctionInvokation;
struct ExpressionStatement;


#define GEN llvm::Value *
#define TODO(type) GEN visit(type * t) { throw std::runtime_error("Not implemented"); }

class LLVMGenerator {
public:
    LLVMGenerator()
        : context(llvm::LLVMContext())
        , module(llvm::Module("Flow", context))
        , builder(llvm::IRBuilder<>(context))
        , localValues()
        , globalValues()
    {}

    void initRuntime();

    void process(Program * program);

    // expressions
    GEN visit(Identifier * ident);
    GEN visit(BinaryOp * op);
    TODO(Lambda)
    TODO(IdentifierWithType)
    TODO(PrefixExpression)
    TODO(StreamOut)
    GEN visit(IntegerLiteral * integer);
    GEN visit(FunctionInvokation * inv);

    // statements
    GEN visit(FunctionDefinition * def);
    GEN visit(ReturnStatement * ret);
    GEN visit(LetStatement * ls);
    GEN visit(ExpressionStatement * es);

private:

    llvm::LLVMContext context;
    llvm::Module module;
    llvm::IRBuilder<> builder;
    Flow::Map<std::string, llvm::Value *> localValues;
    Flow::Map<std::string, llvm::Value *> globalValues;
    Flow::Map<std::string, llvm::Function *> builtIns;
};
