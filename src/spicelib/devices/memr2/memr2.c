/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h" 
#include "memr2defs.h" 
#include "ngspice/devdefs.h" 
#include "ngspice/ifsim.h" 
IFparm MEMR2pTable[] = { /* parameters */ 
	OP("wfluxpseudo",		MEMR2_WFLUXPSEUDO,		IF_REAL,		"W Pseudo Flux through memristor, actural type is w width"), 
	OP("wvpseudo",			MEMR2_WVOLTPSEUDO,		IF_REAL,		"W Pseudo Terminal voltage of  w branch memristor"), 
	IOPP("rinit",			MEMR2_RINIT,			IF_REAL,		"Memristor initial Resistance"), 
	IOP("wic",				MEMR2_WINIT,			IF_REAL,		"Memristor initial width"), 
    IOP("noisetran",        MEMR2_TRNOISE,          IF_INTEGER,     "Memristor Transient noise switch"), 
 }; 

IFparm MEMR2mPTable[] = { 
	IP("memr",		MEMR2_MOD_MEMR,			IF_FLAG,		"memristor device model flag"), 
	IOPXU( "tnom",	MEMR2_MOD_TNOM,			IF_REAL,		"Parameter measurement temperature"), 
	IOPP("wf",		MEMR2_MOD_WF,		IF_INTEGER,		"Memristor window function type" ), 
	IOPP("p",		MEMR2_MOD_P,			IF_INTEGER,		"P coefficient for window function"),	
	IOP("alpha_on",		MEMR2_MOD_Alpha_on,		IF_REAL,		"memristor model parameter Alpha_on") ,
	IOP("alpha_off",		MEMR2_MOD_Alpha_off,		IF_REAL,		"memristor model parameter Alpha_off") ,
	IOP("k_on",		MEMR2_MOD_K_on,		IF_REAL,		"memristor model parameter K_on") ,
	IOP("k_off",		MEMR2_MOD_K_off,		IF_REAL,		"memristor model parameter K_off") ,
	IOP("vt_on",		MEMR2_MOD_Vt_on,		IF_REAL,		"memristor model parameter Vt_on"), 
	IOP("vt_off",		MEMR2_MOD_Vt_off,		IF_REAL,		"memristor model parameter Vt_off") ,
	IOP("r_on",		MEMR2_MOD_R_on,		IF_REAL,		"memristor model parameter R_on") ,
	IOP("r_off",		MEMR2_MOD_R_off,		IF_REAL,		"memristor model parameter R_off") ,
	IOP("d",		MEMR2_MOD_D,		IF_REAL,		"memristor model parameter D") ,
    IOP( "tnoimod", MEMR2_MOD_TNOIMOD, IF_INTEGER, "Thermal noise model selector"),
    IOP( "fnoimod", MEMR2_MOD_FNOIMOD, IF_INTEGER, "Flicker noise model selector"),
    IOP( "snoimod", MEMR2_MOD_SNOIMOD, IF_INTEGER, "Shot noise model selector"),
    IOP( "bnoimod", MEMR2_MOD_BNOIMOD, IF_INTEGER, "Burst(random telegraph) noise model selector"),
    IOP( "dnoimod", MEMR2_MOD_DNOIMOD, IF_INTEGER, "Diffusive noise model selector"),
	IOP("af",		MEMR2_MOD_AF,	   IF_REAL,		"memristor flicker noise model coefficient ") ,
	IOP("kf",		MEMR2_MOD_KF,	   IF_REAL,		"memristor flicker noise model coefficient ") ,
	IOP("ef",		MEMR2_MOD_EF,	   IF_REAL,		"memristor flicker noise model coefficient ") ,
	IOP("efh",		MEMR2_MOD_EFH,	   IF_REAL,		"memristor flicker noise model coefficient ") ,
	IOP("freq_bw",	MEMR2_MOD_FreqBW,	   IF_REAL,		"memristor transient noise frequency bandwidth") ,
	IOP("noise_timestep",	MEMR2_MOD_NoiseTimeStep,	   IF_REAL,		"memristor transient noise time step") ,
	IOP("rtn_amp",	MEMR2_MOD_RTN_AMP,	   IF_REAL,		"memristor transient RTN noise aplitude") ,
	IOP("rtn_tcapt",	MEMR2_MOD_RTN_TCAPT,	   IF_REAL,		"memristor transient RTN noise capture time") ,
	IOP("rtn_temit",	MEMR2_MOD_RTN_TEMIT,	   IF_REAL,		"memristor transient RTN noise emission time") 

}; 

char *MEMR2names[] = { 
	"N+", 
	"N-" 
}; 

int MEMR2nSize = NUMELEMS(MEMR2names); 
int MEMR2pTSize = NUMELEMS(MEMR2pTable); 
int MEMR2mPTSize = NUMELEMS(MEMR2mPTable); 
int MEMR2iSize = sizeof(MEMR2instance); 
int MEMR2mSize = sizeof(MEMR2model); 
