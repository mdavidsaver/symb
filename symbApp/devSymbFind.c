/*************************************************************************\
* Copyright (c) 1991-2004 The University of Chicago, as Operator of Argonne
* National Laboratory.
* Copyright (c) 1991-2003 The Regents of the University of California, as
* Operator of Los Alamos National Laboratory.
* This file is distributed subject to a Software License Agreement found
* in the file LICENSE that is included with this distribution. 
\*************************************************************************/

/* $Id$ */

/*
DESCRIPTION:

This module contains routines for converting a vxWorks global variable
name into its address. The old approach of extracting the variable name
from the record name is no longer supported.

The supplied link field must be of type INST_IO, and its string value is
assumed to be of the form:

   [ "*" ] name [ "[" index "]" ]

where quoted items are literal and square brackets imply optional
items.  White space is ignored. The leading "*", if specified,
implies that the variable in question contains the address of the
desired data. The name is the name of the vxWorks global variable
(a leading underscore will be added if required by the particular
target architecture). The optional array index is multipled by the
data element size (e.g. sizeof(char), sizeof(long) or
sizeof(double)) and applied as an offset to the data address.

For example:

  a) "fred" refers to the value of the vxWorks global variable "fred"

  b) "*fred" refers to the value whose address is the value of the
     vxWorks global variable "fred"

  c) "fred[1]" assumes that the vxWorks global variable "fred" is an
     array and refers to its second element

  d) "*fred[1]" assumes that the vxWorks global variable "fred"
     contains the address of an array and refers to the second element
     of the array

Note that the interpretation of the "*" operator is not the same as
in C.  For example, "fred" and "fred[0]" are the same and "*fred" and
"fred[0]" are not the same.  In this version of the driver, the 
value of the pointer is read at run-time rather than initialisation.

Finally note that strings are not treated any differently from longs
or doubles in that the address returned from this routine is simply
the address of the data. "fred" is the entire string. "fred[2]"
starts at the third character of the string.

*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbDefs.h"
#include "link.h"

#ifdef vxWorks
#include "epicsDynLink.h"
#endif

#include "devSymb.h"

/* forward references */
static int parseInstio(char *string, int *deref, char **name, int *index);

/*
 * Determine vxWorks variable name and return address of data
 */
int devSymbFind(struct link *plink, void *pdpvt)
{
    int  deref;
    char *nptr;
    int  index;
    SYM_TYPE stype;
    void *paddr;
    struct vxSym *priv;
    struct vxSym **pprivate = (struct vxSym **) pdpvt;
    struct instio *pinstio;

    /* link must be of type INST_IO */
    if (plink->type != INST_IO)
    return 1;

    /* parse INST_IO string */
    pinstio = (struct instio *) &plink->value.instio;
    if (parseInstio(pinstio->string, &deref, &nptr, &index))
    return 1;

    if (symFindByNameEPICS(sysSymTbl, nptr, (char **) &paddr, &stype))
    return 1;

    /* Name exists, allocate a priv structure */
    priv = (struct vxSym *) malloc(sizeof (struct vxSym));
    if (priv == NULL)
        return 1;
    
    /* Fill in the fields */
    priv->index = index;
    
    /* Setup pointers to the found symbol address */
    if (deref) {
    priv->ppvar = paddr;
    /* priv->pvar is not needed with deref symbols */
    } else {
        priv->ppvar = &priv->pvar;
        priv->pvar = paddr;
    }

    /* Pass new priv structure back to caller */
    *pprivate = priv;

    return 0;
}

/*
 * Parse string of the form ["*"]name["["index"]"] (white space is ignored).
 */
static int parseInstio(char *string, int *deref, char **name, int *index)
{
    static char pname[256];
    char *begin;

    /* set default return values */
    *deref = 0;
    *name  = pname;
    *index = 0;

    /* skip leading white space */
    for (; isspace(*string); string++);

    /* if next char is "*", will dereference */
    if (*string == '*')
    {
	*deref = 1;
	string++;
    }

    /* skip white space */
    for (; isspace(*string); string++);

    /* variable name begins here */
    begin = string;

    /* search for white space or "[" */
    for (; *string && !isspace(*string) && *string != '['; string++);

    /* copy and terminate variable name */
    strncpy(pname, begin, string-begin);
    pname[string-begin] = '\0';

    /* skip white space */
    for (; isspace(*string); string++);

    /* if found "[", parse index */
    if (*string == '[')
    {
	string++;

	for (; isspace(*string); string++);

	for(; isdigit(*string); string++)
	{
	    *index *= 10;
	    *index += *string - '0';
	}

	for (; isspace(*string); string++);

	if (*string != ']')
	{
	    printf("no trailing ]\n");
        return 1;
	}

	string++;
    }

    /* skip trailing white space */
    for (; isspace(*string); string++);

    /* expect to be at end of string */
    if (*string != '\0')
    {
	printf("unexpected trailing characters\n");
    return 1;
    }

    return 0;
}

#ifdef TEST
/*
 * Optional routine for testing parsing routines
 */
int test(char *string)
{
    int  error;
    int  deref;
    char *nptr;
    int  index;
    char *addr;
    SYM_TYPE stype;

    printf( "instio: %s -> ", string);
    error = parseInstio(string, &deref, &nptr, &index);
    printf("%s: ", error ? "error" : "ok" );
    printf("deref=%d, name=%s, index=%d", deref, nptr, index);
    if (!symFindByNameEPICS(sysSymTbl, nptr, &addr, &stype))
    {
	if (deref) addr = *((char **)addr);
	addr += sizeof(long) * index;
	printf(" -> value = %d", *(long *)addr);
    }
    printf("\n");

    return 0;
}
#endif /* TEST */
