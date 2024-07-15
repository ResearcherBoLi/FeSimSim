/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2021/05/20  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/const.h"
#include "ngspice/ifsim.h"
#include "memr3defs.h"
#include "ngspice/sperror.h"
#include "ngspice/missing_math.h"
#include "ngspice/fteext.h"

int
MEMR3param(int param, IFvalue *value, GENinstance *inst, IFvalue *select)
{
    double scale;

    MEMR3instance *here = (MEMR3instance *)inst;

    NG_IGNORE(select);

    if (!cp_getvar("scale", CP_REAL, &scale, 0))
        scale = 1;

    switch(param) {
    /*
    case RES_TEMP:
        here->REStemp = value->rValue + CONSTCtoK;
        here->REStempGiven = TRUE;
        break;

    case RES_DTEMP:
        here->RESdtemp = value->rValue;
        here->RESdtempGiven = TRUE;
        break;

    case RES_ACRESIST:
        here->RESacResist = value->rValue;
        here->RESacresGiven = TRUE;
        break;
    */
    case MEMR3_RINIT:
        here->MEMR3rinit = value->rValue * scale;
        here->MEMR3rinitGiven = TRUE;
        break;
	case MEMR3_WINIT:
        here->MEMR3winit = value->rValue * scale;
        here->MEMR3winitGiven = TRUE;
		break;

    default:
        return(E_BADPARM);
    }
    //RESupdate_conduct(here, FALSE);
    return(OK);
}
