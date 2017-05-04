
#include <stdlib.h>

#include <epicsUnitTest.h>
#include <testMain.h>

#include "devSymb.h"

/* tests assume all are int */
int foo;
int bar;

int *baz;

int noImagination[2];

static void testFind(const char *inp, const void *expectAddr)
{
    struct link fakelink;
    struct vxSym *pvt = NULL;
    int ret;

    fakelink.type = INST_IO;
    fakelink.value.instio.string = (char*)inp;

    ret = devSymbFind(&fakelink, (void**)&pvt);

    if(ret) {
        testFail("devSymbFind(\"%s\") -> error %d", inp, ret);
    } else {
        int *actual = SYMADDR(int, pvt);
        testOk(actual==expectAddr, "devSymbFind(\"%s\") -> %p==%p", inp, expectAddr, actual);
    }

    free(pvt);
}

MAIN(testInit)
{
    testPlan(10);

    /* make direct use of test globals to ensure that the linker
     * keeps them
     */
    testOk1(foo==0 && bar==0.0 && baz==NULL && noImagination[0]==0.0);

    baz = calloc(2, sizeof(*baz));
    if(!baz)
        testAbort("alloc baz fails");

    testFind("foo", &foo);
    testFind("bar", &bar);

    testFind("foo[0]", &foo);
    testFind("baz", &baz);
    testFind("baz[0]", &baz);
    testFind("*baz", &baz[0]);
    testFind("*baz[0]", &baz[0]);
    testFind("*baz[1]", &baz[1]);

    testDiag("lookup aliased symbol w/ symbInfoByName()");
    testFind("bar_alias", &bar);

    free(baz);

    return testDone();
}
