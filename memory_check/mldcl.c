#include "mldcl.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

extern const char* log_file_name;

#define DATEFORMAT_BUFFER_SIZE 256u

int get_current_time_str(char* szBuf)
{
	if(szBuf == NULL)
        return 0;
    
	struct tm *t;
	time_t tt;
	time(&tt);
	t = localtime(&tt);
	snprintf(szBuf, DATEFORMAT_BUFFER_SIZE, "%4d-%02d-%02d %02d:%02d:%02d ", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);

    return strlen(szBuf);
}

void check_memory(const char *fmt, ...)
{
	FILE *file = 0;
	
	if ((log_file_name != NULL) && (file = fopen(log_file_name, "a+")))
	{
		va_list vaArgs;
		char szTime[DATEFORMAT_BUFFER_SIZE] = {0};
		va_start(vaArgs, fmt);
		get_current_time_str(szTime);

		fwrite(szTime, strlen(szTime), 1, file);
        fwrite("\n", 1, 1, file);
        
		vfprintf(file, fmt, vaArgs);
		va_end(vaArgs);
		fwrite("\n", 1, 1, file);
		fclose(file);
	}
}

/**********************************************************************
* 函数名称： CreateListNode
* 功能描述： 创建新的链表结点
* 输入参数： T_ML_LIST *pList :链表指针
*           VOID *pvNodeData  :待插入的链表结点数据指针
* 输出参数： NA
* 返 回 值： T_ML_LIST_NODE*
***********************************************************************/
static T_ML_LIST_NODE *CreateListNode(T_ML_LIST *pList, VOID *pvNodeData)
{
    T_ML_LIST_NODE *pInsertNode = (T_ML_LIST_NODE*)calloc((sizeof(T_ML_LIST_NODE)+pList->dwNodeDataSize), 1);
    if(NULL == pInsertNode)
    {
        check_memory("[%s]pList(%p) failed to alloc for pInsertNode!\n", FUNC_NAME, pList);
        return NULL;
    }

    pInsertNode->pvNodeData = (INT8U *)pInsertNode + sizeof(T_ML_LIST_NODE);
    if(NULL != pvNodeData)
    {   //创建非头结点时
        memmove(pInsertNode->pvNodeData, pvNodeData, pList->dwNodeDataSize);
    }

    return pInsertNode;
}

/**********************************************************************
* 函数名称： RemoveListNode
* 功能描述： 删除指定的链表结点(释放结点内存并置其前驱后继指针为NULL)
* 输入参数： T_ML_LIST *pList :链表指针
*           VOID *pvNode       :待删除的链表结点指针
* 输出参数： NA
* 返 回 值： LIST_STATUS
* 注意事项： 本函数未置待删除结点指针为NULL，请避免访问已删除结点
***********************************************************************/
static LIST_STATUS RemoveListNode(T_ML_LIST *pList, T_ML_LIST_NODE *pNode)
{
    ML_ISOL_NODE(pNode);
    free(pNode);  //释放链表结点

    return ML_LIST_OK;
}

/**********************************************************************
* 函数名称： DestroyListNode
* 功能描述： 销毁指定的链表结点(释放结点内存并置结点指针为NULL)
* 输入参数： T_ML_LIST *pList :链表指针
*           VOID **pNode       :待销毁的链表结点指针的指针
* 输出参数： NA
* 返 回 值： LIST_STATUS
* 注意事项： 当指向待销毁结点的指针存在多份拷贝且散布程序各处时(尤其当
*           调用链未能保证**pNode指向原始结点时)，无法彻底销毁该结点
***********************************************************************/
static LIST_STATUS DestroyListNode(T_ML_LIST *pList, T_ML_LIST_NODE **pNode)
{
    free(*pNode);  //释放链表结点
    *pNode = NULL;

    return ML_LIST_OK;
}

/**********************************************************************
* 函数名称： GetListOccupation
* 功能描述： 获取链表占用情况
* 输入参数： T_ML_LIST *pList :链表指针
* 输出参数： NA
* 返 回 值： LIST_OCCUPATION
* 注意事项： 本函数仅用于内部测试。
***********************************************************************/
static LIST_OCCUPATION GetListOccupation(T_ML_LIST *pList)
{
    CHECK_SINGLE_POINTER(pList, ML_LIST_NULL);
    CHECK_SINGLE_POINTER(pList->pHead, ML_LIST_NULL);

    return (0 == pList->dwNodeNum) ? ML_LIST_EMPTY : ML_LIST_OCCUPIED;
}

