 /**********
 Copyright 2020 Shanghai Jiao Tong University.	All rights reserved.
 Author: Bo Li	at MSDA Lab, School of Electronic Information and Electrical Engineering	 
 Modified: 2021/05/20  Bo Li
 Refered to NgSPICE Res/Cap related file
 **********/

#include "ngspice/ngspice.h" 
#include "memr3defs.h" 
#include "ngspice/devdefs.h" 
#include "ngspice/ifsim.h" 

IFparm MEMR3pTable[] = { /* parameters */ 
	OP("wfluxpseudo",		MEMR3_WFLUXPSEUDO,		IF_REAL,		"W Pseudo Flux through memristor, actural type is w width"), 
	OP("wvpseudo",			MEMR3_WVOLTPSEUDO,		IF_REAL,		"W Pseudo Terminal voltage of  w branch memristor"), 
	IOPP("rinit",			MEMR3_RINIT,				IF_REAL,		"Memristor initial Resistance"), 
	IOP("wic",				MEMR3_WINIT,				IF_REAL,		"Memristor initial width"), 
 }; 

IFparm MEMR3mPTable[] = { 
	IP("mem",		MEMR3_MOD_MEM,		IF_FLAG,		"memristor device model flag"), 
	IOPXU( "tnom",	MEMR3_MOD_TNOM,		IF_REAL,		"Parameter measurement temperature"), 
	IOPP("wf",		MEMR3_MOD_WF,		IF_INTEGER,		"Memristor window function type" ), 
	//IOPP("p",		MEMR3_MOD_P,		IF_INTEGER,		"P coefficient for window function"),	
	IOP("vthp",		MEMR3_MOD_Vthp,		IF_REAL,		"memristor model parameter Vthp"), 
	IOP("vthn",		MEMR3_MOD_Vthn,		IF_REAL,		"memristor model parameter Vthn"), 
	IOP("gmax",		MEMR3_MOD_Gmax,		IF_REAL,		"memristor model parameter Gmax"), 
	IOP("gmin",		MEMR3_MOD_Gmin,		IF_REAL,		"memristor model parameter Gmin"), 
	IOP("ap",		MEMR3_MOD_Ap,		IF_REAL,		"memristor model parameter Ap"), 
	IOP("an",		MEMR3_MOD_An,		IF_REAL,		"memristor model parameter An"), 
	IOP("xp",		MEMR3_MOD_Xp,		IF_REAL,		"memristor model parameter Xp"), 
	IOP("xn",		MEMR3_MOD_Xn,		IF_REAL,		"memristor model parameter Xn"), 
	IOP("b",		MEMR3_MOD_b,		IF_REAL,		"memristor model parameter B"), 
}; 

char *MEMR3names[] = { 
	"N+", 
	"N-" 
}; 

int MEMR3nSize = NUMELEMS(MEMR3names); 
int MEMR3pTSize = NUMELEMS(MEMR3pTable); 
int MEMR3mPTSize = NUMELEMS(MEMR3mPTable); 
int MEMR3iSize = sizeof(MEMR3instance); 
int MEMR3mSize = sizeof(MEMR3model); 
