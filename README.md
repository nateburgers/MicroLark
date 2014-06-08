# Micro Lark
An itty bitty compiler to teach myself LLVM.
Basically just the enriched lambda calculus.

## Language

   <expression> ::= <let> | <lambda> | <application> | <variable> | <integer>
   <let> ::= let <variable> = <expression> ; <expression>
   <lambda> ::= fun <variable> -> <expression>
   <application> ::= <expression> <expression>
   <variable> ::= [a-zA-Z][a-zA-Z0-9_]*
   <integer> ::= [0-9]+