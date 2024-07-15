/**********
Copyright 2024 Xidian University
Author: 2024 Bo Li
Modified: 2024/01/27  Bo Li
Refered to NgSPICE Res/Cap related file
**********/
#include<math.h>
#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "fecap1defs.h"
#include "ngspice/trandefs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"
#define STEP(x) ((x) < 0.0 ? 0 : 1)
	/*
	BE				MNA:											RHS:
		n+		n-		i			p						
	n+ | C0/h   -C0/h	+1					 |			|												  |
	n - |-C0/h|  C0/h	-1					 |			|												  |
	i  | 1 	   -1	   -pT/A        -dF1/dP	 |			| 								  |
	p  |dF2/dV 	-dF2/dV			-dF2/dP-1/h  |			|F2/h + F^n(t) - dF/dw*w^n(t) - dF/di*i^n(t)|
*/

int
FECAP1load(GENmodel *inModel, CKTcircuit *ckt)
/* actually load the current capacitance value into the
 * sparse matrix previously provided
 */
{
    FECAP1model *model = (FECAP1model*)inModel;
    FECAP1instance *here;
	
    int error;	
    int cond1;
    double vcap;
    double pcap;
    double geq;
    double ceq;
    double p_geq;  // equivalent conductance item in MNA for P branch
    double p_ceq;  // equivalent current item in RHS for P branch

	double F1_pre_itr;
	double F2_pre_itr;
	double WF;
	double v_pre_itr;
	double p_pre_itr;
	double i_pre_itr;
	double dPolyn_item;
	double Polyn_item;


	//double Xi=1.5*sqrt(3)*model->FECAP1defEc/pow(model->FECAP1defPr,3)  //  

    double m;

    /* check if capacitors are in the circuit or are open circuited */
    if(ckt->CKTmode & (MODETRAN|MODEAC|MODETRANOP) ) {
        /* evaluate device independent analysis conditions */
        cond1=
            ( ( (ckt->CKTmode & MODEDC) &&
                (ckt->CKTmode & MODEINITJCT) )
              || ( ( ckt->CKTmode & MODEUIC) &&
                   ( ckt->CKTmode & MODEINITTRAN) ) ) ;
        /*  loop through all the capacitor models */
        for( ; model != NULL; model = FECAP1nextModel(model)) {

            /* loop through all the instances of the model */
            for (here = FECAP1instances(model); here != NULL ;
                    here=FECAP1nextInstance(here)) {

                m = here->FECAP1m;

                if(cond1) {
                    
                    pcap = here->FECAP1initCondP;

					vcap = model->FECAP1_Thickness *( model->FECAP1_Alpha * pcap +\
										model->FECAP1_Beta*pow(pcap,3)+model->FECAP1_Gamma*pow(pcap,5));

					v_pre_itr = 0;
					p_pre_itr = 0;
					
					F1_pre_itr = 0;
					F2_pre_itr =0;
					
					here->FECAP1_dF1dP = 0;
					
					here->FECAP1_dF2dP = 0;
					
					here->FECAP1_dF2dV = 0;
						
                } else {
                    vcap = *(ckt->CKTrhsOld+here->FECAP1posNode) -
                           *(ckt->CKTrhsOld+here->FECAP1negNode) ;
                    pcap = *(ckt->CKTrhsOld+here->FECAP1Pbranch) ;

                    v_pre_itr = *(ckt->CKTrhsOld+here->FECAP1posNode) -
                    			*(ckt->CKTrhsOld+here->FECAP1negNode) ;
                    p_pre_itr = *(ckt->CKTrhsOld+here->FECAP1Pbranch) ;

					Polyn_item = model->FECAP1_Alpha * p_pre_itr + model->FECAP1_Beta*pow(p_pre_itr,3)+ \
						        model->FECAP1_Gamma*pow(p_pre_itr,5);
					
					dPolyn_item = model->FECAP1_Alpha  + 3*model->FECAP1_Beta*pow(p_pre_itr,2)+\
						            5*model->FECAP1_Gamma*pow(p_pre_itr,4);

					F1_pre_itr = model->FECAP1_Thickness * Polyn_item;
					
					F2_pre_itr = 1/model->FECAP1_Rho*(v_pre_itr/model->FECAP1_Thickness -1*Polyn_item); 

									
					
					here->FECAP1_dF1dP = model->FECAP1_Thickness *dPolyn_item;
					
					here->FECAP1_dF2dP = -1/model->FECAP1_Rho* dPolyn_item;
						
					here->FECAP1_dF2dV =1/(model->FECAP1_Rho*model->FECAP1_Thickness);
					
                }


                if(ckt->CKTmode & (MODETRAN | MODEAC)) {
#ifndef PREDICTOR
                    if(ckt->CKTmode & MODEINITPRED) {
                        *(ckt->CKTstate0+here->FECAP1qcap) =
                            *(ckt->CKTstate1+here->FECAP1qcap);

                        *(ckt->CKTstate0+here->FECAP1pcap) =
                            *(ckt->CKTstate1+here->FECAP1pcap);
						
                    } else { /* only const caps - no poly's */
#endif /* PREDICTOR */
                        *(ckt->CKTstate0+here->FECAP1qcap) = here->FECAP1capac * vcap;
                        *(ckt->CKTstate0+here->FECAP1pcap) = here->FECAP1capacPseudo * pcap;

						if((ckt->CKTmode & MODEINITTRAN)) {
                            *(ckt->CKTstate1+here->FECAP1qcap) =
                                *(ckt->CKTstate0+here->FECAP1qcap);

						*(ckt->CKTstate1+here->FECAP1pcap) =
							*(ckt->CKTstate0+here->FECAP1pcap);


						}
#ifndef PREDICTOR
                    }
#endif /* PREDICTOR */
                    error = NIintegrate(ckt,&geq,&ceq,here->FECAP1capac,here->FECAP1qcap);
                    if(error) return(error);
                    if(ckt->CKTmode & MODEINITTRAN) {
                        *(ckt->CKTstate1+here->FECAP1ccap) =
                            *(ckt->CKTstate0+here->FECAP1ccap);
                    }
                    error = NIintegrate(ckt, &p_geq, &p_ceq, here->FECAP1capacPseudo, here->FECAP1pcap);
                    if (error) return (error);
                    if (ckt->CKTmode & MODEINITTRAN)
                    {
                        *(ckt->CKTstate1 + here->FECAP1pccap) =
                            *(ckt->CKTstate0 + here->FECAP1pccap);
                    }
					
					
                    *(here->FECAP1posPosPtr) += m * geq;    
                    *(here->FECAP1posNegPtr) -= m * geq;
					
                    *(here->FECAP1negPosPtr) -= m * geq;
					*(here->FECAP1negNegPtr) += m * geq;
					
                    *(here->FECAP1posIbrPtr) = 1;					
                    *(here->FECAP1negIbrPtr) = -1;
					
                    *(ckt->CKTrhs+here->FECAP1posNode) -= m * ceq;
                    *(ckt->CKTrhs+here->FECAP1negNode) += m * ceq;

					*(here->FECAP1ibrPosPtr) = 1;
					*(here->FECAP1ibrNegPtr) = -1 ;
					*(here->FECAP1ibrIbrPtr) = -1*model->FECAP1_Rho*model->FECAP1_Thickness /here->FECAP1_Area ;
					*(here->FECAP1ibrPbrPtr) = -1*here->FECAP1_dF1dP;
					*(ckt->CKTrhs + (here->FECAP1Ibranch)) = F1_pre_itr-(here->FECAP1_dF1dP) * p_pre_itr;
					
					*(here->FECAP1pbrPosPtr) = here->FECAP1_dF2dV;
					*(here->FECAP1pbrNegPtr) =-1* here->FECAP1_dF2dV;
					
					*(here->FECAP1pbrPbrPtr) = here->FECAP1_dF2dP - p_geq;
					*(ckt->CKTrhs + (here->FECAP1Pbranch)) = p_ceq - F2_pre_itr \
															+ here->FECAP1_dF2dV * v_pre_itr + here->FECAP1_dF2dP * p_pre_itr ;
					
				
                } else
                    *(ckt->CKTstate0+here->FECAP1qcap) = here->FECAP1capac * vcap;
				    *(ckt->CKTstate0+here->FECAP1pcap) = here->FECAP1capacPseudo * pcap;
            }
        }
    }
	else{
        // else if(ckt->CKTmode & (MODEDC|MODEDCOP|MODEINITJCT|MODEAC|MODEINITFLOAT) ) {
        /* evaluate device independent analysis conditions */
        /*  loop through all the MEM2 models */
        for (; model != NULL; model = FECAP1nextModel(model))
        {
            /* loop through all the instances of the model */
            for (here = FECAP1instances(model); here != NULL ;
                    here=FECAP1nextInstance(here)) {

				*(here->FECAP1posPosPtr) += m * geq;	
				*(here->FECAP1posNegPtr) -= m * geq;
				
				*(here->FECAP1negPosPtr) -= m * geq;
				*(here->FECAP1negNegPtr) += m * geq;
				
				*(here->FECAP1posIbrPtr) = 1;					
				*(here->FECAP1negIbrPtr) = -1;


				*(here->FECAP1ibrPosPtr) = 1;
				*(here->FECAP1ibrNegPtr) = -1 ;
                *(here->FECAP1ibrIbrPtr) = 0;
				*(here->FECAP1ibrPbrPtr) = 0;
				*(ckt->CKTrhs + (here->FECAP1Ibranch)) = model->FECAP1_Thickness *( model->FECAP1_Alpha *  here->FECAP1initCondP +\
					model->FECAP1_Beta*pow( here->FECAP1initCondP,3)+   model->FECAP1_Gamma*pow( here->FECAP1initCondP,5));
                
                *(here->FECAP1pbrPosPtr)= 0;            
                *(here->FECAP1pbrNegPtr)  = 0;                
                *(here->FECAP1pbrPbrPtr) = 1;
                *(ckt->CKTrhs + (here->FECAP1Pbranch)) = here->FECAP1initCondP;
            }
        }
    }

    return(OK);
}

