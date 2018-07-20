#ifndef MLMC_H__
#define MLMC_H__

#include "mldcl.h"

typedef enum{
    S_NULL_POINTER    = (INT8U)0,
    S_ILLEGAL_PARAM   = (INT8U)1,
    S_OK              = (INT8U)2,
}FUNC_STATUS;

typedef struct{
    const CHAR  *pFileName;   //内存分配函数调用处所在文件名
    const CHAR  *pFuncName;   //内存分配函数调用处所在函数名
    INT32U      dwCodeLine;   //内存分配函数调用处所在代码行号
    INT32U      dwMemSize;    //内存分配函数申请到的内存字节数
    VOID        *pvMemAddr;   //内存分配函数返回的内存指针地址
}ML_MEM_INFO;

extern T_ML_LIST gtMemLeakCheckList;

typedef struct{
    INT32U dwAllocBytes;   //申请内存的总字节数
    INT32U dwAllocTimes;   //申请内存的总次数
    INT32U dwFreeBytes;    //申请内存的总字节数
    INT32U dwFreeTimes;    //释放内存的总次数
}T_MEM_STATIS;

static T_MEM_STATIS gtMemStatis = {0};

//ML申请内存时的头守护字节数
#define ML_MEM_HEAD_SIZE        8

//ML内存管理特征码0xA5(二进制10100101)
#define ML_MEM_IDEN_CODE        (CHAR)0xA5

//ML内存管理特征域所占字节数
#define ML_MEM_IDEN_SIZE        4

//ML申请内存时的尾守护字节数
#define ML_MEM_TAIL_SIZE        2

#define ML_INIT_MEMINFO() do{ \
    MLInitList(&gtMemLeakCheckList, sizeof(ML_MEM_INFO)); \
}while(0)
    
#define ML_INSERT_MEMINFO(pFile, pFunc, dwLine, dwMemBytes, pvMemBufAddr) do{ \
    ML_MEM_INFO tMemInfo = {0};            \
    tMemInfo.pFileName    = (pFile);         \
    tMemInfo.pFuncName    = (pFunc);         \
    tMemInfo.dwCodeLine   = (dwLine);        \
    tMemInfo.dwMemSize    = (dwMemBytes);    \
    tMemInfo.pvMemAddr    = (pvMemBufAddr);  \
    MLAppendListNode(&gtMemLeakCheckList, &tMemInfo); \
}while(0)
    
#define ML_REMOVE_MEMINFO(pFile, pFunc, dwLine, dwMemBytes, pvMemBufAddr) do{ \
    ML_MEM_INFO tMemInfo = {0};            \
    tMemInfo.pFileName    = (pFile);         \
    tMemInfo.pFuncName    = (pFunc);         \
    tMemInfo.dwCodeLine   = (dwLine);        \
    tMemInfo.dwMemSize    = (dwMemBytes);    \
    tMemInfo.pvMemAddr    = (pvMemBufAddr);  \
    T_ML_LIST_NODE *pDeleteNode = NULL; \
    pDeleteNode = MLLocateListNode(&gtMemLeakCheckList, &tMemInfo, CompareNodeMem); \
    MLRemoveListNode(&gtMemLeakCheckList, pDeleteNode); \
}while(0)
    
#define ML_REPORT_MEMCHECK() do{ \
    MLCheckMemLeak(&gtMemLeakCheckList); \
}while(0)

#endif // MLMC_H__