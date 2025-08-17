%code requires {
  #include <memory>
  #include <string>
  #include "AST.h"//lv1.3新增 确保AST类在生成的头文件中可见
}

%{

#include <iostream>
#include <memory>
#include <string>
#include "AST.h"  // 确保解析器内部实现能使用 AST 类型

// 声明 lexer 函数和错误处理函数
int yylex();
void yyerror(std::unique_ptr<BaseAST> &ast, const char *s);//lv1.3修改 确保错误处理函数能接收 AST 指针参数（与 %parse-param 类型一致）。

using namespace std;

%}

// 定义 parser 函数和错误处理函数的附加参数
// 我们需要返回一个字符串作为 AST, 所以我们把附加参数定义成字符串的智能指针
// 解析完成后, 我们要手动修改这个参数, 把它设置成解析得到的字符串
%parse-param { std::unique_ptr<BaseAST> &ast }//lv1.3修改 将解析器的附加参数从字符串指针改为 AST 指针，允许返回结构化数据。

// yylval 的定义, 我们把它定义成了一个联合体 (union)
// 因为 token 的值有的是字符串指针, 有的是整数
// 之前我们在 lexer 中用到的 str_val 和 int_val 就是在这里被定义的
// 至于为什么要用字符串指针而不直接用 string 或者 unique_ptr<string>?
// 请自行 STFW 在 union 里写一个带析构函数的类会出现什么情况
%union {
  std::string *str_val;
  int int_val;
  BaseAST *ast_val;
}//lv1.3新增BaseAST *ast_val;用于存储AST节点指针

// lexer 返回的所有 token 种类的声明
// 注意 IDENT 和 INT_CONST 会返回 token 的值, 分别对应 str_val 和 int_val
%token INT RETURN
%token <str_val> IDENT
%token <int_val> INT_CONST

// 非终结符的类型定义
%type <ast_val> FuncDef FuncType Block Stmt  // FuncDef 等返回 AST 指针
%type <ast_val> Number                      // Number 返回 AST 指针
//lv1.3修改

%%

// 开始符, CompUnit ::= FuncDef, 大括号后声明了解析完成后 parser 要做的事情
// 之前我们定义了 FuncDef 会返回一个 str_val, 也就是字符串指针
// 而 parser 一旦解析完 CompUnit, 就说明所有的 token 都被解析了, 即解析结束了
// 此时我们应该把 FuncDef 返回的结果收集起来, 作为 AST 传给调用 parser 的函数
// $1 指代规则里第一个符号的返回值, 也就是 FuncDef 的返回值
CompUnit
  : FuncDef {
    auto comp_unit = std::make_unique<CompUnitAST>();
    comp_unit->func_def = unique_ptr<BaseAST>($1);  // 接收 FuncDef 返回的 ast_val
    ast = move(comp_unit);  // 传递给外部的 unique_ptr<BaseAST> &ast
  }
  ;//lv1.3修改

// FuncDef ::= FuncType IDENT '(' ')' Block;
// 我们这里可以直接写 '(' 和 ')', 因为之前在 lexer 里已经处理了单个字符的情况
// 解析完成后, 把这些符号的结果收集起来, 然后拼成一个新的字符串, 作为结果返回
// $$ 表示非终结符的返回值, 我们可以通过给这个符号赋值的方法来返回结果
// 你可能会问, FuncType, IDENT 之类的结果已经是字符串指针了
// 为什么还要用 unique_ptr 接住它们, 然后再解引用, 把它们拼成另一个字符串指针呢
// 因为所有的字符串指针都是我们 new 出来的, new 出来的内存一定要 delete
// 否则会发生内存泄漏, 而 unique_ptr 这种智能指针可以自动帮我们 delete
// 虽然此处你看不出用 unique_ptr 和手动 delete 的区别, 但当我们定义了 AST 之后
// 这种写法会省下很多内存管理的负担
FuncDef
  : FuncType IDENT '(' ')' Block {
    auto ast = new FuncDefAST();
    // 将函数类型（FuncType）的 AST 节点赋值给 FuncDefAST 的 func_type 成员
    // $1 表示当前规则的第一个符号（FuncType），它是一个 BaseAST 类型的指针
    ast->func_type = unique_ptr<BaseAST>($1);
    // 将标识符（IDENT）的值赋值给 FuncDefAST 的 ident 成员
    // $2 表示当前规则的第二个符号（IDENT），它是一个 std::string 类型的指针
    // 使用 std::unique_ptr<string>($2) 来管理内存，并解引用获取字符串值
    ast->ident = *unique_ptr<string>($2);
    // 将块（Block）的 AST 节点赋值给 FuncDefAST 的 block 成员
    // $5 表示当前规则的第五个符号（Block），它是一个 BaseAST 类型的指针
    ast->block = unique_ptr<BaseAST>($5);
    // 将新创建的 FuncDefAST 对象赋值给 Bison 的语义值栈
    // $$ 是 Bison 用于表示当前规则的语义值的符号
    $$ = ast;
  }
  ;//lv1.3修改

// 同上, 不再解释
FuncType
  : INT {
    $$ = new FuncTypeAST();
  }
  ;//lv1.3修改

Block
  : '{' Stmt '}' {
    auto block = new BlockAST(unique_ptr<BaseAST>($2));  // 接收 Stmt 返回的 ast_val
    $$ = block;
  }
  ;//lv1.3修改

Stmt
  : RETURN Number ';' {
    // $2 是 Number 返回的 ast_val（BaseAST*），需转换为 NumberAST* 并提取 value
    auto num_ast = static_cast<NumberAST*>(yyvsp[-1].ast_val);
    auto stmt = new StmtAST(num_ast->value);  // 传递 int 类型参数
    $$ = stmt;
  }

Number
  : INT_CONST {
    $$ = new NumberAST($1);
  }
  ;

%%

// 定义错误处理函数, 其中第二个参数是错误信息
// parser 如果发生错误 (例如输入的程序出现了语法错误), 就会调用这个函数
void yyerror(unique_ptr<string> &ast, const char *s) {
  cerr << "error: " << s << endl;
}


