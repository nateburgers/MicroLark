//
//  main.cpp
//  LearnLLVM
//
//  Created by Nathan Burgers on 6/7/14.
//  Copyright (c) 2014 RefulgentSoftware. All rights reserved.
//


#include "ast.hpp"
#include <iostream>

using namespace ast;

extern const Expression *program;
extern int yyparse();

int main(int argc, const char * argv[]) {
    yyparse();
    std::ostringstream oss;
    std::cout << ">>> ";
    program->write(&oss);
    std::cout << oss.str() << "\n";

    std::ostringstream valueStream;
    std::cout << "<<< ";
    program->evaluate()->write(&valueStream);
    std::cout << valueStream.str() << "\ndone.";
    
    // const Expression *expression = new const Let("id", new const Lambda("x", new const Variable("x")),
    //                                              new const Application(new const Variable("id"), new const Variable("y")));
    // expression->Write(&oss);
    // std::cout << oss.str() << "\n";
    return 0;
}
