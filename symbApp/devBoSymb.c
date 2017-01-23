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
#include "boRecord.h"

static long init_record(struct boRecord *pbo) {
    struct vxSym *private;
    if (devSymbFind(&pbo->out, &pbo->dpvt)) {
        recGblRecordError(S_db_badField, (void *)pbo,
            "devBoSymb (init_record) Illegal NAME or OUT field");
        return S_db_badField;
    }
    private = (struct vxSym *) pbo->dpvt;
    if (private->ppvar != NULL)
        pbo->rval = *((unsigned short *)(*private->ppvar) + private->index);
    return OK;
}

static long write_bo(struct boRecord *pbo) {
    struct vxSym *private = (struct vxSym *) pbo->dpvt;
    if (private) {
        int lockKey = intLock();
        *((unsigned short *)(*private->ppvar) + private->index) = pbo->rval;
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
    DEVSUPFUN write_bo;
} devBoSymb = {
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    write_bo};
 
epicsExportAddress(dset, devBoSymb);