/**********************************************************************
* 函数名称： MLInitList
* 功能描述： 链表初始化，产生空的双向循环链表
* 输入参数： T_ML_LIST *pList    :链表指针
*           INT32U dwNodeDataSize :链表结点保存的数据字节数
* 输出参数： NA
* 返 回 值： LIST_STATUS
***********************************************************************/
LIST_STATUS MLInitList(T_ML_LIST *pList, INT32U dwNodeDataSize)
{
    CHECK_SINGLE_POINTER(pList, ML_LIST_ERROR);

    if(0 == dwNodeDataSize)
    {
        check_memory("[%s]pList=%p, dwNodeDataSize=%uBytes, undesired initialization!\n",
               FUNC_NAME, pList, dwNodeDataSize);
        return ML_LIST_ERROR;
    }
    pList->dwNodeDataSize = dwNodeDataSize;  //给予重新修改结点数据大小的机会

    if(NULL != pList->pHead)
    {
        check_memory("[%s]pList(%p) has been initialized!\n", FUNC_NAME, pList);
        return ML_LIST_OK;
    }

    T_ML_LIST_NODE *pHeadNode = CreateListNode(pList, NULL);
    if(NULL == pHeadNode)
    {
        check_memory("[%s]pList(%p) failed to create pHeadNode!\n", FUNC_NAME, pList);
        return ML_LIST_ERROR;
    }

    ML_INIT_NODE(pHeadNode);
    pList->pHead = pList->pTail = pHeadNode;
    pList->dwNodeNum = 0;

    return ML_LIST_OK;
}

/**********************************************************************
* 函数名称： MLClearList
* 功能描述： 清空双向循环链表除头结点外的结点
* 输入参数： T_ML_LIST *pList :链表指针
* 输出参数： NA
* 返 回 值： LIST_STATUS
* 注意事项： 清空链表结点后，再次插入结点时不需要初始化链表。
***********************************************************************/
LIST_STATUS MLClearList(T_ML_LIST *pList)
{
    LIST_ITER_CHECK(pList, ML_LIST_ERROR);

    T_ML_LIST_NODE *pNextNode, *pListNode = pList->pHead->pNext;
    while(pListNode != pList->pHead)
    {
        pNextNode = pListNode->pNext;
        RemoveListNode(pList, pListNode);
        pListNode = pNextNode;
    }

    ML_INIT_NODE(pList->pHead);
    pList->pTail = pList->pHead;
    pList->dwNodeNum = 0;

    return ML_LIST_OK;
}
/**********************************************************************
* 函数名称： MLDestroyList
* 功能描述： 销毁双向循环链表，包括头结点
* 输入参数： T_ML_LIST *pList :链表指针
* 输出参数： NA
* 返 回 值： LIST_STATUS
* 注意事项： 销毁链表后，再次插入结点时需要初始化链表。
***********************************************************************/
LIST_STATUS MLDestroyList(T_ML_LIST *pList)
{
    LIST_ITER_CHECK(pList, ML_LIST_ERROR);

    T_ML_LIST_NODE *pNextNode, *pListNode = pList->pHead->pNext;
    while(pListNode != pList->pHead)
    {
        pNextNode = pListNode->pNext;
        DestroyListNode(pList, &pListNode);
        pListNode = pNextNode;
    }

    DestroyListNode(pList, &(pList->pHead)); //销毁头结点
    pList->pTail = NULL;                     //尾结点指针置空
    pList->dwNodeNum = 0;
    pList->dwNodeDataSize = 0;

    return ML_LIST_OK;
}

