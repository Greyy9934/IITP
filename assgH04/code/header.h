#ifndef HEADER_H
#define HEADER_H

typedef enum {
    TYPE_BLOCK,
    TYPE_DECISION,
    TYPE_UNCOND
} NodeType;

typedef struct Node {
    NodeType type;
    char *id;
    char *c_code;         // For BLOCK
    char *cond;           // For DECISION
    char *true_goto;      // For DECISION
    char *false_goto;     // For DECISION
    char *uncond_goto;    // For UNCONDDEC
    struct Node *next;
} Node;

void add_block(char *id, char *c_code);
void add_decision(char *id, char *cond, char *true_goto, char *false_goto);
void add_uncond(char *id, char *goto_id);
void generate_c_code();

#endif