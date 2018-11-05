#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if(argc == 1)
        return 0;

    int len;
    __asm__ __volatile__
    (
        "movq %%rsi, %%rdi \t\n"
        "movb $0, %%al\t\n"
        "movl $0, %%ecx \t\n"
        "not %%ecx \t\n"
        "repnz scasb \t\n"
        "subq %%rsi, %%rdi \t\n"
        "subq $1, %%rdi \t\n"
        "movl %%edi, %0 \t\n"
        "movq %%rdi, %%rdx \t\n"
        "movq $1, %%rax \t\n"
        "movq $1, %%rdi \t\n"
        "syscall"
        :"=Q"(len)
        :"S"(argv[1])
        :"rax", "rdi", "ecx", "edx"
    );

    printf(" len: %d\n", len);
}
