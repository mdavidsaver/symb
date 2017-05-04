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

#include	<dbDefs.h>
#include	<dbAccess.h>
#include	<recGbl.h>
#include	<devSup.h>
#include    <epicsInterrupt.h>
#include	<epicsExport.h>


/* This is the device priv structure */

struct vxSym {
    void **ppvar;
    void *pvar;
    long index;
};


/* This is the call to create it */

extern int devSymbFind(struct link *plink, void **pdpvt);

#endif /* DEVSYMB_H */
