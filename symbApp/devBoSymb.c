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

#include "boRecord.h"

static long init_record(struct boRecord *pbo) {
    struct vxSym *priv;
    if (devSymbFind(&pbo->out, &pbo->dpvt)) {
        recGblRecordError(S_db_badField, (void *)pbo,
            "devBoSymb (init_record) Illegal NAME or OUT field");
        return S_db_badField;
    }
    priv = (struct vxSym *) pbo->dpvt;
    if (priv->ppvar != NULL)
        pbo->rval = *SYMADDR(unsigned short, priv);
    return 0;
}

static long write_bo(struct boRecord *pbo) {
    struct vxSym *priv = (struct vxSym *) pbo->dpvt;
    if (priv) {
        int lockKey = epicsInterruptLock();
        *SYMADDR(unsigned short, priv) = pbo->rval;
        epicsInterruptUnlock(lockKey);
        return 0;
    }
    return 1;
}

static struct {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write_bo;
} devBoSymb = {
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    write_bo};
 
epicsExportAddress(dset, devBoSymb);
