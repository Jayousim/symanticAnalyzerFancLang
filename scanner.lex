%{

/* Declarations section */
#include <stdio.h>
#include "attributes.hpp"
#include "parser.tab.hpp"

%}
 
%option yylineno
%option noyywrap

digit   		([0-9])
letter  		([a-zA-Z])
whitespace		([\t\n ])
id [a-zA-Z][a-zA-Z0-9]*
%%
yylineno=1;
"void" {yylval = new Type(typeVoid) ; return VOID ;}
"int"  {yylval = new Type(typeInt) ; return INT ; }
"byte" {yylval = new Type(typeByte) ; return BYTE ; }
"b" {yylval = new Type(typeByte) ; return B ;}
"bool" {yylval = new Type(typeBool) ;   return BOOL; } 
"and" return AND ;
"or" return OR ;
"not" return NOT ;
"true" return TRUE ;
"false"  return FALSE ;
"return" return RETURN ;
"if" return IF ;
"else" return ELSE ;
"while"  return WHILE ;
"break" return BREAK ;
"continue" return CONTINUE ;
";" return SC ;
"," return COMMA ;
"(" return LPAREN ;
")" return RPAREN ;
"{" return  LBRACE ;
"}" return  RBRACE ;
"=" return ASSIGN ;
"=="|"!=" return RELOP1 ;
">"|"<"|">="|"<=" return  RELOP2 ;
"+" return OPLUS ;
"-" return OMINUS ; 
"*" return OMULT ; 
"/" return ODIV ;
{id} { yylval = new Id(yytext);return ID  ;};
\"([^\n\r\"\\]|\\[rnt"\\])+\" {yylval = new StringExp(yytext) ;  return STRING ;  };
[0]|[1-9][0-9]* {yylval = new NumberExp(yytext) ; return NUM ;};
"//"[^\r\n]*[\r|\n|\r\n]? ;
[\r|\n|\r\n] { };
{whitespace}  ; 
. {output::errorLex(yylineno);exit(0) ; };
%%



