%{
#include <string.h>
#include "ast.hpp"
#include "parser.tab.hpp"
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)
    extern "C" int yywrap() { return 1; }
%}

%%

[ \t\n] ;
"fun" return TOKEN(TFUN);
"->" return TOKEN(TARROW);
"let" return TOKEN(TLET);
"=" return TOKEN(TEQUALS);
";" return TOKEN(TSEMICOLON);
"(" return TOKEN(TLPAREN);
")" return TOKEN(TRPAREN);
[0-9]+ SAVE_TOKEN; return TINTEGER;
[a-zA-Z_][a-zA-Z0-9_]* SAVE_TOKEN; return TIDENTIFIER;
. printf("invalid token\n"); yyterminate();

%%
