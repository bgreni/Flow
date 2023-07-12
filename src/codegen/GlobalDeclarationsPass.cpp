#include "GlobalDeclarationsPass.h"
#include "Ast/Statement.h"
#include "Utils.h"

void GlobalDeclarationsPass::run(
    Program * program,
    llvm::LLVMContext &context,
    llvm::Module &module,
    llvm::IRBuilder<> &builder) {

    for (const auto & stmt : program->statements) {
        if (auto def = dynamic_cast<FunctionDefinition *>(stmt.get())) {
            if (def->isMain()) continue;
            std::vector<llvm::Type *> params{};
            for (const auto & arg : def->args) {
                params.emplace_back(Utils::getLLVMType(arg->type, context));
            }
            auto fnType = llvm::FunctionType::get(
                Utils::getLLVMType(def->returnType->token.type, context),
                params,
                false
            );

            auto fun = llvm::Function::Create(
                fnType,
                llvm::Function::ExternalLinkage,
                def->name->tokenLiteral(),
                module);

            for (unsigned int i = 0; i < fun->arg_size(); ++i) {
                fun->getArg(i)->setName(def->args[i]->tokenLiteral());
            }
        }
    }
}
