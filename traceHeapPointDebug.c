#include "traceHeapPoint.h"

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

int insertNode(void *node){
    if (node == NULL || notNodePTR(((heapNode *)node)->addr)) {
        return -1;
    }
    heapNode *newNode = malloc(sizeof(heapNode));
    memset(newNode, 0, sizeof(heapNode));
    newNode->addr = getNewNodePTR(newNode->addr);//应该在函数外部设置，不然要在外部重新设置一次
    heapNode *nodeP = node;
    newNode->next = nodeP->next;
    newNode->head = nodeP->head;
    nodeP->next = newNode;
    return 0;
}

int deleteNode(void *node){
    if (node == NULL || notNodePTR(((heapNode *)node)->addr)) {
        return -1;
    }
    if (((heapNode *)node)->head == node) {
        return -1;
    }
    heapNode *nodeP = ((heapNode *)node)->head;
    while (nodeP->next != node) {
        nodeP = nodeP->next;
    }
    if (nodeP->next == NULL) {
        return -1;
    }
    nodeP->next = nodeP->next->next;
    free(node);
    return 0;
}

void* safeMalloc(size_t size){
    printf("safeMalloc1:%d\n" ,size);
    heapNode *ptr = (heapNode *)malloc(sizeof(heapNode));
    if (ptr) {
        printf("safeMalloc11 :%lx\n" ,ptr);
        ptr->addr = malloc(size);
        printf("safeMalloc12 :%lx\n" ,ptr->addr);
        ptr->addr = (void *)getNodeHeadPTR(ptr->addr);
        printf("safeMalloc13 :%lx\n" ,ptr->addr);
        ptr->head = ptr;
        printf("safeMalloc14 :%lx\n" ,ptr->head);
        ptr->next = NULL;
        printf("safeMalloc14 :%lx\n" ,ptr->next);
        ptr = (heapNode *)getMultiPTR(ptr);
        printf("safeMalloc14 :%lx\n" ,ptr);
        return ptr;
    }else{
        printf("safeMalloc2:%d\n" ,size);
        return NULL;
    }
}


void safeFree(void* ptr){
    printf("safeFree1 %lx\n",  ptr);
    if (ptr == NULL || notMultiPTR(ptr)) {
        return;
    }
    printf("safeFree2 %lx\n",  ptr);
    heapNode *nodeHeadP = (heapNode *)truePtrValue(ptr);
    printf("safeFree3 %lx\n",  nodeHeadP);
    free((void *)truePtrValue(nodeHeadP->addr));
    printf("safeFree4\n");
    nodeHeadP->addr = NULL;
    printf("safeFree5\n");
    if (nodeHeadP->next) {
         printf("safeFree6\n");
        nodeHeadP = nodeHeadP->next;
         printf("safeFree7\n");
        nodeHeadP->addr = NULL;
    }
     printf("safeFree8\n");
}

void getPtr(void **nodeOrigin, void ** nodeOld, void *ptrNew){
    printf("getPtr1 nodeOrigin:%lx\n", nodeOrigin);
    printf("getPtr1 nodeOld:%lx\n", nodeOld);
    printf("getPtr1 ptrNew:%lx\n", ptrNew);
    if (*nodeOrigin != NULL && !notMultiPTR(*nodeOrigin)) {
        heapNode *test = (heapNode *)truePtrValue(*nodeOrigin);
                          
        if (truePtrValue(test)-(unsigned long)ptrNew > 0x100000000) {
            *nodeOld = ptrNew;
            return;
        }else if (!insertNode(truePtrValue(*nodeOrigin))) {
            printf("getPtr3 nodeOrigin:%lx\n", *nodeOrigin);
            printf("getPtr3 nodeOld:%lx\n", *nodeOld);
            printf("getPtr3 ptrNew:%lx\n", ptrNew);
            heapNode *newNode = ((heapNode *)truePtrValue(*nodeOrigin))->next;
            newNode->addr = getNewNodePTR(ptrNew);
            *nodeOld = getMultiPTR(newNode);
            printf("getPtr4 newNode:%lx\n", newNode);
            printf("getPtr4 newNode->addr:%lx\n", newNode->addr);
            printf("getPtr4 *nodeOld:%lx\n", *nodeOld);
            return;
        }else{
            printf("getPtr Error1!\n");
            
            *nodeOld = ptrNew;
            return;
        }
    }
    *nodeOld = ptrNew;
    printf("getPtr5 nodeOrigin:%lx\n", nodeOrigin);
    printf("getPtr5 *nodeOld:%lx\n", *nodeOld);
    printf("getPtr5 ptrNew:%lx\n", ptrNew);
}

void DPprintf(char *inf, void **p){
    printf(inf, *p);
}

