
#include <stdlib.h>

#include <alarm.h>
#include <errlog.h>
#include <dbUnitTest.h>
#include <testMain.h>

#include "devSymb.h"

void testIoc_registerRecordDeviceDriver(struct dbBase *);

double test_double;

static void testInvalid(void)
{
    testDiag("Test that bad variable names result in COMM_ALARM");
    testdbGetFieldEqual("test:invalid.SEVR", DBF_LONG, INVALID_ALARM);
    testdbGetFieldEqual("test:invalid.STAT", DBF_LONG, COMM_ALARM);
}

static void testDouble(void)
{
    testDiag("test ai DTYP=\"vxWorks Variable\"");

    testdbPutFieldOk("test:ai:dbl.PROC", DBF_LONG, 1);
    testdbGetFieldEqual("test:ai:dbl", DBF_DOUBLE, 0.0);

    test_double = 42.0;
    testdbPutFieldOk("test:ai:dbl.PROC", DBF_LONG, 1);
    testdbGetFieldEqual("test:ai:dbl", DBF_DOUBLE, 42.0);

    test_double = 1000000000.0;
    testdbPutFieldOk("test:ai:dbl.PROC", DBF_LONG, 1);
    testdbGetFieldEqual("test:ai:dbl", DBF_DOUBLE, 1000000000.0);

    testdbPutFieldOk("test:ao:dbl", DBF_DOUBLE, 2000000002.0);
    testOk(test_double==2000000002.0, "%f == %f", 2000000002.0, test_double);
}

MAIN(testFull)
{
    testPlan(11);

    testOk1(test_double==0.0);

    testdbPrepare();

    testdbReadDatabase("testIoc.dbd", NULL, NULL);
    testIoc_registerRecordDeviceDriver(pdbbase);
    testdbReadDatabase("testioc.db", NULL, NULL);

    eltc(0);
    testIocInitOk();
    eltc(1);

    testInvalid();

    testDouble();

    testIocShutdownOk();

    testdbCleanup();

    return testDone();
}
