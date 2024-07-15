/**********
Copyright 2024 Xidian University.	All rights reserved.
 Author: Bo Li	at Hangzhou Institute of Technology	 
 Modified: 2020/09/09  Bo Li
 Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "fefet1def.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"


int
FEFET1trunc(
GENmodel *inModel,
CKTcircuit *ckt,
double *timeStep)
{
FEFET1model *model = (FEFET1model*)inModel;
FEFET1instance *here;

#ifdef STEPDEBUG
    double debugtemp;
#endif /* STEPDEBUG */

    for (; model != NULL; model = FEFET1nextModel(model))
    {    for (here = FEFET1instances(model); here != NULL;
	      here = FEFET1nextInstance(here))
	 {

#ifdef STEPDEBUG
            debugtemp = *timeStep;
#endif /* STEPDEBUG */
            CKTterr(here->FECAP1qcap,ckt,timeStep);            
            CKTterr(here->FEFET1qb,ckt,timeStep);
            CKTterr(here->FEFET1qg,ckt,timeStep);
            CKTterr(here->FEFET1qd,ckt,timeStep);
            if (here->FEFET1trnqsMod)
                CKTterr(here->FEFET1qcdump,ckt,timeStep);
            if (here->FEFET1rbodyMod)
            {   CKTterr(here->FEFET1qbs,ckt,timeStep);
                CKTterr(here->FEFET1qbd,ckt,timeStep);
	    }
	    if (here->FEFET1rgateMod == 3)
		CKTterr(here->FEFET1qgmid,ckt,timeStep);
#ifdef STEPDEBUG
            if(debugtemp != *timeStep)
	    {  printf("device %s reduces step from %g to %g\n",
                       here->FEFET1name,debugtemp,*timeStep);
            }
#endif /* STEPDEBUG */
        }
    }
    return(OK);
}
