%{
	#include <string>
	#include "ast.hpp"
	const ast::Expression *program;

	extern int yylex();
	void yyerror(const char *s) { 
	  printf("Error: %s\n", s);
	}
%}

%union {
  const ast::Expression *expression;
  const ast::Variable *variable;
  const ast::Integer *integer;
  const ast::Lambda *lambda;
  const ast::Application *application;
  const ast::Let *let;
  std::string *string;
  int token;
}

%token <string> TIDENTIFIER TINTEGER
%token <token> TFUN TARROW TLET TEQUALS TSEMICOLON TLPAREN TRPAREN

%type <expression> expression
%type <variable> variable
%type <integer> integer
%type <lambda> lambda
%type <application> application
%type <let> let

%start program

%%

program : expression { program = $1; }
	;

expression : lambda				{ $$ = $1; }
	   | let		       	   	{ $$ = $1; }
	   | application	       	   	{ $$ = $1; }
	   | variable				{ $$ = $1; }
	   | integer				{ $$ = $1; }
	   | TLPAREN expression TRPAREN    	{ $$ = $2; }
	   ;

variable : TIDENTIFIER { $$ = new const ast::Variable(*$1); } ;

integer : TINTEGER { $$ = new const ast::Integer(std::stoi(*$1)); } ;

lambda : TFUN TIDENTIFIER TARROW expression { $$ = new const ast::Lambda(*$2, $4); } ;

application : expression expression { $$ = new const ast::Application($1, $2); } ;

let : TLET TIDENTIFIER TEQUALS expression TSEMICOLON expression { $$ = new const ast::Let(*$2,$4,$6); } ;

%%
