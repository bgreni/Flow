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


llvm::Type * LLVMGenerator::getIntType() {
    return llvm::IntegerType::getInt64Ty(context);
}

llvm::ArrayType * LLVMGenerator::getIntArrayType(unsigned int len) {
    const auto intT = getIntType();
    return llvm::ArrayType::get(intT, len);
}

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
    auto ft = llvm::FunctionType::get(getIntType(),params,false);
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
         module.print(llvm::outs(), nullptr);
         throw CodeGenError("\n\n\n" + outs.str());
     }
    std::error_code c;
    llvm::raw_fd_ostream out(outName(), c);
    module.print(out, nullptr);
}

llvm::Value * LLVMGenerator::visit(BinaryOp * op) {
    auto leftVal = op->left->accept(this);
    auto rightVal = op->right->accept(this);

#define CREATE_COMP(t) builder.CreateICmp(t, leftVal, rightVal)

    using namespace FlowToken;
    switch (op->token.type) {
        case Type::PLUS:
            return builder.CreateAdd(leftVal, rightVal);
        case Type::ASTERISK:
            return builder.CreateMul(leftVal, rightVal);
        case Type::MINUS:
            return builder.CreateSub(leftVal, rightVal);
        case Type::SLASH:
            return builder.CreateSDiv(leftVal, rightVal);
        case Type::EQUALS:
            return CREATE_COMP(llvm::CmpInst::ICMP_EQ);
        case Type::NOT_EQUALS:
            return CREATE_COMP(llvm::CmpInst::ICMP_NE);
        case Type::LT:
            return CREATE_COMP(llvm::CmpInst::ICMP_SLT);
        case Type::GT:
            return CREATE_COMP(llvm::CmpInst::ICMP_SGT);
        default:
            THROW(fmt::format("operator '{}' not defined", op->tokenLiteral()), op->token);
    }
#undef CREATE_COMP
}

llvm::Value * LLVMGenerator::visit(IntegerLiteral * integer) {
    return builder.getInt64(integer->value);
}

llvm::Value * LLVMGenerator::visit(Array * arr) {
//    std::vector<llvm::Constant *> values;
//    for (const auto & val : arr->values) {
//        values.emplace_back(val->accept(this));
//    }
}

 llvm::Value * LLVMGenerator::visit(Identifier * ident) {
    if (auto arg = funcArgs.get(ident->tokenLiteral())) {
        return arg.value();
    }

     auto named = localValues.get(ident->tokenLiteral());

     if (!named) {
         THROW(
             fmt::format("Symbol not defined: {}", ident->tokenLiteral()),
             ident->token);
     }

     auto val = named.value();

     return builder.CreateLoad(val->getType(), named.value());
 }

llvm::Value * LLVMGenerator::visit(LetStatement * ls) {
    auto value = ls->value->accept(this);
    const auto varName = ls->name->tokenLiteral();
    auto ai = builder.CreateAlloca(
        Utils::getLLVMType(ls->name->type, context));
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
        funcArgs.emplace_back(arg->accept(this));
    }
    return builder.CreateCall(func, funcArgs);
}

llvm::Value * LLVMGenerator::visit(ExpressionStatement * es) {
    return es->expr->accept(this);
}

llvm::Value * LLVMGenerator::visit(FunctionDefinition * def) {
    localValues.clear();
    funcArgs.clear();
    llvm::Function * func;
    if (def->isMain()) {
        func = module.getFunction("main");
        auto & entry = func->getEntryBlock();
        builder.SetInsertPoint(&entry);
    } else {
        func = module.getFunction(def->name->tokenLiteral());
        for (auto & arg : func->args()) {
            funcArgs[arg.getName().str()] = &arg;
        }
        auto entry = llvm::BasicBlock::Create(context, "entry",func);
        builder.SetInsertPoint(entry);
    }
    
    if (!func) {
        THROW(fmt::format("Failed to create function: {}", def->name->tokenLiteral()), def->token);
    }

    const auto endsWithReturn = def->body.back()->isReturn();

    def->body.accept(this);

    if (!endsWithReturn) {
        builder.CreateRetVoid();
    }

    llvm::verifyFunction(*func);

    return func;
}

llvm::Value * LLVMGenerator::visit(IfStatement * ifs) {
    auto fun = builder.GetInsertBlock()->getParent();
    auto ifTrueBlock = llvm::BasicBlock::Create(context, "ifTrue", fun);
    auto ifFalseBlock = llvm::BasicBlock::Create(context, "ifFalse", fun);
    auto ifEndBlock = llvm::BasicBlock::Create(context, "ifEndBlock", fun);
//    auto endBlock = llvm::BasicBlock::Create(context, "ifEnd");
    auto condValue = ifs->mainBody.condition->accept(this);
    builder.CreateCondBr(condValue, ifTrueBlock, ifFalseBlock);
    builder.SetInsertPoint(ifTrueBlock);
    ifs->mainBody.body.accept(this);

    if (!ifs->mainBody.body.returns()) {
        builder.CreateBr(ifEndBlock);
    }

    auto & elifs = ifs->elifBlocks;
    if (!elifs.empty()) {
        THROW("Elif not implemented yet", ifs->token);
    }

    builder.SetInsertPoint(ifFalseBlock);
    ifs->elseBlock.body.accept(this);

    if (!ifs->elseBlock.body.returns()) {
        builder.CreateBr(ifEndBlock);
    }

    builder.SetInsertPoint(ifEndBlock);
    return nullptr;
}

llvm::Value * LLVMGenerator::visit(Body * b) {
    for (const auto & stmt : b->statements) {
        stmt->accept(this);
    }
    return nullptr;
}