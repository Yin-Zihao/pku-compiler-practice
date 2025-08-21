#pragma once
#include <memory>
#include <fstream>
#include "AST.h"

class IRGenerator {
    private:
        std::ofstream out_;
        
        void visitFuncDefAST(FuncDefAST* node);
        void visitBlockAST(BlockAST* node);
        void visitStmtAST(StmtAST* node);
    public:
        IRGenerator(const std::string& output_file);
        ~IRGenerator();

        void generateIR(std::unique_ptr<BaseAST>& ast);
};