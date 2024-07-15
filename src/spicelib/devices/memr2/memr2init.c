/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


#include "ngspice/config.h"
#include "ngspice/devdefs.h"

#include "memr2itf.h"
#include "memr2ext.h"
#include "memr2init.h"


SPICEdev MEMR2info = {
    .DEVpublic = {
        .name = "MEMR2",
		
        .description = "VTEAM memristor model ",
        .terms = &MEMR2nSize,
        .numNames = &MEMR2nSize,
        .termNames = MEMR2names,
        .numInstanceParms = &MEMR2pTSize,
        .instanceParms = MEMR2pTable,
        .numModelParms = &MEMR2mPTSize,
        .modelParms = MEMR2mPTable,
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

    .DEVparam = MEMR2param,
    .DEVmodParam = MEMR2mParam,
    
    .DEVload = MEMR2load,
    .DEVsetup = MEMR2setup,
    .DEVtrunc = MEMR2trunc,
    
    .DEVfindBranch = MEMR2findBr,	
    .DEVsetic = MEMR2getic,
    .DEVacLoad = MEMR2acLoad,
    
    /*
    .DEVunsetup = NULL,
    .DEVpzSetup = RESsetup,
    .DEVtemperature = REStemp,


    .DEVaccept = NULL,
    .DEVdestroy = NULL,
    .DEVmodDelete = NULL,
    .DEVdelete = NULL,
    */
    .DEVask = MEMR2ask,
    .DEVmodAsk = MEMR2modAsk,
	.DEVconvTest =MEMR2convTest,
    .DEVnoise = MEMR2noise,
	/*
    .DEVpzLoad = RESpzLoad,

    .DEVsenSetup = RESsSetup,
    .DEVsenLoad = RESsLoad,
    .DEVsenUpdate = NULL,
    .DEVsenAcLoad = RESsAcLoad,
    .DEVsenPrint = RESsPrint,
    .DEVsenTrunc = NULL,
    .DEVdisto = NULL,
    
    .DEVsoaCheck = RESsoaCheck,
    */
    .DEVinstSize = &MEMR2iSize,
    .DEVmodSize = &MEMR2mSize,

#ifdef CIDER
    .DEVdump = NULL,
    .DEVacct = NULL,
#endif
};


SPICEdev *
get_memr2_info(void)
{
    return &MEMR2info;
}
