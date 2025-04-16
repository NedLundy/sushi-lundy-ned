%{
#include <string>
#include "Sushi.hh"
#include "sushi_yyparser.tab.h"
#ifdef __APPLE__
extern int yylex(void);
extern int yyparse(void);
#endif
%}

%option noyywrap
%option nounput

digit         [0-9]
letter        [a-zA-Z_]
alnum         ({letter}|{digit})
string        \"([^\\\"\n]|\\.)*\"
quoted_string \'([^\\\'\n]|\\.)*\'
permitted     [-%@_+\[\]\\/,.:=a-zA-Z0-9]
unsupported   [;()*^?~{}`]

%%

"#".*[\n]?           { /* Comment line */ }

exit                { return YY_SUSHI_EXIT; }
pwd                 { return YY_SUSHI_PWD; }
cd                  { return YY_SUSHI_CD; }
history             { return YY_SUSHI_HISTORY; }

"!"{digit}+         {
                      yylval.i = std::atoi(yytext + 1);
                      return YY_SUSHI_BANG;
                   }

\$({alnum}+|\?)      {
                      yylval.s = Sushi::getenv(yytext + 1);
                      return YY_SUSHI_TOK;
                   }

{quoted_string}     {
                      yytext[strlen(yytext) - 1] = 0;
                      yylval.s = Sushi::unquote_and_dup(yytext + 1);
                      return YY_SUSHI_TOK;
                   }

{string}            {
                      yytext[strlen(yytext) - 1] = 0;
                      yylval.s = Sushi::unquote_and_dup(yytext + 1);
                      return YY_SUSHI_TOK;
                   }

{letter}({alnum})*={permitted}+ {
                      yylval.s = Sushi::unquote_and_dup(yytext);
                      return YY_SUSHI_TOK;
                   }

{permitted}+        {
                      yylval.s = Sushi::unquote_and_dup(yytext);
                      return YY_SUSHI_TOK;
                   }

"<"                 { return YY_SUSHI_LESS; }
">>"                { return YY_SUSHI_MOREMORE; }
">"                 { return YY_SUSHI_MORE; }
"&"                 { return YY_SUSHI_AMP; }
"|"                 { return YY_SUSHI_BAR; }
"="                 { return YY_SUSHI_SET; }

{unsupported}       {
                      std::cerr << "Unsupported command: " << yytext[0] << std::endl;
                      return YY_SUSHI_UNKNOWN;
                   }

[ \t\n\b\v]+        { /* skip whitespace */ }

.                   {
                      std::cerr << "Illegal character: " << yytext[0] << std::endl;
                      return YY_SUSHI_UNKNOWN;
                   }

%%

int Sushi::parse_command(const std::string command) {
  YY_BUFFER_STATE buffer = yy_scan_string(command.c_str());
  int retval = yyparse();
  yy_delete_buffer(buffer);
  yylex_destroy();
  return retval;
}
