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
#include <llvm/ADT/ImmutableMap.h>
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
    const Expression *replace(const std::string variable, const Expression *expression) const;
    const Expression *evaluate() const;
    virtual void write(std::ostringstream *stream) const = 0;
    virtual llvm::Value *generateCode(const llvm::LLVMContext *context) const = 0;
  };
#pragma mark - Variable
  class Variable: public Expression {
    const std::string _name;
  public:
    const Variable(const std::string name)
    : Expression(ExpressionKindVariable), _name(name) {}
    static bool classof(const Expression *expression) {
      return expression->kind() == Expression::ExpressionKindVariable;
    };
    const std::string name() const;
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
    const std::string _argument;
    const Expression *_body;
  public:
    const Lambda(const std::string argument, const Expression *body)
    : Expression(ExpressionKindLambda), _argument(argument), _body(body) {}
    static bool classof(const Expression *expression) {
      return expression->kind() == Expression::ExpressionKindLambda;
    };
    const std::string argument() const;
    const Expression *body() const;
    virtual void write(std::ostringstream *stream) const;
    virtual llvm::Value *generateCode(const llvm::LLVMContext *context) const;
  };
#pragma mark - Application
  class Application: public Expression {
    const Expression *_lambda;
    const Expression *_argument;
  public:
    const Application(const Expression *lambda, const Expression *argument)
    : Expression(ExpressionKindApplication), _lambda(lambda), _argument(argument) {}
    static bool classof(const Expression *expression) {
      return expression->kind() == Expression::ExpressionKindApplication;
    };
    const Expression *lambda() const;
    const Expression *argument() const;
    virtual void write(std::ostringstream *stream) const;
    virtual llvm::Value *generateCode(const llvm::LLVMContext *context) const;
  };
#pragma mark - Let
  class Let: public Expression {
    const std::string _name;
    const Expression *_value;
    const Expression *_continuation;
  public:
    const Let(const std::string name, const Expression *value, const Expression *continuation)
    : Expression(ExpressionKindLet), _name(name), _value(value), _continuation(continuation) {}
    static bool classof(const Expression *expression) {
      return expression->kind() == Expression::ExpressionKindLet;
    };
    const std::string name() const;
    const Expression *value() const;
    const Expression *continuation() const;
    virtual void write(std::ostringstream *stream) const;
    virtual llvm::Value *generateCode(const llvm::LLVMContext *context) const;
  };
}

#pragma mark - Context
using namespace ast;
class Context {
  typedef std::pair<const Expression *, const type::Type *> ValueAndType;
  typedef llvm::ImmutableMap<const Expression *, const ValueAndType *> Bindings;
  const Bindings *_context;
public:
  const Context(const Bindings *context)
  : _context(context) {}
  const Context();
  const Context(const Expression *expression);
  const Context(const Context *context, const Expression *expression);
  const type::Type *type(const Expression *expression) const;
  const Expression *value(const Expression *expression) const;
};

#endif /* _AST_H_ */
