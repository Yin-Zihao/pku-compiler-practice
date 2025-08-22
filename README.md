# pku-compiler-practice README
> English is not my native language; please excuse typing errors.
## summarize

This project is a compiler learning project based on [the guidance of Peking University's online compilation practice document](https://pku-minic.github.io/online-doc/#/), aiming to record my learning route and the pit I stepped on. As the online documentation of Peking University's compilation practice is published by [CC BY-NC-SA 4.0 protocol](https://creativecommons.org/licenses/by-nc-sa/4.0/), this protocol is also used in this project.
Due to my lack of knowledge, there should be many unreasonable points in the project structure and code, and I hope you will criticize and correct me.

## 综述
本项目是基于[北大编译实践在线文档](https://pku-minic.github.io/online-doc/#/)的指导进行的编译器学习项目，旨在记录本人的学习路线和踩过的坑。由于北大编译实践在线文档采用 [CC BY-NC-SA 4.0 协议](https://creativecommons.org/licenses/by-nc-sa/4.0/)发布，本项目亦沿用此协议。
由于本人才疏学浅,项目结构与代码不合理之处应该颇多,还望各位大佬批评指正。

## Project structure(updated on August 22, 2025)
```
├── src/
├── AST.h # abstract syntax tree base class and node definition
├── IRGenerator.h # IR generator interface declaration
├ ── irgenerator. CPP # Implementation of IR generator (AST ergodic logic)
├── main.cpp # Compiler main program (parsing entry/command line processing)
├── sysy.y # Bison grammar file (AST construction rules)
└── sysy.l # Flex Lexical Definition (Generative Lexical Analyzer)
```

## 项目结构 （2025/08/22更新）
```
├── src/ 
├── AST.h # 抽象语法树基类与节点定义 
├── IRGenerator.h # IR生成器接口声明 
├── IRGenerator.cpp # IR生成器实现（AST遍历逻辑） 
├── main.cpp # 编译器主程序（解析入口/命令行处理） 
├── sysy.y # Bison语法文件（AST构建规则）
└── sysy.l # Flex 词法定义（生成词法分析器）
```

## progress record
### [Lv1. `main` function](https://pku-minic.github.io/online-doc/#/lv1-main/)(implemented on August 22, 2025)
#### Project function
-Support analytic function definition (`int main() { ... }`)
-Support return statement parsing (`return 0;`)
-Identify integer constants (`INT_CONST`) and identifiers (`IDENT`).
-AST design and IR generation of the above structures

#### Step on pit points in this chapter
- Only the definition and implementation of line comments are given in the document.
	sysy.l `LineComment   "//"[^\n]*`
	sysy.l `{LineComment} {/* Ignore and do nothing */} `
- But there will be a comment `/* */` during the evaluation, so we need to define it ourselves.
	sysy.l `BlockComment   \/\*.*? \*\/`
	sysy.l `{BlockComment} {/* Ignore and do nothing */} `

## 进度记录
### [Lv1. `main` 函数](https://pku-minic.github.io/online-doc/#/lv1-main/?id=lv1-main-%e5%87%bd%e6%95%b0)（2025/08/22实现）
#### 项目功能
- 支持解析函数定义（`int main() { ... }`）
- 支持返回语句解析（`return 0;`）
- 识别整型常量（`INT_CONST`）和标识符（`IDENT`）
- 以上结构的AST设计和IR生成

#### 本章节踩坑点
- 文档中只给出了行注释的定义及实现
	sysy.l  `LineComment   "//"[^\n]*`
	sysy.l  `{LineComment}   { /* 忽略, 不做任何操作 */ }`
- 但是在测评时会有块注释 `/**/` 出现，这时需要我们自己定义
	sysy.l `BlockComment   \/\*.*?\*\/`
	sysy.l `{BlockComment}  { /* 忽略, 不做任何操作 */ }`
