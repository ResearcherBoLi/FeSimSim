#include "ngspice/config.h"

#include "ngspice/devdefs.h"

#include "fefet1itf.h"
#include "fefet1ext.h"
#include "fefet1init.h"


SPICEdev FEFET1info = {
    .DEVpublic = {
        .name = "FEFET1",
        .description = "Berkeley Short Channel IGFET Model-4 with LK feCap model",
        .terms = &FEFET1nSize,
        .numNames = &FEFET1nSize,
        .termNames = FEFET1names,
        .numInstanceParms = &FEFET1pTSize,
        .instanceParms = FEFET1pTable,
        .numModelParms = &FEFET1mPTSize,
        .modelParms = FEFET1mPTable,
        .flags = DEV_DEFAULT,

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

    .DEVparam = FEFET1param,
    .DEVmodParam = FEFET1mParam,
    .DEVload = FEFET1load,
    .DEVsetup = FEFET1setup,
    .DEVunsetup = FEFET1unsetup,
    .DEVpzSetup = FEFET1setup,
    .DEVtemperature = FEFET1temp,
    .DEVtrunc = FEFET1trunc,
    .DEVfindBranch = FEFET1findBr,
    .DEVacLoad = FEFET1acLoad,
    .DEVaccept = NULL,
    .DEVdestroy = NULL,
    .DEVmodDelete = FEFET1mDelete,
    .DEVdelete = NULL,
    .DEVsetic = FEFET1getic,
    .DEVask = FEFET1ask,
    .DEVmodAsk = FEFET1mAsk,
    .DEVpzLoad = FEFET1pzLoad,
    .DEVconvTest = FEFET1convTest,
    .DEVsenSetup = NULL,
    .DEVsenLoad = NULL,
    .DEVsenUpdate = NULL,
    .DEVsenAcLoad = NULL,
    .DEVsenPrint = NULL,
    .DEVsenTrunc = NULL,
    .DEVdisto = NULL,
    .DEVnoise = FEFET1noise,
    .DEVsoaCheck = FEFET1soaCheck,
    .DEVinstSize = &FEFET1iSize,
    .DEVmodSize = &FEFET1mSize,

#ifdef CIDER
    .DEVdump = NULL,
    .DEVacct = NULL,
#endif
};


SPICEdev *
get_fefet1_info(void)
{
    return &FEFET1info;
}
