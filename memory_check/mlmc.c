#include "mlmc.h"

T_ML_LIST gtMemLeakCheckList = {.pHead=NULL, .pTail=NULL, .dwNodeNum=0, .dwNodeDataSize=0};

/**********************************************************************
* 函数名称： GenerateMemChecker
* 功能描述： 构造动态内存检测区
* 输入参数： VOID *pMemBuffer :内存管理函数申请到的内存地址
*           INT32U dwMemSize :内存管理函数申请到的内存字节数
* 输出参数： NA
* 返 回 值： CHAR *：指向可用动态内存(跳过信息头)的指针
***********************************************************************/
static CHAR *GenerateMemChecker(VOID *pvMemBuf, INT32U dwMemSize)
{
    CHAR *pHeader = (CHAR *)pvMemBuf;
    memset(pHeader, ML_MEM_IDEN_CODE, ML_MEM_IDEN_SIZE);
    memmove(&pHeader[ML_MEM_IDEN_SIZE], &dwMemSize, sizeof(dwMemSize));
    memset(&pHeader[ML_MEM_HEAD_SIZE+dwMemSize], ML_MEM_IDEN_CODE, ML_MEM_TAIL_SIZE);

    return pHeader + ML_MEM_HEAD_SIZE;
}

/**********************************************************************
* 函数名称： DeriveMemHeader
* 功能描述： 提取动态内存信息头
* 输入参数： VOID *pvMemBuf    :内存管理函数申请到的内存地址
* 输出参数： INT32U *dwMemSize :内存管理函数申请到的内存字节数
* 返 回 值： CHAR *：指向动态内存信息头的指针
***********************************************************************/
static CHAR *DeriveMemHeader(VOID *pvMemBuf, INT32U *dwMemSize)
{
    CHAR *pHeader = (CHAR *)pvMemBuf - ML_MEM_HEAD_SIZE;

    //若动态内存由ML_ALLOC申请，则由信息头获取待释放内存大小
    if((ML_MEM_IDEN_CODE == pHeader[2]) && (ML_MEM_IDEN_CODE == pHeader[3]))
    {
        *dwMemSize = *(INT32U*)&(pHeader[ML_MEM_IDEN_SIZE]);
    }

    return pHeader;
}

/**********************************************************************
* 函数名称： IsMemHeadOverrun/IsMemTailOverrun
* 功能描述： 检查动态内存是否存在头(低地址)/尾(高地址)越界
***********************************************************************/
static BOOL IsMemHeadOverrun(CHAR *pMemHeader)
{
    return ((ML_MEM_IDEN_CODE != *pMemHeader) || (ML_MEM_IDEN_CODE != *(pMemHeader+1)));
}

static BOOL IsMemTailOverrun(VOID *pvMemBuf, INT32U dwMemSize)
{
    return ((ML_MEM_IDEN_CODE != *((CHAR*)pvMemBuf+dwMemSize)) ||
            (ML_MEM_IDEN_CODE != *((CHAR*)pvMemBuf+dwMemSize+1)));
}

/**********************************************************************
* 函数名称： PrintListMem
* 功能描述： 打印ML内存泄露检测结果
* 输入参数： VOID *pvNodeData :链表结点数据指针
*           INT32U dwNodeNum :链表结点数目
***********************************************************************/
static VOID PrintListMem(VOID *pvNodeData, INT32U dwNodeNum)
{
    ML_MEM_INFO *ptMemInfo = (ML_MEM_INFO *)pvNodeData;
    check_memory("[%s(%d)<%s>]%uBytes(Address:%p) were allocated, but unfreed!\n",
           ptMemInfo->pFileName, ptMemInfo->dwCodeLine, ptMemInfo->pFuncName,
           ptMemInfo->dwMemSize, (CHAR*)ptMemInfo->pvMemAddr);
}

