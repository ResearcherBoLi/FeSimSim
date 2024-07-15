
/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/



#include "ngspice/ngspice.h"
#include "ngspice/const.h"
#include "memr1defs.h"
#include "ngspice/ifsim.h"
#include "ngspice/cktdefs.h"
#include "ngspice/sperror.h"


/* TODO : there are "double" value compared with 0 (eg: vm == 0)
 *        Need to substitute this check with a suitable eps.
 *        PN 2003
 */

int
MEMR1ask(CKTcircuit *ckt, GENinstance *inst, int which, IFvalue *value,
       IFvalue *select)
{
    MEMR1instance *fast = (MEMR1instance *)inst;

    switch(which) {
        case MEMR1_WFLUXPSEUDO:
            value->rValue = *(ckt->CKTstate0+fast->MEMR1qcapPseudo);
            return(OK);
        case MEMR1_WVOLTPSEUDO:
            value->rValue = *(ckt->CKTstate0+fast->MEMR1ccapPseudo);

	
        case MEMR1_RINIT:
            value->rValue = fast->MEMR1rinit;
            return(OK);
        case MEMR1_P:
            value->rValue = fast->MEMR1p;
            return(OK);
        case MEMR1_WF:
            value->rValue = fast->MEMR1wf;
            return(OK);
        case MEMR1_TRNOISE:
            value->iValue = fast->MEMR1doNoiseTran;
            return(OK);	          
    default:
        return(E_BADPARM);
    }
    /* NOTREACHED */
}
