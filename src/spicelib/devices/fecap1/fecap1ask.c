/**********
Copyright 2024 Xidian University
Author: 2024 Bo Li
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "ngspice/devdefs.h"
#include "fecap1defs.h"
#include "ngspice/ifsim.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

/* ARGSUSED */
int
FECAP1ask(CKTcircuit *ckt, GENinstance *inst, int which, IFvalue *value,
       IFvalue *select)
{
    FECAP1instance *here = (FECAP1instance *)inst;
    double vr;
    double vi;
    double sr;
    double si;
    double vm;
    static char *msg = "Current and power not available for ac analysis";

    switch(which) {
        case FECAP1_TEMP:
            value->rValue = here->FECAP1temp - CONSTCtoK;
            return(OK);

        case FECAP1_IC:
            value->rValue = here->FECAP1initCondP;
            return(OK);

	
        case FECAP1_AREA:
            value->rValue=here->FECAP1_Area;

            return(OK);

		
        case FECAP1_M:
            value->rValue = here->FECAP1m;
            return(OK);

        case FECAP1_POWER:
            if (ckt->CKTcurrentAnalysis & DOING_AC) {
                errMsg = TMALLOC(char, strlen(msg) + 1);
                errRtn = "FECAP1ask";
                strcpy(errMsg,msg);
                return(E_ASKPOWER);
            } else if (ckt->CKTcurrentAnalysis & (DOING_DCOP | DOING_TRCV)) {
                value->rValue = 0;
            } else if (ckt->CKTcurrentAnalysis & DOING_TRAN) {
                if (ckt->CKTmode & MODETRANOP) {
                    value->rValue = 0;
                } else {
                    value->rValue = *(ckt->CKTstate0 + here->FECAP1ccap) *
                            (*(ckt->CKTrhsOld + here->FECAP1posNode) -
                            *(ckt->CKTrhsOld + here->FECAP1negNode));
                }
            } else value->rValue = *(ckt->CKTstate0 + here->FECAP1ccap) *
                            (*(ckt->CKTrhsOld + here->FECAP1posNode) -
                            *(ckt->CKTrhsOld + here->FECAP1negNode));

            value->rValue *= here->FECAP1m;

            return(OK);
        
  
        default:
            return(E_BADPARM);
        }
}

