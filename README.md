"The Piece of Code that Understandeth All Parsing"
==================================================
 
Precedence rule for understanding C declarations
("Syntax only a compiler could love")

Start by finding the name of the thing being declared (the declarator)
Then the precedence is:
  1. parenthesis that group things to the declarator
  2. postfix operators for functions "()" and arrays "[]"
  3. prefix operator for pointer-to "*"
 
Also, if a const/volatile type-specifier is beside a type, it applies to that
type; otherwise the const/volatile specifier applies to the asterisk to its immediate left.

