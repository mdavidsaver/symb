/*************************************************************************\
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
* National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
* Operator of Los Alamos National Laboratory.
* This file is distributed subject to a Software License Agreement found
* in the file LICENSE that is included with this distribution. 
\*************************************************************************/

/* $Id$ */


/*
DESCRIPTION:

This module contains routines for testing vxWorks global variable 
device support.
*/

#include <stdlib.h>
#include <string.h>
#include "devSymb.h"
#include <epicsThread.h>
#include <iocsh.h>
#include <epicsExport.h>
#include <registryFunction.h>

/* global variables for testing */

/* long scalar */
long testLongScalar = 42;

/* long array */
long testLongArray[] = {0,1,2,3,4,5,6,7,8,9};

/* double scalar */
double testDoubleScalar = 125.0;

/* double array */
double testDoubleArray[] = {20.0,21.0,22.0,23.0,24.0,25.0,26.0,27.0,28.0,29.0};

/* string */
char testString[] = "0123456789";

/* structure and variables referencing its fields */
struct {
    long xxxx;
    double yyyy[3];
    char zzzz[80];
} testStruct = {35, {1.0,2.0,3.0}, "hello dolly"};

long *testLongPtr = &testStruct.xxxx;
double *testDoublePtr = testStruct.yyyy;
char *testStringPtr = testStruct.zzzz;

/* routines for starting and stopping auto-change of values */
int symbTestRunning = 0;

int symbTestStart() {
    int i, j;

    symbTestRunning = 1;

    for ( i = 0; symbTestRunning; i++ ) {

	/* change global data values */
 	testLongScalar++;
	for ( j = 0; j < sizeof( testLongArray ) / sizeof( long ); j++ )
	    testLongArray[j]++;
	testDoubleScalar++;
	for ( j = 0; j < sizeof( testDoubleArray ) / sizeof( double ); j++ )
	    testDoubleArray[j]++;
	for ( j = 0; j < strlen( testString ) - 1; j++ )
	    testString[j] = testString[j+1];
 	testString[j] = testString[0];
	testStruct.xxxx++;
	testStruct.yyyy[0]++;
	testStruct.yyyy[1]++;
	testStruct.yyyy[2]++;
	if ( i % 2 )
	    strcpy( testStruct.zzzz, "hello dolly" );
	else
	    strcpy( testStruct.zzzz, "dolly hello" );

	/* wait a second */
	epicsThreadSleep( 1 );
    }
    printf( "test stopped\n" );
    return 0;
}

int symbTestStop() {
    symbTestRunning = 0;
    return 0;
}

static const iocshFuncDef symbTestStartFuncDef = {"symTestStart", 0, NULL}; 
static void symbTestStartCallFunc (const iocshArgBuf *args) {symbTestStart();}

static const iocshFuncDef symbTestStopFuncDef = {"symTestStart", 0, NULL}; 
static void symbTestStopCallFunc (const iocshArgBuf *args) {symbTestStop();}

static void symbTestRegisterCommands (void) {
    iocshRegister(&symbTestStartFuncDef, symbTestStartCallFunc);
    iocshRegister(&symbTestStopFuncDef, symbTestStopCallFunc);
}

epicsExportRegistrar(symbTestRegisterCommands);
epicsExportAddress(int, symbTestRunning);
epicsExportAddress(double, testDoubleScalar);

