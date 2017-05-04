
#include <epicsUnitTest.h>
#include <testMain.h>

#include "devSymb.h"

static
void testSymParse(int err, const char *inp, int isdref, const char *expectName, int expectIndex)
{
    int ret, dref = -1, idx = -1;
    const char *name = NULL;

    ret = parseInstio(inp, &dref, &name, &idx);
    if(ret==0)
        testOk(!err && dref==isdref && idx==expectIndex && name!=NULL && strcmp(name, expectName)==0,
               "parseInstio(\"%s\") -> dref %d==%d index %d=%d, name=\"%s\"==\"%s\"",
               inp, isdref, dref, expectIndex, idx, expectName, name);
    else
        testOk(err, "parseInstio(\"%s\") fails with %d", inp, err);
}

MAIN(testParse)
{
    testPlan(15);

    testSymParse(0, "foo", 0, "foo", 0);
    testSymParse(0, "  foo", 0, "foo", 0);
    testSymParse(0, "foo  ", 0, "foo", 0);
    testSymParse(0, "  foo  ", 0, "foo", 0);
    testSymParse(0, "_bar", 0, "_bar", 0);

    testSymParse(0, "*foo", 1, "foo", 0);
    testSymParse(0, "* foo", 1, "foo", 0);
    testSymParse(0, " * foo", 1, "foo", 0);

    testSymParse(0, "* foo [5]", 1, "foo", 5);
    testSymParse(0, "* foo [ 15 ]", 1, "foo", 15);
    testSymParse(0, "  foo [5]", 0, "foo", 5);
    testSymParse(0, "  foo [ 15 ]", 0, "foo", 15);

    testSymParse(1, "foo bar", 0, "foo", 0);

    testSymParse(1, "* foo [ 15 ", 1, "foo", 15);

    testSymParse(1, "* foo  15] ", 1, "foo", 15);

    return testDone();
}
