/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


#include "ngspice/config.h"

#include "ngspice/devdefs.h"

#include "memr1itf.h"
#include "memr1ext.h"
#include "memr1init.h"


SPICEdev MEMR1info = {
    .DEVpublic = {
        .name = "MEMR1",
        .description = "HP linear memristor model",
        .terms = &MEMR1nSize,
        .numNames = &MEMR1nSize,
        .termNames = MEMR1names,
        .numInstanceParms = &MEMR1pTSize,
        .instanceParms = MEMR1pTable,
        .numModelParms = &MEMR1mPTSize,
        .modelParms = MEMR1mPTable,
        .flags = 0,

#ifdef XSPICE
        .cm_func = NULL,
        .num_conn = 0,
        .conn = NULL,
        .num_param = 0,
        .param = NULL,
        .num_inst_var = 0,
        .inst_var = NULL,
#endif
    },

    .DEVparam = MEMR1param,
    .DEVmodParam = MEMR1mParam,
    
    .DEVload = MEMR1load,
    .DEVsetup = MEMR1setup,
    .DEVtrunc = MEMR1trunc,
    
    .DEVfindBranch = MEMR1findBr,	
    .DEVsetic = MEMR1getic,
    .DEVacLoad = MEMR1acLoad,
    /*
    .DEVunsetup = NULL,
    .DEVpzSetup = RESsetup,
    .DEVtemperature = REStemp,


    .DEVaccept = NULL,
    .DEVdestroy = NULL,
    .DEVmodDelete = NULL,
    .DEVdelete = NULL,
    */
    .DEVask = MEMR1ask,
    .DEVmodAsk = MEMR1modAsk,
	.DEVconvTest =MEMR1convTest,

	/*
    .DEVpzLoad = RESpzLoad,

    .DEVsenSetup = RESsSetup,
    .DEVsenLoad = RESsLoad,
    .DEVsenUpdate = NULL,
    .DEVsenAcLoad = RESsAcLoad,
    .DEVsenPrint = RESsPrint,
    .DEVsenTrunc = NULL,
    .DEVdisto = NULL,
    .DEVnoise = MEMnoise,
    .DEVsoaCheck = RESsoaCheck,
    */
    .DEVinstSize = &MEMR1iSize,
    .DEVmodSize = &MEMR1mSize,

#ifdef CIDER
    .DEVdump = NULL,
    .DEVacct = NULL,
#endif
};


SPICEdev *
get_memr1_info(void)
{
    return &MEMR1info;
}
