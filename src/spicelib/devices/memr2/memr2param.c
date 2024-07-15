/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified: Apr 2000 - Paolo Nenzi
**********/

#include "ngspice/ngspice.h"
#include "ngspice/const.h"
#include "ngspice/ifsim.h"
#include "memr2defs.h"
#include "ngspice/sperror.h"
#include "ngspice/missing_math.h"
#include "ngspice/fteext.h"

int
MEMR2param(int param, IFvalue *value, GENinstance *inst, IFvalue *select)
{
    double scale;

    MEMR2instance *here = (MEMR2instance *)inst;

    NG_IGNORE(select);

    if (!cp_getvar("scale", CP_REAL, &scale, 0))
        scale = 1;

    switch(param) {
 
    case MEMR2_RINIT:
        here->MEMR2rinit = value->rValue * scale;
        here->MEMR2rinitGiven = TRUE;
        break;
		
	case MEMR2_WINIT:
        here->MEMR2winit = value->rValue * scale;
        here->MEMR2winitGiven = TRUE;
		break;

    case MEMR2_TRNOISE:
        here->MEMR2doNoiseTran = value->iValue;
        here->MEMR2noiseTranGiven = TRUE;
        break;    

    default:
        return(E_BADPARM);
    }
    //RESupdate_conduct(here, FALSE);
    return(OK);
}
