 /**********
 Copyright 2020 Shanghai Jiao Tong University.	All rights reserved.
 Author: Bo Li	at MSDA Lab, School of Electronic Information and Electrical Engineering	 
 Modified: 2020/09/09  Bo Li
 Refered to NgSPICE Res/Cap related file
 **********/
 
#include "ngspice/ngspice.h" 
#include "memi1defs.h" 
#include "ngspice/devdefs.h" 
#include "ngspice/ifsim.h" 
IFparm MEMI1pTable[] = { /* parameters */ 
	OP("wfluxpseudo",		MEMI1_WFLUXPSEUDO,		IF_REAL,		"W Pseudo Flux through memristor, actural type is w width"), 
	OP("wvpseudo",			MEMI1_WVOLTPSEUDO,		IF_REAL,		"W Pseudo Terminal voltage of  w branch memristor"), 
	IOP("wic",		MEMI1_WINIT, 		IF_REAL,		"Memristor initial width"),	
	IOP("iic",				MEMI1_IC,				IF_REAL,		"initial meminductor current"), 
 }; 
 
IFparm MEMI1mPTable[] = { 
	IP("mem",		MEMI1_MOD_MEM,		IF_FLAG,		"memristor device model flag"), 
	IOPXU( "tnom",	MEMI1_MOD_TNOM,		IF_REAL,		"Parameter measurement temperature"), 
	IOPP("wf",		MEMI1_MOD_WF,		IF_INTEGER,		"Memristor window function type" ), 
	IOPP("p",		MEMI1_MOD_P,			IF_INTEGER,		"P coefficient for window function"),	
	IOP("l_min",		MEMI1_MOD_L_min,		IF_REAL,		"memristor model parameter L_min"), 
	IOP("l_max",		MEMI1_MOD_L_max,		IF_REAL,		"memristor model parameter L_max"), 
	IOP("k",		MEMI1_MOD_K,		IF_REAL,		"memristor model parameter K"), 
	IOP("d",		MEMI1_MOD_D,		IF_REAL,		"memristor model parameter D"), 
}; 

char *MEMI1names[] = { 
	"N+", 
	"N-" 
}; 

int MEMI1nSize = NUMELEMS(MEMI1names); 
int MEMI1pTSize = NUMELEMS(MEMI1pTable); 
int MEMI1mPTSize = NUMELEMS(MEMI1mPTable); 
int MEMI1iSize = sizeof(MEMI1instance); 
int MEMI1mSize = sizeof(MEMI1model); 