/**********************************************************************
* 函数名称： MLIsListEmpty
* 功能描述： 判断链表是否为空(仅含头结点或不含任何结点)
* 输入参数： T_ML_LIST *pList :链表指针
* 输出参数： NA
* 返 回 值： BOOL
***********************************************************************/
BOOL MLIsListEmpty(T_ML_LIST *pList)
{
    CHECK_SINGLE_POINTER(pList, ML_LIST_TRUE);
    CHECK_SINGLE_POINTER(pList->pHead, ML_LIST_TRUE);

    T_ML_LIST_NODE *pHeadNode = pList->pHead;
    if((0 == pList->dwNodeNum) &&
       (pHeadNode->pPrev == pHeadNode) && //冗余校验以加强安全性
       (pHeadNode->pNext == pHeadNode))
    {
        return ML_LIST_TRUE;
    }
    else
    {
        return ML_LIST_FALSE;
    }
}

/**********************************************************************
* 函数名称： MLPrependListNode
* 功能描述： 在链表头结点后逆序增加结点，尾结点恒为头结点
*           在头结点指针pHead所指向结点和pHead->pNext所指向结点
*           之间插入新结点，先插入的结点向右移动。遍历链表时
*           从pHead开始向右依次访问至最先插入的结点，类似于栈。
* 输入参数： T_ML_LIST *pList :链表指针
*           VOID *pvNodeData   :待插入的链表结点数据指针
* 输出参数： NA
* 返 回 值： LIST_STATUS
***********************************************************************/
LIST_STATUS MLPrependListNode(T_ML_LIST *pList, VOID *pvNodeData)
{
    CHECK_DOUBLE_POINTER(pList, pvNodeData, ML_LIST_ERROR);

    if(0 == pList->dwNodeDataSize)
    {
        check_memory("[%s]pList=%p, dwNodeDataSize=0Bytes, probably uninitialized or initialized improperly. See 'MLInitList'!\n",
               FUNC_NAME, pList);
        return ML_LIST_ERROR;
    }
    T_ML_LIST_NODE *pInsertNode = CreateListNode(pList, pvNodeData);
    if(NULL == pInsertNode)
    {
        check_memory("[%s]pList(%p) failed to create pInsertNode!\n", FUNC_NAME, pList);
        return ML_LIST_ERROR;
    }

    ML_INSERT_NODE(pList->pHead, pInsertNode); //在链表头结点后增加一个结点

    pList->dwNodeNum++;

    return ML_LIST_OK;
}

/**********************************************************************
* 函数名称： MLAppendListNode
* 功能描述： 在链表头结点后顺序增加结点，新结点作为尾结点
*           在头结点指针pHead所指向结点前(即尾结点后)插入新结点，
*           先插入的结点向左移动。遍历链表时从pHead开始向右依次
*           访问至最后插入的结点，类似于队列。
*           双向循环链表已保证pList->pTail(即pHead->pPrev)非空。
* 输入参数： T_ML_LIST *pList :链表指针
*           VOID *pvNodeData   :待插入的链表结点数据指针
* 输出参数： NA
* 返 回 值： LIST_STATUS
***********************************************************************/
LIST_STATUS MLAppendListNode(T_ML_LIST *pList, VOID *pvNodeData)
{
    CHECK_DOUBLE_POINTER(pList, pvNodeData, ML_LIST_ERROR);

    if(0 == pList->dwNodeDataSize)
    {
        check_memory("[%s]pList=%p, dwNodeDataSize=0Bytes, probably uninitialized or initialized improperly. See 'MLInitList'!\n",
               FUNC_NAME, pList);
        return ML_LIST_ERROR;
    }

    T_ML_LIST_NODE *pInsertNode = CreateListNode(pList, pvNodeData);
    if(NULL == pInsertNode)
    {
        check_memory("[%s]pList(%p) failed to create pInsertNode!\n", FUNC_NAME, pList);
        return ML_LIST_ERROR;
    }

    ML_INSERT_NODE(pList->pTail, pInsertNode); //在链表尾结点后增加一个结点
    pList->pTail = pInsertNode;                  //新的尾结点指向当前添加的结点

    pList->dwNodeNum++;

    return ML_LIST_OK;
}

