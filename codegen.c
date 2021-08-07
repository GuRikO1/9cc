#include "9cc.h"


void gen_lval(Node *node) {
    if (node->kind != ND_LVAR) {
        error("Left side value of assignment is not a variable");
    }
    printf("\tmov rax, rbp\n");
    printf("\tsub rax, %d\n", node->offset);
    printf("\tpush rax\n");
}


void gen(Node *node) {
    switch (node->kind) {
        case ND_RETURN:
            gen(node->lhs);
            printf("\tpop rax\n");
            printf("\tmov rsp, rbp\n");
            printf("\tpop rbp\n");
            printf("\tret\n");
            return;
        case ND_NUM:
            printf("\tpush %d\n", node->val);
            return;
        case ND_LVAR:
            gen_lval(node);
            printf("\tpop rax\n");
            printf("\tmov rax, [rax]\n");
            printf("\tpush rax\n");
            return;
        case ND_ASSIGN:
            gen_lval(node->lhs);
            gen(node->rhs);
            printf("\tpop rdi\n");
            printf("\tpop rax\n");
            printf("\tmov [rax], rdi\n");
            printf("\tpush rdi\n");
            return;
        case ND_IF:
            gen(node->lhs);
            printf("\tpop rax\n");
            printf("\tcmp rax, 0\n");
            if (node->els != NULL) {
                printf("\tje  .Lelse%d\n", serial_num);
                gen(node->rhs);
                printf("\tjmp .Lend%d\n", serial_num);
            } else {
                printf("\tje .Lend%d\n", serial_num);
                gen(node->rhs);
            }

            if (node->els != NULL) {
                printf(".Lelse%d:\n", serial_num);
                gen(node->els);
            }
            printf(".Lend%d:\n", serial_num);

            ++serial_num;
            return;

        case ND_WHILE:
            printf(".Lbegin%d:\n", serial_num);
            gen(node->lhs);
            printf("\tpop rax\n");
            printf("\tcmp rax, 0\n");
            printf("\tje .Lend%d\n", serial_num);
            gen(node->rhs);
            printf("jmp .Lbegin%d\n", serial_num);
            printf(".Lend%d:\n", serial_num);

            ++serial_num;
            return;

        case ND_FOR:
            gen(node->init);
            printf(".Lbegin%d:\n", serial_num);
            gen(node->cond);
            printf("\tpop rax\n");
            printf("\tcmp rax, 0\n");
            printf("\tje .Lend%d\n", serial_num);
            gen(node->lhs);
            gen(node->routine);
            printf("jmp .Lbegin%d\n", serial_num);
            printf(".Lend%d:\n", serial_num);

            ++serial_num;
            return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("\tpop rdi\n");
    printf("\tpop rax\n");

    switch (node->kind) {
        case ND_ADD:
            printf("\tadd rax, rdi\n");
            break;
        case ND_SUB:
            printf("\tsub rax, rdi\n");
            break;
        case ND_MUL:
            printf("\timul rax, rdi\n");
            break;
        case ND_DIV:
            printf("\tcqo\n");
            printf("\tidiv rdi\n");
            break;
        case ND_EQ:
            printf("\tcmp rax, rdi\n");
            printf("\tsete al\n");
            printf("\tmovzb rax, al\n");
            break;
        case ND_NE:
            printf("\tcmp rax, rdi\n");
            printf("\tsetne al\n");
            printf("\tmovzb rax, al\n");
            break;
        case ND_LE:
            printf("\tcmp rax, rdi\n");
            printf("\tsetle al\n");
            printf("\tmovzb rax, al\n");
            break;
        case ND_LT:
            printf("\tcmp rax, rdi\n");
            printf("\tsetl al\n");
            printf("\tmovzb rax, al\n");
            break;
    }
    printf("\tpush rax\n");
}
