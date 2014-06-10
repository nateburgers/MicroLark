//
//  ast.cpp
//  XcodeLLVM
//
//  Created by Nathan Burgers on 6/7/14.
//  Copyright (c) 2014 Refulgent Software. All rights reserved.
//

#include "ast.hpp"
#include <llvm/ADT/APFloat.h>
#include <llvm/IR/Constants.h>
#include <llvm/Support/Casting.h>

namespace type {
#pragma mark - Type
  Type::Kind Type::kind() const {
    return _kind;
  }
#pragma mark - Error
  const std::string Error::message() const {
    return _message;
  }
  void Error::write(std::ostringstream *stream) const {
    *stream << "Type Error: " << message();
  }
  const Type *Error::evaluate() const {
    return this;
  }
#pragma mark - Integer
  void Integer::write(std::ostringstream *stream) const {
    *stream << "Integer";
  }
  const Type *Integer::evaluate() const {
    return new Integer();
  }
#pragma mark - Lambda
  const Type *Lambda::operand() const {
    return _operand;
  }
  const Type *Lambda::result() const {
    return _result;
  }
  void Lambda::write(std::ostringstream *stream) const {
    *stream << "(";
    operand()->write(stream);
    *stream << " -> ";
    result()->write(stream);
    *stream << ")";
  }
  const Type *Lambda::evaluate() const {
    return this;
  }
#pragma mark - Application
  const Type *Application::operation() const {
    return _operation;
  }
  const Type *Application::operand() const {
    return _operand;
  }
  void Application::write(std::ostringstream *stream) const {
    *stream << "(";
    operation()->write(stream);
    *stream << " <- ";
    operand()->write(stream);
    *stream << ")";
  }
  const Type *Application::evaluate() const {
    if (llvm::isa<Lambda>(operation())) {
      return llvm::cast<Lambda>(operation())->result(); // FIXME: do real type checking
    } else {
      return new const Error("Invalid Application");
    }
  }
}

namespace ast {
#pragma mark - Debugging
  static llvm::Value *DebugConstant = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(1337.0));
#pragma mark - Expression
  Expression::Kind Expression::kind() const {
    return _kind;
  }
#pragma mark - Variable
  void Variable::write(std::ostringstream *stream) const {
    *stream << name;
  }
  llvm::Value *Variable::generateCode(const llvm::LLVMContext *context) const {
    return DebugConstant;
  }
#pragma mark - Integer
  void Integer::write(std::ostringstream *stream) const {
    *stream << value;
  }
  llvm::Value *Integer::generateCode(const llvm::LLVMContext *context) const {
    return DebugConstant;
  }
#pragma mark - Lambda
  void Lambda::write(std::ostringstream *stream) const {
    *stream << "(fun " << argument << " -> ";
    body->write(stream);
    *stream << ")";
  }
  llvm::Value *Lambda::generateCode(const llvm::LLVMContext *context) const {
    return DebugConstant;
  }
#pragma mark - Application
  void Application::write(std::ostringstream *stream) const {
    *stream << "(";
    lambda->write(stream);
    *stream << " ";
    argument->write(stream);
    *stream << ")";
  }
  llvm::Value *Application::generateCode(const llvm::LLVMContext *context) const {
    return DebugConstant;
  }
#pragma mark - Let
  void Let::write(std::ostringstream *stream) const {
    *stream << "let " << name << " = ";
    value->write(stream);
    *stream << "; ";
    continuation->write(stream);
  }
  llvm::Value *Let::generateCode(const llvm::LLVMContext *context) const {
    return DebugConstant;
  }
}

