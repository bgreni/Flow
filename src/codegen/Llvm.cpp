#include "include/CodeGen/Llvm.h"
#include "Ast/Exceptions.h"
#include "Ast/Expressions/FunctionInvokation.h"
#include "Ast/Statements/FunctionDefinition.h"
#include "Token/TokenDefs.h"
#include "fmt/core.h"
#include "Ast/Expression.h"
#include "Ast/Statement.h"
#include "Ast/Node.h"
#include <cstddef>
#include <fstream>
#include <ios>
#include <iostream>
#include <system_error>
#include "GlobalDeclarationsPass.h"
#include "Utils.h"


#define THROW(msg, tok) \
    throw CodeGenError( \
        fmt::format( \
            "{}\nline: {}, column: {}",\
            msg, \
            tok.line, \
            tok.column \
        ) \
    )

void LLVMGenerator::initRuntime() {
    using namespace llvm;
    
    auto printf = Function::Create(
        FunctionType::get(
            IntegerType::getInt32Ty(context),
            std::vector<Type *>{Type::getInt8PtrTy(context)},
            true),
            llvm::Function::ExternalLinkage,
            "printf",
            module  
    );

    builtIns["print"] = printf;

     /* Emit the main function */
    std::vector<llvm::Type*> params;
    auto ft = llvm::FunctionType::get(llvm::Type::getInt64Ty(context),params,false);
    auto inz = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "main", module);

     auto BB = llvm::BasicBlock::Create(context, "entry",inz);
     builder.SetInsertPoint(BB);

    auto str = builder.CreateGlobalString("%ld\n");
    globalValues["printStr"] = str;
}

void LLVMGenerator::process(Program * program) {
    initRuntime();

    GlobalDeclarationsPass().run(program, context, module, builder);

    for (const auto & stmt : program->statements) {
        stmt->accept(this);
    }

    std::string errors;
     llvm::raw_string_ostream outs(errors);
     if (llvm::verifyModule(module, &outs)) {
         throw CodeGenError(outs.str());
     }
    std::error_code c;
    llvm::raw_fd_ostream out("../out.ll", c);
    module.print(out, nullptr);
}

llvm::Value * LLVMGenerator::visit(BinaryOp * op) {
    auto leftVal = op->left->accept(this);
    auto rightVal = op->right->accept(this);

    using namespace FlowToken;
    switch (op->token.type) {
        case Type::PLUS:
            return builder.CreateAdd(leftVal, rightVal);
        case Type::ASTERISK:
            return builder.CreateMul(leftVal, rightVal);
        default:
            THROW(fmt::format("operator '{}' not defined", op->tokenLiteral()), op->token);
    }
}

llvm::Value * LLVMGenerator::visit(IntegerLiteral * integer) {
    return builder.getInt64(integer->value);
}

 llvm::Value * LLVMGenerator::visit(Identifier * ident) {
     auto named = localValues.get(ident->tokenLiteral());
     if (!named) {
         THROW(
             fmt::format("Symbol not defined: {}", ident->tokenLiteral()),
             ident->token);
     }

     return builder.CreateLoad(named.value()->getType(), named.value());
 }

llvm::Value * LLVMGenerator::visit(LetStatement * ls) {
    auto value = ls->value->accept(this);
    const auto varName = ls->name->tokenLiteral();
    auto ai = builder.CreateAlloca(
        Utils::getLLVMTypeFromString(ls->name->type, context));
    localValues[varName] = ai;
    return builder.CreateStore(value, ai);
}

llvm::Value * LLVMGenerator::visit(ReturnStatement * ret) {
    auto value = ret->value->accept(this);
    return builder.CreateRet(value);
}

llvm::Value * LLVMGenerator::visit(FunctionInvokation * inv) {
    auto func = module.getFunction(inv->tokenLiteral());

    std::vector<llvm::Value *> funcArgs;
    const auto name = inv->tokenLiteral();
    if (name == "print") {
        funcArgs.emplace_back(globalValues["printStr"]);
        func = builtIns[name];
    }

    if (!func) {
        THROW(fmt::format("Function not defined: {}", inv->tokenLiteral()), inv->token);
    }

    for (const auto & arg : inv->args) {
        // if (auto nested = dynamic_cast<FunctionInvokation *>(arg.get())) {
        //     auto value = nested->accept(LLVMGenerator *v)
        // }

        funcArgs.emplace_back(arg->accept(this));
    }
    return builder.CreateCall(func, funcArgs);
}

llvm::Value * LLVMGenerator::visit(ExpressionStatement * es) {
    return es->expr->accept(this);
}

llvm::Value * LLVMGenerator::visit(FunctionDefinition * def) {
    localValues.clear();
    std::vector<llvm::Type *> params{};
    for (const auto & arg : def->args) {
        params.emplace_back(Utils::getLLVMType(arg->token.type, context));
    }
    llvm::Function * func;
    if (def->isMain()) {
        func = module.getFunction("main");
        auto & entry = func->getEntryBlock();
        builder.SetInsertPoint(&entry);
    } else {
        func = module.getFunction(def->name->tokenLiteral());
        auto entry = llvm::BasicBlock::Create(context, "entry",func);
        builder.SetInsertPoint(entry);
    }
    
    if (!func) {
        THROW(fmt::format("Failed to create function: {}", def->name->tokenLiteral()), def->token);
    }

    const auto endsWithReturn = def->body.back()->isReturn();

    for (const auto & stmt : def->body) {
        stmt->accept(this);
    }

    if (!endsWithReturn) {
        builder.CreateRetVoid();
    }

    llvm::verifyFunction(*func);

    return func;
}