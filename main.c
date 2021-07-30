#include "9cc.h"


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of argument.\n");
        return 1;
    }

    user_input = argv[1];

    token = tokenize(user_input);

    Node *node = program();

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    gen(node);

    printf("\tpop rax\n");
    printf("\tret\n");
    return 0;
}
