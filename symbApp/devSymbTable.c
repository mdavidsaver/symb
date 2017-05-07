
#include <string.h>

#include <dbDefs.h>

#define epicsExportSharedSymbols
#include "devSymbTable.h"

static ELLLIST allSymbTables = ELLLIST_INIT;

void symbInstallTable(symbInfoTable *table)
{
    ELLNODE *cur;

    if(!table) return;

    for(cur = ellFirst(&allSymbTables); cur; cur = ellNext(cur)) {
        symbInfoTable *T = CONTAINER(cur, symbInfoTable, node);
        if(T==table)
            return;
    }

    ellAdd(&allSymbTables, &table->node);
}


const symbInfo* symbInfoByName(const char* name)
{
    ELLNODE *cur;

    for(cur = ellFirst(&allSymbTables); cur; cur = ellNext(cur)) {
        symbInfoTable *table = CONTAINER(cur, symbInfoTable, node);
        size_t i;

        if(!table->symbols) continue;

        for(i=0; i<table->nsymbols; i++) {
            if(strcmp(table->symbols[i].name, name)==0)
                return &table->symbols[i];
        }
    }

    return NULL;
}

const symbInfo* symbInfoByAddr(const void* addr)
{
    ELLNODE *cur;

    for(cur = ellFirst(&allSymbTables); cur; cur = ellNext(cur)) {
        symbInfoTable *table = CONTAINER(cur, symbInfoTable, node);
        size_t i;

        if(!table->symbols) continue;

        for(i=0; i<table->nsymbols; i++) {
            if(table->symbols[i].addr==addr)
                return &table->symbols[i];
        }
    }

    return NULL;
}

void symbInfoFree(const symbInfo* info) {}
