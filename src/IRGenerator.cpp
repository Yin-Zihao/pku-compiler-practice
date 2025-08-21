#include "IRGenerator.h"
#include <cassert>

IRGenerator::IRGenerator(const std::string& output_file) 
    : out_(output_file) {}

IRGenerator::~IRGenerator() {
    if (out_.is_open()) out_.close();
}

void IRGenerator::generateIR(std::unique_ptr<BaseAST>& ast) {
    if (!out_.is_open()) return;
    
    // 生成固定前导
    out_ << "fun @main(): i32 {\n%entry:\n";
    
    // 遍历AST生成IR
    if (ast) {
        auto* comp_unit = dynamic_cast<CompUnitAST*>(ast.get());
        if (comp_unit && comp_unit->func_def) {
            visitFuncDefAST(static_cast<FuncDefAST*>(comp_unit->func_def.get()));
        }
    }
    
    // 生成固定尾随
    out_ << "}\n";
}

void IRGenerator::visitFuncDefAST(FuncDefAST* node) {
    if (node->block) {
        visitBlockAST(static_cast<BlockAST*>(node->block.get()));
    }
}

void IRGenerator::visitBlockAST(BlockAST* node) {
    if (node->getStmt()) { // 使用getter方法
        visitStmtAST(static_cast<StmtAST*>(node->getStmt().get()));
    }
}

void IRGenerator::visitStmtAST(StmtAST* node) {
    out_ << "  ret " << node->number << "\n";
}