#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memr2defs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"
#include "ngspice/trandefs.h"
#include "ngspice/1-f-code.h"
#include <math.h>
#define STEP(x) ((x) < 0.0 ? 0 : 1)
#define BOLTZMANN      1.38062259e-23  

int MEMR2load(GENmodel *inModel, CKTcircuit *ckt)
{

    MEMR2model *model = (MEMR2model *)inModel;
    MEMR2instance *here;

    int error;
    double geq;
    double ceq;
    int cond1;
    double wvcap;
    double F;
    double wf;
    double v_last_itr;
    double i_last_itr;
    if (ckt->CKTmode & (MODETRAN | MODEAC | MODETRANOP))
    {
        /* evaluate device independent analysis conditions */
        cond1 =
            (((ckt->CKTmode & MODEDC) &&
              (ckt->CKTmode & MODEINITJCT)) ||
             ((ckt->CKTmode & MODEUIC) &&
              (ckt->CKTmode & MODEINITTRAN)));
        /*  loop through all the MEM2 models */
        for (; model != NULL; model = MEMR2nextModel(model))
        {
            /* loop through all the instances of the model */
            for (here = MEMR2instances(model); here != NULL;
                 here = MEMR2nextInstance(here))
            {
                if (cond1)
                {
                    wvcap = here->MEMR2winit;

                    here->MEMR2conduct = 1 / (model->MEMR2defR_off * here->MEMR2winit + model->MEMR2defR_on * (1 - here->MEMR2winit));
                    here->MEMR2_dIdV = 1 / (model->MEMR2defR_off * here->MEMR2winit + model->MEMR2defR_on * (1 - here->MEMR2winit));
                    here->MEMR2_dIdW = 0;
                    F = 0;
                    i_last_itr = 0;
                    v_last_itr = 0;
                    here->MEMR2_dFdW = 0;
                    here->MEMR2_dFdV = 0;
                    here->MEMR2_dFdI = 0;
                    // wvcap = here->MEM2winit;
                    //*(here->MEM2wbrIbrPtr) = 0;
                }
                else
                {
                    wvcap = *(ckt->CKTrhsOld + here->MEMR2Wbranch);
					here->MEMR2conduct = 1 / (model->MEMR2defR_off * wvcap + model->MEMR2defR_on * (1 -wvcap));
                    // limiting w range
                    if (wvcap > 1)
                    {
                        wvcap = 1;
                        *(ckt->CKTrhsOld + here->MEMR2Wbranch) = 1;
                    }
                    else if (wvcap < 0)
                    {
                        wvcap = 0;
                        *(ckt->CKTrhsOld + here->MEMR2Wbranch) = 0;
                    }
                    i_last_itr = *(ckt->CKTrhsOld + (here->MEMR2Ibranch));
                    v_last_itr = *(ckt->CKTrhsOld + (here->MEMR2posNode)) - *(ckt->CKTrhsOld + (here->MEMR2negNode));
                    here->MEMR2_dIdV = 1 / (model->MEMR2defR_off * wvcap + model->MEMR2defR_on * (1 - wvcap));
                    here->MEMR2_dIdW = i_last_itr * here->MEMR2_dIdV * (model->MEMR2defR_on - model->MEMR2defR_off);
                    if (model->MEMR2defWf = 2)
                    {
                        // take care of wf form from original paper due to the w selection part and current reference direction
                        wf = 1 - pow(1 - wvcap - STEP(-1 * i_last_itr), 2 * (model->MEMR2defP));
                    }
                    if (v_last_itr <= -1 * model->MEMR2defVt_off)
                    {
                        F = model->MEMR2defK_off * wf / model->MEMR2defD * pow((-1 * v_last_itr / model->MEMR2defVt_off - 1), model->MEMR2defAlpha_off);

                        here->MEMR2_dFdV = -1 * model->MEMR2defK_off * model->MEMR2defAlpha_off * wf / model->MEMR2defD / model->MEMR2defVt_off * pow((-1 * v_last_itr / model->MEMR2defVt_off - 1), (model->MEMR2defAlpha_off - 1));

                        here->MEMR2_dFdW = model->MEMR2defK_off / model->MEMR2defD * pow((-1 * v_last_itr / model->MEMR2defVt_off - 1), model->MEMR2defAlpha_off) * 2 * model->MEMR2defP * pow(1 - wvcap - STEP(-1 * i_last_itr), 2 * (model->MEMR2defP) - 1);
                        here->MEMR2_dFdI = 0;
                    }
                    else if (v_last_itr >= -1 * model->MEMR2defVt_on)
                    {
                        F = model->MEMR2defK_on * wf / model->MEMR2defD * pow((-1 * v_last_itr / model->MEMR2defVt_on - 1), model->MEMR2defAlpha_on);

                        here->MEMR2_dFdV = -1 * model->MEMR2defK_on * model->MEMR2defAlpha_on * wf / model->MEMR2defD / model->MEMR2defVt_on * pow((-1 * v_last_itr / model->MEMR2defVt_on - 1), (model->MEMR2defAlpha_on - 1));

                        here->MEMR2_dFdW = model->MEMR2defK_on / model->MEMR2defD * pow((-1 * v_last_itr / model->MEMR2defVt_on - 1), model->MEMR2defAlpha_on) * 2 * model->MEMR2defP * pow(1 - wvcap - STEP(-1 * i_last_itr), 2 * (model->MEMR2defP) - 1);

                        here->MEMR2_dFdI = 0;
                    }
                    else
                    {
                        F = 0;
                        here->MEMR2_dFdW = 0;
                        here->MEMR2_dFdV = 0;
                        here->MEMR2_dFdI = 0;
                    }
                }

                if (ckt->CKTmode & (MODETRAN | MODEAC))
                {
                    // if ((ckt->CKTmode & (MODEDCTRANCURVE | MODETRAN | MODEAC | MODEINITSMSIG)) ||
                    //					((ckt->CKTmode & MODETRANOP) && (ckt->CKTmode & MODEUIC))) {
#ifndef PREDICTOR
                    if (ckt->CKTmode & MODEINITPRED)
                    {
                        *(ckt->CKTstate0 + here->MEMR2qcapPseudo) =
                            *(ckt->CKTstate1 + here->MEMR2qcapPseudo);
                    }
                    else
                    { /* only const caps - no poly's */
#endif                /* PREDICTOR */
                        *(ckt->CKTstate0 + here->MEMR2qcapPseudo) = here->MEMR2capacPseudo * wvcap;
                        if ((ckt->CKTmode & MODEINITTRAN))
                        {
                            *(ckt->CKTstate1 + here->MEMR2qcapPseudo) =
                                *(ckt->CKTstate0 + here->MEMR2qcapPseudo);
                        }
#ifndef PREDICTOR
                    }
#endif /* PREDICTOR */
                    error = NIintegrate(ckt, &geq, &ceq, here->MEMR2capacPseudo, here->MEMR2qcapPseudo);
                    if (error)
                        return (error);
                    if (ckt->CKTmode & MODEINITTRAN)
                    {
                        *(ckt->CKTstate1 + here->MEMR2ccapPseudo) =
                            *(ckt->CKTstate0 + here->MEMR2ccapPseudo);
                    }
                    *(here->MEMR2posIbrPtr) = 1;
                    *(here->MEMR2negIbrPtr) = -1;

                    // add tran noise
                    if (here->MEMR2noiseTran)
                    {
                        double value;
                        double time = ckt->CKTtime;


                        struct trnoise_state *state = here->MEMR2trnoise_state;
                        /* reset top (hack for repeated tran commands)
                        when there is the jump from time=0 to time>0 */
                        double TS = state->TS;
                        if (time == 0.0)
                            state->timezero = TRUE;
                        else if (state->timezero)
                        {
                            state->top = 0;
                            state->timezero = FALSE;
                        }

                        /* no noise or time == 0 */
                        if (TS == 0.0 || time == 0.0)
                        {
                            value = 0.0;
                        }
                        else
                        {

						
							double NA = sqrt(4*BOLTZMANN*ckt->CKTtemp* here->MEMR2conduct *  1e12);
							double TS = 1e-6;
							double NALPHA = 1;
							double NAMP = 1e-5;
							double RTSAM = 5e-4;
							double RTSCAPT = 22e-6;
							double RTSEMT = 50e-6;				
							trnoise_state_free(here->MEMR2trnoise_state);
							here->MEMR2trnoise_state =
								  trnoise_state_init(NA, TS, NALPHA, NAMP, RTSAM, RTSCAPT, RTSEMT);

		                    /* 1/f and white noise */
                            size_t n1 = (size_t)floor(time / TS);

                            double V1 = trnoise_state_get(state, ckt, n1);
                            double V2 = trnoise_state_get(state, ckt, n1 + 1);

                            value = V1 + (V2 - V1) * (time / TS - (double)n1);
                        }

                        /* RTS noise */
                        double RTSAM = state->RTSAM; 
                        if (RTSAM > 0)
                        {
                            double RTScapTime = state->RTScapTime;
                            if (time >= RTScapTime)
                                value += RTSAM;
                        }

                        *(ckt->CKTrhs + (here->MEMR2posNode)) += value;
                        *(ckt->CKTrhs + (here->MEMR2negNode)) -= value;
                    }

                    *(here->MEMR2ibrPosPtr) = here->MEMR2_dIdV;
                    *(here->MEMR2ibrNegPtr) = -1 * here->MEMR2_dIdV;
                    *(here->MEMR2ibrIbrPtr) = -1;
                    *(here->MEMR2ibrWbrPtr) = here->MEMR2_dIdW;
                    *(ckt->CKTrhs + (here->MEMR2Ibranch)) = (here->MEMR2_dIdW) * wvcap;

                    *(here->MEMR2wbrPosPtr) = -1 * here->MEMR2_dFdV;
                    *(here->MEMR2wbrNegPtr) = here->MEMR2_dFdV;
                    *(here->MEMR2wbrIbrPtr) = -1 * here->MEMR2_dFdI;
                    *(here->MEMR2wbrWbrPtr) = geq - here->MEMR2_dFdW;
                    *(ckt->CKTrhs + (here->MEMR2Wbranch)) = -1 * ceq + F - here->MEMR2_dFdW * wvcap - here->MEMR2_dFdV * v_last_itr - here->MEMR2_dFdI * i_last_itr;
                }
                else
                {
                    *(ckt->CKTstate0 + here->MEMR2qcapPseudo) = here->MEMR2capacPseudo * wvcap;
                }
            }
        }
    }
    else
    {
        // else if(ckt->CKTmode & (MODEDC|MODEDCOP|MODEINITJCT|MODEAC|MODEINITFLOAT) ) {
        /* evaluate device independent analysis conditions */
        /*  loop through all the MEM2 models */
        for (; model != NULL; model = MEMR2nextModel(model))
        {
            /* loop through all the instances of the model */
            for (here = MEMR2instances(model); here != NULL;
                 here = MEMR2nextInstance(here))
            {
                // wvcap = here->MEM2winit;
                // here->MEM2conduct =1/( model->MEM2defR_off * here->MEM2winit + model->MEM2defR_on* (1 - here->MEM2winit));
                here->MEMR2_dIdV = 1 / (model->MEMR2defR_off * here->MEMR2winit + model->MEMR2defR_on * (1 - here->MEMR2winit));
                /*
                here->MEM2_dIdW = 0;
                F = 0;
                i_last_itr = 0;
                v_last_itr = 0;
                here->MEM2_dFdW = 0;
                here->MEM2_dFdV = 0;
                here->MEM2_dFdI = 0;
                */

                *(here->MEMR2posIbrPtr) = 1;
                *(here->MEMR2negIbrPtr) = -1;

                *(here->MEMR2ibrPosPtr) = here->MEMR2_dIdV;
                *(here->MEMR2ibrNegPtr) = -1 * here->MEMR2_dIdV;
                *(here->MEMR2ibrIbrPtr) = -1;
                //*(here->MEM2ibrWbrPtr) = here->MEM2_dIdW;
                *(here->MEMR2ibrWbrPtr) = 0;

                //*(ckt->CKTrhs + (here->MEM2Ibranch)) = (here->MEM2_dIdW) *wvcap;
                *(ckt->CKTrhs + (here->MEMR2Ibranch)) = 0;
                //*(here->MEM2wbrPosPtr) = -1* here->MEM2_dFdV;
                *(here->MEMR2wbrPosPtr) = 0;
                //*(here->MEM2wbrNegPtr) = here->MEM2_dFdV;
                *(here->MEMR2wbrNegPtr) = 0;
                //*(here->MEM2wbrIbrPtr) = -1* here->MEM2_dFdI;
                *(here->MEMR2wbrIbrPtr) = 0;
                //*(here->MEM2wbrWbrPtr) = geq- here->MEM2_dFdW;
                *(here->MEMR2wbrWbrPtr) = 1;
                *(ckt->CKTrhs + (here->MEMR2Wbranch)) = here->MEMR2winit;
            }
        }
    }
    return (OK);
}
