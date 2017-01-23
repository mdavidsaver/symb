# Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG

DIRS := configure

DIRS += symbApp
symbApp_DEPEND_DIRS = configure

DIRS += testApp
testApp_DEPEND_DIRS = symbApp

include $(TOP)/configure/RULES_TOP
