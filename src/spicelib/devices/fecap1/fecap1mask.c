/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
Modified: September 2003 Paolo Nenzi
**********/
/*
 */

#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "ngspice/devdefs.h"
#include "fecap1defs.h"
#include "ngspice/sperror.h"
#include "ngspice/ifsim.h"
#include "ngspice/suffix.h"


/* ARGSUSED */
int
FECAP1mAsk(CKTcircuit *ckt, GENmodel *inModel, int which, IFvalue *value)
{
    FECAP1model *model = (FECAP1model*)inModel;

    NG_IGNORE(ckt);

    switch(which) {

    case FECAP1_MOD_FECAP:
        return(OK);
        
    case FECAP1_MOD_TNOM:
        value->rValue = model->FECAP1_Tnom-CONSTCtoK;
        return(OK);

    case FECAP1_MOD_Alpha:
        value->rValue = model->FECAP1_Alpha;
        return(OK);

    case FECAP1_MOD_Beta: 
        value->rValue = model->FECAP1_Beta; 
        return(OK); 
    
    case FECAP1_MOD_Gamma: 
        value->rValue = model->FECAP1_Gamma; 
        return(OK); 

    case FECAP1_MOD_Ec: 
        value->rValue = model->FECAP1_Ec; 
        return(OK);

    case FECAP1_MOD_Ps: 
        value->rValue = model->FECAP1_Ps; 
        return(OK);

    case FECAP1_MOD_Rho: 
        value->rValue = model->FECAP1_Rho; 
        return(OK);

    case FECAP1_MOD_Thickness: 
        value->rValue = model->FECAP1_Thickness; 
        return(OK);    

    default:
        return(E_BADPARM);
    }
}
