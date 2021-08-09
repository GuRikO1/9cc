#include<ctype.h>
#include<stdarg.h>
#include<stdbool.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>

// #define DEBUG

typedef struct {
  void **data;
  int capacity;
  int len;
} Vector;

typedef struct {
  Vector *keys;
  Vector *vals;
} Map;

typedef enum {
    TK_RESERVED,
    TK_IDENT,
    TK_NUM,
    TK_RETURN,
    TK_EOF,
    TK_IF,
    TK_ELSE,
    TK_WHILE,
    TK_FOR,
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
    ND_IF,
    ND_WHILE,
    ND_FOR,
    ND_BLOCK,
    ND_RETURN,
} NodeKind;


typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
    int offset;
    Node *init;
    Node *cond;
    Node *routine;
    Vector *stmts;
    Vector *elsstmts;
};


typedef struct LVar LVar;

struct LVar {
    LVar *next;
    char *name;
    int len;
    int offset;
};



char *user_input;
Token *token;
Node *code[100];
int serial_num;
LVar *locals;

void error_at(char *loc, char *fmt, ...);
void error(char *fmt, ...);

Vector *new_vec();
void vec_push(Vector *v, void *elem);
void vec_push_val(Vector *v, int val);

Map *new_map();
void map_put(Map *map, char *key, int val);

Token *tokenize(char *p);

Node *stmt();
void *program();

void gen(Node *node);
