/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
* National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
* Operator of Los Alamos National Laboratory.
* This file is distributed subject to a Software License Agreement found
* in the file LICENSE that is included with this distribution. 
\*************************************************************************/

/* $Id$ */

#include	"dbDefs.h"
#include	"dbAccess.h"
#include	"recGbl.h"
#include	"devSup.h"
#include	"devSymb.h"
#include	"epicsExport.h"
#include	"mbboRecord.h"

static long init_record();
static long write_mbbo();

/* Create the dset for devMbboSymbRaw */

struct {
	long		number;
	DEVSUPFUN	report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record;
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	write_mbbo;
}devMbboSymbRaw={
	5,
	NULL,
	NULL,
	init_record,
	NULL,
	write_mbbo};

epicsExportAddress(dset, devMbboSymbRaw);


static long init_record(struct mbboRecord	*prec)
{
    /* determine address of record value */
    if (devSymbFind(&prec->out, &prec->dpvt))
    {
        recGblRecordError(S_db_badField,(void *)prec,
            "devMbboSymbRaw (init_record) Illegal NAME or OUT field");
        return(S_db_badField);
    }

    return(0);		
}

static long write_mbbo(struct mbboRecord	*prec)
{
    struct vxSym *priv = (struct vxSym *) prec->dpvt;
    
    if (priv)
       *SYMADDR(long, priv) = prec->rval;
    else {
        (void)recGblSetSevr(prec, COMM_ALARM, INVALID_ALARM);
        return 1;
    }

    prec->udf=FALSE;

    return(0);
}
