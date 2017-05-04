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

#include "mbbiRecord.h"

struct mbbi_DSET {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read_mbbi;
};

static long init_record(struct mbbiRecord *pmbbi) {
    if (devSymbFind(&pmbbi->inp, &pmbbi->dpvt)) {
        recGblRecordError(S_db_badField, (void *)pmbbi,
            "devMbbiSymb (init_record) Illegal NAME or INP field");
        return S_db_badField;
    }
    return 0;
}

static long read_mbbi(struct mbbiRecord *pmbbi) {
    struct vxSym *priv = (struct vxSym *) pmbbi->dpvt;
    if (priv) {
        int lockKey = epicsInterruptLock();
        pmbbi->val = *((unsigned short *)(*priv->ppvar) + priv->index);
        epicsInterruptUnlock(lockKey);
        pmbbi->udf = FALSE;
        return 2; /* Don't convert */
    }
    return 1;
}

static long read_mbbiRaw(struct mbbiRecord *pmbbi) {
    struct vxSym *priv = (struct vxSym *) pmbbi->dpvt;
    if (priv) {
        int lockKey = epicsInterruptLock();
        pmbbi->rval = *((long *)(*priv->ppvar) + priv->index);
        epicsInterruptUnlock(lockKey);
        pmbbi->udf = FALSE;
        return 0; /* Convert */
    }
    return 1;
}

static struct mbbi_DSET devMbbiSymb = {
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    read_mbbi};

static struct mbbi_DSET devMbbiSymbRaw = {
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    read_mbbiRaw};

epicsExportAddress(dset, devMbbiSymb);
epicsExportAddress(dset, devMbbiSymbRaw);