/**********************************************************************
* 函数名称： MLInsertListNode
* 功能描述： 在链表中任意位置插入结点
* 输入参数： T_ML_LIST *pList          :链表指针
*           T_ML_LIST_NODE *pPrevNode :待插入结点的前驱结点指针
*           VOID *pvNodeData            :待插入结点的数据域指针
* 输出参数： NA
* 返 回 值： LIST_STATUS
* 注意事项： 若pPrevNode恒为头结点或尾结点，请使用MLPrependListNode
*           或MLAppendListNode函数
***********************************************************************/
LIST_STATUS MLInsertListNode(T_ML_LIST *pList, T_ML_LIST_NODE *pPrevNode, VOID *pvNodeData)
{
    CHECK_TRIPLE_POINTER(pList, pPrevNode, pvNodeData, ML_LIST_ERROR);

    if(0 == pList->dwNodeDataSize)
    {
        check_memory("[%s]pList=%p, dwNodeDataSize=0Bytes, probably uninitialized or initialized improperly. See 'MLInitList'!\n",
               FUNC_NAME, pList);
        return ML_LIST_ERROR;
    }

    T_ML_LIST_NODE *pInsertNode = CreateListNode(pList, pvNodeData);
    if(NULL == pInsertNode)
    {
        check_memory("[%s]pList(%p) failed to create pInsertNode!\n", FUNC_NAME, pList);
        return ML_LIST_ERROR;
    }

    ML_INSERT_NODE(pPrevNode, pInsertNode);
    if(pPrevNode == pList->pTail)
        pList->pTail = pInsertNode;

    pList->dwNodeNum++;

    return ML_LIST_OK;
}

/**********************************************************************
* 函数名称： MLRemoveListNode
* 功能描述： 删除双向循环链表中除头结点外的某一结点
* 输入参数： T_ML_LIST *pList      :链表指针
*           T_ML_LIST_NODE *pNode :待删除的链表结点指针
* 输出参数： NA
* 返 回 值： LIST_STATUS
***********************************************************************/
LIST_STATUS MLRemoveListNode(T_ML_LIST *pList, T_ML_LIST_NODE *pNode)
{
    CHECK_DOUBLE_POINTER(pList, pNode, ML_LIST_ERROR);
    CHECK_DOUBLE_POINTER(pNode->pPrev, pNode->pNext, ML_LIST_ERROR);

    if(0 == pList->dwNodeNum)
    {
        check_memory("[%s]pList(%p) has no node to be Removed!\n", FUNC_NAME, pList);
        return ML_LIST_ERROR;
    }

    ML_REMOVE_NODE(pNode);
    if(pNode->pNext == pList->pHead)
    {
        pList->pTail = pNode->pPrev; //删除尾结点
    }

    RemoveListNode(pList, pNode);
    pList->dwNodeNum--;

    return ML_LIST_OK;
}
/**********************************************************************
* 函数名称： MLDestroyListNode
* 功能描述： 销毁双向循环链表中除头结点外的某一结点
* 输入参数： T_ML_LIST *pList       :链表指针
*           T_ML_LIST_NODE **pNode :待销毁的链表结点二级指针
* 输出参数： NA
* 返 回 值： LIST_STATUS
***********************************************************************/
LIST_STATUS MLDestroyListNode(T_ML_LIST *pList, T_ML_LIST_NODE **pNode)
{
    CHECK_DOUBLE_POINTER(pList, pNode, ML_LIST_ERROR);
    CHECK_SINGLE_POINTER(*pNode, ML_LIST_ERROR);

    if(0 == pList->dwNodeNum)
    {
        check_memory("[%s]pList(%p) has no node to be Removed!\n", FUNC_NAME, pList);
        return ML_LIST_ERROR;
    }

    ML_REMOVE_NODE(*pNode);
    if((*pNode)->pNext == pList->pHead)
    {
        pList->pTail = (*pNode)->pPrev; //删除尾结点
    }

    DestroyListNode(pList, pNode);
    pList->dwNodeNum--;

    return ML_LIST_OK;
}

