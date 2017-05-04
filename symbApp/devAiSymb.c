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

static long init_record(struct aiRecord *pai) {
    if (devSymbFind(&pai->inp, &pai->dpvt)) {
        recGblRecordError(S_db_badField, (void *)pai,
            "devAiSymb (init_record) Illegal NAME or INP field");
        return S_db_badField;
    }
    return 0;
}

static long read_ai(struct aiRecord *pai) {
    struct vxSym *priv = (struct vxSym *) pai->dpvt;
    if (priv) {
        int lockKey = epicsInterruptLock();
        pai->val = *SYMADDR(double, priv);
        epicsInterruptUnlock(lockKey);
        pai->udf = FALSE;
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

static long special_linconvLong(struct aiRecord *pai) {
    pai->eoff = pai->egul;
    pai->eslo = (pai->eguf - pai->egul) / ((double) LONG_MAX - LONG_MIN);
    return 0;
}

static long init_recordLong(struct aiRecord *pai) {
    long status = init_record(pai);
    if (!status) special_linconvLong(pai);
    return status;
}

static long read_aiLong(struct aiRecord *pai) {
    struct vxSym *priv = (struct vxSym *) pai->dpvt;
    if (priv) {
        int lockKey = epicsInterruptLock();
        pai->rval = *SYMADDR(long, priv);
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

static long special_linconvShort(struct aiRecord *pai) {
    pai->eoff = pai->egul;
    pai->eslo = (pai->eguf - pai->egul) / ((double) SHRT_MAX - SHRT_MIN);
    return 0;
}

static long init_recordShort(struct aiRecord *pai) {
    long status = init_record(pai);
    if (!status) status = special_linconvShort(pai);
    return status;
}

static long read_aiShort(struct aiRecord *pai) {
    struct vxSym *priv = (struct vxSym *) pai->dpvt;
    if (priv) {
        int lockKey = epicsInterruptLock();
        pai->rval = *SYMADDR(short, priv);
        epicsInterruptUnlock(lockKey);
        return 0; /* Convert */
    }
    return 1;
}

static struct ai_DSET devAiSymbShort = {
    6, NULL, NULL, init_recordShort, NULL, read_aiShort, special_linconvShort};

epicsExportAddress(dset, devAiSymbShort);
