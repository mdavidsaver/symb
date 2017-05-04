#!/usr/bin/env python
"""Generate symbInfoTable struct

./genSymTable.py out.c in.txt

input file format is lines of text with the form

  symbolName

which exports the named symbol or

  aliasName = symbolName

which exported the symbol 'symbolName', but under the name 'aliasName'
"""

import sys, re

# C identifier
_C = r'[A-Za-z_][A-Za-z0-9_]*'

# [[ ID = ] ID ] [ # comment ]

_line = re.compile(r'^\s*(?:(?:(' + _C + ')\s*=\s*)?(' + _C + '))?\s*(?:#.*)?$')

table = []
externs = set()

with open(sys.argv[2],'r') as F:
    for i,line in enumerate(F):
        M = _line.match(line)
        if M is None:
            print 'Syntax error on line', i, ' ', line
            sys.exit(1)

        alias, symbol = M.groups()
        if symbol is not None:
            externs.add(symbol)
            table.append((alias or symbol, symbol))

try:
    with open(sys.argv[1], 'w') as F:
        F.write("#include <devSymbTable.h>\n\n")

        for symbol in externs:
            F.write("extern char %s;\n"%symbol)


        F.write("\n\nstatic const symbInfo magicInfos[] = {\n")

        for alias, symbol in table:
            F.write('    {"%s", &%s},\n'%(alias, symbol))

        F.write("""};

const symbInfoTable magicSymbTable = {
    sizeof(magicInfos)/sizeof(magicInfos[0]),
    magicInfos
};
""")

except:
    os.remove(sys.argv[1])
    raise
