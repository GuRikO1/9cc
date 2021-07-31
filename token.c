#include "9cc.h"


Token *new_token(TokenKind kind, Token *cur, const char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    char *name = calloc(len + 1, sizeof(char));
    strncpy(name, str, 3);
    name[len] = '\0';
    tok->str = name;
// #ifdef DEBUG
//     printf("in new_token(): tok->str=%s\n", tok->str);
//     printf("in new_token(): str=%s\n", str);
// #endif
    tok->len = len;
    cur->next = tok;
    return tok;
}


bool startswith(char *p, char *q) {
    return (memcmp(p, q, strlen(q)) == 0);
}

Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (startswith(p, "==") || startswith(p, "!=") ||
            startswith(p, "<=") || startswith(p, ">=")) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        if (strchr("+-*/()<>=;", *p)) {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        if (isalpha(*p) || *p == '_') {
            int len = 1;
            while(isalpha(p[len]) || *p == '_') {
                len++;
            }
#ifdef DEBUG
            printf("in tokenize(): lval_len = %d\n", len);
#endif
            cur = new_token(TK_IDENT, cur, p, len);
            p += len;
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 0);
            cur->val = strtol(p, &p , 10);
            continue;
        }

        error_at(p, "Cannot tokenize");
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}
