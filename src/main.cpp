#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include "AST.h"
#include "IRGenerator.h"
#include "KoopaIRConverter.h"

using namespace std;

// 声明 lexer 的输入, 以及 parser 函数
// 为什么不引用 sysy.tab.hpp 呢? 因为首先里面没有 yyin 的定义
// 其次, 因为这个文件不是我们自己写的, 而是被 Bison 生成出来的
// 你的代码编辑器/IDE 很可能找不到这个文件, 然后会给你报错 (虽然编译不会出错)
// 看起来会很烦人, 于是干脆采用这种看起来 dirty 但实际很有效的手段
extern FILE *yyin;
extern int yyparse(std::unique_ptr<BaseAST> &ast);//lv1.3修改 保证 main.cpp 中对 yyparse 的声明与 Bison 生成的解析器接口一致。

int main(int argc, const char *argv[]) {
  // 解析命令行参数. 测试脚本/评测平台要求你的编译器能接收如下参数:
  // compiler 模式 输入文件 -o 输出文件
  assert(argc == 5);
  auto mode = argv[1];
  auto input = argv[2];
  auto output = argv[4];

  // 打开输入文件, 并且指定 lexer 在解析的时候读取这个文件
  yyin = fopen(input, "r");
  assert(yyin);

  // 调用 parser 函数, parser 函数会进一步调用 lexer 解析输入文件的
  unique_ptr<BaseAST> ast;
  auto ret = yyparse(ast);
  ast->Dump();  // 使用 Dump() 输出 AST 结构
  cout << endl;

  // 生成IR文件
    auto ir_output = std::string(argv[4]);  // lv1.4新增
    IRGenerator generator(ir_output);                  // lv1.4新增
    generator.generateIR(ast);                         // lv1.4新增

  // lv2.1新增：读取生成的IR文件并转换
    std::ifstream ir_file(ir_output);
    std::string ir_str((std::istreambuf_iterator<char>(ir_file)),std::istreambuf_iterator<char>());
    
    // 转换为内存形式的Koopa IR
    koopa_raw_program_t raw_program = KoopaIRConverter::ConvertToRawProgram(ir_str);

  return 0;
}