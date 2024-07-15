#include "ngspice/config.h"

#include "ngspice/devdefs.h"

#include "memi1itf.h"
#include "memi1ext.h"
#include "memi1init.h"


SPICEdev MEMI1info = {
    .DEVpublic = {
        .name = "MEMI1",
		
        .description = "Meminductor ideal model ",
        .terms = &MEMI1nSize,
        .numNames = &MEMI1nSize,
        .termNames = MEMI1names,
        .numInstanceParms = &MEMI1pTSize,
        .instanceParms = MEMI1pTable,
        .numModelParms = &MEMI1mPTSize,
        .modelParms = MEMI1mPTable,
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

    .DEVparam = MEMI1param,
    .DEVmodParam = MEMI1mParam,
    
    .DEVload = MEMI1load,
    .DEVsetup = MEMI1setup,
    .DEVtrunc = MEMI1trunc,
    
    .DEVfindBranch = MEMI1findBr,	
    .DEVsetic = MEMI1getic,
    /*
    .DEVunsetup = NULL,
    .DEVpzSetup = RESsetup,
    .DEVtemperature = REStemp,

    .DEVacLoad = RESacload,
    .DEVaccept = NULL,
    .DEVdestroy = NULL,
    .DEVmodDelete = NULL,
    .DEVdelete = NULL,
    */
    .DEVask = MEMI1ask,
    .DEVmodAsk = MEMI1modAsk,
	.DEVconvTest =MEMI1convTest,

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
    .DEVinstSize = &MEMI1iSize,
    .DEVmodSize = &MEMI1mSize,

#ifdef CIDER
    .DEVdump = NULL,
    .DEVacct = NULL,
#endif
};


SPICEdev *
get_memi1_info(void)
{
    return &MEMI1info;
}
