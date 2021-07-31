#include "9cc.h"


int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Wrong number of argument.\n");
        return 1;
    }

    user_input = argv[1];

    token = tokenize(user_input);

#ifdef DEBUG
    printf("Finish Tokenize\n");
#endif

    program();



#ifdef DEBUG
    printf("Finish Program\n");
#endif

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    printf("\tpush rbp\n");
    printf("\tmov rbp, rsp\n");
    printf("\tsub rsp, 208\n");


#ifdef DEBUG
    printf("%d\n", code[0]->kind==ND_NUM);
#endif

    for (int i = 0; code[i]; i++) {
        gen(code[i]);
        printf("\tpop rax\n");
    }

    printf("\tmov rsp, rbp\n");
    printf("\tpop rbp\n");
    printf("\tret\n");
    return 0;
}
