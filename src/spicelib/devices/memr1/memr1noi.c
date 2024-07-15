#include "ngspice/ngspice.h"
#include "memr1defs.h"
#include "ngspice/cktdefs.h"
#include "ngspice/iferrmsg.h"
#include "ngspice/noisedef.h"
#include "ngspice/suffix.h"
#include "ngspice/const.h"

int MEMR1noise(
    int mode, int operation,
    GENmodel *inModel,
    CKTcircuit *ckt,
    Ndata *data,
    double *OnDens)
{
    NOISEAN *job = (NOISEAN *)ckt->CKTcurJob;
    MEMR1model *model = (MEMR1model *)inModel;
    MEMR1instance *here;

    double tempOnoise;
    double tempInoise;
    double noizDens[MEMR1NSRCS];
    double lnNdens[MEMR1NSRCS];

    int i;
    double m;

    static char *MEMR1nNames[MEMR1NSRCS] =
        {
            /* Note that we have to keep the order */
            ".thermal",       /* noise due to ron and roff */
            ".shot",    /* shot noise due to barrier or parasatic diode */
            ".1overf", /* ron(low R state) part flicker (1/f) noise */
            //".1overfH", /* roff(high R state) part flicker (1/f) noise */
            ".diff",    /* diffusion noise for diffusive memristor */
            ".rtn",     /* random telegraph signal noise */
            ""          /* total transistor noise */
        };
    for (; model != NULL; model = MEMR1nextModel(model))
    {
        for (here = MEMR1instances(model); here != NULL;
             here = MEMR1nextInstance(here))
        {
            //pParam = here->pParam;
            switch (operation)
            {
            case N_OPEN:
                /* see if we have to to produce a summary report */
                /* if so, name all the noise generators */

                if (job->NStpsSm != 0)
                {
                    switch (mode)
                    {
                    case N_DENS:
                        for (i = 0; i < MEMR1NSRCS; i++)
                        {
                            NOISE_ADD_OUTVAR(ckt, data, "onoise.%s%s", here->MEMR1name, MEMR1nNames[i]);
                        }
                        break;
                    case INT_NOIZ:
                        for (i = 0; i < MEMR1NSRCS; i++)
                        {
                            NOISE_ADD_OUTVAR(ckt, data, "onoise_total.%s%s", here->MEMR1name, MEMR1nNames[i]);
                            NOISE_ADD_OUTVAR(ckt, data, "inoise_total.%s%s", here->MEMR1name, MEMR1nNames[i]);
                        }
                        break;
                    }
                }
                break;

            case N_CALC:
                switch (mode)
                {
                case N_DENS:
                    //g_mem = 1/model->MEMR1defR_on  + 1/model->MEMR1defR_off
                    if (model->MEMR1tnoiMod != 0){
                        NevalSrc(&noizDens[MEMR1TNOIZ],&lnNdens[MEMR1TNOIZ],
                                ckt,THERMNOISE,
                                here->MEMR1posNode, here->MEMR1negNode,
                                here->MEMR1conduct);
                    }else{
                        noizDens[MEMR1TNOIZ] = 0;
                        lnNdens[MEMR1TNOIZ] = log(MAX(noizDens[MEMR1TNOIZ], N_MINLOG));             
                    }

                    if (model->MEMR1snoiMod != 0){
                        NevalSrc(&noizDens[MEMR1SNOIZ],&lnNdens[MEMR1SNOIZ],
                                ckt,SHOTNOISE,
                                here->MEMR1posNode, here->MEMR1negNode,
                                *(ckt->CKTrhsOld + (here->MEMR1Ibranch)));
                    }else{
                        noizDens[MEMR1SNOIZ] = 0;
                        lnNdens[MEMR1SNOIZ] = log(MAX(noizDens[MEMR1SNOIZ], N_MINLOG));                         
                    }
                    /*
                    NevalSrc(&noizDens[BJTFLNOIZ], NULL, ckt,
                        N_GAIN,inst->BJTbasePrimeNode, inst->BJTemitPrimeNode,
                        (double)0.0);
                    */
                    NevalSrc(&noizDens[MEMR1FNOIZ], NULL,
                            ckt, N_GAIN,
                            here->MEMR1posNode, here->MEMR1negNode,
                            (double) 0.0);                   
                    if (model->MEMR1fnoiMod != 0){ 
                        //double efef = model->MEMR1ef*here->MEMR1winit +model->MEMR1efh*(1-here->MEMR1winit);
						double efef = model->MEMR1ef*(1+ here->MEMR1winit) ;
						/*
                        noizDens[MEMR1FNOIZ] *= model->MEMR1kf
                                            * exp(model->MEMR1af
                                            * log(MAX(fabs(*(ckt->CKTrhsOld + (here->MEMR1Ibranch))),
                                            N_MINLOG)))
                                            / (pow(data->freq, efef));
                        */   
                        double tmp_check1 = fabs(*(ckt->CKTrhsOld + (here->MEMR1Ibranch)));
                        double tmp_check2 = MAX(fabs(*(ckt->CKTrhsOld + (here->MEMR1Ibranch))),N_MINLOG);
                        double tmp_check3 = pow(MAX(fabs(*(ckt->CKTrhsOld + (here->MEMR1Ibranch))),N_MINLOG) ,model->MEMR1af);
                        double tmp_check4 = pow(data->freq, efef);
						// double FlickerAmpl = model->MEMR1kf/here->MEMR1conduct*i_last_itr*i_last_itr;
						/*
						noizDens[MEMR1FNOIZ] *= model->MEMR1kf 
                                            *pow(MAX(fabs(*(ckt->CKTrhsOld + (here->MEMR1Ibranch))),N_MINLOG) , model->MEMR1af) 
                                            / (pow(data->freq, efef));
						*/
						noizDens[MEMR1FNOIZ] *= model->MEMR1kf / here->MEMR1conduct
                                            *pow(MAX(fabs(*(ckt->CKTrhsOld + (here->MEMR1Ibranch))),N_MINLOG) , model->MEMR1af) 
                                            / (pow(data->freq, efef));
                        
                        lnNdens[MEMR1FNOIZ] = log(MAX(noizDens[MEMR1FNOIZ], N_MINLOG)); 
                    }else{
                        noizDens[MEMR1FNOIZ] = 0; 
                        lnNdens[MEMR1FNOIZ] = log(MAX(noizDens[MEMR1FNOIZ], N_MINLOG));             
                    }

                    if (model->MEMR1dnoiMod != 0){                    
                        noizDens[MEMR1DNOIZ] = 0;
                    }else{
                        noizDens[MEMR1DNOIZ] = 0;
                        lnNdens[MEMR1DNOIZ] = log(MAX(noizDens[MEMR1DNOIZ], N_MINLOG));                         
                    }

                    if (model->MEMR1bnoiMod != 0){  
                        noizDens[MEMR1BNOIZ] = 0;
                    }else{
                        noizDens[MEMR1BNOIZ] = 0;
                        lnNdens[MEMR1BNOIZ] = log(MAX(noizDens[MEMR1BNOIZ], N_MINLOG));                        
                    }
                    noizDens[MEMR1TOTNOIZ] = noizDens[MEMR1TNOIZ] +  noizDens[MEMR1FNOIZ] + noizDens[MEMR1SNOIZ] + noizDens[MEMR1DNOIZ] + noizDens[MEMR1BNOIZ];
                    lnNdens[MEMR1TOTNOIZ] =
                        log(MAX(noizDens[MEMR1TOTNOIZ], N_MINLOG));

                    *OnDens += noizDens[MEMR1TOTNOIZ];

                    if (data->delFreq == 0.0)
                    { /* if we haven't done any previous
                         integration, we need to initialize our
                         "history" variables.
                        */

                        for (i = 0; i < MEMR1NSRCS; i++)
                        {
                            here->MEMR1nVar[LNLSTDENS][i] =
                                lnNdens[i];
                        }

                        /* clear out our integration variables
                           if it's the first pass
                         */
                        if (data->freq ==
                            job->NstartFreq)
                        {
                            for (i = 0; i < MEMR1NSRCS; i++)
                            {
                                here->MEMR1nVar[OUTNOIZ][i] = 0.0;
                                here->MEMR1nVar[INNOIZ][i] = 0.0;
                            }
                        }
                    }
                    else
                    { /* data->delFreq != 0.0,
                         we have to integrate.
                       */
                        for (i = 0; i < MEMR1NSRCS; i++)
                        {
                            if (i != MEMR1TOTNOIZ)
                            {
                                tempOnoise = Nintegrate(noizDens[i],
                                                        lnNdens[i],
                                                        here->MEMR1nVar[LNLSTDENS][i],
                                                        data);
                                tempInoise = Nintegrate(noizDens[i] * data->GainSqInv, lnNdens[i] + data->lnGainInv,
                                                        here->MEMR1nVar[LNLSTDENS][i] + data->lnGainInv, data);
                                here->MEMR1nVar[LNLSTDENS][i] =
                                    lnNdens[i];
                                data->outNoiz += tempOnoise;
                                data->inNoise += tempInoise;
                                if (job->NStpsSm != 0)
                                {
                                    here->MEMR1nVar[OUTNOIZ][i] += tempOnoise;
                                    here->MEMR1nVar[OUTNOIZ][MEMR1TOTNOIZ] += tempOnoise;
                                    here->MEMR1nVar[INNOIZ][i] += tempInoise;
                                    here->MEMR1nVar[INNOIZ][MEMR1TOTNOIZ] += tempInoise;
                                }
                            }
                        }
                    }
                    if (data->prtSummary)
                    {
                        for (i = 0; i < MEMR1NSRCS; i++)
                        { /* print a summary report */
                            data->outpVector[data->outNumber++] = noizDens[i];
                        }
                    }
                    break;
                case INT_NOIZ:
                    /* already calculated, just output */
                    if (job->NStpsSm != 0)
                    {
                        for (i = 0; i < MEMR1NSRCS; i++)
                        {
                            data->outpVector[data->outNumber++] = here->MEMR1nVar[OUTNOIZ][i];
                            data->outpVector[data->outNumber++] = here->MEMR1nVar[INNOIZ][i];
                        }
                    }
                    break;
                case N_CLOSE:
                    /* do nothing, the main calling routine will close */
                    return (OK);
                    break; /* the plots */

                } /* switch (operation) */
            }     /* for here */
        }         /* for model */

        return (OK);
    }
}
