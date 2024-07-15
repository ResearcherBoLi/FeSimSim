 /**********
 Copyright 2020 Shanghai Jiao Tong University.	All rights reserved.
 Author: Bo Li	at MSDA Lab, School of Electronic Information and Electrical Engineering	 
 Modified: 2020/09/09  Bo Li
 Refered to NgSPICE Res/Cap related file
 **********/


#include "ngspice/ngspice.h"
#include "memc1defs.h"
#include "ngspice/devdefs.h"
#include "ngspice/ifsim.h"

IFparm MEMC1pTable[] = { /* parameters */
    OP(   "wfluxpseudo", 	MEMC1_WFLUXPSEUDO,	      IF_REAL, 		"W Pseudo Flux through memristor, actural type is w width"),
    OP(   "wvpseudo",         MEMC1_WVOLTPSEUDO,		     IF_REAL, 		"W Pseudo Terminal voltage of  w branch memristor"),
	//IOPP("ic",		MEM_WINIT, 		IF_REAL,		"Memristor initial width"),
	IOP("wic",		MEMC1_WINIT, 		IF_REAL,		"Memcapacitor initial width"),		
	IOPAU("vic", 		MEMC1_IC,			IF_REAL,		"Initial memcapacitor voltage")


};

IFparm MEMC1mPTable[] = { /* model parameters */
	//IOP("ron", 		MEM_MOD_RON,		IF_REAL,	"memristor on state resistance"),
	//IOP("roff",		MEM_MOD_ROFF,		IF_REAL,	"memristor off state resistance"),
	//IOP("mu",		MEM_MOD_MU,			IF_REAL,	"memristor mobility coefficient"),
	//IOP("d",		MEM_MOD_DEPTH,		IF_REAL,	"memristor device depth of specific process"),
	
	IP("memc",   	MEMC1_MOD_MEMC, 		IF_FLAG,	"memcapacitor device model flag"),
	IOPXU( "tnom",  MEMC1_MOD_TNOM,	    IF_REAL,	"Parameter measurement temperature"),
	//IOP("wf",		MEM1_MOD_WF,		IF_REAL,	"memristor default window function"),
	IOPP("wf",		MEMC1_MOD_Wf,		IF_INTEGER,		"Memcapacitor window function type" ),
	IOPP("p",		MEMC1_MOD_P, 		IF_INTEGER, 	"P coefficient for window function"),

	IOP("c_min",	MEMC1_MOD_C_min,		IF_REAL,		"memcapacitor model parameter C_min"), 
	IOP("c_max",	MEMC1_MOD_C_max,		IF_REAL,		"memcapacitor model parameter C_max"), 
	IOP("k",		MEMC1_MOD_K,		IF_REAL,		"memcapacitor model parameter K"), 
	IOP("d",		MEMC1_MOD_D,		IF_REAL,		"memcapacitor model parameter D")

};

char *MEMC1names[] = {
    "N+",
    "N-"
};

int	MEMC1nSize = NUMELEMS(MEMC1names);
int	MEMC1pTSize = NUMELEMS(MEMC1pTable);
int	MEMC1mPTSize = NUMELEMS(MEMC1mPTable);
int	MEMC1iSize = sizeof(MEMC1instance);
int	MEMC1mSize = sizeof(MEMC1model);
