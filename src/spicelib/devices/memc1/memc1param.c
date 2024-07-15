/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified: Apr 2000 - Paolo Nenzi
**********/

#include "ngspice/ngspice.h"
#include "ngspice/const.h"
#include "ngspice/ifsim.h"
#include "memc1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/missing_math.h"
#include "ngspice/fteext.h"

int
MEMC1param(int param, IFvalue *value, GENinstance *inst, IFvalue *select)
{
    double scale;

    MEMC1instance *here = (MEMC1instance *)inst;

    NG_IGNORE(select);

    if (!cp_getvar("scale", CP_REAL, &scale, 0))
        scale = 1;

    switch(param) {

    case MEMC1_RINIT:
        here->MEMC1rinit = value->rValue * scale;
        here->MEMC1rinitGiven = TRUE;
        break;
	case MEMC1_WINIT:
        here->MEMC1winit = value->rValue * scale;
        here->MEMC1winitGiven = TRUE;
		break;
			
	case MEMC1_IC:
		here->MEMC1initCond = value->rValue;
		here->MEMC1icGiven = TRUE;
		break;

    default:
        return(E_BADPARM);
    }
    //RESupdate_conduct(here, FALSE);
    return(OK);
}
