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
#include <string.h>

#include "dbAccess.h"
#include "recGbl.h"
#include "devSup.h"
#include "devSymb.h"
#include "epicsExport.h"
#include "waveformRecord.h"

static int sizeofTypes[] = {MAX_STRING_SIZE,1,1,2,2,4,4,4,8,2};

static long init_record(struct waveformRecord *pwf) {
    if (devSymbFind(&pwf->inp, &pwf->dpvt)) {
        recGblRecordError(S_db_badField, (void *)pwf,
            "devWfSymb (init_record) Illegal NAME or INP field");
        return S_db_badField;
    }
    return 0;
}

static long read_wf(struct waveformRecord *pwf) {
    struct vxSym *priv = (struct vxSym *) pwf->dpvt;
    if (priv) {
        int typesize = sizeofTypes[pwf->ftvl];
        int lockKey = intLock();
        memcpy(pwf->bptr, 
               (char *)(*priv->ppvar) + typesize * priv->index, 
               pwf->nelm * typesize);
        intUnlock(lockKey);
        pwf->nord = pwf->nelm; /* We always get it all */
        return 0;
    }
    return 1;
}

static struct {
    long  number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read_wf;
}devWfSymb={
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    read_wf};

epicsExportAddress(dset, devWfSymb);
