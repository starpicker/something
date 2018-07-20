#include "mlalloc.h"

CHAR *gpGlobalMem = NULL;
VOID GlobalAllocTest(VOID){
  gpGlobalMem = malloc(50);
}
VOID GlobalFreeTest(VOID){
  free(gpGlobalMem);
}

INT32S main(VOID)
{
    INT8U ucTestIndex = 1;
    ML_INIT_MEMINFO();

    printf("\n<Test Case %u>: Simple Alloc-Free within Function!\n", ucTestIndex++);
    CHAR *ptr =  malloc(10);
    CHAR *ptr1 = malloc(20);
    CHAR *ptr2 = malloc(30);
    CHAR *ptr3 = malloc(40);
    strcpy(ptr, "123456789");
    printf("ptr=%s(PreFree)\n", ptr);
    free(ptr); //为测试需要，此处释放内存后不置ptr为NULL
    printf("ptr=%s(PostFree)\n", ptr);
    MLShowMemInfo();

    printf("\n<Test Case %u>: Simple Alloc-Free outside Function!\n", ucTestIndex++);
    GlobalAllocTest();
    MLShowMemInfo();

    printf("\nTest Case %u: Free all Allocated Memories!\n", ucTestIndex++);
    free(ptr1);
    free(ptr2);
    free(ptr3);
    GlobalFreeTest();
    MLShowMemInfo();

    printf("\nTest Case %u: Overrun Downward!\n", ucTestIndex++);
    CHAR *ptr5 = malloc(33);
    *(ptr5-8) = 10;
    free(ptr5);

    printf("\nTest Case %u: Overrun Upward!\n", ucTestIndex++);
    CHAR *ptr6 = malloc(5);
    strcpy(ptr6, "123456789");
    free(ptr6);

    printf("\nTest Case %u: Doubly Free!\n", ucTestIndex++);
    free(ptr6);

    printf("\nTest Case %u: Free Memory not allocated by malloc!\n", ucTestIndex++);
    CHAR *ptr7 = calloc(44, 1);
    free(ptr7);

    return 0;
}