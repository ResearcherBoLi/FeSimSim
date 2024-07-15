#include "ngspice/config.h"

#include "ngspice/devdefs.h"

#include "memr3itf.h"
#include "memr3ext.h"
#include "memr3init.h"


SPICEdev MEMR3info = {
    .DEVpublic = {
        .name = "MEMR3",
		
        .description = "Yakopcic memristor model ",
        .terms = &MEMR3nSize,
        .numNames = &MEMR3nSize,
        .termNames = MEMR3names,
        .numInstanceParms = &MEMR3pTSize,
        .instanceParms = MEMR3pTable,
        .numModelParms = &MEMR3mPTSize,
        .modelParms = MEMR3mPTable,
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

    .DEVparam = MEMR3param,
    .DEVmodParam = MEMR3mParam,
    
    .DEVload = MEMR3load,
    .DEVsetup = MEMR3setup,
    .DEVtrunc = MEMR3trunc,
    
    .DEVfindBranch = MEMR3findBr,	
    .DEVsetic = MEMR3getic,
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
    .DEVask = MEMR3ask,
    .DEVmodAsk = MEMR3modAsk,
	.DEVconvTest =MEMR3convTest,

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
    .DEVinstSize = &MEMR3iSize,
    .DEVmodSize = &MEMR3mSize,

#ifdef CIDER
    .DEVdump = NULL,
    .DEVacct = NULL,
#endif
};


SPICEdev *
get_memr3_info(void)
{
    return &MEMR3info;
}
