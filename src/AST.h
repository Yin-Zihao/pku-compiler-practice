// AST.h
#pragma once

#include <memory>
#include <string>
#include <iostream>

// 所有 AST 的基类
class BaseAST {
 public:
  virtual ~BaseAST() = default;
  virtual void Dump() const = 0;  // 用于输出 AST 结构
  virtual void GenIR(IRGenerator& generator) {}  // lv1.4新增
};

// CompUnit 是 BaseAST
class CompUnitAST : public BaseAST {
 public:
  // 用智能指针管理对象
  std::unique_ptr<BaseAST> func_def;
  void Dump() const override {
    std::cout << "CompUnitAST { ";
    func_def->Dump();
    std::cout << " }";
  }
};

// FuncDef 也是 BaseAST
class FuncDefAST : public BaseAST {
 public:
  std::unique_ptr<BaseAST> func_type;
  std::string ident;
  std::unique_ptr<BaseAST> block;

  void Dump() const override {
    std::cout << "FuncDefAST { ";
    func_type->Dump();
    std::cout << ", " << ident << ", ";
    block->Dump();
    std::cout << " }";
  }
};

class FuncTypeAST : public BaseAST {
 public:
  explicit FuncTypeAST(const std::string& type_name = "int")
      : type_name_(type_name) {}

  void Dump() const override {
    std::cout << "FuncTypeAST { " << type_name_ << " }";
  }

 private:
  std::string type_name_;
};

class BlockAST : public BaseAST {
 public:
  explicit BlockAST(std::unique_ptr<BaseAST> stmt)
      : stmt_(std::move(stmt)) {}

  void Dump() const override {
    std::cout << "BlockAST { ";
    if (stmt_) {
      stmt_->Dump();
    }
    std::cout << " }";
  }

 private:
  std::unique_ptr<BaseAST> stmt_;
};

class StmtAST : public BaseAST {
 public:
  int number;

  explicit StmtAST(int num) : number(num) {}

  void Dump() const override {
    std::cout << "StmtAST { " << number << " }";
  }
};

class NumberAST : public BaseAST {
 public:
  int value;

  explicit NumberAST(int val) : value(val) {}

  void Dump() const override {
    std::cout << "NumberAST { " << value << " }";
  }
};