/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
* National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
* Operator of Los Alamos National Laboratory.
* This file is distributed subject to a Software License Agreement found
* in the file LICENSE that is included with this distribution. 
\*************************************************************************/

/* $Id$ */

#include "devSymb.h"

#include	<mbbiRecord.h>


/* Create the dset for devMbbiSymbRaw */
static long init_record();
static long read_mbbi();

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_mbbi;
}devMbbiSymbRaw={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	read_mbbi};
epicsExportAddress( dset, devMbbiSymbRaw );


static long init_record(struct mbbiRecord	*prec)
{
    /* determine address of record value */
    if (devSymbFind(prec->name, &prec->inp, &prec->dpvt))
    {
        recGblRecordError(S_db_badField,(void *)prec,
            "devMbbiSymbRaw (init_record) Illegal NAME or INP field");
        return(S_db_badField);
    }

    return(0);		
}

static long read_mbbi(struct mbbiRecord	*prec)
{
    long status;
    struct vxSym *priv = (struct vxSym *) prec->dpvt;

    if (prec->dpvt)
    {
       prec->rval = *SYMADDR(long, priv);
       status = 0;
    }
    else
       status = 1;

    return(0);
}
