#include "epicsUnitTest.h"
#include "epicsExit.h"

int testParse(void);
int testInit(void);
int testFull(void);

void symbTestHarness(void)
{
    testHarness();

    runTest(testParse);
    runTest(testInit);
    runTest(testFull);

    epicsExit(0);
}

#ifndef vxWorks
int main(int argc, char *argv[])
{
    symbTestHarness();
    return 0;
}
#endif
