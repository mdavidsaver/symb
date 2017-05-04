TOP=../..
include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

USR_CPPFLAGS += -I$(TOP)/symbApp

TESTPROD_HOST += testParse
testParse_SRCS += testParse.c
testParse_LIBS += Symb
testParse_LIBS += $(EPICS_BASE_IOC_LIBS)
TESTS += testParse

TESTPROD_HOST += testInit
testInit_SRCS += testInit.c
testInit_LIBS += Symb
testInit_LIBS += $(EPICS_BASE_IOC_LIBS)
TESTS += testInit

TESTSCRIPTS_HOST += $(TESTS:%=%.t)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
