/**********
Copyright 2024 Xidian University
Author: 2024 Bo Li
Modified: 2024/01/27  Bo Li
Refered to NgSPICE Res/Cap related file
**********/
#include "ngspice/const.h"
#include "ngspice/ngspice.h"
#include "ngspice/ifsim.h"
#include "fecap1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"
#include "ngspice/fteext.h"
#include "ngspice/missing_math.h"

/* ARGSUSED */
int
FECAP1param(int param, IFvalue *value, GENinstance *inst, IFvalue *select)
{
    double scale;

    FECAP1instance *here = (FECAP1instance*)inst;

    NG_IGNORE(select);

    if (!cp_getvar("scale", CP_REAL, &scale, 0))
        scale = 1;

    switch(param) {

	/*		
    case FECAP1_M:
        //here->FECAP1capac = value->rValue;
        if (!here->FECAP1mGiven)
            here->FECAP1m = 1.0;
        here->FECAP1mGiven = TRUE;
        break;
		
    case FECAP1_IC_V:
        here->FECAP1initCondV = value->rValue;
        here->FECAP1icVGiven = TRUE;
        break;
	*/	
    case FECAP1_IC:
        here->FECAP1initCondP = value->rValue;
        here->FECAP1_icGiven = TRUE;
        break;   
		
    case FECAP1_TEMP:
        here->FECAP1temp = value->rValue + CONSTCtoK;
        here->FECAP1tempGiven = TRUE;
        break;

    case FECAP1_AREA:
        here->FECAP1_Area = value->rValue * scale;
        here->FECAP1_AreaGiven = TRUE;
        break;
	/*	
    case FECAP1_WIDTH:
        here->FECAP1width = value->rValue * scale;
        here->FECAP1widthGiven = TRUE;
        break;
    case FECAP1_LENGTH:
        here->FECAP1length = value->rValue * scale;
        here->FECAP1lengthGiven = TRUE;
        break;
		}
    case FECAP1_SCALE:
        here->FECAP1scale = value->rValue;
        here->FECAP1scaleGiven = TRUE;
        break;		
	
    case FECAP1_M:
        here->FECAP1m = value->rValue;
        here->FECAP1mGiven = TRUE;
        break;
    */


    default:
        return(E_BADPARM);
    }
    return(OK);
}
