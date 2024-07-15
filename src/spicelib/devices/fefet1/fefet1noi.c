/**** BSIM4.5.0 Released by Xuemei (Jane) Xi 07/29/2005 ****/

/**********
 * Copyright 2005 Regents of the University of California. All rights reserved.
 * File: b4noi.c of BSIM4.5.0.
 * Author: 2000 Weidong Liu
 * Authors: 2001- Xuemei Xi, Mohan Dunga, Ali Niknejad, Chenming Hu.
 * Project Director: Prof. Chenming Hu.
 * Modified by Xuemei Xi, 04/06/2001.
 * Modified by Xuemei Xi, 10/05/2001.
 * Modified by Xuemei Xi, 11/15/2002.
 * Modified by Xuemei Xi, 05/09/2003.
 * Modified by Xuemei Xi, 03/04/2004.
 * Modified by Xuemei Xi, 07/29/2005.
 **********/

#include "ngspice/ngspice.h"
#include "fefet1def.h"
#include "ngspice/cktdefs.h"
#include "ngspice/iferrmsg.h"
#include "ngspice/noisedef.h"
#include "ngspice/suffix.h"
#include "ngspice/const.h"


/*
 * WDL: 1/f noise model has been smoothed out and enhanced with
 * bulk charge effect as well as physical N* equ. and necessary
 * conversion into the SI unit system.
 */

static double
FEFET1Eval1ovFNoise(
double Vds,
FEFET1model *model,
FEFET1instance *here,
double freq, double temp)
{
struct fefet1SizeDependParam *pParam;
double cd, esat, DelClm, EffFreq, N0, Nl, Leff, Leffsq;
double T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, Ssi;

    pParam = here->pParam;
    cd = fabs(here->FEFET1cd);
    Leff = pParam->FEFET1leff - 2.0 * model->FEFET1lintnoi;
    Leffsq = Leff * Leff;
    esat = 2.0 * here->FEFET1vsattemp / here->FEFET1ueff;
    if(model->FEFET1em<=0.0) DelClm = 0.0; /* flicker noise modified -JX  */
    else {
            T0 = ((((Vds - here->FEFET1Vdseff) / pParam->FEFET1litl)
                       + model->FEFET1em) / esat);
            DelClm = pParam->FEFET1litl * log (MAX(T0, N_MINLOG));
            if (DelClm < 0.0)        DelClm = 0.0;  /* bugfix */
    }
    EffFreq = pow(freq, model->FEFET1ef);
    T1 = CHARGE * CHARGE * CONSTboltz * cd * temp * here->FEFET1ueff;
    T2 = 1.0e10 * EffFreq * here->FEFET1Abulk * model->FEFET1coxe * Leffsq;
    N0 = model->FEFET1coxe * here->FEFET1Vgsteff / CHARGE;
    Nl = model->FEFET1coxe * here->FEFET1Vgsteff
       * (1.0 - here->FEFET1AbovVgst2Vtm * here->FEFET1Vdseff) / CHARGE;

    T3 = model->FEFET1oxideTrapDensityA
       * log(MAX(((N0 + here->FEFET1nstar) / (Nl + here->FEFET1nstar)), N_MINLOG));
    T4 = model->FEFET1oxideTrapDensityB * (N0 - Nl);
    T5 = model->FEFET1oxideTrapDensityC * 0.5 * (N0 * N0 - Nl * Nl);

    T6 = CONSTboltz * temp * cd * cd;
    T7 = 1.0e10 * EffFreq * Leffsq * pParam->FEFET1weff * here->FEFET1nf;
    T8 = model->FEFET1oxideTrapDensityA + model->FEFET1oxideTrapDensityB * Nl
       + model->FEFET1oxideTrapDensityC * Nl * Nl;
    T9 = (Nl + here->FEFET1nstar) * (Nl + here->FEFET1nstar);
    Ssi = T1 / T2 * (T3 + T4 + T5) + T6 / T7 * DelClm * T8 / T9;
    return Ssi;
}


