/*************************************************************************\
* Copyright (c) 1991-2004 The University of Chicago, as Operator of Argonne
* National Laboratory.
* Copyright (c) 1991-2003 The Regents of the University of California, as
* Operator of Los Alamos National Laboratory.
* This file is distributed subject to a Software License Agreement found
* in the file LICENSE that is included with this distribution. 
\*************************************************************************/

/* $Id$ */

#include <vxWorks.h>
#include <intLib.h>

#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "devSup.h"
#include "devSymb.h"
#include "epicsExport.h"
#include "aoRecord.h"

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

static long init_record(struct aoRecord *pao) {
    struct vxSym *priv;
    if (devSymbFind(&pao->out, &pao->dpvt)) {
        recGblRecordError(S_db_badField, (void *)pao,
            "devAoSymb (init_record) Illegal NAME or OUT field");
        return S_db_badField;
    }
    priv = (struct vxSym *) pao->dpvt;
    if (priv->ppvar != NULL)
        pao->val = *((double *)(*priv->ppvar) + priv->index);
    return 2; /* Don't convert */
}

static long write_ao(struct aoRecord *pao) {
    struct vxSym *priv = (struct vxSym *) pao->dpvt;
    if (priv) {
       int lockKey = intLock();
       *((double *)(*priv->ppvar) + priv->index) = pao->oval;
       intUnlock(lockKey);
       return 0;
    }
    return 1;
}

static struct ao_DSET devAoSymb = {
    6, NULL, NULL, init_record, NULL, write_ao, NULL};
epicsExportAddress(dset, devAoSymb);
