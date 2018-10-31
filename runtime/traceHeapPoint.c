#include "traceHeapPoint.h"

long long DPPtrCheckerCount = 0;
long long DPPtrProtectCount = 0;
long long DPObjectAllocCount = 0;
long long DPObjectFreeCount = 0;
long long DPPtrTrackerCount = 0;
long long DPPtrTrackeredCount = 0;
/*
 *              F                   F       F      F
 *   0      0       0       0      0000    0000    0000
 *  mulP  isnode  ishead  isfree       referenceCount
 */

////TODO:
//void *safeRealloc(void * ptr, size_t new_size){
//    void *tmp = realloc(((heapNode *)ptr)->addr, new_size);
//    if (tmp == ((heapNode *)ptr)->addr) {
//        return ptr;
//    }
//    heapNode *newHeadP = malloc(sizeof(heapNode));
//    if (tmp && newHeadP) {
//        if (newHeadP) {
//            memset(newHeadP, 0, sizeof(heapNode));
//            newHeadP->head = newHeadP;
//            newHeadP->referenceCount = 1;
//            newHeadP->addr = tmp;
//        }
//    }
//    heapNode *nodeHeadP = ptr;
//    nodeHeadP->addr = NULL;
//    heapNode *nodeP = nodeHeadP->next;
//    while (nodeP != NULL) {
//        nodeP->addr = NULL;
//        nodeP = nodeP->next;
//    }
//    if (tmp && newHeadP) {
//        return newHeadP;
//    }else{
//        return NULL;
//    }
//}
//
////TODO:
//void *safeCalloc(size_t numitems, size_t size){
//    heapNode *ptr = malloc(sizeof(heapNode));
//    if (ptr) {
//        memset(ptr, 0, sizeof(heapNode));
//        ptr->head = ptr;
//        ptr->referenceCount = 1;
//        ptr->addr = calloc(numitems, size);
//        return ptr;
//    }else{
//        return NULL;
//    }
//}


//#define notMultiPTR(a) (!(((unsigned long)(a))&0x8000000000000000))
#define notMultiPTR(a) (!((((unsigned long)(a)) & 0xFFFF000000000000) ==  0x8000000000000000))
#define notNodeHeadPTR(a) (!(((unsigned long)(a))&0x6000000000000000))
#define notNodePTR(a) (!(((unsigned long)(a))&0x4000000000000000))
#define isFree(a) (((unsigned long)(a))&0x1000000000000000)

#define truePtrValue(a) (((unsigned long)(a))&0x00007FFFFFFFFFFF)

#define getMultiPTR(a) (((unsigned long)(a))|0x8000000000000000)
#define getNodeHeadPTR(a) (((unsigned long)(a))|0x6000000000000000)
#define getNodePTR(a) (((unsigned long)(a))|0x4000000000000000)

#define getNodePTRChange(a,b) (((unsigned long)(b))|(0xFFFF000000000000 & (unsigned long)a))

#define getNewNodePTR(a) (((unsigned long)(a))|0x4001000000000000)
#define getNewHeadNodePTR(a) (((unsigned long)(a))|0x6001000000000000)

#define addNodeRefer(a) ((((unsigned long)(a))&0x07FF000000000000)>0x07FE000000000000?(a):(((unsigned long)(a))+0x0001000000000000))
#define subNodeRefer(a) ((((unsigned long)(a))&0x07FF000000000000)>0?(((unsigned long)(a))-0x0001000000000000):(a))

#define getNodeRefer(a) ((unsigned int)((((unsigned long)(a))&0x07FF000000000000)>>48))
#define getFree(a) (((unsigned long)(a))|0x1000000000000000)

//int insertNode(void *node){
////    if (node == NULL || notNodePTR(((heapNode *)node)->addr)) {
////        return 1;
////    }
//    heapNode *newNode = malloc(sizeof(heapNode));
////    memset(newNode, 0, sizeof(heapNode));
////    newNode->addr = getNewNodePTR(newNode->addr);//应该在函数外部设置，不然要在外部重新设置一次
//    heapNode *nodeP = node;
//    newNode->next = nodeP->next;
//    newNode->head = nodeP->head;
//    nodeP->next = newNode;
//    return 0;
//}


void* safeMalloc(size_t size){
    heapNode *ptr = (heapNode *)malloc(sizeof(heapNode));
    if (ptr) {
        ptr->addr = malloc(size);
        ptr->head = ptr;
        ptr->next = NULL;
        ++DPObjectAllocCount;
        return getMultiPTR(ptr);
    }else{
        return NULL;
    }
}


void safeFree(void* ptr){
    if (ptr == NULL || notMultiPTR(((heapNode *)(ptr)))) {
        return;
    }
    ++DPObjectFreeCount;
    heapNode *nodeHeadP = (heapNode *)truePtrValue(ptr);
    free((void *)truePtrValue(nodeHeadP->addr));
#ifdef MP_DEBUG
    nodeHeadP->addr = (unsigned long)(nodeHeadP->addr) | 0x6000000000000000;
#else
    nodeHeadP->addr = NULL;
#endif
    while (nodeHeadP->next) {
        nodeHeadP = nodeHeadP->next;
#ifdef MP_DEBUG
        nodeHeadP->addr = (unsigned long)(nodeHeadP->addr) | 0x6000000000000000;
#else
        nodeHeadP->addr = NULL;
#endif
    }
}

void getPtr(void **nodeOrigin, void ** nodeOld, void *ptrNew){
    ++DPPtrTrackerCount;
    if (*nodeOrigin != NULL && !notMultiPTR(*nodeOrigin)) {
        heapNode *nodeP = truePtrValue(*nodeOrigin);
        if (abs((unsigned long)nodeP-(unsigned long)ptrNew) > 0x100000000) {
            *nodeOld = ptrNew;
        } else {
            ++DPPtrTrackeredCount;
            heapNode *newNode = malloc(sizeof(heapNode));
            newNode->next = nodeP->next;
            newNode->head = nodeP->head;
            nodeP->next = newNode;
            newNode->addr = ptrNew;
            *nodeOld = getMultiPTR(newNode);
        }
//        else {
//            printf("getPtr Error1!\n");
//            *nodeOld = ptrNew;
//            return;
//        }
    } else {
        *nodeOld = ptrNew;
    }
}

//void DPprintf(char *inf, void **p){
//    printf(inf, p);
//    printf(inf, *p);
//}
//
//void _Z8DPprintfPcPPv(char *inf, void **p){
//    printf(inf, p);
//    printf(inf, *p);
//}

