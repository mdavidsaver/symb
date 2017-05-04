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

#include	"longoutRecord.h"

static long init_record();
static long write_longout();

/* Create the dset for devLoSymb */

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	write_longout;
}devLoSymb={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	write_longout};
epicsExportAddress( dset, devLoSymb );


static long init_record(struct longoutRecord	*prec)
{
    /* determine address of record value */
    if (devSymbFind(&prec->out, &prec->dpvt))
    {
        recGblRecordError(S_db_badField,(void *)prec,
            "devLoSymb (init_record) Illegal NAME or OUT field");
        return(S_db_badField);
    }

    return(0);
}

static long write_longout(struct longoutRecord	*prec)
{
    struct vxSym *priv = (struct vxSym *) prec->dpvt;
	int lockKey;

    if (priv)
	{
       lockKey = epicsInterruptLock();
       *SYMADDR(long, priv) = prec->val;
       epicsInterruptUnlock(lockKey);
	}
    else
       return(1);

    prec->udf=FALSE;

    return(0);
}
