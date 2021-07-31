#include<ctype.h>
#include<stdarg.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// #define DEBUG

typedef enum {
    TK_RESERVED,
    TK_IDENT,
    TK_NUM,
    TK_EOF,
} TokenKind;


typedef struct Token Token;

struct Token {
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    int len;
};


typedef enum {
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_ASSIGN, // =
    ND_EQ, // ==
    ND_NE, // !=
    ND_LE, // <
    ND_LT, // <=
    ND_LVAR, // local variable
    ND_NUM,
} NodeKind;


typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
    int offset;
};


char *user_input;
Token *token;
Node *code[100];
void error_at(char *loc, char *fmt, ...);
void error(char *fmt, ...);
Token *tokenize(char *p);
Node *stmt();
void *program();
void gen(Node *node);
