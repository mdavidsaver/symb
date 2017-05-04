#ifndef DEVSYMB_H
#define DEVSYMB_H

/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
* National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
* Operator of Los Alamos National Laboratory.
* This file is distributed subject to a Software License Agreement found
* in the file LICENSE that is included with this distribution. 
\*************************************************************************/

/* $Id$ */

#include	<stdio.h>
#include	<string.h>

#ifdef epicsExportSharedSymbols
#   define devSymb_epicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include	<dbDefs.h>
#include	<dbAccess.h>
#include	<recGbl.h>
#include	<devSup.h>
#include    <epicsInterrupt.h>
#include	<epicsExport.h>

#ifdef devSymb_epicsExportSharedSymbols
#   define epicsExportSharedSymbols
#   include <shareLib.h>
#endif


/* This is the device priv structure */

struct vxSym {
    void **ppvar;
    void *pvar;
    long index;
};

/* Fetch address as TYPE* */
#define SYMADDR(TYPE, PSYM) ((TYPE*)(*(PSYM)->ppvar) + (PSYM)->index)

/* This is the call to create it */

epicsShareFunc int devSymbFind(struct link *plink, void **pdpvt);
epicsShareFunc int parseInstio(const char *string, int *deref, const char **name, int *index);

#endif /* DEVSYMB_H */
