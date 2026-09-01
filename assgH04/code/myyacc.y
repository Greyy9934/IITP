%{
#include <stdio.h>
#include <stdlib.h>
#include "header.h"

void yyerror(const char *s);
int yylex(void);
%}

%union {
    char *sval;
}

%token SYSTEM PROCESS PROCESSDEF START END BLOCK DECISION UNCONDDEC IF THEN ELSE GOTO SEMICOLON COMMA
%token <sval> ID COND_TEXT C_CODE

%%
program: system ;

system: SYSTEM ID SEMICOLON PROCESS process_list SEMICOLON PROCESSDEF ID START process_body END ;

process_list: ID
            | process_list COMMA ID
            ;

process_body: constructs
            | /* empty */
            ;

constructs: construct
          | constructs construct
          ;

construct: block_def
         | decision_def
         | uncond_def
         ;

block_def: BLOCK ID START C_CODE END {
    add_block($2, $4);
} ;

decision_def: DECISION ID IF COND_TEXT THEN GOTO ID {
    add_decision($2, $4, $7, NULL);
}
| DECISION ID IF COND_TEXT THEN GOTO ID ELSE GOTO ID {
    add_decision($2, $4, $7, $10);
} ;

uncond_def: UNCONDDEC ID GOTO ID {
    add_uncond($2, $4);
} ;

%%
void yyerror(const char *s) {
    fprintf(stderr, "Parsing Error: %s\n", s);
}