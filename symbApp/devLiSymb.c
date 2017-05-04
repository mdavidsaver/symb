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

#include "longinRecord.h"

static long init_record(struct longinRecord *prec) {
    if (devSymbFind(&prec->inp, &prec->dpvt)) {
        recGblRecordError(S_db_badField, (void *)prec,
            "devLiSymb (init_record) Illegal NAME or INP field");
        return S_db_badField;
    }
    return 0;
}

static long read_longin(struct longinRecord *prec) {
    struct vxSym *priv = (struct vxSym *) prec->dpvt;
    if (priv) {
        int lockKey = epicsInterruptLock();
        prec->val = *SYMADDR(long, priv);
        epicsInterruptUnlock(lockKey);
        return 0;
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
    DEVSUPFUN read_longin;
} devLiSymb = {
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    read_longin};

epicsExportAddress(dset, devLiSymb);
