%{
#include "y.tab.h"
#include <string.h>
#include <stdlib.h>

char buf[100000];
int buf_len = 0;
int in_block = 0;
%}

%x ST_CCODE
%x ST_COND

%%
<INITIAL>[ \t\n\r]+     { /* ignore whitespace */ }
<INITIAL>"SYSTEM"       { return SYSTEM; }
<INITIAL>"PROCESS"      { return PROCESS; }
<INITIAL>"PROCESSDEF"   { return PROCESSDEF; }
<INITIAL>"BLOCK"        { in_block = 1; return BLOCK; }
<INITIAL>"DECISION"     { return DECISION; }
<INITIAL>"UNCONDDEC"    { return UNCONDDEC; }
<INITIAL>"START"        {
                          if (in_block) {
                              buf_len = 0;
                              buf[0] = '\0';
                              BEGIN ST_CCODE;
                          }
                          return START;
                        }
<INITIAL>"END"          { return END; }
<INITIAL>"IF"           { buf_len = 0; buf[0] = '\0'; BEGIN ST_COND; return IF; }
<INITIAL>"THEN"         { return THEN; }
<INITIAL>"ELSE"         { return ELSE; }
<INITIAL>"GOTO"         { return GOTO; }
<INITIAL>";"            { return SEMICOLON; }
<INITIAL>","            { return COMMA; }
<INITIAL>[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return ID; }

<ST_COND>[ \t\n\r]*"THEN" {
                          int keep = yyleng - 4;
                          for(int i=0; i<keep; i++) buf[buf_len++] = yytext[i];
                          // Trim trailing whitespace/newlines from condition
                          while(buf_len > 0 && (buf[buf_len-1] == ' ' || buf[buf_len-1] == '\t' || buf[buf_len-1] == '\n' || buf[buf_len-1] == '\r')) {
                              buf_len--;
                          }
                          buf[buf_len] = '\0';
                          BEGIN INITIAL;
                          yylval.sval = strdup(buf);
                          yyless(keep);
                          return COND_TEXT;
                        }
<ST_COND>.|\n           { buf[buf_len++] = yytext[0]; buf[buf_len] = '\0'; }

<ST_CCODE>\n[ \t]*"END" {
                          int keep = yyleng - 3;
                          for(int i=0; i<keep; i++) buf[buf_len++] = yytext[i];
                          buf[buf_len] = '\0';
                          BEGIN INITIAL;
                          yylval.sval = strdup(buf);
                          in_block = 0;
                          yyless(keep);
                          return C_CODE;
                        }
<ST_CCODE>.|\n          { buf[buf_len++] = yytext[0]; buf[buf_len] = '\0'; }

%%
int yywrap() {
    return 1;
}