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

#define SYMTAB_EXTERN(SYMB) extern char SYMB;

#define SYMTAB_START() static const symbInfo magicSymbols[] = {
#define SYMTAB_ENTRY(SYMB)  {#SYMB, &SYMB}
#define SYMTAB_ENTRY_ALIAS(SYMB, NAME)  {NAME, &SYMB}
#define SYMTAB_END() }; const symbInfoTable magicSymbTable = { sizeof(magicSymbols)/sizeof(magicSymbols[0]), magicSymbols };

#endif // DEVSYMBTABLE_H
