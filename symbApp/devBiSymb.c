/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
* National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
* Operator of Los Alamos National Laboratory.
* This file is distributed subject to a Software License Agreement found
* in the file LICENSE that is included with this distribution. 
\*************************************************************************/

/* $Id$ */

#include <biRecord.h>

#define epicsExportSharedSymbols
#include "devSymb.h"

static long init_record(struct biRecord *prec) {
    if (devSymbFind(&prec->inp, &prec->dpvt)) {
        recGblRecordError(S_db_badField, (void *)prec,
            "devBiSymb (init_record) Illegal NAME or INP field");
        return S_db_badField;
    }
    return 0;
}

static long read_bi(struct biRecord *prec) {
    struct vxSym *priv = (struct vxSym *) prec->dpvt;
    if (priv) {
        int lockKey = epicsInterruptLock();
        prec->val = *SYMADDR(unsigned short, priv);
        epicsInterruptUnlock(lockKey);
        prec->udf = FALSE;
        return 2; /* Don't convert */
    }
    (void)recGblSetSevr(prec, COMM_ALARM, INVALID_ALARM);
    return 1;
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
