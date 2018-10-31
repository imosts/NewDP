#include <stdlib.h>
#include <stdio.h>

typedef struct heapNode{
    void *addr;
    struct heapNode *head;
    struct heapNode *next;
}heapNode;

extern void* safeMalloc(size_t size);
extern void* safeRealloc(void * ptr, size_t new_size);
extern void* safeCalloc(size_t numitems, size_t size);
extern void safeFree(void* ptr);
extern void getPtr(void **nodeOrigin, void ** nodeOld, void *ptrNew);
extern void traceDoublePoint(void **value, void ***ptr);
extern void DPprintf(char *inf, void **p);
