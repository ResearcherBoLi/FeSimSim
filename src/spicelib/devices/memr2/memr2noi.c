#include "ngspice/ngspice.h"
#include "memr2defs.h"
#include "ngspice/cktdefs.h"
#include "ngspice/iferrmsg.h"
#include "ngspice/noisedef.h"
#include "ngspice/suffix.h"
#include "ngspice/const.h"

int MEMR2noise(
    int mode, int operation,
    GENmodel *inModel,
    CKTcircuit *ckt,
    Ndata *data,
    double *OnDens)
{
    NOISEAN *job = (NOISEAN *)ckt->CKTcurJob;
    MEMR2model *model = (MEMR2model *)inModel;
    MEMR2instance *here;

    double tempOnoise;
    double tempInoise;
    double noizDens[MEMR2NSRCS];
    double lnNdens[MEMR2NSRCS];

    int i;
    double m;

    static char *MEMR2nNames[MEMR2NSRCS] =
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
    for (; model != NULL; model = MEMR2nextModel(model))
    {
        for (here = MEMR2instances(model); here != NULL;
             here = MEMR2nextInstance(here))
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
                        for (i = 0; i < MEMR2NSRCS; i++)
                        {
                            NOISE_ADD_OUTVAR(ckt, data, "onoise.%s%s", here->MEMR2name, MEMR2nNames[i]);
                        }
                        break;
                    case INT_NOIZ:
                        for (i = 0; i < MEMR2NSRCS; i++)
                        {
                            NOISE_ADD_OUTVAR(ckt, data, "onoise_total.%s%s", here->MEMR2name, MEMR2nNames[i]);
                            NOISE_ADD_OUTVAR(ckt, data, "inoise_total.%s%s", here->MEMR2name, MEMR2nNames[i]);
                        }
                        break;
                    }
                }
                break;

            case N_CALC:
                switch (mode)
                {
                case N_DENS:
                    //g_mem = 1/model->MEMR2defR_on  + 1/model->MEMR2defR_off
                    if (model->MEMR2tnoiMod != 0){
                        NevalSrc(&noizDens[MEMR2TNOIZ],&lnNdens[MEMR2TNOIZ],
                                ckt,THERMNOISE,
                                here->MEMR2posNode, here->MEMR2negNode,
                                here->MEMR2conduct);
                    }else{
                        noizDens[MEMR2TNOIZ] = 0;
                        lnNdens[MEMR2TNOIZ] = log(MAX(noizDens[MEMR2TNOIZ], N_MINLOG));             
                    }

                    if (model->MEMR2snoiMod != 0){
                        NevalSrc(&noizDens[MEMR2SNOIZ],&lnNdens[MEMR2SNOIZ],
                                ckt,SHOTNOISE,
                                here->MEMR2posNode, here->MEMR2negNode,
                                *(ckt->CKTrhsOld + (here->MEMR2Ibranch)));
                    }else{
                        noizDens[MEMR2SNOIZ] = 0;
                        lnNdens[MEMR2SNOIZ] = log(MAX(noizDens[MEMR2SNOIZ], N_MINLOG));                         
                    }
                    /*
                    NevalSrc(&noizDens[BJTFLNOIZ], NULL, ckt,
                        N_GAIN,inst->BJTbasePrimeNode, inst->BJTemitPrimeNode,
                        (double)0.0);
                    */
                    NevalSrc(&noizDens[MEMR2FNOIZ], NULL,
                            ckt, N_GAIN,
                            here->MEMR2posNode, here->MEMR2negNode,
                            (double) 0.0);                   
                    if (model->MEMR2fnoiMod != 0){ 
                        //double efef = model->MEMR2ef*here->MEMR2winit +model->MEMR2efh*(1-here->MEMR2winit);
						double efef = model->MEMR2ef*(1+ here->MEMR2winit) ;
						/*
                        noizDens[MEMR2FNOIZ] *= model->MEMR2kf
                                            * exp(model->MEMR2af
                                            * log(MAX(fabs(*(ckt->CKTrhsOld + (here->MEMR2Ibranch))),
                                            N_MINLOG)))
                                            / (pow(data->freq, efef));
                        */   
                        double tmp_check1 = fabs(*(ckt->CKTrhsOld + (here->MEMR2Ibranch)));
                        double tmp_check2 = MAX(fabs(*(ckt->CKTrhsOld + (here->MEMR2Ibranch))),N_MINLOG);
                        double tmp_check3 = pow(MAX(fabs(*(ckt->CKTrhsOld + (here->MEMR2Ibranch))),N_MINLOG) ,model->MEMR2af);
                        double tmp_check4 = pow(data->freq, efef);
						// double FlickerAmpl = model->MEMR2kf/here->MEMR2conduct*i_last_itr*i_last_itr;
						/*
						noizDens[MEMR2FNOIZ] *= model->MEMR2kf 
                                            *pow(MAX(fabs(*(ckt->CKTrhsOld + (here->MEMR2Ibranch))),N_MINLOG) , model->MEMR2af) 
                                            / (pow(data->freq, efef));
						*/
						noizDens[MEMR2FNOIZ] *= model->MEMR2kf / here->MEMR2conduct
                                            *pow(MAX(fabs(*(ckt->CKTrhsOld + (here->MEMR2Ibranch))),N_MINLOG) , model->MEMR2af) 
                                            / (pow(data->freq, efef));
                        
                        lnNdens[MEMR2FNOIZ] = log(MAX(noizDens[MEMR2FNOIZ], N_MINLOG)); 
                    }else{
                        noizDens[MEMR2FNOIZ] = 0; 
                        lnNdens[MEMR2FNOIZ] = log(MAX(noizDens[MEMR2FNOIZ], N_MINLOG));             
                    }

                    if (model->MEMR2dnoiMod != 0){                    
                        noizDens[MEMR2DNOIZ] = 0;
                    }else{
                        noizDens[MEMR2DNOIZ] = 0;
                        lnNdens[MEMR2DNOIZ] = log(MAX(noizDens[MEMR2DNOIZ], N_MINLOG));                         
                    }

                    if (model->MEMR2bnoiMod != 0){  
                        noizDens[MEMR2BNOIZ] = 0;
                    }else{
                        noizDens[MEMR2BNOIZ] = 0;
                        lnNdens[MEMR2BNOIZ] = log(MAX(noizDens[MEMR2BNOIZ], N_MINLOG));                        
                    }
                    noizDens[MEMR2TOTNOIZ] = noizDens[MEMR2TNOIZ] +  noizDens[MEMR2FNOIZ] + noizDens[MEMR2SNOIZ] + noizDens[MEMR2DNOIZ] + noizDens[MEMR2BNOIZ];
                    lnNdens[MEMR2TOTNOIZ] =
                        log(MAX(noizDens[MEMR2TOTNOIZ], N_MINLOG));

                    *OnDens += noizDens[MEMR2TOTNOIZ];

                    if (data->delFreq == 0.0)
                    { /* if we haven't done any previous
                         integration, we need to initialize our
                         "history" variables.
                        */

                        for (i = 0; i < MEMR2NSRCS; i++)
                        {
                            here->MEMR2nVar[LNLSTDENS][i] =
                                lnNdens[i];
                        }

                        /* clear out our integration variables
                           if it's the first pass
                         */
                        if (data->freq ==
                            job->NstartFreq)
                        {
                            for (i = 0; i < MEMR2NSRCS; i++)
                            {
                                here->MEMR2nVar[OUTNOIZ][i] = 0.0;
                                here->MEMR2nVar[INNOIZ][i] = 0.0;
                            }
                        }
                    }
                    else
                    { /* data->delFreq != 0.0,
                         we have to integrate.
                       */
                        for (i = 0; i < MEMR2NSRCS; i++)
                        {
                            if (i != MEMR2TOTNOIZ)
                            {
                                tempOnoise = Nintegrate(noizDens[i],
                                                        lnNdens[i],
                                                        here->MEMR2nVar[LNLSTDENS][i],
                                                        data);
                                tempInoise = Nintegrate(noizDens[i] * data->GainSqInv, lnNdens[i] + data->lnGainInv,
                                                        here->MEMR2nVar[LNLSTDENS][i] + data->lnGainInv, data);
                                here->MEMR2nVar[LNLSTDENS][i] =
                                    lnNdens[i];
                                data->outNoiz += tempOnoise;
                                data->inNoise += tempInoise;
                                if (job->NStpsSm != 0)
                                {
                                    here->MEMR2nVar[OUTNOIZ][i] += tempOnoise;
                                    here->MEMR2nVar[OUTNOIZ][MEMR2TOTNOIZ] += tempOnoise;
                                    here->MEMR2nVar[INNOIZ][i] += tempInoise;
                                    here->MEMR2nVar[INNOIZ][MEMR2TOTNOIZ] += tempInoise;
                                }
                            }
                        }
                    }
                    if (data->prtSummary)
                    {
                        for (i = 0; i < MEMR2NSRCS; i++)
                        { /* print a summary report */
                            data->outpVector[data->outNumber++] = noizDens[i];
                        }
                    }
                    break;
                case INT_NOIZ:
                    /* already calculated, just output */
                    if (job->NStpsSm != 0)
                    {
                        for (i = 0; i < MEMR2NSRCS; i++)
                        {
                            data->outpVector[data->outNumber++] = here->MEMR2nVar[OUTNOIZ][i];
                            data->outpVector[data->outNumber++] = here->MEMR2nVar[INNOIZ][i];
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
