%{
  #include "value.h"
  #include "collection.h"
  #include "instruction.h"
  #include "defines.h"
  
  #include "parser.hpp"
  #include "lexer.h"
  #include "compiler.h"
  
  #define yyterminate() compiler::Parser::make_END(loc);

  using token = compiler::Parser::token;

  static compiler::location loc;
%}

%option nodefault
%option noyywrap
%option c++
%option yyclass="Lexer"
%option prefix="Base"

%{
  # define YY_USER_ACTION  loc.columns((int)yyleng);
%}

%%

%{
  loc.step();
%}

"(?" { return Parser::make_LARRAY_OPEN(loc); }
"{>" { return Parser::make_STACK_OPEN(loc); }

"{#}" { return Parser::make_OPCODE(OP_RECURV, loc); }

"(>" { return Parser::make_OPCODE(OP_RISE, loc); }
"<)" { return Parser::make_OPCODE(OP_SINK, loc); }

"@>" { return Parser::make_OPCODE(OP_RAND, loc); }
"#>" { return Parser::make_OPCODE(OP_SORT, loc); }
"$>" { return Parser::make_OPCODE(OP_SHUFFLE, loc); }

"<#" { return Parser::make_OPCODE(OP_HASH_FRONT, loc); }
">#" { return Parser::make_OPCODE(OP_HASH_BACK, loc); }
"<@" { return Parser::make_OPCODE(OP_AT_FRONT, loc); }
">@" { return Parser::make_OPCODE(OP_AT_BACK, loc); }

"!!" { return Parser::make_OPCODE(OP_DBANG, loc); }
"??" { return Parser::make_OPCODE(OP_DQUESTION, loc); }
"<>" { return Parser::make_OPCODE(OP_WHILE, loc); }

".>." { return Parser::make_OPCODE(OP_MAP, loc); }
":>:" { return Parser::make_OPCODE(OP_DMAP, loc); }
":>." { return Parser::make_OPCODE(OP_FOLD, loc); }
":*." { return Parser::make_OPCODE(OP_CARTESIAN, loc); }
".>:" { return Parser::make_OPCODE(OP_ITERI, loc); }
".>" { return Parser::make_OPCODE(OP_ITER, loc); }

".?" { return Parser::make_OPCODE(OP_ANY, loc); }
":?" { return Parser::make_OPCODE(OP_EVERY, loc); }

"+." { return Parser::make_OPCODE(OP_PLUS_MON, loc); }
"+:" { return Parser::make_OPCODE(OP_PLUS_DIA, loc); }
"-." { return Parser::make_OPCODE(OP_MINUS_MON, loc); }
"-:" { return Parser::make_OPCODE(OP_MINUS_DIA, loc); }
"*." { return Parser::make_OPCODE(OP_TIMES_MON, loc); }
"*:" { return Parser::make_OPCODE(OP_TIMES_DIA, loc); }
"/." { return Parser::make_OPCODE(OP_DIVIDE_MON, loc); }
"/:" { return Parser::make_OPCODE(OP_DIVIDE_DIA, loc); }

"//" { return Parser::make_OPCODE(OP_MOD, loc); }

"+" { return Parser::make_OPCODE(OP_PLUS, loc); }
"-" { return Parser::make_OPCODE(OP_MINUS, loc); }
"/" { return Parser::make_OPCODE(OP_DIVIDE, loc); }
"*" { return Parser::make_OPCODE(OP_TIMES, loc); }
"_" { return Parser::make_OPCODE(OP_NEG, loc); }

">>" { return Parser::make_OPCODE(OP_RSHIFT, loc); }
"<<" { return Parser::make_OPCODE(OP_LSHIFT, loc); }

"<" { return Parser::make_OPCODE(OP_LESSER, loc); }
">" { return Parser::make_OPCODE(OP_GREATER, loc); }

"&" { return Parser::make_OPCODE(OP_AND, loc); }
"|" { return Parser::make_OPCODE(OP_OR, loc); }
"~" { return Parser::make_OPCODE(OP_NOT, loc); }

