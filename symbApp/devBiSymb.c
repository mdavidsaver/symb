/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
* National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
* Operator of Los Alamos National Laboratory.
* This file is distributed subject to a Software License Agreement found
* in the file LICENSE that is included with this distribution. 
\*************************************************************************/

/* $Id$ */

#include <vxWorks.h>
#include <intLib.h>

#include "dbAccess.h"
#include "recGbl.h"
#include "devSup.h"
#include "devSymb.h"
#include "epicsExport.h"
#include "biRecord.h"

static long init_record(struct biRecord *pbi) {
    if (devSymbFind(&pbi->inp, &pbi->dpvt)) {
        recGblRecordError(S_db_badField, (void *)pbi,
            "devBiSymb (init_record) Illegal NAME or INP field");
        return S_db_badField;
    }
    return OK;
}

static long read_bi(struct biRecord *pbi) {
    struct vxSym *priv = (struct vxSym *) pbi->dpvt;
    if (priv) {
        int lockKey = intLock();
        pbi->val = *((unsigned short *)(*priv->ppvar) + priv->index);
        intUnlock(lockKey);
        pbi->udf = FALSE;
        return 2; /* Don't convert */
    }
    return ERROR;
}

static struct {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read_bi;
} devBiSymb = {
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    read_bi};

epicsExportAddress(dset, devBiSymb);