int
FEFET1noise (
int mode, int operation,
GENmodel *inModel,
CKTcircuit *ckt,
Ndata *data,
double *OnDens)
{
NOISEAN *job = (NOISEAN *) ckt->CKTcurJob;

FEFET1model *model = (FEFET1model *)inModel;
FEFET1instance *here;
struct fefet1SizeDependParam *pParam;
double tempOnoise;
double tempInoise;
double noizDens[FEFET1NSRCS];
double lnNdens[FEFET1NSRCS];

double T0, T1, T2, T5, T10, T11;
double Vds, Ssi, Swi;
double tmp=0.0, gdpr, gspr, npart_theta=0.0, npart_beta=0.0, igsquare, bodymode;

double m;

int i;

    /* define the names of the noise sources */
    static char *FEFET1nNames[FEFET1NSRCS] =
    {   /* Note that we have to keep the order */
        ".rd",              /* noise due to rd */
        ".rs",              /* noise due to rs */
        ".rg",              /* noise due to rgeltd */
        ".rbps",            /* noise due to rbps */
        ".rbpd",            /* noise due to rbpd */
        ".rbpb",            /* noise due to rbpb */
        ".rbsb",            /* noise due to rbsb */
        ".rbdb",            /* noise due to rbdb */
        ".id",              /* noise due to id */
        ".1overf",          /* flicker (1/f) noise */
        ".igs",             /* shot noise due to IGS */
        ".igd",             /* shot noise due to IGD */
        ".igb",             /* shot noise due to IGB */
        ""                  /* total transistor noise */
    };

    for (; model != NULL; model = FEFET1nextModel(model))
    {    for (here = FEFET1instances(model); here != NULL;
              here = FEFET1nextInstance(here))
         {    pParam = here->pParam;
              switch (operation)
              {  case N_OPEN:
                     /* see if we have to to produce a summary report */
                     /* if so, name all the noise generators */

                      if (job->NStpsSm != 0)
                      {   switch (mode)
                          {  case N_DENS:
                                  for (i = 0; i < FEFET1NSRCS; i++)
                                  {    NOISE_ADD_OUTVAR(ckt, data, "onoise.%s%s", here->FEFET1name, FEFET1nNames[i]);
                                  }
                                  break;
                             case INT_NOIZ:
                                  for (i = 0; i < FEFET1NSRCS; i++)
                                  {    NOISE_ADD_OUTVAR(ckt, data, "onoise_total.%s%s", here->FEFET1name, FEFET1nNames[i]);
                                       NOISE_ADD_OUTVAR(ckt, data, "inoise_total.%s%s", here->FEFET1name, FEFET1nNames[i]);
                                  }
                                  break;
                          }
                      }
                      break;
                 case N_CALC:
                      m = here->FEFET1m;
                      switch (mode)
                      {  case N_DENS:
                              if (model->FEFET1tnoiMod == 0)
                              {   if (model->FEFET1rdsMod == 0)
                                  {   gspr = here->FEFET1sourceConductance;
                                      gdpr = here->FEFET1drainConductance;
                                      if (here->FEFET1grdsw > 0.0)
                                          tmp = 1.0 / here->FEFET1grdsw; /* tmp used below */ 
                                      else
                                          tmp = 0.0;
                                  }
                                  else
                                  {   gspr = here->FEFET1gstot;
                                      gdpr = here->FEFET1gdtot;
                                      tmp = 0.0;
                                  }
                              }
                              else
                              {   T5 = here->FEFET1Vgsteff / here->FEFET1EsatL;
                                  T5 *= T5;
                                  npart_beta = model->FEFET1rnoia * (1.0 + T5
                                             * model->FEFET1tnoia * pParam->FEFET1leff);
                                  npart_theta = model->FEFET1rnoib * (1.0 + T5
                                              * model->FEFET1tnoib * pParam->FEFET1leff);

                                  if (model->FEFET1rdsMod == 0)
                                  {   gspr = here->FEFET1sourceConductance;
                                      gdpr = here->FEFET1drainConductance;
                                  }
                                  else
                                  {   gspr = here->FEFET1gstot;
                                      gdpr = here->FEFET1gdtot;
                                  }

                                  if ((*(ckt->CKTstates[0] + here->FEFET1vds)) >= 0.0)
                                      gspr = gspr / (1.0 + npart_theta * npart_theta * gspr
                                            / here->FEFET1IdovVds);  /* bugfix */
                                  else
                                      gdpr = gdpr / (1.0 + npart_theta * npart_theta * gdpr
                                           / here->FEFET1IdovVds);
                              } 

                              NevalSrc(&noizDens[FEFET1RDNOIZ],
                                       &lnNdens[FEFET1RDNOIZ], ckt, THERMNOISE,
                                       here->FEFET1dNodePrime, here->FEFET1dNode,
                                       gdpr * m);

                              NevalSrc(&noizDens[FEFET1RSNOIZ],
                                       &lnNdens[FEFET1RSNOIZ], ckt, THERMNOISE,
                                       here->FEFET1sNodePrime, here->FEFET1sNode,
                                       gspr * m);


                              if ((here->FEFET1rgateMod == 1) || (here->FEFET1rgateMod == 2))
                              {   NevalSrc(&noizDens[FEFET1RGNOIZ],
                                       &lnNdens[FEFET1RGNOIZ], ckt, THERMNOISE,
                                       here->FEFET1gNodePrime, here->FEFET1gNodeExt,
                                       here->FEFET1grgeltd * m);
                              }
                              else if (here->FEFET1rgateMod == 3)
                              {   NevalSrc(&noizDens[FEFET1RGNOIZ],
                                       &lnNdens[FEFET1RGNOIZ], ckt, THERMNOISE,
                                       here->FEFET1gNodeMid, here->FEFET1gNodeExt,
                                       here->FEFET1grgeltd * m);
                              }
                              else
                              {    noizDens[FEFET1RGNOIZ] = 0.0;
                                   lnNdens[FEFET1RGNOIZ] =
                                          log(MAX(noizDens[FEFET1RGNOIZ], N_MINLOG));
                              }

                                    bodymode = 5;
                                    if (here->FEFET1rbodyMod == 2)
                                    {        if( ( !model->FEFET1rbps0Given) || 
                                      ( !model->FEFET1rbpd0Given) )
                                             bodymode = 1;
                                           else 
                                     if( (!model->FEFET1rbsbx0Given && !model->FEFET1rbsby0Given) ||
                                          (!model->FEFET1rbdbx0Given && !model->FEFET1rbdby0Given) )
                                             bodymode = 3;
                                }

                              if (here->FEFET1rbodyMod)
                              { 
                                if(bodymode == 5)
                                  {
                                    NevalSrc(&noizDens[FEFET1RBPSNOIZ],
                                             &lnNdens[FEFET1RBPSNOIZ], ckt, THERMNOISE,
                                             here->FEFET1bNodePrime, here->FEFET1sbNode,
                                             here->FEFET1grbps * m);
                                    NevalSrc(&noizDens[FEFET1RBPDNOIZ],
                                             &lnNdens[FEFET1RBPDNOIZ], ckt, THERMNOISE,
                                             here->FEFET1bNodePrime, here->FEFET1dbNode,
                                             here->FEFET1grbpd * m);
                                    NevalSrc(&noizDens[FEFET1RBPBNOIZ],
                                             &lnNdens[FEFET1RBPBNOIZ], ckt, THERMNOISE,
                                             here->FEFET1bNodePrime, here->FEFET1bNode,
                                             here->FEFET1grbpb * m);
                                    NevalSrc(&noizDens[FEFET1RBSBNOIZ],
                                             &lnNdens[FEFET1RBSBNOIZ], ckt, THERMNOISE,
                                             here->FEFET1bNode, here->FEFET1sbNode,
                                             here->FEFET1grbsb * m);
                                    NevalSrc(&noizDens[FEFET1RBDBNOIZ],
                                             &lnNdens[FEFET1RBDBNOIZ], ckt, THERMNOISE,
                                             here->FEFET1bNode, here->FEFET1dbNode,
                                             here->FEFET1grbdb * m);
                                  }
                                if(bodymode == 3)
                                  {
                                    NevalSrc(&noizDens[FEFET1RBPSNOIZ],
                                             &lnNdens[FEFET1RBPSNOIZ], ckt, THERMNOISE,
                                             here->FEFET1bNodePrime, here->FEFET1sbNode,
                                             here->FEFET1grbps * m);
                                    NevalSrc(&noizDens[FEFET1RBPDNOIZ],
                                             &lnNdens[FEFET1RBPDNOIZ], ckt, THERMNOISE,
                                             here->FEFET1bNodePrime, here->FEFET1dbNode,
                                             here->FEFET1grbpd * m);
                                    NevalSrc(&noizDens[FEFET1RBPBNOIZ],
                                             &lnNdens[FEFET1RBPBNOIZ], ckt, THERMNOISE,
                                             here->FEFET1bNodePrime, here->FEFET1bNode,
                                             here->FEFET1grbpb * m);
                                     noizDens[FEFET1RBSBNOIZ] = noizDens[FEFET1RBDBNOIZ] = 0.0;
                                     lnNdens[FEFET1RBSBNOIZ] =
                                       log(MAX(noizDens[FEFET1RBSBNOIZ], N_MINLOG));
                                     lnNdens[FEFET1RBDBNOIZ] =
                                       log(MAX(noizDens[FEFET1RBDBNOIZ], N_MINLOG));                                     
                                  }
                                if(bodymode == 1)
                                  {
                                    NevalSrc(&noizDens[FEFET1RBPBNOIZ],
                                             &lnNdens[FEFET1RBPBNOIZ], ckt, THERMNOISE,
                                             here->FEFET1bNodePrime, here->FEFET1bNode,
                                             here->FEFET1grbpb * m);                                    
                                    noizDens[FEFET1RBPSNOIZ] = noizDens[FEFET1RBPDNOIZ] = 0.0;                                    
                                    noizDens[FEFET1RBSBNOIZ] = noizDens[FEFET1RBDBNOIZ] = 0.0;
                                    lnNdens[FEFET1RBPSNOIZ] =
                                      log(MAX(noizDens[FEFET1RBPSNOIZ], N_MINLOG));
                                    lnNdens[FEFET1RBPDNOIZ] =
                                      log(MAX(noizDens[FEFET1RBPDNOIZ], N_MINLOG));
                                    lnNdens[FEFET1RBSBNOIZ] =
                                      log(MAX(noizDens[FEFET1RBSBNOIZ], N_MINLOG));
                                    lnNdens[FEFET1RBDBNOIZ] =
                                      log(MAX(noizDens[FEFET1RBDBNOIZ], N_MINLOG));
                                  }
                              }
                              else
                              {   noizDens[FEFET1RBPSNOIZ] = noizDens[FEFET1RBPDNOIZ] = 0.0;   
                                  noizDens[FEFET1RBPBNOIZ] = 0.0;
                                  noizDens[FEFET1RBSBNOIZ] = noizDens[FEFET1RBDBNOIZ] = 0.0;
                                  lnNdens[FEFET1RBPSNOIZ] =
                                          log(MAX(noizDens[FEFET1RBPSNOIZ], N_MINLOG));
                                  lnNdens[FEFET1RBPDNOIZ] =
                                          log(MAX(noizDens[FEFET1RBPDNOIZ], N_MINLOG));
                                  lnNdens[FEFET1RBPBNOIZ] =
                                          log(MAX(noizDens[FEFET1RBPBNOIZ], N_MINLOG));
                                  lnNdens[FEFET1RBSBNOIZ] =
                                          log(MAX(noizDens[FEFET1RBSBNOIZ], N_MINLOG));
                                  lnNdens[FEFET1RBDBNOIZ] =
                                          log(MAX(noizDens[FEFET1RBDBNOIZ], N_MINLOG));
                              }


                              switch(model->FEFET1tnoiMod)
                              {  case 0:
                                      T0 = here->FEFET1ueff * fabs(here->FEFET1qinv);
                                      T1 = T0 * tmp + pParam->FEFET1leff
                                         * pParam->FEFET1leff;
                                      NevalSrc(&noizDens[FEFET1IDNOIZ],
                                               &lnNdens[FEFET1IDNOIZ], ckt,
                                               THERMNOISE, here->FEFET1dNodePrime,
                                               here->FEFET1sNodePrime,
                                               m * (T0 / T1) * model->FEFET1ntnoi);
                                      break;
                                 case 1:
                                      T0 = here->FEFET1gm + here->FEFET1gmbs + here->FEFET1gds;
                                      T0 *= T0;
                                      igsquare = npart_theta * npart_theta * T0 / here->FEFET1IdovVds;
                                      T1 = npart_beta * (here->FEFET1gm
                                         + here->FEFET1gmbs) + here->FEFET1gds;
                                      T2 = T1 * T1 / here->FEFET1IdovVds;
                                      NevalSrc(&noizDens[FEFET1IDNOIZ],
                                               &lnNdens[FEFET1IDNOIZ], ckt,
                                               THERMNOISE, here->FEFET1dNodePrime,
                                               here->FEFET1sNodePrime, m * (T2 - igsquare));
                                      break;
                              }

                              NevalSrc(&noizDens[FEFET1FLNOIZ], NULL,
                                       ckt, N_GAIN, here->FEFET1dNodePrime,
                                       here->FEFET1sNodePrime, (double) 0.0);

                              switch(model->FEFET1fnoiMod)
                              {  case 0:
                                      noizDens[FEFET1FLNOIZ] *= m * model->FEFET1kf
                                            * exp(model->FEFET1af
                                            * log(MAX(fabs(here->FEFET1cd),
                                            N_MINLOG)))
                                            / (pow(data->freq, model->FEFET1ef)
                                            * pParam->FEFET1leff
                                            * pParam->FEFET1leff
                                            * model->FEFET1coxe);
                                      break;
                                 case 1:
                                      Vds = *(ckt->CKTstates[0] + here->FEFET1vds);
                                      if (Vds < 0.0)
                                          Vds = -Vds;

                                      Ssi = FEFET1Eval1ovFNoise(Vds, model, here,
                                          data->freq, ckt->CKTtemp);
                                      T10 = model->FEFET1oxideTrapDensityA
                                          * CONSTboltz * ckt->CKTtemp;
                                      T11 = pParam->FEFET1weff * here->FEFET1nf * pParam->FEFET1leff
                                          * pow(data->freq, model->FEFET1ef) * 1.0e10
                                          * here->FEFET1nstar * here->FEFET1nstar;
                                      Swi = T10 / T11 * here->FEFET1cd
                                          * here->FEFET1cd;
                                      T1 = Swi + Ssi;
                                      if (T1 > 0.0)
                                          noizDens[FEFET1FLNOIZ] *= m * (Ssi * Swi) / T1;
                                      else
                                          noizDens[FEFET1FLNOIZ] *= 0.0;
                                      break;
                              }

                              lnNdens[FEFET1FLNOIZ] =
                                     log(MAX(noizDens[FEFET1FLNOIZ], N_MINLOG));


                               if(here->FEFET1mode >= 0) {  /* bugfix  */
                              NevalSrc(&noizDens[FEFET1IGSNOIZ],
                                   &lnNdens[FEFET1IGSNOIZ], ckt, SHOTNOISE,
                                   here->FEFET1gNodePrime, here->FEFET1sNodePrime,
                                   m * (here->FEFET1Igs + here->FEFET1Igcs));
                              NevalSrc(&noizDens[FEFET1IGDNOIZ],
                                   &lnNdens[FEFET1IGDNOIZ], ckt, SHOTNOISE,
                                   here->FEFET1gNodePrime, here->FEFET1dNodePrime,
                                   m * (here->FEFET1Igd + here->FEFET1Igcd));
                        } else {
                              NevalSrc(&noizDens[FEFET1IGSNOIZ],
                                   &lnNdens[FEFET1IGSNOIZ], ckt, SHOTNOISE,
                                   here->FEFET1gNodePrime, here->FEFET1sNodePrime,
                                   m * (here->FEFET1Igs + here->FEFET1Igcd));
                              NevalSrc(&noizDens[FEFET1IGDNOIZ],
                                   &lnNdens[FEFET1IGDNOIZ], ckt, SHOTNOISE,
                                   here->FEFET1gNodePrime, here->FEFET1dNodePrime,
                                   m * (here->FEFET1Igd + here->FEFET1Igcs));
                        }
                              NevalSrc(&noizDens[FEFET1IGBNOIZ],
                                   &lnNdens[FEFET1IGBNOIZ], ckt, SHOTNOISE,
                                   here->FEFET1gNodePrime, here->FEFET1bNodePrime,
                                   m * here->FEFET1Igb);


                              noizDens[FEFET1TOTNOIZ] = noizDens[FEFET1RDNOIZ]
                                     + noizDens[FEFET1RSNOIZ] + noizDens[FEFET1RGNOIZ]
                                     + noizDens[FEFET1RBPSNOIZ] + noizDens[FEFET1RBPDNOIZ]
                                     + noizDens[FEFET1RBPBNOIZ]
                                     + noizDens[FEFET1RBSBNOIZ] + noizDens[FEFET1RBDBNOIZ]
                                     + noizDens[FEFET1IDNOIZ] + noizDens[FEFET1FLNOIZ]
                                     + noizDens[FEFET1IGSNOIZ] + noizDens[FEFET1IGDNOIZ]
                                     + noizDens[FEFET1IGBNOIZ];
                              lnNdens[FEFET1TOTNOIZ] = 
                                     log(MAX(noizDens[FEFET1TOTNOIZ], N_MINLOG));

                              *OnDens += noizDens[FEFET1TOTNOIZ];

                              if (data->delFreq == 0.0)
                              {   /* if we haven't done any previous 
                                     integration, we need to initialize our
                                     "history" variables.
                                    */

                                  for (i = 0; i < FEFET1NSRCS; i++)
                                  {    here->FEFET1nVar[LNLSTDENS][i] =
                                             lnNdens[i];
                                  }

                                  /* clear out our integration variables
                                     if it's the first pass
                                   */
                                  if (data->freq ==
                                      job->NstartFreq)
                                  {   for (i = 0; i < FEFET1NSRCS; i++)
                                      {    here->FEFET1nVar[OUTNOIZ][i] = 0.0;
                                           here->FEFET1nVar[INNOIZ][i] = 0.0;
                                      }
                                  }
                              }
                              else
                              {   /* data->delFreq != 0.0,
                                     we have to integrate.
                                   */
                                  for (i = 0; i < FEFET1NSRCS; i++)
                                  {    if (i != FEFET1TOTNOIZ)
                                       {   tempOnoise = Nintegrate(noizDens[i],
                                                lnNdens[i],
                                                here->FEFET1nVar[LNLSTDENS][i],
                                                data);
                                           tempInoise = Nintegrate(noizDens[i]
                                                * data->GainSqInv, lnNdens[i]
                                                + data->lnGainInv,
                                                here->FEFET1nVar[LNLSTDENS][i]
                                                + data->lnGainInv, data);
                                           here->FEFET1nVar[LNLSTDENS][i] =
                                                lnNdens[i];
                                           data->outNoiz += tempOnoise;
                                           data->inNoise += tempInoise;
                                           if (job->NStpsSm != 0)
                                           {   here->FEFET1nVar[OUTNOIZ][i]
                                                     += tempOnoise;
                                               here->FEFET1nVar[OUTNOIZ][FEFET1TOTNOIZ]
                                                     += tempOnoise;
                                               here->FEFET1nVar[INNOIZ][i]
                                                     += tempInoise;
                                               here->FEFET1nVar[INNOIZ][FEFET1TOTNOIZ]
                                                     += tempInoise;
                                           }
                                       }
                                  }
                              }
                              if (data->prtSummary)
                              {   for (i = 0; i < FEFET1NSRCS; i++)
                                  {    /* print a summary report */
                                       data->outpVector[data->outNumber++]
                                             = noizDens[i];
                                  }
                              }
                              break;
                         case INT_NOIZ:
                              /* already calculated, just output */
                              if (job->NStpsSm != 0)
                              {   for (i = 0; i < FEFET1NSRCS; i++)
                                  {    data->outpVector[data->outNumber++]
                                             = here->FEFET1nVar[OUTNOIZ][i];
                                       data->outpVector[data->outNumber++]
                                             = here->FEFET1nVar[INNOIZ][i];
                                  }
                              }
                              break;
                      }
                      break;
                 case N_CLOSE:
                      /* do nothing, the main calling routine will close */
                      return (OK);
                      break;   /* the plots */
              }       /* switch (operation) */
         }    /* for here */
    }    /* for model */

    return(OK);
}
