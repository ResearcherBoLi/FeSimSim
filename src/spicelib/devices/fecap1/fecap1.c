/**********
Copyright 2024 Xidian University
Author: 2024 Bo Li
Modified: 2024/01/27  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "fecap1defs.h"
#include "ngspice/devdefs.h"
#include "ngspice/suffix.h"
#include "ngspice/ifsim.h"

IFparm FECAP1pTable[] = { /* parameters */
    //IOPAU("ic_v",         FECAP1_IC_V,              IF_REAL, "Initial fe-capacitor voltage"),
    IOPAU("ic_p",         FECAP1_IC,              IF_REAL, "Initial fe-capacitor polarization"), 
    IOPAU("area",        	  FECAP1_AREA,           	IF_REAL, "Device area"),
    IOPU( "m",            FECAP1_M, 				IF_REAL, "multiflier"),
    /*    
    IOPAU("w",           FECAP1_WIDTH,           IF_REAL, "Device width"),
    IOPAU("l",           FECAP1_LENGTH,          IF_REAL, "Device length"),
    IOPZU("temp",        FECAP1_TEMP,            IF_REAL, "Instance operating temperature"),
    IOPZ( "dtemp",       FECAP1_DTEMP,           IF_REAL, "Instance temperature difference from the rest of the circuit"),
    IOPAPR("c",          FECAP1_CAP,       
    IOPU( "tc1",         FECAP1_TC1,             IF_REAL, "First order temp. coefficient"),
    IOPU( "tc2",         FECAP1_TC2,             IF_REAL, "Second order temp. coefficient"),
    IOP(  "bv_max",      FECAP1_BV_MAX,          IF_REAL, "maximum voltage over capacitance"),
    IOPU( "scale",       FECAP1_SCALE,           IF_REAL, "Scale factor"),
    OP(   "i",           FECAP1_CURRENT,         IF_REAL, "Device current"),
    OP(   "p",           FECAP1_POWER,           IF_REAL, "Instantaneous device power"),

    IOPAP("capacitance", FECAP1_FECAP1,             IF_REAL, "Device FECAP1acitance"),
    IOPAPR("cap",        FECAP1_FECAP1,             IF_REAL, "Device capacitance"),
          IF_REAL, "Device capacitance"),
            IF_REAL, "Parallel multiplier"),

    */
};

IFparm FECAP1mPTable[] = { /* names of model parameters */
    IP( "fecap",    FECAP1_MOD_FECAP,      IF_REAL, 		"fecapacitor device model flag"),
	IOPXU( "tnom",    FECAP1_MOD_TNOM,	    IF_REAL,		"Parameter measurement temperature"),
	//IOPP("n",		  FECAP1_MOD_N, 		IF_INTEGER, 	"N coefficient for window function"),
	IOP("Ec",	      FECAP1_MOD_Ec,		IF_REAL,		"fecapacitor model parameter Ec"), 
	IOP("Ps",	      FECAP1_MOD_Ps,		IF_REAL,		"fecapacitor model parameter Pr"), 
	IOP("rho",	      FECAP1_MOD_Rho,		IF_REAL,		"fecapacitor model parameter Rho"), 
	IOP("alpha",	  FECAP1_MOD_Alpha,		IF_REAL,		"fecapacitor model parameter Alpha"), 
	IOP("beta",	      FECAP1_MOD_Beta,		IF_REAL,		"fecapacitor model parameter Beta"), 
	IOP("gamma",	  FECAP1_MOD_Gamma,		IF_REAL,		"fecapacitor model parameter Gamma"), 	
	IOP("thick",	  FECAP1_MOD_Thickness,	IF_REAL,		"fecapacitor model parameter Thickness"), 	
    /*
    IOPA( "d",        FECAP1_MOD_Depth,       IF_REAL, "Model FeCap1 depth(i.e. thickness"),
    IOPA( "cj",     FECAP1_MOD_CJ,       IF_REAL, "Bottom Capacitance per area"),
    IOPAR( "cox",   FECAP1_MOD_CJ,       IF_REAL, "Bottom Capacitance per area"),
    IOPA( "cjsw",   FECAP1_MOD_CJSW,     IF_REAL, "Sidewall capacitance per meter"),
    IOPAR( "FECAP1sw", FECAP1_MOD_CJSW,     IF_REAL, "Sidewall capacitance per meter"),
    IOPX( "defw",   FECAP1_MOD_DEFWIDTH, IF_REAL, "Default width"),
    IOPXR( "w",     FECAP1_MOD_DEFWIDTH, IF_REAL, "Default width"),
    IOPX( "defl",   FECAP1_MOD_DEFLENGTH,IF_REAL, "Default length"),
    IOPXR( "l",     FECAP1_MOD_DEFLENGTH,IF_REAL, "Default length"),
    IOPA( "narrow", FECAP1_MOD_NARROW,   IF_REAL, "width correction factor"),
    IOPA( "short",  FECAP1_MOD_SHORT,    IF_REAL, "length correction factor"),
    IOPA( "del",    FECAP1_MOD_DEL,      IF_REAL, "length and width correction factor"),
    IOPA( "tc1",    FECAP1_MOD_TC1,      IF_REAL, "First order temp. coefficient"),
    IOPA( "tc2",    FECAP1_MOD_TC2,      IF_REAL, "Second order temp. coefficient"),
    IOPXU("tnom",   FECAP1_MOD_TNOM,     IF_REAL, "Parameter measurement temperature"),
    IOPA( "di",     FECAP1_MOD_DI,       IF_REAL, "Relative dielectric constant"),
    IOPA( "thick",  FECAP1_MOD_THICK,    IF_REAL, "Insulator thickness"),
    IOP(  "bv_max", FECAP1_MOD_BV_MAX,   IF_REAL, "maximum voltage over capacitance"),
    IP( "c",        FECAP1_MOD_C,        IF_FLAG, "Capacitor model")
    */
};

char *FECAP1names[] = {
    "C+",
    "C-"
};


int	FECAP1nSize = NUMELEMS(FECAP1names);
int	FECAP1pTSize = NUMELEMS(FECAP1pTable);
int	FECAP1mPTSize = NUMELEMS(FECAP1mPTable);
int	FECAP1iSize = sizeof(FECAP1instance);
int	FECAP1mSize = sizeof(FECAP1model);
