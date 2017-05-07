TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

USR_CPPFLAGS += -I$(TOP)/symbApp

TESTPROD_HOST += testParse
testParse_SRCS += testParse.c
testHarness_SRCS += testParse.c
testParse_LIBS += Symb
testParse_LIBS += $(EPICS_BASE_IOC_LIBS)
TESTS += testParse

TESTPROD_HOST += testInit
testInit_SRCS += testInit.c
testInit_SRCS += testInit.syms.cpp
testHarness_SRCS += testInit.c
testHarness_SRCS += testInit.syms.cpp
testInit_LIBS += Symb
testInit_LIBS += $(EPICS_BASE_IOC_LIBS)
TESTS += testInit

# build dset tests for >=3.15
ifdef BASE_3_15

TARGETS += $(COMMON_DIR)/testIoc.dbd
DBDDEPENDS_FILES += testIoc.dbd$(DEP)
testIoc_DBD += base.dbd
testIoc_DBD += symb.dbd
TESTFILES += $(COMMON_DIR)/testIoc.dbd

testHarness_SRCS += testIoc_registerRecordDeviceDriver.cpp

TESTPROD_HOST += testFull
testFull_SRCS += testFull.c
testFull_SRCS += testFull.syms.cpp
testFull_SRCS += testIoc_registerRecordDeviceDriver.cpp
testHarness_SRCS += testFull.c
testHarness_SRCS += testFull.syms.cpp
testFull_LIBS += Symb
testFull_LIBS += $(EPICS_BASE_IOC_LIBS)
TESTFILES += ../testIoc.db
TESTS += testFull

endif

TESTSCRIPTS_HOST += $(TESTS:%=%.t)

symbTestHarness_SRCS += $(testHarness_SRCS)
symbTestHarness_SRCS += symbTestHarness.c

symbTestHarness_LIBS += Symb
symbTestHarness_LIBS += $(EPICS_BASE_IOC_LIBS)

PROD_vxWorks += symbTestHarness
PROD_RTEMS += symbTestHarness

# external modules will place this line in configure/RELEASE
DEVSYMB = $(TOP)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
