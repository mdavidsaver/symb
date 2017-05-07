/*************************************************************************\
* Copyright (c) 1991-2004 The University of Chicago, as Operator of Argonne
* National Laboratory.
* Copyright (c) 1991-2003 The Regents of the University of California, as
* Operator of Los Alamos National Laboratory.
* This file is distributed subject to a Software License Agreement found
* in the file LICENSE that is included with this distribution. 
\*************************************************************************/

/* $Id$ */

#include <aoRecord.h>

#define epicsExportSharedSymbols
#include "devSymb.h"

struct ao_DSET {
    long  number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write_ao;
    DEVSUPFUN special_linconv;
};

/*
 * Double support, no conversions
 */

static long init_record(struct aoRecord *prec) {
    struct vxSym *priv;
    if (devSymbFind(&prec->out, &prec->dpvt)) {
        recGblRecordError(S_db_badField, (void *)prec,
            "devAoSymb (init_record) Illegal NAME or OUT field");
        return S_db_badField;
    }
    priv = (struct vxSym *) prec->dpvt;
    if (priv->ppvar != NULL)
        prec->val = *SYMADDR(double, priv);
    return 2; /* Don't convert */
}

static long write_ao(struct aoRecord *prec) {
    struct vxSym *priv = (struct vxSym *) prec->dpvt;
    if (priv) {
       int lockKey = epicsInterruptLock();
       *SYMADDR(double, priv) = prec->oval;
       epicsInterruptUnlock(lockKey);
       return 0;
    }
    (void)recGblSetSevr(prec, COMM_ALARM, INVALID_ALARM);
    return 1;
}

static struct ao_DSET devAoSymb = {
    6, NULL, NULL, init_record, NULL, write_ao, NULL};
epicsExportAddress(dset, devAoSymb);
