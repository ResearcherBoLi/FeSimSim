#include "ngspice/config.h"
#include "ngspice/devdefs.h"
#include "memc1itf.h"
#include "memc1ext.h"
#include "memc1init.h"


SPICEdev MEMC1info = {
    .DEVpublic = {
        .name = "MEMC1",
        .description = "Memcapacitor Ideal Model ",
        .terms = &MEMC1nSize,
        .numNames = &MEMC1nSize,
        .termNames = MEMC1names,
        .numInstanceParms = &MEMC1pTSize,
        .instanceParms = MEMC1pTable,
        .numModelParms = &MEMC1mPTSize,
        .modelParms = MEMC1mPTable,
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

    .DEVparam = MEMC1param,
    .DEVmodParam = MEMC1mParam,
    
    .DEVload = MEMC1load,
    .DEVsetup = MEMC1setup,
    .DEVtrunc = MEMC1trunc,
    
    .DEVfindBranch = MEMC1findBr,	
    .DEVsetic = MEMC1getic,
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
    .DEVask = MEMC1ask,
    .DEVmodAsk = MEMC1modAsk,
	.DEVconvTest =MEMC1convTest,

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
    .DEVinstSize = &MEMC1iSize,
    .DEVmodSize = &MEMC1mSize,

#ifdef CIDER
    .DEVdump = NULL,
    .DEVacct = NULL,
#endif
};


SPICEdev *
get_memc1_info(void)
{
    return &MEMC1info;
}
