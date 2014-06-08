//
//  ast.h
//  XcodeLLVM
//
//  Created by Nathan Burgers on 6/7/14.
//  Copyright (c) 2014 Refulgent Software. All rights reserved.
//

#ifndef __AST_H
#define __AST_H

#include <sstream>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>

namespace ast {
#pragma mark - Expression
  class Expression {
  public:
    virtual void Write(std::ostringstream *stream) const = 0;
    virtual llvm::Value *Codegen() const = 0;
  };
#pragma mark - Variable
  class Variable: public Expression {
    const std::string name;
  public:
    const Variable(const std::string name)
    : name(name) {}
    virtual void Write(std::ostringstream *stream) const;
    virtual llvm::Value *Codegen() const;
  };
#pragma mark - Integer
    class Integer: public Expression {
	const int value;
    public:
	const Integer(const int value)
	: value(value) {}
	virtual void Write(std::ostringstream *stream) const;
	virtual llvm::Value *Codegen() const;
    };
#pragma mark - Lambda
    class Lambda: public Expression {
	const std::string argument;
	const Expression *body;
  public:
    const Lambda(const std::string argument, const Expression *body)
    : argument(argument), body(body) {}
    virtual void Write(std::ostringstream *stream) const;
    virtual llvm::Value *Codegen() const;
  };
#pragma mark - Application
  class Application: public Expression {
    const Expression *lambda;
    const Expression *argument;
  public:
    const Application(const Expression *lambda, const Expression *argument)
    : lambda(lambda), argument(argument) {}
    virtual void Write(std::ostringstream *stream) const;
    virtual llvm::Value *Codegen() const;
  };
#pragma mark - Let
  class Let: public Expression {
    const std::string name;
    const Expression *value;
    const Expression *continuation;
  public:
    const Let(const std::string name, const Expression *value, const Expression *continuation)
    : name(name), value(value), continuation(continuation) {}
    virtual void Write(std::ostringstream *stream) const;
    virtual llvm::Value *Codegen() const;
  };
}

#endif
 
