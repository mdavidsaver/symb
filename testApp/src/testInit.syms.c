#include <devSymbTable.h>
SYMTAB_EXTERN(bar)
SYMTAB_START()
//SYMTAB_ENTRY(bar)
SYMTAB_ENTRY_ALIAS(bar, "bar_alias")
SYMTAB_END()
