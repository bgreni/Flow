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
            auto fnType = llvm::FunctionType::get(
                Utils::getLLVMType(def->returnType->token.type, context),
                false
            );

            llvm::Function::Create(
                fnType,
                llvm::Function::ExternalLinkage,
                def->name->tokenLiteral(),
                module);
        }
    }
}
