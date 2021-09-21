%option noyywrap

%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "AST.h"
#include "parser.h"
#include "errorHandle.h"

#define YY_INPUT(buf,result,max_size)  {\
    result = getNextChar(buf, max_size); \
    if (  result <= 0  ) \
      result = YY_NULL; \
    }

#define TOKENLENGTH strlen(yytext)

extern int yylineno;
%}

DIGIT [0-9]
LETTER [a-zA-Z]
ID {LETTER}({LETTER}|{DIGIT})*
DIGITS {DIGIT}+
FRACTION "."{DIGITS}
EXPONENT [Ee][+-]?{DIGITS}
INTNUM {DIGITS}
REALNUM {DIGITS}({FRACTION})?({EXPONENT})?
WRAP [\n]
DELIM [" "\t\f\v]
WS {DELIM}+
OTHERS .
%%

"+"         { beginToken(TOKENLENGTH); return PLUS; }
"-"         { beginToken(TOKENLENGTH); return MINUS; }
"*"         { beginToken(TOKENLENGTH); return MULTIPLY; }
"/"         { beginToken(TOKENLENGTH); return DIVIDE; }
"="         { beginToken(TOKENLENGTH); return EQ; }
"("         { beginToken(TOKENLENGTH); return OP; }
")"         { beginToken(TOKENLENGTH); return CP; }
"["         { beginToken(TOKENLENGTH); return OB; }
"]"         { beginToken(TOKENLENGTH); return CB; }
"."         { beginToken(TOKENLENGTH); return DOT; }
","         { beginToken(TOKENLENGTH); return COMMA; }
":"         { beginToken(TOKENLENGTH); return COLON; }
";"         { beginToken(TOKENLENGTH); return SEMI; }

"<>"        { beginToken(TOKENLENGTH); return NE; }
">="        { beginToken(TOKENLENGTH); return GE; }
">"         { beginToken(TOKENLENGTH); return GT; }
"<="        { beginToken(TOKENLENGTH); return LE; }
"<"         { beginToken(TOKENLENGTH); return LT; }
":="        { beginToken(TOKENLENGTH); return AS; }

begin       { beginToken(TOKENLENGTH); return BEGINN; }
do          { beginToken(TOKENLENGTH); return DO; }
downto      { beginToken(TOKENLENGTH); return DOWNTO; }
repeat      { beginToken(TOKENLENGTH); return REPEAT; }
end         { beginToken(TOKENLENGTH); return END; }
then        { beginToken(TOKENLENGTH); return THEN; }
to          { beginToken(TOKENLENGTH); return TO; }
for         { beginToken(TOKENLENGTH); return FOR; }
until       { beginToken(TOKENLENGTH); return UNTIL; }
while       { beginToken(TOKENLENGTH); return WHILE; }
function    { beginToken(TOKENLENGTH); return FUNCTION; }
procedure   { beginToken(TOKENLENGTH); return PROCEDURE; }
program     { beginToken(TOKENLENGTH); return PROGRAM; }
var         { beginToken(TOKENLENGTH); return VAR; }
record      { beginToken(TOKENLENGTH); return RECORD; }
type        { beginToken(TOKENLENGTH); return TYPE; }
array       { beginToken(TOKENLENGTH); return ARRAY; }
const       { beginToken(TOKENLENGTH); return CONST; }
if          { beginToken(TOKENLENGTH); return IF; }
else        { beginToken(TOKENLENGTH); return ELSE; }
case        { beginToken(TOKENLENGTH); return CASE; }
of          { beginToken(TOKENLENGTH); return OF; }
and         { beginToken(TOKENLENGTH); return AND; }
or          { beginToken(TOKENLENGTH); return OR; }
not         { beginToken(TOKENLENGTH); return NOT; }
boolean     { beginToken(TOKENLENGTH); return BOOLEAN; }
div         { beginToken(TOKENLENGTH); return DIV; }
mod         { beginToken(TOKENLENGTH); return MOD; }
integer     { beginToken(TOKENLENGTH); return INTEGER; }
real        { beginToken(TOKENLENGTH); return REAL; }
char        { beginToken(TOKENLENGTH); return CHAR; }
false       { beginToken(TOKENLENGTH); return FALSE; }
true        { beginToken(TOKENLENGTH); return TRUE; }

"'"{OTHERS}"'"    { beginToken(TOKENLENGTH); yylval.c = yytext[1]; return LETTER; }
{ID}        { beginToken(TOKENLENGTH); yylval.s = strdup(yytext); return ID; }
{INTNUM}    { beginToken(TOKENLENGTH); yylval.t = atoi(yytext); return INTNUM; }
{REALNUM}   { beginToken(TOKENLENGTH); yylval.f = atof(yytext); return REALNUM; }
{WRAP}      { beginToken(TOKENLENGTH); }
{DELIM}     { beginToken(TOKENLENGTH); }
{WS}        { beginToken(TOKENLENGTH); }
{OTHERS}    { beginToken(TOKENLENGTH); parseError = 1; printError("Invalid Input"); }

%%
