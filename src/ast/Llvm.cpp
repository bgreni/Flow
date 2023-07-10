#include "Llvm.h"
#include "Exceptions.h"
#include "Expressions/FunctionInvokation.h"
#include "Statements/FunctionDefinition.h"
#include "Token/TokenDefs.h"
#include "fmt/core.h"
#include "Expression.h"
#include "Statement.h"
#include "Node.h"
#include <cstddef>
#include <fstream>
#include <ios>
#include <iostream>
#include <system_error>


#define THROW(msg, tok) \
    throw CodeGenError( \
        fmt::format( \
            "{}\nline: {}, column: {}",\
            msg, \
            tok.line, \
            tok.column \
        ) \
    )

#define LOG(str) std::cout << str << std::endl;

llvm::Type * LLVMGenerator::getLLVMType(FlowToken::Type t) {
    switch (t) {
        case FlowToken::Type::INT:
            return llvm::Type::getInt64Ty(context);
        default:
            return llvm::Type::getVoidTy(context);
    }
}

void LLVMGenerator::initRuntime() {
    using namespace llvm;
    
    auto printf = Function::Create(
        FunctionType::get(
            IntegerType::getInt32Ty(context),
            std::vector<Type *>{Type::getInt8PtrTy(context)},
            // {PointerType::get(Type::getInt8Ty(context), 0)},
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
//    auto globalEntry = llvm::BasicBlock::Create(context, "entry");
//    builder.SetInsertPoint(globalEntry);

    auto str = builder.CreateGlobalString("%ld\n");
    namedValues["printStr"] = str;

    // builder.CreateCall(printf, {str});

    // builder.CreateRetVoid();
}

void LLVMGenerator::process(Program * program) {
    initRuntime();
    for (const auto & stmt : program->statements) {
        stmt->accept(this);
    }

    std::string errors;
    // llvm::raw_string_ostream out(errors);
    // if (llvm::verifyModule(module, &out)) {
    //     throw CodeGenError(out.str());
    // }
    std::error_code c;
    llvm::raw_fd_ostream out("../out.ll", c);
    module.print(out, nullptr);
}

llvm::Value * LLVMGenerator::visit(IntegerLiteral * integer) {
    return builder.getInt64(integer->value);
}

// llvm::Value * LLVMGenerator::visit(Identifier * ident) {
//     auto named = namedValues.get(ident->tokenLiteral());
//     if (!named) {
//         THROW(
//             fmt::format("Symbol not defined: {}", ident->tokenLiteral()),
//             ident->token);
//     }

//     // auto ft = llvm::FunctionType::get();
// }

llvm::Value * LLVMGenerator::visit(ReturnStatement * ret) {
    auto value = ret->value->accept(this);
    return builder.CreateRet(value);
}

llvm::Value * LLVMGenerator::visit(FunctionInvokation * inv) {
    auto func = module.getFunction(inv->tokenLiteral());

    std::vector<llvm::Value *> funcArgs;
    const auto name = inv->tokenLiteral();
    if (name == "print") {
        funcArgs.emplace_back(namedValues["printStr"]);
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
    return nullptr;
}

llvm::Value * LLVMGenerator::visit(FunctionDefinition * def) {

    std::vector<llvm::Type *> params{};
    for (const auto & arg : def->args) {
        params.emplace_back(getLLVMType(arg->token.type));
    }
    llvm::Function * func;
    if (def->isMain()) {
        func = module.getFunction("main");
        auto & entry = func->getEntryBlock();
        builder.SetInsertPoint(&entry);
    } else {
        auto fnType = llvm::FunctionType::get(
            getLLVMType(def->returnType->token.type),
            false
        );

        func = llvm::Function::Create(
            fnType,
            llvm::Function::ExternalLinkage,
            def->name->tokenLiteral(),
            module
        );
        auto entry = llvm::BasicBlock::Create(context, "entry",func);
        builder.SetInsertPoint(entry);
    }
    
    if (!func) {
        THROW(fmt::format("Failed to create function: {}", def->name->tokenLiteral()), def->token);
    }

    // non-void return type must return a value
    const auto endsWithReturn = !def->body.back()->isReturn();
    const auto voidFunc = !def->returnType->isVoid();
    if (!endsWithReturn && !voidFunc) {
        THROW("Non-void function must return a value", def->name->token);
    }

    for (const auto & stmt : def->body) {
        stmt->accept(this);
    }

    // if (voidFunc && !endsWithReturn) {
    //     builder.CreateRetVoid();   
    // }

    // if (def->isMain()) {
    //     std::vector<llvm::Value *> args;
    //     auto c = builder.CreateCall(func, args);
    // }

    llvm::verifyFunction(*func);

    return func;
}