/**********************************************************************
* 函数名称： MLCheckMemLeak
* 功能描述： ML内存泄露检测
* 输入参数： T_ML_LIST *gpMemLeakCheckList :泄露结点链表指针
***********************************************************************/
static VOID MLCheckMemLeak(T_ML_LIST *gpMemLeakCheckList)
{
    INT32U dwMemLeakNum = MLGetListNodeNum(gpMemLeakCheckList);
    if(0 == dwMemLeakNum)
    {
        check_memory("No Memory Leakage Detected!\n");
        return;        
    }
    check_memory("Suspected Memory Leakage Occurrence(%u Time(s)):\n", dwMemLeakNum);
    MLPrintListNode(gpMemLeakCheckList, PrintListMem);
}

/**********************************************************************
* 函数名称： CompareNodeMem
* 功能描述： 可疑泄露结点信息比较
* 输入参数： VOID *pNodeData       :链表结点数据指针
*           VOID *pData           :待比较外部数据指针
*           INT32U dwNodeDataSize :链表结点数据大小
* 输出参数： NA
* 返 回 值： 0：Equal; !0:Unequal
* 注意事项： 比较长度为结点数据字节数，即默认与外部数据大小一致
***********************************************************************/
static INT8U CompareNodeMem(VOID *pNodeData, VOID *pData, INT32U dwNodeDataSize)
{
    return (((ML_MEM_INFO *)pNodeData)->pvMemAddr != ((ML_MEM_INFO *)pData)->pvMemAddr);
}

INT8U CompareNodeMem2(VOID *pNodeData, VOID *pData, INT32U dwNodeDataSize)
{
    ML_MEM_INFO *ptMemInfo1 = (ML_MEM_INFO *)pNodeData;
    ML_MEM_INFO *ptMemInfo2 = (ML_MEM_INFO *)pData;

    if(ptMemInfo1->pvMemAddr != ptMemInfo2->pvMemAddr)
        return 1;

    if(ptMemInfo1->dwMemSize != ptMemInfo2->dwMemSize)
    {   //因内存指针匹配，故仍可正常释放，但提示某处可能存在内存越界
        check_memory("MemSize Field of Head Guard was Overwritten([%s(%u)<%s>]Addr:%p, Size:%u->%u)!\n", 
                ptMemInfo2->pFileName, ptMemInfo2->dwCodeLine, ptMemInfo2->pFuncName, 
                ptMemInfo2->pvMemAddr, ptMemInfo1->dwMemSize, ptMemInfo2->dwMemSize);
    }

    return 0;
}

/**********************************************************************
* 函数名称： MLInitMemCheck
* 功能描述： 初始化内存泄露检测机制
* 注意事项： 检测内存泄露时必须先调用本函数进行初始化，然后分配内存
***********************************************************************/
VOID MLInitMemCheck(VOID)
{
    ML_INIT_MEMINFO();
}

/**********************************************************************
* 函数名称： MLAlloc
* 功能描述： 动态分配内存并初始化
* 输入参数： INT32U dwMemSize      :内存管理函数申请到的内存字节数
*           const CHAR *pFileName :内存管理函数调用处所在文件名
*           const CHAR *pFuncName :内存管理函数调用处所在函数名
*           INT32U dwCodeLine     :内存管理函数调用处所在代码行号
* 输出参数： NA
* 返 回 值： VOID *：内存管理函数返回的内存指针地址
***********************************************************************/
VOID *MLAlloc(INT32U dwMemSize, const CHAR *pFileName, const CHAR *pFuncName, INT32U dwCodeLine)
{
    if(0 == dwMemSize)
    {
        check_memory("[%s(%u)<%s>] Memmory to be allocated must be larger than Zero Byte!\n",
                pFileName, dwCodeLine, pFuncName);
        return NULL;
    }

    VOID *pvMemBuf = calloc((dwMemSize+ML_MEM_HEAD_SIZE+ML_MEM_TAIL_SIZE), 1);
    if(NULL == pvMemBuf)
    {
        check_memory("[%s(%d)<%s>] Allocating %uBytes failed, no memory available!\n",
                pFileName, dwCodeLine, pFuncName, dwMemSize);
        return NULL;
    }

    gtMemStatis.dwAllocBytes += dwMemSize;
    gtMemStatis.dwAllocTimes++;

    pvMemBuf = GenerateMemChecker(pvMemBuf, dwMemSize);
    ML_INSERT_MEMINFO(pFileName, pFuncName, dwCodeLine, dwMemSize, pvMemBuf);

    return pvMemBuf;
}

