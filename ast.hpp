//
//  ast.h
//  XcodeLLVM
//
//  Created by Nathan Burgers on 6/7/14.
//  Copyright (c) 2014 Refulgent Software. All rights reserved.
//

#ifndef _AST_H_
#define _AST_H_

#include <sstream>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace type {
#pragma mark - Type
  class Type {
  public:
    enum Kind {
      TypeKindError,
      TypeKindInteger,
      TypeKindLambda,
      TypeKindApplication
    };
  private:
    const Kind _kind;
  public:
    const Type(const Kind kind)
    : _kind(kind) {}
    Kind kind() const;
    virtual void write(std::ostringstream *stream) const = 0;
    virtual const Type *evaluate() const = 0;
    virtual bool operator==(const Type *type) const = 0;
  };
#pragma mark - Error
  class Error: public Type {
    const std::string _message;
  public:
    const Error(const std::string message) 
    : Type(TypeKindError), _message(message) {}
    static bool classof(const Type *type) {
      return type->kind() == TypeKindError;
    };
    const std::string message() const;
    virtual void write(std::ostringstream *stream) const;
    virtual const Type *evaluate() const;
    virtual bool operator==(const Type *type) const;
  };
#pragma mark - Integer
  class Integer: public Type {
  public:
    const Integer() 
    : Type(TypeKindInteger) {}
    static bool classof(const Type *type) {
      return type->kind() == TypeKindInteger;
    };
    virtual void write(std::ostringstream *stream) const;
    virtual const Type *evaluate() const;
    virtual bool operator==(const Type *type) const;
  };
#pragma mark - Lambda
  class Lambda: public Type {
    const Type *_operand;
    const Type *_result;
  public:
    const Lambda(const Type *operand, const Type *result)
    : Type(TypeKindLambda), _operand(operand), _result(result) {}
    static bool classof(const Type *type) {
      return type->kind() == TypeKindLambda;
    };
    const Type *operand() const;
    const Type *result() const;
    virtual void write(std::ostringstream *stream) const;
    virtual const Type *evaluate() const;
    virtual bool operator==(const Type *type) const;
  };
#pragma mark - Application
  class Application: public Type {
    const Type *_operation;
    const Type *_operand;
  public:
    const Application(const Type *operation, const Type *operand)
    : Type(TypeKindApplication), _operation(operation), _operand(operand) {}
    static bool classof(const Type *type) {
      return type->kind() == TypeKindApplication;
    };
    const Type *operation() const;
    const Type *operand() const;
    virtual void write(std::ostringstream *stream) const;
    virtual const Type *evaluate() const;
    virtual bool operator==(const Type *type) const;
  };
}

namespace ast {
#pragma mark - Expression
  class Expression {
  public:
    enum Kind {
      ExpressionKindVariable,
      ExpressionKindInteger,
      ExpressionKindLambda,
      ExpressionKindApplication,
      ExpressionKindLet
    };
  private:
    const Kind _kind;
  public:
    const Expression(const Kind kind)
    : _kind(kind) {}
    Kind kind() const;
    virtual void write(std::ostringstream *stream) const = 0;
    virtual llvm::Value *generateCode(const llvm::LLVMContext *context) const = 0;
  };
#pragma mark - Variable
  class Variable: public Expression {
    const std::string name;
  public:
    const Variable(const std::string name)
    : Expression(ExpressionKindVariable), name(name) {}
    static bool classof(const Expression *expression) {
      return expression->kind() == Expression::ExpressionKindVariable;
    };
    virtual void write(std::ostringstream *stream) const;
    virtual llvm::Value *generateCode(const llvm::LLVMContext *context) const;
  };
#pragma mark - Integer
  class Integer: public Expression {
    const int value;
  public:
    const Integer(const int value)
    : Expression(ExpressionKindInteger), value(value) {}
    static bool classof(const Expression *expression) {
      return expression->kind() == Expression::ExpressionKindInteger;
    };
    virtual void write(std::ostringstream *stream) const;
    virtual llvm::Value *generateCode(const llvm::LLVMContext *context) const;
  };
#pragma mark - Lambda
  class Lambda: public Expression {
    const std::string argument;
    const Expression *body;
  public:
    const Lambda(const std::string argument, const Expression *body)
    : Expression(ExpressionKindLambda), argument(argument), body(body) {}
    static bool classof(const Expression *expression) {
      return expression->kind() == Expression::ExpressionKindLambda;
    };
    virtual void write(std::ostringstream *stream) const;
    virtual llvm::Value *generateCode(const llvm::LLVMContext *context) const;
  };
#pragma mark - Application
  class Application: public Expression {
    const Expression *lambda;
    const Expression *argument;
  public:
    const Application(const Expression *lambda, const Expression *argument)
    : Expression(ExpressionKindApplication), lambda(lambda), argument(argument) {}
    static bool classof(const Expression *expression) {
      return expression->kind() == Expression::ExpressionKindApplication;
    };
    virtual void write(std::ostringstream *stream) const;
    virtual llvm::Value *generateCode(const llvm::LLVMContext *context) const;
  };
#pragma mark - Let
  class Let: public Expression {
    const std::string name;
    const Expression *value;
    const Expression *continuation;
  public:
    const Let(const std::string name, const Expression *value, const Expression *continuation)
    : Expression(ExpressionKindLet), name(name), value(value), continuation(continuation) {}
    static bool classof(const Expression *expression) {
      return expression->kind() == Expression::ExpressionKindLet;
    };
    virtual void write(std::ostringstream *stream) const;
    virtual llvm::Value *generateCode(const llvm::LLVMContext *context) const;
  };
}

#endif /* _AST_H_ */
