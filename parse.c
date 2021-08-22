#include "9cc.h"


bool consume(char *op) {
    if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len)) {
        return false;
    }
    token = token->next;
    return true;
}


void expect(char *op) {
    if (token == NULL) {
        error("Cannot access NULL");
    } else if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len)) {
        error_at(token->pos, "Not '%c'", op);
    }
    token = token->next;
}

int expect_number() {
    if (token->kind != TK_NUM) {
        error_at(token->pos, "Not number");
    }
    int val = token->val;
    token = token->next;
    return val;
}


Token *consume_kind(TokenKind kind) {
    if (token->kind != kind) {
        return NULL;
    }
    Token *tok = token;
    token = token->next;
    return tok;
}


LVar *find_ident(Token *tok) {
    for (LVar *var = locals; var; var = var->next) {
        if (var->len == tok->len && !memcmp(tok->str, var->name, var->len)) {
            return var;
        }
    }
    return NULL;
}


bool at_eof() {
    return token->kind == TK_EOF;
}


Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}


Node *expr();
Node *assign();

Node *primary() {
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }

    Token *tok = consume_kind(TK_IDENT);
    if (tok) {
        Node *node = calloc(1, sizeof(Node));
        LVar *ident = find_ident(tok);
        if (ident) {
            node->kind = ND_LVAR;
            node->offset = ident->offset;
        } else {
            if (consume("(")) {
                node->kind = ND_CALL;
                node->name = tok->str;
                node->args = new_vec();
                int len=0;
                while (!consume(")")) {
                    vec_push(node->args, assign());
                    consume(",");
                    ++len;
                }
            } else {
                error_at(token->pos, "Undefined variable");
            }
        }
        return node;
    }

    return new_node_num(expect_number());
}


Node *unary() {
    if (consume("+")) {
        return primary();
    } else if (consume("-")) {
        return new_node(ND_SUB, new_node_num(0), unary());
    } else if (consume("&")) {
        return new_node(ND_ADDR, unary(), NULL);
    } else if (consume("*")) {
        return new_node(ND_DEREF, unary(), NULL);
    }
    return primary();
}


Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume("*")) {
            node = new_node(ND_MUL, node, unary());
        } else if (consume("/")) {
            node = new_node(ND_DIV, node, unary());
        } else {
            return node;
        }
    }
}


Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume("+")) {
            node = new_node(ND_ADD, node, mul());
        } else if (consume("-")) {
            node = new_node(ND_SUB, node, mul());
        } else {
            return node;
        }
    }
}


Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume("<")) {
            node = new_node(ND_LT, node, add());
        } else if (consume("<=")) {
            node = new_node(ND_LE, node, add());
        } else if (consume(">")) {
            node = new_node(ND_LT, add(), node);
        } else if (consume(">=")) {
            node = new_node(ND_LE, add(), node);
        } else {
            return node;
        }
    }
}


Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume("==")) {
            node = new_node(ND_EQ, node, relational());
        } else if (consume("!=")) {
            node = new_node(ND_NE, node, relational());
        } else {
            return node;
        }
    }
}


Node *assign() {
    Node *node = equality();
    if (consume("=")) {
        node = new_node(ND_ASSIGN, node, assign());
    }
    return node;
}


Node *expr() {
    if (consume_kind(TK_INT)) {
        Token *tok = consume_kind(TK_IDENT);
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;
        LVar *ident = calloc(1, sizeof(LVar));
        ident->next = locals;
        ident->name = tok->str;
        ident->len = tok->len;
        ident->offset = locals->offset + 8;
        node->offset = ident->offset;
        locals = ident;
        return node;
    }
    return assign();
}


Vector *new_stmt_vec() {
    Vector *stmts = new_vec();
    if (consume("{")) {
        while (!consume("}")) {
            vec_push(stmts, stmt());
        }
    } else {
        vec_push(stmts, stmt());
    }
    return stmts;
}

Node *stmt() {
    Node *node;
    if (consume_kind(TK_RETURN))  {
        node = new_node(ND_RETURN, expr(), NULL);
        expect(";");
    } else if (consume_kind(TK_IF)) {
        node = calloc(1, sizeof(Node));
        node->kind = ND_IF;
        expect("(");
        node->cond = expr();
        expect(")");
        node->stmts = new_stmt_vec();
        if (consume_kind(TK_ELSE)) {
            node->elsstmts = new_stmt_vec();
        }
    } else if (consume_kind(TK_WHILE)) {
        node = calloc(1, sizeof(Node));
        node->kind = ND_WHILE;
        expect("(");
        node->cond = expr();
        expect(")");
        node->stmts = new_stmt_vec();
    } else if (consume_kind(TK_FOR)) {
        node = calloc(1, sizeof(Node));
        node->kind = ND_FOR;
        expect("(");
        node->init = stmt();
        node->cond = stmt();
        node->routine = expr();
        expect(")");
        node->stmts = new_stmt_vec();
    } else {
        node = expr();
        expect(";");
    }


    return node;
}


Node *top_level() {
    locals = calloc(1, sizeof(LVar));
    locals->offset = 0;
    if (consume_kind(TK_INT)) {
        Token *tok = consume_kind(TK_IDENT);
        expect("(");
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_FUNC;
        node->name = tok->str;
        node->args = new_vec();
        while (!consume(")")) {
            vec_push(node->args, expr());
            consume(",");
        }
        node->stmts = new_stmt_vec();
        return node;
    } else {
        error_at(token->pos, "Data definition has no type");
    }


}


void *program() {
    int i = 0;
    while (!at_eof()) {
        code[i++] = top_level();
#ifdef DEBUG
        printf("in program(): i=%d\n", i);
        printf("in program(): %d\n", code[0]->kind == ND_NUM);
#endif
    }
    code[i++] = NULL;
}

