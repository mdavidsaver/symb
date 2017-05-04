/*************************************************************************\
* Copyright (c) 1991-2004 The University of Chicago, as Operator of Argonne
* National Laboratory.
* Copyright (c) 1991-2003 The Regents of the University of California, as
* Operator of Los Alamos National Laboratory.
* This file is distributed subject to a Software License Agreement found
* in the file LICENSE that is included with this distribution. 
\*************************************************************************/

/* $Id$ */

#include <limits.h>

#include "devSymb.h"
#include <aiRecord.h>

struct ai_DSET {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read_ai;
    DEVSUPFUN special_linconv;
};


/*
 * Double support, no conversion
 */

static long init_record(struct aiRecord *prec) {
    if (devSymbFind(&prec->inp, &prec->dpvt)) {
        recGblRecordError(S_db_badField, (void *)prec,
            "devAiSymb (init_record) Illegal NAME or INP field");
        return S_db_badField;
    }
    return 0;
}

static long read_ai(struct aiRecord *prec) {
    struct vxSym *priv = (struct vxSym *) prec->dpvt;
    if (priv) {
        int lockKey = epicsInterruptLock();
        prec->val = *SYMADDR(double, priv);
        epicsInterruptUnlock(lockKey);
        prec->udf = FALSE;
        return 2; /* Don't convert */
    }
    return 1;
}

static struct ai_DSET devAiSymb = {
    6, NULL, NULL, init_record, NULL, read_ai, NULL};
epicsExportAddress(dset, devAiSymb);


/*
 * Long integer support with conversion
 */

static long special_linconvLong(struct aiRecord *prec) {
    prec->eoff = prec->egul;
    prec->eslo = (prec->eguf - prec->egul) / ((double) LONG_MAX - LONG_MIN);
    return 0;
}

static long init_recordLong(struct aiRecord *prec) {
    long status = init_record(prec);
    if (!status) special_linconvLong(prec);
    return status;
}

static long read_aiLong(struct aiRecord *prec) {
    struct vxSym *priv = (struct vxSym *) prec->dpvt;
    if (priv) {
        int lockKey = epicsInterruptLock();
        prec->rval = *SYMADDR(long, priv);
        epicsInterruptUnlock(lockKey);
        return 0; /* Convert */
    }
    return 1;
}

static struct ai_DSET devAiSymbLong = {
    6, NULL, NULL, init_recordLong, NULL, read_aiLong, special_linconvLong};
epicsExportAddress(dset, devAiSymbLong);


/*
 * Short integer support with conversion
 */

static long special_linconvShort(struct aiRecord *prec) {
    prec->eoff = prec->egul;
    prec->eslo = (prec->eguf - prec->egul) / ((double) SHRT_MAX - SHRT_MIN);
    return 0;
}

static long init_recordShort(struct aiRecord *prec) {
    long status = init_record(prec);
    if (!status) status = special_linconvShort(prec);
    return status;
}

static long read_aiShort(struct aiRecord *prec) {
    struct vxSym *priv = (struct vxSym *) prec->dpvt;
    if (priv) {
        int lockKey = epicsInterruptLock();
        prec->rval = *SYMADDR(short, priv);
        epicsInterruptUnlock(lockKey);
        return 0; /* Convert */
    }
    return 1;
}

static struct ai_DSET devAiSymbShort = {
    6, NULL, NULL, init_recordShort, NULL, read_aiShort, special_linconvShort};

epicsExportAddress(dset, devAiSymbShort);
