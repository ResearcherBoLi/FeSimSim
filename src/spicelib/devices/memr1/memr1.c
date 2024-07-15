/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab  
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

/*
add supporting tran noise simulation
*/

#include "ngspice/ngspice.h"
#include "memr1defs.h"
#include "ngspice/devdefs.h"
#include "ngspice/ifsim.h"

IFparm MEMR1pTable[] = { /* parameters */
    OP(   "wfluxpseudo", 	MEMR1_WFLUXPSEUDO,	      IF_REAL, 		"W Pseudo Flux through memristor, actural type is w width"),
    OP(   "wvpseudo",         MEMR1_WVOLTPSEUDO,		     IF_REAL, 		"W Pseudo Terminal voltage of  w branch memristor"),
	IOPP("rinit",	MEMR1_RINIT, 		IF_REAL,		"Memristor initial Resistance"),
	//IOPP("ic",		MEM_WINIT, 		IF_REAL,		"Memristor initial width"),
	IOP("wic",		MEMR1_WINIT, 		IF_REAL,		"Memristor initial width"),		
	IOPP("p",		MEMR1_P, 		IF_INTEGER, 	"P coefficient for window function"),
	IOPP("wf",		MEMR1_WF,			IF_INTEGER,		"Memristor window function type" ),
    IOP("noisetran", MEMR1_TRNOISE,     IF_INTEGER,     "Memristor Transient noise switch")

};

IFparm MEMR1mPTable[] = { /* model parameters */
	IOP("ron", 		MEMR1_MOD_RON,		IF_REAL,	"memristor on state resistance"),
	IOP("roff",		MEMR1_MOD_ROFF,		IF_REAL,	"memristor off state resistance"),
	IOP("mu",		MEMR1_MOD_MU,			IF_REAL,	"memristor mobility coefficient"),
	IOP("d",		MEMR1_MOD_DEPTH,		IF_REAL,	"memristor device depth of specific process"),
	IP("memr",   	MEMR1_MOD_MEMR,		IF_FLAG,	"memristor device model flag"),
	IOPXU( "tnom",  MEMR1_MOD_TNOM,	    IF_REAL,	"Parameter measurement temperature"),
	IOP("wf",		MEMR1_MOD_WF,			IF_REAL,	"memristor default window function"),
    IOP( "tnoimod", MEMR1_MOD_TNOIMOD, IF_INTEGER, "Thermal noise model selector"),
    IOP( "fnoimod", MEMR1_MOD_FNOIMOD, IF_INTEGER, "Flicker noise model selector"),
    IOP( "snoimod", MEMR1_MOD_SNOIMOD, IF_INTEGER, "Shot noise model selector"),
    IOP( "bnoimod", MEMR1_MOD_BNOIMOD, IF_INTEGER, "Burst(random telegraph) noise model selector"),
    IOP( "dnoimod", MEMR1_MOD_DNOIMOD, IF_INTEGER, "Diffusive noise model selector"),
	IOP("af",		MEMR1_MOD_AF,	   IF_REAL,		"memristor flicker noise model coefficient ") ,
	IOP("kf",		MEMR1_MOD_KF,	   IF_REAL,		"memristor flicker noise model coefficient ") ,
	IOP("ef",		MEMR1_MOD_EF,	   IF_REAL,		"memristor flicker noise model coefficient ") ,
	IOP("efh",		MEMR1_MOD_EFH,	   IF_REAL,		"memristor flicker noise model coefficient ") ,
	IOP("freq_bw",	MEMR1_MOD_FreqBW,	   IF_REAL,		"memristor transient noise frequency bandwidth") ,
	IOP("noise_timestep",	MEMR1_MOD_NoiseTimeStep,	   IF_REAL,		"memristor transient noise time step") ,
	IOP("rtn_amp",	MEMR1_MOD_RTN_AMP,	   IF_REAL,		"memristor transient RTN noise aplitude") ,
	IOP("rtn_tcapt",	MEMR1_MOD_RTN_TCAPT,	   IF_REAL,		"memristor transient RTN noise capture time") ,
	IOP("rtn_temit",	MEMR1_MOD_RTN_TEMIT,	   IF_REAL,		"memristor transient RTN noise emission time") 

};

char *MEMR1names[] = {
    "N+",
    "N-"
};

int	MEMR1nSize = NUMELEMS(MEMR1names);
int	MEMR1pTSize = NUMELEMS(MEMR1pTable);
int	MEMR1mPTSize = NUMELEMS(MEMR1mPTable);
int	MEMR1iSize = sizeof(MEMR1instance);
int	MEMR1mSize = sizeof(MEMR1model);
