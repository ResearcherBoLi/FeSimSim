#include "ngspice/ngspice.h"
#include "ngspice/cktdefs.h"
#include "memr2defs.h"
#include "ngspice/trandefs.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"
#include "ngspice/missing_math.h"
#include "ngspice/1-f-code.h"

int MEMR2accept(CKTcircuit *ckt, GENmodel *inModel)
/* set up the breakpoint table.  */
{

    MEMR2model *model = (MEMR2model *)inModel;
    MEMR2instance *here;
    int error;
    /*  loop through all memr2 models */
    for (; model != NULL; model = MEMR2nextModel(model))
    {

        /* loop through all the instances of the model */
        for (here = MEMR2instances(model); here != NULL;
             here = MEMR2nextInstance(here))
        {

            if (!(ckt->CKTmode & (MODETRAN | MODETRANOP)))
            {
                /* not transient, so shouldn't be here */
                return (OK);
            }
            else
            {
                struct trnoise_state *state = here->MEMR2trnoise_state;
                double TS = state->TS;
                double RTSAM = state->RTSAM;

                if ((TS == 0.0) && (RTSAM == 0.0)) // no further breakpoint if value not given
                    break;

                if (ckt->CKTbreak)
                {

                    int n = (int)floor(ckt->CKTtime / TS + 0.5);
                    volatile double nearest = n * TS;

                    if (AlmostEqualUlps(nearest, ckt->CKTtime, 3))
                    {
                        /* carefull calculate `next'
                         *  make sure it is really identical
                         *  with the next calculated `nearest' value
                         */
                        volatile double next = (n + 1) * TS;
                        error = CKTsetBreak(ckt, next);
                        if (error)
                            return (error);
                    }
                }

                if (RTSAM > 0)
                {
                    double RTScapTime = state->RTScapTime;
                    double RTSemTime = state->RTSemTime;
                    double RTSCAPT = state->RTSCAPT;
                    double RTSEMT = state->RTSEMT;

                    if (ckt->CKTtime == 0)
                    {
                        /* initialzing here again needed for repeated calls to tran command */
                        state->RTScapTime = RTScapTime = exprand(RTSCAPT);
                        state->RTSemTime = RTSemTime = RTScapTime + exprand(RTSEMT);
                        if (ckt->CKTbreak)
                        {
                            error = CKTsetBreak(ckt, RTScapTime);
                            if (error)
                                return (error);
                        }
                    }

                    if (AlmostEqualUlps(RTScapTime, ckt->CKTtime, 3))
                    {
                        if (ckt->CKTbreak)
                        {
                            error = CKTsetBreak(ckt, RTSemTime);
                            if (error)
                                return (error);
                        }
                    }

                    if (AlmostEqualUlps(RTSemTime, ckt->CKTtime, 3))
                    {
                        /* new values */
                        RTScapTime = here->MEMR2trnoise_state->RTScapTime = ckt->CKTtime + exprand(RTSCAPT);
                        here->MEMR2trnoise_state->RTSemTime = RTScapTime + exprand(RTSEMT);

                        if (ckt->CKTbreak)
                        {
                            error = CKTsetBreak(ckt, RTScapTime);
                            if (error)
                                return (error);
                        }
                    }
                }
            }
        }
    }
    return (OK);
}