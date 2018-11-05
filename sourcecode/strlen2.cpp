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
        "movq %%rsi, %%rdi \n\t"
        "movb $0, %%al \n\t"
        "movl $0, %%ecx \n\t"
        "not %%ecx \n\t"
        "movl %%ecx, %%edx \n\t"
        "repnz scasb \n\t"
        "subl %%edx, %%ecx \n\t"
        "neg %%ecx \n\t"
        "dec %%ecx \n\t"
        "movl %%ecx, %0 \n\t"
        "movq %%rcx, %%rdx \n\t"
        "movq $1, %%rax \n\t"
        "movq $1, %%rdi \n\t"
        "syscall"
        :"=Q"(len)
        :"S"(argv[1])
        :"rax", "rdi", "ecx", "edx"
    );

    printf(" len: %d\n", len);
}
