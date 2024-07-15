#include "ngspice/config.h"

#include "ngspice/devdefs.h"

#include "fecap1itf.h"
#include "fecap1ext.h"
#include "fecap1init.h"


SPICEdev FECAP1info = {
    .DEVpublic = {
        .name = "FECAP1",
        .description = "LK model FeCapacitor",
        .terms = &FECAP1nSize,
        .numNames = &FECAP1nSize,
        .termNames = FECAP1names,
        .numInstanceParms = &FECAP1pTSize,
        .instanceParms = FECAP1pTable,
        .numModelParms = &FECAP1mPTSize,
        .modelParms = FECAP1mPTable,
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

    .DEVparam = FECAP1param,
    .DEVmodParam = FECAP1mParam,
    .DEVload = FECAP1load,
    .DEVsetup = FECAP1setup,
    .DEVunsetup = NULL,
    /*
    //.DEVpzSetup = FECAP1setup,
    .DEVtemperature = FECAP1temp,
    .DEVpzLoad = FECAP1pzLoad,
    .DEVsenSetup = FECAP1sSetup,
    .DEVsenLoad = FECAP1sLoad,
    .DEVsenUpdate = FECAP1sUpdate,
    .DEVsenAcLoad = FECAP1sAcLoad,
    .DEVsenPrint = FECAP1sPrint,
    .DEVsenTrunc = NULL,
    .DEVdisto = NULL,
    .DEVnoise = NULL,
    .DEVsoaCheck = FECAP1soaCheck,
    */
    .DEVtrunc = FECAP1trunc,
    .DEVfindBranch = FECAP1findBr,
    .DEVacLoad = FECAP1acLoad,
    .DEVaccept = NULL,
    .DEVdestroy = NULL,
    .DEVmodDelete = NULL,
    .DEVdelete = NULL,
    .DEVsetic = FECAP1getic,
    .DEVask = FECAP1ask,
    .DEVmodAsk = FECAP1mAsk,
    .DEVconvTest = FECAP1convTest,


    .DEVinstSize = &FECAP1iSize,
    .DEVmodSize = &FECAP1mSize,

#ifdef CIDER
    .DEVdump = NULL,
    .DEVacct = NULL,
#endif
};


SPICEdev *
get_fecap1_info(void)
{
    return &FECAP1info;
}
