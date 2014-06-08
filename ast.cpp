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

namespace ast {
#pragma mark - Encoding Constants
    static llvm::Module *TheModule;
    static llvm::IRBuilder<> Builder(llvm::getGlobalContext());
#pragma mark - Debugging
    static llvm::Value *DebugConstant = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(1337.0));
#pragma mark - Variable
    void Variable::Write(std::ostringstream *stream) const {
	*stream << name;
    }
    llvm::Value *Variable::Codegen() const {
	return DebugConstant;
    }
#pragma mark - Integer
    void Integer::Write(std::ostringstream *stream) const {
	*stream << value;
    }
    llvm::Value *Integer::Codegen() const {
	return DebugConstant;
    }
#pragma mark - Lambda
    void Lambda::Write(std::ostringstream *stream) const {
	*stream << "(fun " << argument << " -> ";
	body->Write(stream);
	*stream << ")";
    }
    llvm::Value *Lambda::Codegen() const {
	return DebugConstant;
    }
#pragma mark - Application
    void Application::Write(std::ostringstream *stream) const {
	*stream << "(";
	lambda->Write(stream);
	*stream << " ";
	argument->Write(stream);
	*stream << ")";
    }
    llvm::Value *Application::Codegen() const {
	return DebugConstant;
    }
#pragma mark - Let
    void Let::Write(std::ostringstream *stream) const {
	*stream << "let " << name << " = ";
	value->Write(stream);
	*stream << "; ";
	continuation->Write(stream);
    }
    llvm::Value *Let::Codegen() const {
	return DebugConstant;
    }
}