/**********************************************************************
* 函数名称： MLFree
* 功能描述： 释放通过MLAlloc分配的动态内存
* 输入参数： VOID* pvMemBuf        :指向待释放内存的指针
*           const CHAR *pFileName :内存管理函数调用处所在文件名
*           const CHAR *pFuncName :内存管理函数调用处所在函数名
*           INT32U dwCodeLine     :内存管理函数调用处所在代码行号
* 输出参数： NA
* 返 回 值： FUNC_STATUS
***********************************************************************/
FUNC_STATUS MLFree(VOID* pvMemBuf, const CHAR* pFileName, const CHAR* pFuncName, INT32U dwCodeLine)
{
    if(NULL == pvMemBuf)
    {
        check_memory("[%s(%d)<%s>] Cannot free Null Address!\n", pFileName, dwCodeLine, pFuncName);
        return S_NULL_POINTER;
    }

    INT32U dwMemSize = 0;
    CHAR *pMemHeader = DeriveMemHeader(pvMemBuf, &dwMemSize);

    //对于Double Free，无论头守护字节是否已被改写，free均会发生段错误；
    //对于非MLAlloc申请的内存，free可能造成越界释放。
    //但本函数难以区分两种情况。综合考虑，决定拦截并警告。
    if(0 == dwMemSize)
    {
        check_memory("Warning: [%s(%d)<%s>]Free Memory(%p) that was 1) not allocated by MLAlloc or 2) freed by MLFree;\n",
               pFileName, dwCodeLine, pFuncName, pvMemBuf);
        check_memory("         Both cases are critical, and NO free action takes place to minimize the casualties!!!\n");
        return S_ILLEGAL_PARAM;
    }

    if(IsMemHeadOverrun(pMemHeader))
    {
        check_memory("Warning: [%s(%d)<%s>]Overrun Occurs at Lower Address(%p);\n",
               pFileName, dwCodeLine, pFuncName, pvMemBuf);
    }    
    if(IsMemTailOverrun(pvMemBuf, dwMemSize))
    {
        check_memory("Warning: [%s(%d)<%s>]Overrun Occurs at Higher Address(%p);\n",
               pFileName, dwCodeLine, pFuncName, pvMemBuf);
    }    
        
    ML_REMOVE_MEMINFO(pFileName, pFuncName, dwCodeLine, dwMemSize, pvMemBuf);

    free(pMemHeader);

    gtMemStatis.dwFreeBytes += dwMemSize;
    gtMemStatis.dwFreeTimes++;

    return S_OK;
}

/**********************************************************************
* 函数名称： MLShowMemInfo
* 功能描述： 查看ML模块当前内存申请与释放信息
* 输入参数： NA
* 输出参数： NA
* 返 回 值： VOID
***********************************************************************/
VOID MLShowMemInfo(VOID)
{
    check_memory("-------------------ML Memory Info-------------------\n");
    check_memory("%-15s%-15s%-15s%-15s%-15s\n", "AllocBytes", "AllocTimes", "FreeBytes", "FreeTimes", "<UnFreeBytes>");
    check_memory("%-15u%-15u%-15u%-15u%-15u\n", gtMemStatis.dwAllocBytes, gtMemStatis.dwAllocTimes,
           gtMemStatis.dwFreeBytes, gtMemStatis.dwFreeTimes, gtMemStatis.dwAllocBytes-gtMemStatis.dwFreeBytes);

    ML_REPORT_MEMCHECK();
}
