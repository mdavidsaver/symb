
#include <string.h>

#define epicsExportSharedSymbols
#include "devSymbTable.h"

extern const symbInfoTable magicSymbTable;

const symbInfo* symbInfoByName(const char* name)
{
    size_t i;
    const symbInfoTable * const table = &magicSymbTable;

    if(!table->symbols) return NULL;

    for(i=0; i<table->nsymbols; i++) {
        if(strcmp(table->symbols[i].name, name)==0)
            return &table->symbols[i];
    }

    return NULL;
}

const symbInfo* symbInfoByAddr(const void* addr)
{
    size_t i;
    const symbInfoTable * const table = &magicSymbTable;

    if(!table->symbols) return NULL;

    for(i=0; i<table->nsymbols; i++) {
        if(table->symbols[i].addr==addr)
            return &table->symbols[i];
    }

    return NULL;
}

void symbInfoFree(const symbInfo* info) {}


const symbInfoTable magicSymbTable __attribute__((weak));
