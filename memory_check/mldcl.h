#ifndef MLDCL_H__
#define MLDCL_H__

#define VOID    void
#define BOOL    int 
#define CHAR    char
#define INT32U  unsigned int
#define INT8U   unsigned char
#define INT32S  int
#define INT16U  unsigned short

#define NULL    0

void check_memory(const char *fmt, ...);

typedef struct T_ML_LIST_NODE{
    struct T_ML_LIST_NODE  *pPrev;      /* 指向链表直接前驱结点的指针 */
    struct T_ML_LIST_NODE  *pNext;      /* 指向链表直接后继结点的指针 */
    VOID                   *pvNodeData; /* 指向链表数据的指针。获取具体数据时需显式转换该指针类型为目标类型 */
}T_ML_LIST_NODE;

typedef struct{
    T_ML_LIST_NODE   *pHead;          /* 指向链表头结点的指针 */
    T_ML_LIST_NODE   *pTail;          /* 指向链表尾结点的指针 */
    INT32U           dwNodeNum;       /* 链表结点数目 */
    INT32U           dwNodeDataSize;  /* 链表结点保存的数据字节数 */
}T_ML_LIST;

//链表函数返回状态枚举值
typedef enum{
    ML_LIST_OK    = (INT8U)0,
    ML_LIST_ERROR = (INT8U)1
}LIST_STATUS;

//链表结点空闲情况枚举值
typedef enum{
    ML_LIST_OCCUPIED = (INT8U)0,
    ML_LIST_EMPTY    = (INT8U)1,
    ML_LIST_NULL     = (INT8U)2
}LIST_OCCUPATION;

//BOOL型常量，适用于'Is'前缀函数
#define  ML_LIST_TRUE   (BOOL)1
#define  ML_LIST_FALSE  (BOOL)0

#define   FUNC_NAME    __FUNCTION__ //(__func__)

/* 指针校验宏 */
//若无返回值则retVal置RETURN_VOID
#define RETURN_VOID
#define CHECK_SINGLE_POINTER(ptr1, retVal) do{\
    if(NULL == (ptr1)) \
    { \
        check_memory("[%s(%d)]Null Pointer: "#ptr1"!\n\r", FUNC_NAME, __LINE__); \
        return retVal; \
    } \
}while(0)
    
#define CHECK_DOUBLE_POINTER(ptr1, ptr2, retVal) do{\
    if((NULL == (ptr1)) || (NULL == (ptr2))) \
    { \
        check_memory("[%s(%d)]Null Pointer: "#ptr1"(%p), "#ptr2"(%p)!\n\r", FUNC_NAME, __LINE__, ptr1, ptr2); \
        return retVal; \
    } \
}while(0)
    
#define CHECK_TRIPLE_POINTER(ptr1, ptr2, ptr3, retVal) do{\
    if((NULL == (ptr1)) || (NULL == (ptr2)) || (NULL == (ptr3))) \
    { \
        check_memory("[%s(%d)]Null Pointer: "#ptr1"(%p), "#ptr2"(%p), "#ptr3"(%p)!\n\r", FUNC_NAME, __LINE__, ptr1, ptr2, ptr3); \
        return retVal; \
    } \
}while(0)

//创建结点为作为链表头以生成双向循环空链表
#define ML_INIT_NODE(pNode) do{ \
    (pNode)->pNext = (pNode)->pPrev = (pNode); \
}while(0)
    
//"孤立"链表结点，避免通过该结点访问其前驱和后继结点(进而遍历链表)
#define ML_ISOL_NODE(pNode) do{ \
    (pNode)->pNext = (pNode)->pPrev = NULL; \
}while(0)
    
//判断链表是否仅含头结点
#define ML_LIST_WITH_HEAD(pHeadNode) do{ \
    (((pHeadNode)->pPrev == (pHeadNode)) && ((pHeadNode->pNext == pHeadNode))); \
}while(0)

//插入链表结点
#define ML_INSERT_NODE(prevNode, insertNode) do{ \
    (insertNode)->pNext      = (prevNode)->pNext;  \
    (insertNode)->pPrev      = (prevNode);         \
    (prevNode)->pNext->pPrev = (insertNode);       \
    (prevNode)->pNext        = (insertNode);       \
}while(0)
    
//删除链表结点
#define ML_REMOVE_NODE(removeNode) do{ \
    (removeNode)->pPrev->pNext = (removeNode)->pNext;  \
    (removeNode)->pNext->pPrev = (removeNode)->pPrev;  \
}while(0)

//获取链表结点及其数据(不做安全性检查)
#define GET_NODE_NUM(pList)      ((pList)->dwNodeNum)
#define GET_HEAD_NODE(pList)     ((pList)->pHead)
#define GET_TAIL_NODE(pList)     ((pList)->pTail)
#define GET_PREV_NODE(pNode)     ((pNode)->pPrev)
#define GET_NEXT_NODE(pNode)     ((pNode)->pNext)
#define GET_NODE_DATA(pNode)     ((pNode)->pvNodeData)

//双向循环链表遍历校验宏
#define LIST_ITER_CHECK(pList, retVal) do{\
    CHECK_SINGLE_POINTER((pList), retVal); \
    CHECK_SINGLE_POINTER((pList)->pHead, retVal); \
    CHECK_SINGLE_POINTER((pList)->pHead->pNext, retVal); \
}while(0)
    
//双向循环链表遍历宏
//pList: 链表指针；pLoopNode: 链表结点，用作循环计数器；
//pTmpNode: 链表结点，用作删除pLoopNode时临时保存pLoopNode->pNext
//某些情况下采用遍历宏代替MLLocateListNode或MLTraverseListNode函数可提高执行效率。
//如外部数据和结点数据需按共同的规则转换时，采用遍历宏可使外部数据不必重复转换。
#define LIST_ITER_LOOP(pList, pLoopNode) \
  for(pLoopNode = (pList)->pHead->pNext; \
      pLoopNode != (pList)->pHead; \
      pLoopNode = pLoopNode->pNext)
      
#define LIST_ITER_LOOP_SAFE(pList, pLoopNode, pTmpNode) \
  for(pLoopNode = (pList)->pHead->pNext, pTmpNode = pLoopNode->pNext; \
      pLoopNode != (pList)->pHead; \
      pLoopNode = pTmpNode, pTmpNode = pLoopNode->pNext)
      
#endif // MLDCL_H__