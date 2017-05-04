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
#include "longinRecord.h"

static long init_record(struct longinRecord *plongin) {
    if (devSymbFind(&plongin->inp, &plongin->dpvt)) {
        recGblRecordError(S_db_badField, (void *)plongin,
            "devLiSymb (init_record) Illegal NAME or INP field");
        return S_db_badField;
    }
    return OK;
}

static long read_longin(struct longinRecord *plongin) {
    struct vxSym *priv = (struct vxSym *) plongin->dpvt;
    if (priv) {
        int lockKey = intLock();
        plongin->val = *((long *)(*priv->ppvar) + priv->index);
        intUnlock(lockKey);
        return OK;
    }
    return ERROR;
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
