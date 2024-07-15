/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


#include "ngspice/ngspice.h"
#include "ngspice/const.h"
#include "ngspice/ifsim.h"
#include "memr1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/missing_math.h"
#include "ngspice/fteext.h"

int
MEMR1param(int param, IFvalue *value, GENinstance *inst, IFvalue *select)
{
    double scale;

    MEMR1instance *here = (MEMR1instance *)inst;

    NG_IGNORE(select);

    if (!cp_getvar("scale", CP_REAL, &scale, 0))
        scale = 1;

    switch(param) {

	case MEMR1_RINIT:
        here->MEMR1rinit = value->rValue * scale;
        here->MEMR1rinitGiven = TRUE;
        break;

	case MEMR1_WINIT:
        here->MEMR1winit = value->rValue * scale;
        here->MEMR1winitGiven = TRUE;
		break;

    case MEMR1_P:
        here->MEMR1p = value->rValue;  //* scale;
        here->MEMR1pGiven = TRUE;
        break;
		
    case MEMR1_WF:
        here->MEMR1wf = value->rValue;  //* scale;
        here->MEMR1wfGiven = TRUE;
        break;

    case MEMR1_TRNOISE:
        here->MEMR1doNoiseTran = value->iValue;
        here->MEMR1noiseTranGiven = TRUE;
        break;  

    default:
        return(E_BADPARM);
    }
    //RESupdate_conduct(here, FALSE);
    return(OK);
}
