%{
	/*extern "C"
	{
    int yylex(void);
	}*/

  #include "Values.h"
  #include "instruction.h"
  #include "defines.h"
  
  #include "impossible.tab.hpp"
%}

%option noyywrap

%%

"[#]" { return T_RECUR; }
"{#}" { return T_RECURV; }

"(>" { return T_RISE; }
"<)" { return T_SINK; }

"@>" { return T_RAND; }
"#>" { return T_SORT; }
"$>" { return T_SHUFFLE; }

"<#" { return T_HASH_FRONT; }
">#" { return T_HASH_BACK; }
"<@" { return T_AT_FRONT; }
">@" { return T_AT_BACK; }

"!!" { return T_DBANG; }
"??" { return T_DQUESTION; }
"<>" { return T_WHILE; }

".>." { return T_MAP; }
":>:" { return T_DMAP; }
":>." { return T_FOLD; }
":*." { return T_CARTESIAN; }
".>:" { return T_ITERI; }
".>" { return T_ITER; }

".?" { return T_ANY; }
":?" { return T_EVERY; }

"+." { return T_PLUS_MON; }
"+:" { return T_PLUS_DIA; }
"-." { return T_MINUS_MON; }
"-:" { return T_MINUS_DIA; }
"*." { return T_TIMES_MON; }
"*:" { return T_TIMES_DIA; }
"/." { return T_DIVIDE_MON; }
"/:" { return T_DIVIDE_DIA; }

"//" { return T_DDIVIDE; }

"+" { return T_PLUS; }
"-" { return T_MINUS; }
"/" { return T_DIVIDE; }
"*" { return T_TIMES; }
"_" { return T_UNDERSCORE; }

">>" { return T_RSHIFT; }
"<<" { return T_LSHIFT; }

"<" { return T_LESSER; }
">" { return T_GREATER; }

"&" { return T_AND; }
"|" { return T_OR; }
"~" { return T_NOT; }

"@" { return T_AT; }
"#" { return T_HASH; }

"=" { return T_EQUAL; }
"^" { return T_PEEK; }

"(" { return T_LPAREN; }
")" { return T_RPAREN; }
"{" { return T_LBRACE; }
"}" { return T_RBRACE; }
"[" { return T_LBRACK; }
"]" { return T_RBRACK; }

"!" { return T_BANG; }
"?" { return T_QUESTION; }

"$" { return T_DOLLAR; }
"%" { return T_PERCENT; }
";" { return T_SEMICOL; }
"," { return T_COMMA; }

"." { return T_PERIOD; }
":" { return T_COLON; }

"t" { yylval.v = new Bool(true); return BOOLEAN; }
"f" { yylval.v = new Bool(false); return BOOLEAN; }

"R" { yylval.op = OP_RAISE_STACK; return T_OPCODE; }
"L" { yylval.op = OP_LOWER_STACK; return T_OPCODE; }

"r" { yylval.op = OP_RAISE_STACKV; return T_OPCODE; }
"l" { yylval.op = OP_LOWER_STACKV; return T_OPCODE; }

"s" { return T_S; }

"i" { return T_I; }

"'"[0-9a-zA-Z] { yylval.v = new Char(yytext[1]); return CHARACTER; }

"-"?[0-9]+"..""-"?[0-9]+ {
  int i = 0;
  while (yytext[i] != '.') ++i;
  yytext[i] = '\0';
  
  yylval.v = new Range(RangeVector(atoi(yytext), atoi(&yytext[i+2])));
  return RANGE;
}

"x"[0-9a-fA-F]+ { yylval.v = new Int((int)strtoul(&yytext[1], NULL, 16)); return INTEGER; }
"b"[0-1]+ { yylval.v = new Int((int)strtoul(&yytext[1], NULL, 2)); return INTEGER; }
"-"?[0-9]+ { yylval.v = new Int(atoi(yytext)); return INTEGER;}
"-"?[0-9]+"k" {
  char *tmp = strdup(yytext);
  char *index = strchr(tmp, 'k') - 1;
  *index = NULL;

  free(tmp);
  yylval.v = new Int(atoi(yytext)*1000); return INTEGER;
}

"-"?[0-9]+"kk" {
  char *tmp = strdup(yytext);
  char *index = strstr(tmp, "kk") - 1;
  *index = '\0';
  
  free(tmp);
  yylval.v = new Int(atoi(yytext)*1000000); return INTEGER;
}


"-"?[1-9][0-9]*"e"[1-9][0-9]* {
  char *tmp = strdup(yytext);
  
  char *i1 = strtok(tmp,"e");
  char *i2 = strtok(NULL,"e");

  int exp = atoi(i2);
  int r = 1;
  
  for (int i = 0; i < exp; ++i)
    r *= 10;
  
  r *= atoi(i1);
  
  free(tmp);
  yylval.v = new Int(r); return INTEGER;  
}
"-"?[0-9]*\.[0-9]+"f"? { yylval.v = new Float(atof(yytext)); return FLOAT; }

\"(\\.|[^\\"])*\" { yytext[strlen(yytext)-1] = '\0'; yylval.v = new String(&yytext[1]); return STRING_LITERAL; }

[a-zA-Z][a-zA-Z0-9]* { /*yylval.str = strdup(yytext);*/ return STRING; }



\n|"\r\n" { return T_EOL; }
[ \t] { /* ignore whitespace */ }
. { printf("Mystery character %c\n", *yytext); }

%%

