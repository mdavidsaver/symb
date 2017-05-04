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
#include <stringinRecord.h>

static long init_record(struct stringinRecord *prec) {
    if (devSymbFind(&prec->inp, &prec->dpvt)) {
        recGblRecordError(S_db_badField,(void *)prec,
            "devSiSymb (init_record) Illegal NAME or INP field");
        return S_db_badField;
    }
    return 0;
}

static long read_stringin(struct stringinRecord *prec) {
    struct vxSym *priv = (struct vxSym *) prec->dpvt;
    if (priv) {
        int lockKey = epicsInterruptLock();
        strncpy(prec->val, SYMADDR(char, priv), 39);
        epicsInterruptUnlock(lockKey);
        prec->val[39] = '\0';
        prec->udf = FALSE;
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
    DEVSUPFUN read_stringin;
} devSiSymb = {
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    read_stringin};

epicsExportAddress(dset, devSiSymb);
