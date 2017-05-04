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

#include	"mbboRecord.h"

static long init_record();
static long write_mbbo();

/* Create the dset for devMbboSymb */

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	write_mbbo;
}devMbboSymb={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	write_mbbo};
epicsExportAddress( dset, devMbboSymb );
 

static long init_record(struct mbboRecord	*pmbbo)
{
    /* determine address of record value */
    if (devSymbFind(pmbbo->name, &pmbbo->out, &pmbbo->dpvt))
    {
        recGblRecordError(S_db_badField,(void *)pmbbo,
            "devMbboSymb (init_record) Illegal NAME or OUT field");
        return(S_db_badField);
    }

    return(0);		
}

static long write_mbbo(struct mbboRecord	*pmbbo)
{
    int lockKey;
    struct vxSym *priv = (struct vxSym *) pmbbo->dpvt;
    
    if (priv)
    {
        lockKey = epicsInterruptLock();
        *((long *)(*priv->ppvar) + priv->index) = pmbbo->val;
        epicsInterruptUnlock(lockKey);
    }
    else
        return(1);

    pmbbo->udf=FALSE;

    return(0);
}
