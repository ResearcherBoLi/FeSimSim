/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified: Apr 2000 - Paolo Nenzi
**********/

#include "ngspice/ngspice.h"
#include "ngspice/const.h"
#include "ngspice/ifsim.h"
#include "memi1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/missing_math.h"
#include "ngspice/fteext.h"

int
MEMI1param(int param, IFvalue *value, GENinstance *inst, IFvalue *select)
{
    double scale;

    MEMI1instance *here = (MEMI1instance *)inst;

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

	case MEMI1_WINIT:
        here->MEMI1winit = value->rValue * scale;
        here->MEMI1winitGiven = TRUE;
		break;

	case MEMI1_IC:
		here->MEMI1initCond = value->rValue;
		here->MEMI1icGiven = TRUE;
		break;

	/*
    case MEM_RON:
        here->MEMron = value->rValue //* scale;
        here->MEMronGiven = TRUE;
        break;
    case MEM_ROFF:
        here->MEMroff = value->rValue //* scale;
        here->MEMroffGiven = TRUE;
        break;
    case MEM_MU:
        here->MEMmu = value->rValue //* scale;
        here->MEMmuGiven = TRUE;
        break;
    */
    /*
    case MEM1_P:
        here->MEM1p = value->rValue;  //* scale;
        here->MEM1pGiven = TRUE;
        break;
    case MEM1_WF:
        here->MEM1wf = value->rValue;  //* scale;
        here->MEM1wfGiven = TRUE;
        break;
	*/
    /*
    case RES_SCALE:
        here->RESscale = value->rValue;
        here->RESscaleGiven = TRUE;
        break;
    case RES_RESIST_SENS:
        here->RESsenParmNo = value->iValue;
        break;
    case RES_M:
        here->RESm = value->rValue;
        here->RESmGiven = TRUE;
        break;
    case RES_TC1:
        here->REStc1 = value->rValue;
        here->REStc1Given = TRUE;
        break;
    case RES_TC2:
        here->REStc2 = value->rValue;
        here->REStc2Given = TRUE;
        break;
    case RES_TCE:
        here->REStce = value->rValue;
        here->REStceGiven = TRUE;
        break;
    case RES_NOISY:
        here->RESnoisy = value->iValue;
        here->RESnoisyGiven = TRUE;
        break;
    case RES_BV_MAX:
        here->RESbv_max = value->rValue;
        here->RESbv_maxGiven = TRUE;
        break;
    */
    default:
        return(E_BADPARM);
    }
    //RESupdate_conduct(here, FALSE);
    return(OK);
}
