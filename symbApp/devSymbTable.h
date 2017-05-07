#ifndef DEVSYMBTABLE_H
#define DEVSYMBTABLE_H

#include <stdlib.h>

#ifdef epicsExportSharedSymbols
#   define devSymbTable_epicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <ellLib.h>

#ifdef devSymbTable_epicsExportSharedSymbols
#   define epicsExportSharedSymbols
#   include <shareLib.h>
#endif

#include <shareLib.h>

#ifdef __cplusplus
extern "C" {
#endif

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
    ELLNODE node;
    const size_t nsymbols;
    const symbInfo * const symbols;
} symbInfoTable;

epicsShareFunc void symbInstallTable(symbInfoTable *table);

#define SYMTAB_EXTERN(SYMB) extern char SYMB;

#define SYMTAB_START() static const symbInfo magicSymbols[] = {
#define SYMTAB_ENTRY(SYMB)  {#SYMB, &SYMB},
#define SYMTAB_ENTRY_ALIAS(SYMB, NAME)  {NAME, &SYMB},
#define SYMTAB_END() }; static symbInfoTable magicSymbTable = { ELLNODE_INIT, sizeof(magicSymbols)/sizeof(magicSymbols[0]), magicSymbols }; \
    namespace {struct tableLoader {tableLoader() {symbInstallTable(&magicSymbTable);}} theLoader;}

#ifdef __cplusplus
}
#endif

#endif // DEVSYMBTABLE_H
