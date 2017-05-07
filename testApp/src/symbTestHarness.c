#include "epicsUnitTest.h"
#include "epicsVersion.h"
#include "epicsExit.h"

#ifndef VERSION_INT
#  define VERSION_INT(V,R,M,P) ( ((V)<<24) | ((R)<<16) | ((M)<<8) | (P))
#  define EPICS_VERSION_INT VERSION_INT(EPICS_VERSION, EPICS_REVISION, EPICS_MODIFICATION, EPICS_PATCH_LEVEL)
#endif

int testParse(void);
int testInit(void);
#if EPICS_VERSION_INT>=VERSION_INT(3,15,0,0)
int testFull(void);
#endif

void symbTestHarness(void)
{
    testHarness();

    runTest(testParse);
    runTest(testInit);
#if EPICS_VERSION_INT>=VERSION_INT(3,15,0,0)
    runTest(testFull);
#endif

    epicsExit(0);
}

#ifndef vxWorks
int main(int argc, char *argv[])
{
    symbTestHarness();
    return 0;
}
#endif
