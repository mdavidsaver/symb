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

#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "devSup.h"
#include "stringinRecord.h"
#include "epicsExport.h"
#include "devSymb.h"

static long init_record(struct stringinRecord *pstringin) {
    if (devSymbFind(&pstringin->inp, &pstringin->dpvt)) {
        recGblRecordError(S_db_badField,(void *)pstringin,
            "devSiSymb (init_record) Illegal NAME or INP field");
        return S_db_badField;
    }
    return 0;
}

static long read_stringin(struct stringinRecord *pstringin) {
    struct vxSym *priv = (struct vxSym *) pstringin->dpvt;
    if (priv) {
        int lockKey = intLock();
        strncpy(pstringin->val, (char *)(*priv->ppvar) + priv->index, 39);
        intUnlock(lockKey);
        pstringin->val[39] = '\0';
        pstringin->udf = FALSE;
        return 0;
    }
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
