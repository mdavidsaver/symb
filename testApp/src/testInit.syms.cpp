#include <devSymbTable.h>
SYMTAB_EXTERN(foo)
SYMTAB_EXTERN(bar)
SYMTAB_EXTERN(baz)
SYMTAB_START()
SYMTAB_ENTRY(foo)
SYMTAB_ENTRY(bar)
SYMTAB_ENTRY_ALIAS(bar, "bar_alias")
SYMTAB_ENTRY(baz)
SYMTAB_END()