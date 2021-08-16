#include "9cc.h"


Token *new_token(TokenKind kind, Token *cur, const char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    char *name = calloc(len + 1, sizeof(char));
    strncpy(name, str, len);
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


bool is_alnum(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '1'));
}

static Map *keyword_map() {
  Map *map = new_map();
  map_put(map, "return", TK_RETURN);
  map_put(map, "if", TK_IF);
  map_put(map, "else", TK_ELSE);
  map_put(map, "while", TK_WHILE);
  map_put(map, "for", TK_FOR);
  return map;
}

Token *tokenize(char *p) {
    Map *keywords = keyword_map();

    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        // printf("s = %s\n", p);
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

        if (strchr("+-*/()<>=;{}", *p)) {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        bool ident = false;
        for (int i = 0; i < keywords->keys->len; i++) {
            char *key = keywords->keys->data[i];
            int key_len = strlen(key);
            TokenKind val = (intptr_t)keywords->vals->data[i];
            if (startswith(p, key) && !is_alnum(p[key_len])) {
                cur = new_token(val, cur, p, key_len);
                p += key_len;
                ident = true;
                break;
            }
        }
        if (ident) continue;

        if (isalpha(*p) || *p == '_') {
            int len = 1;
            while(isalpha(p[len]) || isdigit(p[len]) || p[len] == '_') {
                len++;
            }
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

    cur = new_token(TK_EOF, cur, p, 0);
    return head.next;
}