"@" { return Parser::make_OPCODE(OP_AT, loc); }
"#" { return Parser::make_OPCODE(OP_HASH, loc); }

"=" { return Parser::make_OPCODE(OP_EQUAL, loc); }
"^" { return Parser::make_OPCODE(OP_PEEK, loc); }

"(" { return Parser::make_LPAREN(loc); }
")" { return Parser::make_RPAREN(loc); }
"{" { return Parser::make_LBRACE(loc); }
"}" { return Parser::make_RBRACE(loc); }
"[" { return Parser::make_LBRACK(loc); }
"]" { return Parser::make_RBRACK(loc); }
":" { return Parser::make_COLON(loc); }


"!" { return Parser::make_OPCODE(OP_BANG, loc); }
"?" { return Parser::make_OPCODE(OP_QUESTION, loc); }

"$" { return Parser::make_OPCODE(OP_DUPE, loc); }
"%" { return Parser::make_OPCODE(OP_SWAP, loc); }
";" { return Parser::make_OPCODE(OP_DROP, loc); }
"," { return Parser::make_OPCODE(OP_PICK, loc); }

"." { return Parser::make_PERIOD(loc); }

"t" { return Parser::make_BOOLEAN(true, loc); }
"f" { return Parser::make_BOOLEAN(false, loc); }

"R" { return Parser::make_OPCODE(OP_RAISE_STACK, loc); }
"L" { return Parser::make_OPCODE(OP_LOWER_STACK, loc); }

"r" { return Parser::make_OPCODE(OP_RAISE_STACKV, loc); }
"l" { return Parser::make_OPCODE(OP_LOWER_STACKV, loc); }

"s" { return Parser::make_OPCODE(OP_STRING_CAST, loc); }
"i" { return Parser::make_I(loc); }

"'"[0-9a-zA-Z] { return Parser::make_CHARACTER(yytext[1], loc); }

"-"?[0-9]+"..""-"?[0-9]+ {
  int i = 0;
  while (yytext[i] != '.') ++i;
  yytext[i] = '\0';
  
  integral_t s = atoi(yytext), e = atoi(&yytext[i+2]);
  return Parser::make_RANGE(new Range(RangeVector(s, e)), loc);
}

"x"[0-9a-fA-F]+ { return Parser::make_INTEGER((integral_t)strtoul(&yytext[1], NULL, 16), loc); }
"b"[0-1]+ { return Parser::make_INTEGER((integral_t)strtoul(&yytext[1], NULL, 2), loc); }
"-"?[0-9]+ { return Parser::make_INTEGER((integral_t)atoi(yytext), loc);}
"-"?[0-9]+"k" {
  char *tmp = strdup(yytext);
  char *index = strchr(tmp, 'k') - 1;
  *index = NULL;

  free(tmp);
  return Parser::make_INTEGER((integral_t)atoi(yytext)*1000, loc);
}

"-"?[0-9]+"kk" {
  char *tmp = strdup(yytext);
  char *index = strstr(tmp, "kk") - 1;
  *index = '\0';
  
  free(tmp);
  return Parser::make_INTEGER((integral_t)atoi(yytext)*1000000, loc);
}


"-"?[1-9][0-9]*"e"[1-9][0-9]* {
  char *tmp = strdup(yytext);
  
  char *i1 = strtok(tmp,"e");
  char *i2 = strtok(NULL,"e");

  integral_t exp = atoi(i2);
  integral_t r = 1;
  
  for (int i = 0; i < exp; ++i)
    r *= 10;
  
  r *= atoi(i1);
  
  free(tmp);
  return Parser::make_INTEGER(r, loc);
}
"-"?[0-9]*\.[0-9]+"f"? { return Parser::make_FLOAT(atof(yytext), loc); }

\"(\\.|[^\\"])*\" { yytext[strlen(yytext)-1] = '\0'; return Parser::make_STRING(new String(yytext+1), loc); }

\n|"\r\n" { Parser::make_EOL(loc); loc.lines((int)yyleng); }

[ \t] { /* ignore whitespace */ }

<<EOF>> { return yyterminate(); }

%%