/**********************************************************************
* 函数名称： MLGetListNodeByIndex
* 功能描述： 获取链表中指定序号的结点(按头结点后继方向排序)
* 输入参数： T_ML_LIST* pList :链表指针
*           INT32U dwNodeIndex :结点序号(从1开始)
* 输出参数： NA
* 返 回 值： T_ML_LIST_NODE* 链表结点指针(空表返回NULL)
***********************************************************************/
T_ML_LIST_NODE* MLGetListNodeByIndex(T_ML_LIST *pList, INT32U dwNodeIndex)
{
    CHECK_SINGLE_POINTER(pList, NULL);
    
    if(0 == dwNodeIndex)
        return pList->pHead;  //也可返回NULL
    if(dwNodeIndex >= pList->dwNodeNum)
        return pList->pTail;

    INT32U dwNodeIdx = 1;
    T_ML_LIST_NODE *pListNode = pList->pHead;
    for(; dwNodeIdx <= dwNodeIndex; dwNodeIdx++)
        pListNode = pListNode->pNext;

    return pListNode;
}
/**********************************************************************
* 函数名称： MLLocateListNode
* 功能描述： 查找链表中首个与pData满足函数fpCompareNode判定关系的结点
* 输入参数： T_ML_LIST* pList            :链表指针
*           VOID* pvData                  :待比较数据指针
*           CompareNodeFunc fpCompareNode :比较回调函数指针
* 输出参数： NA
* 返 回 值： T_ML_LIST_NODE* 链表结点指针(未找到时返回NULL)
***********************************************************************/
/* 比较回调函数原型，用来自定义链表节点比较 */
typedef INT8U (*CompareNodeFunc)(VOID *pvNodeData, VOID *pvData, INT32U dwNodeDataSize);
T_ML_LIST_NODE* MLLocateListNode(T_ML_LIST *pList, VOID *pvData, CompareNodeFunc fpCompareNode)
{
    CHECK_TRIPLE_POINTER(pList, pvData, fpCompareNode, NULL);
    CHECK_SINGLE_POINTER(pList->pHead, NULL);
    CHECK_SINGLE_POINTER(pList->pHead->pNext, NULL);

    T_ML_LIST_NODE *pListNode = pList->pHead->pNext;
    while(pListNode != pList->pHead)
    {
        if(0 == fpCompareNode(pListNode->pvNodeData, pvData, pList->dwNodeDataSize))
            return pListNode;

        pListNode = pListNode->pNext;
    }

    return NULL;
}

/**********************************************************************
* 函数名称： MLTraverseListNode
* 功能描述： 链表结点遍历函数，遍历操作由fpTravNode指定
* 输入参数： T_ML_LIST* pList      :链表指针
*           VOID* pvTravInfo        :遍历操作回调函数所需信息
*                                    也可为空，取决于回调函数具体实现
*           TravNodeFunc fpTravNode :遍历操作回调函数指针
* 输出参数： NA
* 返 回 值： LIST_STATUS
* 注意事项： 本函数可间接实现Print等操作，但不建议代替后者。
*           fpTravNode返回非0(ML_LIST_OK)值时中止遍历
***********************************************************************/
typedef LIST_STATUS (*TravNodeFunc)(VOID *pvNode, VOID *pvTravInfo, INT32U dwNodeDataSize);
LIST_STATUS MLTraverseListNode(T_ML_LIST *pList, VOID *pvTravInfo, TravNodeFunc fpTravNode)
{
    CHECK_DOUBLE_POINTER(pList, fpTravNode, ML_LIST_ERROR);
    CHECK_SINGLE_POINTER(pList->pHead, ML_LIST_ERROR);
    CHECK_SINGLE_POINTER(pList->pHead->pNext, ML_LIST_ERROR);

    T_ML_LIST_NODE *pListNode = pList->pHead->pNext;
    while(pListNode != pList->pHead)
    {
        T_ML_LIST_NODE *pTmpNode = pListNode->pNext; //fpTravNode内可能会销毁结点pListNode
        if(ML_LIST_OK != fpTravNode(pListNode, pvTravInfo, pList->dwNodeDataSize))
            break;

        pListNode = pTmpNode;
    }

    return ML_LIST_OK;
}

/**********************************************************************
* 函数名称： MLPrintListNode
* 功能描述： 打印输出链表结点的数据域内容
* 输入参数： T_ML_LIST* pList        :链表指针
*           PrintListFunc fpPrintList :打印回调函数指针
* 输出参数： NA
* 返 回 值： LIST_STATUS
***********************************************************************/
/* 打印回调函数原型，用来自定义链表内容打印 */
typedef VOID (*PrintListFunc)(VOID *pNodeData, INT32U dwNodeNum);
LIST_STATUS MLPrintListNode(T_ML_LIST *pList, PrintListFunc fpPrintList)
{
    CHECK_DOUBLE_POINTER(pList, fpPrintList, ML_LIST_ERROR);
    CHECK_SINGLE_POINTER(pList->pHead, ML_LIST_ERROR);
    CHECK_SINGLE_POINTER(pList->pHead->pNext, ML_LIST_ERROR);

    T_ML_LIST_NODE *pListNode = pList->pHead->pNext;
    while(pListNode != pList->pHead)
    {
        //具体打印格式交给回调函数灵活处理(可直接打印也可拷贝至本地处理后打印)
        fpPrintList(pListNode->pvNodeData, pList->dwNodeNum);
        pListNode = pListNode->pNext;
    }
    check_memory("\n");

    return ML_LIST_OK;
}

