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
FEFET1getic(
GENmodel *inModel,
CKTcircuit *ckt)
{
FEFET1model *model = (FEFET1model*)inModel;
FEFET1instance *here;

    for (; model ; model = FEFET1nextModel(model)) 
    {    for (here = FEFET1instances(model); here; here = FEFET1nextInstance(here))
	      {
	           if (!here->FEFET1icVDSGiven) 
	      {   here->FEFET1icVDS = *(ckt->CKTrhs + here->FEFET1dNode) 
				   - *(ckt->CKTrhs + here->FEFET1sNode);
              }
              if (!here->FEFET1icVGSGiven) 
	      {   here->FEFET1icVGS = *(ckt->CKTrhs + here->FEFET1gNodeExt) 
				   - *(ckt->CKTrhs + here->FEFET1sNode);
              }
              if(!here->FEFET1icVBSGiven)
              {  here->FEFET1icVBS = *(ckt->CKTrhs + here->FEFET1bNode)
                                  - *(ckt->CKTrhs + here->FEFET1sNode);
              }
               if(!here->FECAP1_icGiven) 
               {  here->FECAP1initCondP = 
                        *(ckt->CKTrhs + here->FECAP1Pbranch);
               }              
         }
    }
    return(OK);
}
