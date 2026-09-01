#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

Node *head = NULL;
Node *tail = NULL;

extern int yyparse();
extern FILE *yyin;

void add_node(Node *n) {
    if (!head) {
        head = tail = n;
    } else {
        tail->next = n;
        tail = n;
    }
}

void add_block(char *id, char *c_code) {
    Node *n = (Node *)malloc(sizeof(Node));
    n->type = TYPE_BLOCK;
    n->id = strdup(id);
    n->c_code = strdup(c_code);
    n->next = NULL;
    add_node(n);
}

void add_decision(char *id, char *cond, char *true_goto, char *false_goto) {
    Node *n = (Node *)malloc(sizeof(Node));
    n->type = TYPE_DECISION;
    n->id = strdup(id);
    n->cond = strdup(cond);
    n->true_goto = strdup(true_goto);
    n->false_goto = false_goto ? strdup(false_goto) : NULL;
    n->next = NULL;
    add_node(n);
}

void add_uncond(char *id, char *goto_id) {
    Node *n = (Node *)malloc(sizeof(Node));
    n->type = TYPE_UNCOND;
    n->id = strdup(id);
    n->uncond_goto = strdup(goto_id);
    n->next = NULL;
    add_node(n);
}

int check_id_exists(char *id) {
    Node *curr = head;
    while(curr) {
        if(strcmp(curr->id, id) == 0) return 1;
        curr = curr->next;
    }
    return 0;
}

void validate_gotos() {
    Node *curr = head;
    while(curr) {
        if (curr->type == TYPE_DECISION) {
            if (!check_id_exists(curr->true_goto)) {
                fprintf(stderr, "Error: Target %s not defined in process scope.\n", curr->true_goto);
                exit(1);
            }
            if (curr->false_goto && !check_id_exists(curr->false_goto)) {
                fprintf(stderr, "Error: Target %s not defined in process scope.\n", curr->false_goto);
                exit(1);
            }
        } else if (curr->type == TYPE_UNCOND) {
            if (!check_id_exists(curr->uncond_goto)) {
                fprintf(stderr, "Error: Target %s not defined in process scope.\n", curr->uncond_goto);
                exit(1);
            }
        }
        curr = curr->next;
    }
}

void generate_c_code() {
    validate_gotos();

    printf("#include <stdio.h>\n");
    printf("#include <stdlib.h>\n\n");
    printf("#define GOTO goto\n\n");

    printf("int main() {\n");
    Node *curr = head;
    while(curr) {
        printf("%s:\n", curr->id);
        if (curr->type == TYPE_BLOCK) {
            printf("%s\n", curr->c_code);
        } else if (curr->type == TYPE_DECISION) {
            printf("if (%s) {\n    goto %s;\n}\n", curr->cond, curr->true_goto);
            if (curr->false_goto) {
                printf("else {\n    goto %s;\n}\n", curr->false_goto);
            }
        } else if (curr->type == TYPE_UNCOND) {
            printf("goto %s;\n", curr->uncond_goto);
        }
        curr = curr->next;
    }
    printf("return 0;\n}\n");
}

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            perror("Error opening input file");
            return 1;
        }
    }
    yyparse();
    generate_c_code();
    return 0;
}