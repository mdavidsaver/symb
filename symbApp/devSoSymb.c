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

#include <stringoutRecord.h>

static long init_record();
static long write_stringout();

/* Create the dset for devSoSymb */

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	write_stringout;
}devSoSymb={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	write_stringout};
epicsExportAddress( dset, devSoSymb );
 

static long init_record(struct stringoutRecord	*pstringout)
{
    /* determine address of record value */
    if (devSymbFind(&pstringout->out, &pstringout->dpvt))
    {
        recGblRecordError(S_db_badField,(void *)pstringout,
            "devSoSymb (init_record) Illegal NAME or OUT field");
        return(S_db_badField);
    }

    return(0);
}

static long write_stringout(struct stringoutRecord	*pstringout)
{
	int lockKey;
    struct vxSym *priv = (struct vxSym *) pstringout->dpvt;

    if (priv)
    {
        pstringout->val[39] = '\0';
        lockKey = epicsInterruptLock();
        strcpy((char *)(*priv->ppvar) + priv->index, pstringout->val);
        epicsInterruptUnlock(lockKey);
    }
    else 
        return(1);

    pstringout->udf = FALSE;

    return(0);
}