/**********************************************************************
* 函数名称： CompareNodeGeneric
* 功能描述： 通用链表结点内存比较
* 输入参数： VOID *pvNodeData      :链表结点数据指针
*           VOID *pvData          :待比较外部数据指针
*           INT32U dwNodeDataSize :链表结点数据大小
* 输出参数： NA
* 返 回 值： 0：Equal; !0:Unequal
* 注意事项： 比较长度为结点数据字节数，即默认与外部数据大小一致
***********************************************************************/
INT8U CompareNodeGeneric(VOID *pvNodeData, VOID *pvData, INT32U dwNodeDataSize)
{
    CHECK_DOUBLE_POINTER(pvNodeData, pvData, 1);
    return memcmp(pvNodeData, pvData, dwNodeDataSize);
}
/**********************************************************************
* 函数名称： PrintListWord
* 功能描述： 打印链表结点，结点数据域为两字节整数
* 输入参数： VOID *pvNodeData   :链表节点数据指针
*           INT32U dwNodeNum  :链表节点数目
* 输出参数： NA
* 返 回 值： VOID
* 注意事项： 仅作示例，未考虑字节序等问题。
***********************************************************************/
VOID PrintListWord(VOID *pvNodeData, INT32U dwNodeNum)
{
    CHECK_SINGLE_POINTER(pvNodeData, RETURN_VOID);
    check_memory("%d ", *((INT16U *)pvNodeData));
}

/**********************************************************************
* 函数名称： MLGetListNodeNum
* 功能描述： 获取链表结点数目
* 输入参数： T_ML_LIST *pList :链表指针
* 输出参数： NA
* 返 回 值： INT32U 链表结点数目
***********************************************************************/
INT32U MLGetListNodeNum(T_ML_LIST *pList)
{
    CHECK_SINGLE_POINTER(pList, 0);
    return (pList->dwNodeNum);
}

/**********************************************************************
* 函数名称： MLGetListHead/MLGetListTail
* 功能描述： 获取链表头结点/尾结点指针
* 输入参数： T_ML_LIST *pList :链表指针
***********************************************************************/
T_ML_LIST_NODE* MLGetListHead(T_ML_LIST *pList)
{
    CHECK_SINGLE_POINTER(pList, NULL);
    return (pList->pHead);
}
T_ML_LIST_NODE* MLGetListTail(T_ML_LIST *pList)
{
    CHECK_SINGLE_POINTER(pList, NULL);
    return (pList->pTail);
}

/**********************************************************************
* 函数名称： MLGetPrevNode/MLGetNextNode
* 功能描述： 获取链表指定结点的前驱结点/后继结点指针
* 输入参数： T_ML_LIST_NODE *pNode :指定结点的指针
***********************************************************************/
T_ML_LIST_NODE* MLGetPrevNode(T_ML_LIST_NODE *pNode)
{
    CHECK_SINGLE_POINTER(pNode, NULL);
    return (pNode->pPrev);
}
T_ML_LIST_NODE* MLGetNextNode(T_ML_LIST_NODE *pNode)
{
    CHECK_SINGLE_POINTER(pNode, NULL);
    return (pNode->pNext);
}

/**********************************************************************
* 函数名称： MLGetNodeData
* 功能描述： 获取链表指定结点的数据域
* 输入参数： T_ML_LIST_NODE *pNode :指定结点的指针
***********************************************************************/
VOID* MLGetNodeData(T_ML_LIST_NODE *pNode)
{
    CHECK_DOUBLE_POINTER(pNode, pNode->pvNodeData, NULL);
    return (pNode->pvNodeData);
}