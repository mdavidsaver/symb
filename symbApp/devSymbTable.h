#ifndef DEVSYMBTABLE_H
#define DEVSYMBTABLE_H

#include <stdlib.h>

#include <shareLib.h>

typedef enum {
    symbTypeUnknown,
} symbType;

typedef struct {
    const char *name;
    void *addr;
    size_t size; /* 0 = unknown, >0 size in bytes */
    symbType type;
} symbInfo;

epicsShareFunc const symbInfo* symbInfoByName(const char*);
epicsShareFunc const symbInfo* symbInfoByAddr(const void*);
epicsShareFunc void symbInfoFree(const symbInfo*);



typedef struct {
    size_t nsymbols;
    const symbInfo * const symbols;
} symbInfoTable;

#endif // DEVSYMBTABLE_H
