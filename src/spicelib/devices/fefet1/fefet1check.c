/**** BSIM4.5.0 Released by Xuemei (Jane) Xi 07/29/2005 ****/

/**********
 * Copyright 2005 Regents of the University of California. All rights reserved.
 * File: b4check.c of BSIM4.5.0.
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
#include "ngspice/cktdefs.h"
#include "fefet1def.h"
#include "ngspice/trandefs.h"
#include "ngspice/const.h"
#include "ngspice/sperror.h"
#include "ngspice/devdefs.h"
#include "ngspice/suffix.h"

int
FEFET1checkModel(
FEFET1model *model,
FEFET1instance *here,
CKTcircuit *ckt)
{
struct fefet1SizeDependParam *pParam;
int Fatal_Flag = 0;
FILE *fplog;
    
    if ((fplog = fopen("fefet1.out", "w")) != NULL)
    {   pParam = here->pParam;
        fprintf(fplog, "FEFET1: Berkeley Short Channel IGFET Model-4\n");
        fprintf(fplog, "Developed by Xuemei (Jane) Xi, Mohan Dunga, Prof. Ali Niknejad and Prof. Chenming Hu in 2003.\n");
        fprintf(fplog, "\n");
        fprintf(fplog, "++++++++++ FEFET1 PARAMETER CHECKING BELOW ++++++++++\n");

        if ((strcmp(model->FEFET1version, "4.5.0")) && (strncmp(model->FEFET1version, "4.50", 4)) && (strncmp(model->FEFET1version, "4.5", 3)))
        {  fprintf(fplog, "Warning: This model is BSIM4.5.0; you specified a wrong version number '%s'.\n", model->FEFET1version);
           printf("Warning: This model is BSIM4.5.0; you specified a wrong version number '%s'.\n", model->FEFET1version);
        }
        fprintf(fplog, "Model = %s\n", model->FEFET1modName);


        if ((here->FEFET1rgateMod == 2) || (here->FEFET1rgateMod == 3))
        {   if ((here->FEFET1trnqsMod == 1) || (here->FEFET1acnqsMod == 1))
            {   fprintf(fplog, "Warning: You've selected both Rg and charge deficit NQS; select one only.\n");
                printf("Warning: You've selected both Rg and charge deficit NQS; select one only.\n");
            }
        }

	if (model->FEFET1toxe <= 0.0)
	{   fprintf(fplog, "Fatal: Toxe = %g is not positive.\n",
		    model->FEFET1toxe);
	    printf("Fatal: Toxe = %g is not positive.\n", model->FEFET1toxe);
	    Fatal_Flag = 1;
	}
        if (model->FEFET1toxp <= 0.0)
        {   fprintf(fplog, "Fatal: Toxp = %g is not positive.\n",
                    model->FEFET1toxp);
            printf("Fatal: Toxp = %g is not positive.\n", model->FEFET1toxp);
            Fatal_Flag = 1;
        }

        if (model->FEFET1toxm <= 0.0)
        {   fprintf(fplog, "Fatal: Toxm = %g is not positive.\n",
                    model->FEFET1toxm);
            printf("Fatal: Toxm = %g is not positive.\n", model->FEFET1toxm);
            Fatal_Flag = 1;
        }

        if (model->FEFET1toxref <= 0.0)
        {   fprintf(fplog, "Fatal: Toxref = %g is not positive.\n",
                    model->FEFET1toxref);
            printf("Fatal: Toxref = %g is not positive.\n", model->FEFET1toxref);
            Fatal_Flag = 1;
        }

        if (pParam->FEFET1lpe0 < -pParam->FEFET1leff)
        {   fprintf(fplog, "Fatal: Lpe0 = %g is less than -Leff.\n",
                    pParam->FEFET1lpe0);
            printf("Fatal: Lpe0 = %g is less than -Leff.\n",
                        pParam->FEFET1lpe0);
            Fatal_Flag = 1;
        }
        if (model->FEFET1lintnoi > pParam->FEFET1leff/2)
        {   fprintf(fplog, "Fatal: Lintnoi = %g is too large - Leff for noise is negative.\n",
                    model->FEFET1lintnoi);
            printf("Fatal: Lintnoi = %g is too large - Leff for noise is negative.\n",
                    model->FEFET1lintnoi);
            Fatal_Flag = 1;
        }
        if (pParam->FEFET1lpeb < -pParam->FEFET1leff)
        {   fprintf(fplog, "Fatal: Lpeb = %g is less than -Leff.\n",
                    pParam->FEFET1lpeb);
            printf("Fatal: Lpeb = %g is less than -Leff.\n",
                        pParam->FEFET1lpeb);
            Fatal_Flag = 1;
        }
	if (pParam->FEFET1ndep <= 0.0)
	{   fprintf(fplog, "Fatal: Ndep = %g is not positive.\n",
		    pParam->FEFET1ndep);
	    printf("Fatal: Ndep = %g is not positive.\n",
		   pParam->FEFET1ndep);
	    Fatal_Flag = 1;
	}
        if (pParam->FEFET1phi <= 0.0)
        {   fprintf(fplog, "Fatal: Phi = %g is not positive. Please check Phin and Ndep\n",
                    pParam->FEFET1phi);
            fprintf(fplog, "	   Phin = %g  Ndep = %g \n", 
            	    pParam->FEFET1phin, pParam->FEFET1ndep);
            printf("Fatal: Phi = %g is not positive. Please check Phin and Ndep\n",
                    pParam->FEFET1phi);
            printf("	   Phin = %g  Ndep = %g \n", 
            	    pParam->FEFET1phin, pParam->FEFET1ndep);
            Fatal_Flag = 1;
        }
	if (pParam->FEFET1nsub <= 0.0)
	{   fprintf(fplog, "Fatal: Nsub = %g is not positive.\n",
		    pParam->FEFET1nsub);
	    printf("Fatal: Nsub = %g is not positive.\n",
		   pParam->FEFET1nsub);
	    Fatal_Flag = 1;
	}
	if (pParam->FEFET1ngate < 0.0)
	{   fprintf(fplog, "Fatal: Ngate = %g is not positive.\n",
		    pParam->FEFET1ngate);
	    printf("Fatal: Ngate = %g Ngate is not positive.\n",
		   pParam->FEFET1ngate);
	    Fatal_Flag = 1;
	}
	if (pParam->FEFET1ngate > 1.e25)
	{   fprintf(fplog, "Fatal: Ngate = %g is too high.\n",
		    pParam->FEFET1ngate);
	    printf("Fatal: Ngate = %g Ngate is too high\n",
		   pParam->FEFET1ngate);
	    Fatal_Flag = 1;
	}
	if (pParam->FEFET1xj <= 0.0)
	{   fprintf(fplog, "Fatal: Xj = %g is not positive.\n",
		    pParam->FEFET1xj);
	    printf("Fatal: Xj = %g is not positive.\n", pParam->FEFET1xj);
	    Fatal_Flag = 1;
	}

	if (pParam->FEFET1dvt1 < 0.0)
	{   fprintf(fplog, "Fatal: Dvt1 = %g is negative.\n",
		    pParam->FEFET1dvt1);   
	    printf("Fatal: Dvt1 = %g is negative.\n", pParam->FEFET1dvt1);   
	    Fatal_Flag = 1;
	}
	    
	if (pParam->FEFET1dvt1w < 0.0)
	{   fprintf(fplog, "Fatal: Dvt1w = %g is negative.\n",
		    pParam->FEFET1dvt1w);
	    printf("Fatal: Dvt1w = %g is negative.\n", pParam->FEFET1dvt1w);
	    Fatal_Flag = 1;
	}
	    
	if (pParam->FEFET1w0 == -pParam->FEFET1weff)
	{   fprintf(fplog, "Fatal: (W0 + Weff) = 0 causing divided-by-zero.\n");
	    printf("Fatal: (W0 + Weff) = 0 causing divided-by-zero.\n");
	    Fatal_Flag = 1;
        }   

	if (pParam->FEFET1dsub < 0.0)
	{   fprintf(fplog, "Fatal: Dsub = %g is negative.\n", pParam->FEFET1dsub);
	    printf("Fatal: Dsub = %g is negative.\n", pParam->FEFET1dsub);
	    Fatal_Flag = 1;
	}
	if (pParam->FEFET1b1 == -pParam->FEFET1weff)
	{   fprintf(fplog, "Fatal: (B1 + Weff) = 0 causing divided-by-zero.\n");
	    printf("Fatal: (B1 + Weff) = 0 causing divided-by-zero.\n");
	    Fatal_Flag = 1;
        }  
        if (here->FEFET1u0temp <= 0.0)
	{   fprintf(fplog, "Fatal: u0 at current temperature = %g is not positive.\n", here->FEFET1u0temp);
	    printf("Fatal: u0 at current temperature = %g is not positive.\n",
		   here->FEFET1u0temp);
	    Fatal_Flag = 1;
        }
    
        if (pParam->FEFET1delta < 0.0)
	{   fprintf(fplog, "Fatal: Delta = %g is less than zero.\n",
		    pParam->FEFET1delta);
	    printf("Fatal: Delta = %g is less than zero.\n", pParam->FEFET1delta);
	    Fatal_Flag = 1;
        }      

	if (here->FEFET1vsattemp <= 0.0)
	{   fprintf(fplog, "Fatal: Vsat at current temperature = %g is not positive.\n", here->FEFET1vsattemp);
	    printf("Fatal: Vsat at current temperature = %g is not positive.\n",
		   here->FEFET1vsattemp);
	    Fatal_Flag = 1;
	}

	if (pParam->FEFET1pclm <= 0.0)
	{   fprintf(fplog, "Fatal: Pclm = %g is not positive.\n", pParam->FEFET1pclm);
	    printf("Fatal: Pclm = %g is not positive.\n", pParam->FEFET1pclm);
	    Fatal_Flag = 1;
	}

	if (pParam->FEFET1drout < 0.0)
	{   fprintf(fplog, "Fatal: Drout = %g is negative.\n", pParam->FEFET1drout);
	    printf("Fatal: Drout = %g is negative.\n", pParam->FEFET1drout);
	    Fatal_Flag = 1;
	}

        if (here->FEFET1m <= 0.0)
        {   fprintf(fplog, "Fatal: multiplier = %g is not positive.\n", here->FEFET1m);
            printf("Fatal: multiplier = %g is not positive.\n", here->FEFET1m);
            Fatal_Flag = 1;
        }
        if (here->FEFET1nf < 1.0)
        {   fprintf(fplog, "Fatal: Number of finger = %g is smaller than one.\n", here->FEFET1nf);
            printf("Fatal: Number of finger = %g is smaller than one.\n", here->FEFET1nf);
            Fatal_Flag = 1;
        }

        if((here->FEFET1sa > 0.0) && (here->FEFET1sb > 0.0) && 
       	((here->FEFET1nf == 1.0) || ((here->FEFET1nf > 1.0) && (here->FEFET1sd > 0.0))) )
        {   if (model->FEFET1saref <= 0.0)
            {   fprintf(fplog, "Fatal: SAref = %g is not positive.\n",model->FEFET1saref);
             	printf("Fatal: SAref = %g is not positive.\n",model->FEFET1saref);
             	Fatal_Flag = 1;
            }
            if (model->FEFET1sbref <= 0.0)
            {   fprintf(fplog, "Fatal: SBref = %g is not positive.\n",model->FEFET1sbref);
            	printf("Fatal: SBref = %g is not positive.\n",model->FEFET1sbref);
            	Fatal_Flag = 1;
            }
 	}

        if ((here->FEFET1l + model->FEFET1xl) <= model->FEFET1xgl)
        {   fprintf(fplog, "Fatal: The parameter xgl must be smaller than Ldrawn+XL.\n");
            printf("Fatal: The parameter xgl must be smaller than Ldrawn+XL.\n");
            Fatal_Flag = 1;
        }
        if (here->FEFET1ngcon < 1.0)
        {   fprintf(fplog, "Fatal: The parameter ngcon cannot be smaller than one.\n");
            printf("Fatal: The parameter ngcon cannot be smaller than one.\n");
            Fatal_Flag = 1;
        }
        if ((here->FEFET1ngcon != 1.0) && (here->FEFET1ngcon != 2.0))
        {   here->FEFET1ngcon = 1.0;
            fprintf(fplog, "Warning: Ngcon must be equal to one or two; reset to 1.0.\n");
            printf("Warning: Ngcon must be equal to one or two; reset to 1.0.\n");
        }

        if (model->FEFET1gbmin < 1.0e-20)
        {   fprintf(fplog, "Warning: Gbmin = %g is too small.\n",
                    model->FEFET1gbmin);
            printf("Warning: Gbmin = %g is too small.\n", model->FEFET1gbmin);
        }

        /* Check saturation parameters */
        if (pParam->FEFET1fprout < 0.0)
        {   fprintf(fplog, "Fatal: fprout = %g is negative.\n",
                    pParam->FEFET1fprout);
            printf("Fatal: fprout = %g is negative.\n", pParam->FEFET1fprout);
	    Fatal_Flag = 1;
        }
        if (pParam->FEFET1pdits < 0.0)
        {   fprintf(fplog, "Fatal: pdits = %g is negative.\n",
                    pParam->FEFET1pdits);
            printf("Fatal: pdits = %g is negative.\n", pParam->FEFET1pdits);
            Fatal_Flag = 1;
        }
        if (model->FEFET1pditsl < 0.0)
        {   fprintf(fplog, "Fatal: pditsl = %g is negative.\n",
                    model->FEFET1pditsl);
            printf("Fatal: pditsl = %g is negative.\n", model->FEFET1pditsl);
            Fatal_Flag = 1;
        }

        /* Check gate current parameters */
      if (model->FEFET1igbMod) {
        if (pParam->FEFET1nigbinv <= 0.0)
        {   fprintf(fplog, "Fatal: nigbinv = %g is non-positive.\n",
                    pParam->FEFET1nigbinv);
            printf("Fatal: nigbinv = %g is non-positive.\n", pParam->FEFET1nigbinv);
            Fatal_Flag = 1;
        }
        if (pParam->FEFET1nigbacc <= 0.0)
        {   fprintf(fplog, "Fatal: nigbacc = %g is non-positive.\n",
                    pParam->FEFET1nigbacc);
            printf("Fatal: nigbacc = %g is non-positive.\n", pParam->FEFET1nigbacc);
            Fatal_Flag = 1;
        }
      }
      if (model->FEFET1igcMod) {
        if (pParam->FEFET1nigc <= 0.0)
        {   fprintf(fplog, "Fatal: nigc = %g is non-positive.\n",
                    pParam->FEFET1nigc);
            printf("Fatal: nigc = %g is non-positive.\n", pParam->FEFET1nigc);
            Fatal_Flag = 1;
        }
        if (pParam->FEFET1poxedge <= 0.0)
        {   fprintf(fplog, "Fatal: poxedge = %g is non-positive.\n",
                    pParam->FEFET1poxedge);
            printf("Fatal: poxedge = %g is non-positive.\n", pParam->FEFET1poxedge);
            Fatal_Flag = 1;
        }
        if (pParam->FEFET1pigcd <= 0.0)
        {   fprintf(fplog, "Fatal: pigcd = %g is non-positive.\n",
                    pParam->FEFET1pigcd);
            printf("Fatal: pigcd = %g is non-positive.\n", pParam->FEFET1pigcd);
            Fatal_Flag = 1;
        }
      }

        /* Check capacitance parameters */
        if (pParam->FEFET1clc < 0.0)
	{   fprintf(fplog, "Fatal: Clc = %g is negative.\n", pParam->FEFET1clc);
	    printf("Fatal: Clc = %g is negative.\n", pParam->FEFET1clc);
	    Fatal_Flag = 1;
        }      

        /* Check overlap capacitance parameters */
        if (pParam->FEFET1ckappas < 0.02)
        {   fprintf(fplog, "Warning: ckappas = %g is too small. Set to 0.02\n",
                    pParam->FEFET1ckappas);
            printf("Warning: ckappas = %g is too small.\n", pParam->FEFET1ckappas);
            pParam->FEFET1ckappas = 0.02;
       }
        if (pParam->FEFET1ckappad < 0.02)
        {   fprintf(fplog, "Warning: ckappad = %g is too small. Set to 0.02\n",
                    pParam->FEFET1ckappad);
            printf("Warning: ckappad = %g is too small.\n", pParam->FEFET1ckappad);
            pParam->FEFET1ckappad = 0.02;
        }

	if (model->FEFET1vtss < 0.0)
	{   fprintf(fplog, "Fatal: Vtss = %g is negative.\n",
			model->FEFET1vtss);
	    printf("Fatal: Vtss = %g is negative.\n",
			model->FEFET1vtss);
	    Fatal_Flag = 1;
	}
	if (model->FEFET1vtsd < 0.0)
	{   fprintf(fplog, "Fatal: Vtsd = %g is negative.\n",
			model->FEFET1vtsd);
	    printf("Fatal: Vtsd = %g is negative.\n",
		    model->FEFET1vtsd);
	    Fatal_Flag = 1;
	}
	if (model->FEFET1vtssws < 0.0)
	{   fprintf(fplog, "Fatal: Vtssws = %g is negative.\n",
		    model->FEFET1vtssws);
	    printf("Fatal: Vtssws = %g is negative.\n",
		    model->FEFET1vtssws);
	    Fatal_Flag = 1;
	}
	if (model->FEFET1vtsswd < 0.0)
	{   fprintf(fplog, "Fatal: Vtsswd = %g is negative.\n",
		    model->FEFET1vtsswd);
	    printf("Fatal: Vtsswd = %g is negative.\n",
		    model->FEFET1vtsswd);
	    Fatal_Flag = 1;
	}
	if (model->FEFET1vtsswgs < 0.0)
	{   fprintf(fplog, "Fatal: Vtsswgs = %g is negative.\n",
		    model->FEFET1vtsswgs);
	    printf("Fatal: Vtsswgs = %g is negative.\n",
		    model->FEFET1vtsswgs);
	    Fatal_Flag = 1;
	}
	if (model->FEFET1vtsswgd < 0.0)
	{   fprintf(fplog, "Fatal: Vtsswgd = %g is negative.\n",
		    model->FEFET1vtsswgd);
	    printf("Fatal: Vtsswgd = %g is negative.\n",
			model->FEFET1vtsswgd);
	    Fatal_Flag = 1;
	}


      if (model->FEFET1paramChk ==1)
      {
/* Check L and W parameters */ 
	if (pParam->FEFET1leff <= 1.0e-9)
	{   fprintf(fplog, "Warning: Leff = %g <= 1.0e-9. Recommended Leff >= 1e-8 \n", 
		    pParam->FEFET1leff);
	    printf("Warning: Leff = %g <= 1.0e-9. Recommended Leff >= 1e-8 \n",
		    pParam->FEFET1leff);
	}    
	
	if (pParam->FEFET1leffCV <= 1.0e-9)
	{   fprintf(fplog, "Warning: Leff for CV = %g <= 1.0e-9. Recommended LeffCV >=1e-8 \n",
		    pParam->FEFET1leffCV);
	    printf("Warning: Leff for CV = %g <= 1.0e-9. Recommended LeffCV >=1e-8 \n",
		    pParam->FEFET1leffCV);
	}  
	
        if (pParam->FEFET1weff <= 1.0e-9)
	{   fprintf(fplog, "Warning: Weff = %g <= 1.0e-9. Recommended Weff >=1e-7 \n",
		    pParam->FEFET1weff);
	    printf("Warning: Weff = %g <= 1.0e-9. Recommended Weff >=1e-7 \n",
		   pParam->FEFET1weff);
	}             
	
	if (pParam->FEFET1weffCV <= 1.0e-9)
	{   fprintf(fplog, "Warning: Weff for CV = %g <= 1.0e-9. Recommended WeffCV >= 1e-7 \n",
		    pParam->FEFET1weffCV);
	    printf("Warning: Weff for CV = %g <= 1.0e-9. Recommended WeffCV >= 1e-7 \n",
		    pParam->FEFET1weffCV);
	}        
	
        /* Check threshold voltage parameters */
	if (model->FEFET1toxe < 1.0e-10)
	{   fprintf(fplog, "Warning: Toxe = %g is less than 1A. Recommended Toxe >= 5A\n",
	            model->FEFET1toxe);
	    printf("Warning: Toxe = %g is less than 1A. Recommended Toxe >= 5A\n", model->FEFET1toxe);
        }
        if (model->FEFET1toxp < 1.0e-10)
        {   fprintf(fplog, "Warning: Toxp = %g is less than 1A. Recommended Toxp >= 5A\n",
                    model->FEFET1toxp);
            printf("Warning: Toxp = %g is less than 1A. Recommended Toxp >= 5A\n", model->FEFET1toxp);
        }
        if (model->FEFET1toxm < 1.0e-10)
        {   fprintf(fplog, "Warning: Toxm = %g is less than 1A. Recommended Toxm >= 5A\n",
                    model->FEFET1toxm);
            printf("Warning: Toxm = %g is less than 1A. Recommended Toxm >= 5A\n", model->FEFET1toxm);
        }

        if (pParam->FEFET1ndep <= 1.0e12)
	{   fprintf(fplog, "Warning: Ndep = %g may be too small.\n",
	            pParam->FEFET1ndep);
	    printf("Warning: Ndep = %g may be too small.\n",
	           pParam->FEFET1ndep);
	}
	else if (pParam->FEFET1ndep >= 1.0e21)
	{   fprintf(fplog, "Warning: Ndep = %g may be too large.\n",
	            pParam->FEFET1ndep);
	    printf("Warning: Ndep = %g may be too large.\n",
	           pParam->FEFET1ndep);
	}

	 if (pParam->FEFET1nsub <= 1.0e14)
	{   fprintf(fplog, "Warning: Nsub = %g may be too small.\n",
	            pParam->FEFET1nsub);
	    printf("Warning: Nsub = %g may be too small.\n",
	           pParam->FEFET1nsub);
	}
	else if (pParam->FEFET1nsub >= 1.0e21)
	{   fprintf(fplog, "Warning: Nsub = %g may be too large.\n",
	            pParam->FEFET1nsub);
	    printf("Warning: Nsub = %g may be too large.\n",
	           pParam->FEFET1nsub);
	}

	if ((pParam->FEFET1ngate > 0.0) &&
	    (pParam->FEFET1ngate <= 1.e18))
	{   fprintf(fplog, "Warning: Ngate = %g is less than 1.E18cm^-3.\n",
	            pParam->FEFET1ngate);
	    printf("Warning: Ngate = %g is less than 1.E18cm^-3.\n",
	           pParam->FEFET1ngate);
	}
       
        if (pParam->FEFET1dvt0 < 0.0)
	{   fprintf(fplog, "Warning: Dvt0 = %g is negative.\n",
		    pParam->FEFET1dvt0);   
	    printf("Warning: Dvt0 = %g is negative.\n", pParam->FEFET1dvt0);   
	}
	    
	if (fabs(1.0e-8 / (pParam->FEFET1w0 + pParam->FEFET1weff)) > 10.0)
	{   fprintf(fplog, "Warning: (W0 + Weff) may be too small.\n");
	    printf("Warning: (W0 + Weff) may be too small.\n");
        }

/* Check subthreshold parameters */
	if (pParam->FEFET1nfactor < 0.0)
	{   fprintf(fplog, "Warning: Nfactor = %g is negative.\n",
		    pParam->FEFET1nfactor);
	    printf("Warning: Nfactor = %g is negative.\n", pParam->FEFET1nfactor);
	}
	if (pParam->FEFET1cdsc < 0.0)
	{   fprintf(fplog, "Warning: Cdsc = %g is negative.\n",
		    pParam->FEFET1cdsc);
	    printf("Warning: Cdsc = %g is negative.\n", pParam->FEFET1cdsc);
	}
	if (pParam->FEFET1cdscd < 0.0)
	{   fprintf(fplog, "Warning: Cdscd = %g is negative.\n",
		    pParam->FEFET1cdscd);
	    printf("Warning: Cdscd = %g is negative.\n", pParam->FEFET1cdscd);
	}
/* Check DIBL parameters */
	if (here->FEFET1eta0 < 0.0)
	{   fprintf(fplog, "Warning: Eta0 = %g is negative.\n",
		    here->FEFET1eta0); 
	    printf("Warning: Eta0 = %g is negative.\n", here->FEFET1eta0); 
	}
	      
/* Check Abulk parameters */	    
	 if (fabs(1.0e-8 / (pParam->FEFET1b1 + pParam->FEFET1weff)) > 10.0)
       	{   fprintf(fplog, "Warning: (B1 + Weff) may be too small.\n");
       	    printf("Warning: (B1 + Weff) may be too small.\n");
        }    
    

/* Check Saturation parameters */
     	if (pParam->FEFET1a2 < 0.01)
	{   fprintf(fplog, "Warning: A2 = %g is too small. Set to 0.01.\n", pParam->FEFET1a2);
	    printf("Warning: A2 = %g is too small. Set to 0.01.\n",
		   pParam->FEFET1a2);
	    pParam->FEFET1a2 = 0.01;
	}
	else if (pParam->FEFET1a2 > 1.0)
	{   fprintf(fplog, "Warning: A2 = %g is larger than 1. A2 is set to 1 and A1 is set to 0.\n",
		    pParam->FEFET1a2);
	    printf("Warning: A2 = %g is larger than 1. A2 is set to 1 and A1 is set to 0.\n",
		   pParam->FEFET1a2);
	    pParam->FEFET1a2 = 1.0;
	    pParam->FEFET1a1 = 0.0;
	}

        if (pParam->FEFET1prwg < 0.0)
        {   fprintf(fplog, "Warning: Prwg = %g is negative. Set to zero.\n",
                    pParam->FEFET1prwg);
            printf("Warning: Prwg = %g is negative. Set to zero.\n",
                   pParam->FEFET1prwg);
            pParam->FEFET1prwg = 0.0;
        }

	if (pParam->FEFET1rdsw < 0.0)
	{   fprintf(fplog, "Warning: Rdsw = %g is negative. Set to zero.\n",
		    pParam->FEFET1rdsw);
	    printf("Warning: Rdsw = %g is negative. Set to zero.\n",
		   pParam->FEFET1rdsw);
	    pParam->FEFET1rdsw = 0.0;
	    pParam->FEFET1rds0 = 0.0;
	}

	if (pParam->FEFET1rds0 < 0.0)
	{   fprintf(fplog, "Warning: Rds at current temperature = %g is negative. Set to zero.\n",
		    pParam->FEFET1rds0);
	    printf("Warning: Rds at current temperature = %g is negative. Set to zero.\n",
		   pParam->FEFET1rds0);
	    pParam->FEFET1rds0 = 0.0;
	}

        if (pParam->FEFET1rdswmin < 0.0)
        {   fprintf(fplog, "Warning: Rdswmin at current temperature = %g is negative. Set to zero.\n",
                    pParam->FEFET1rdswmin);
            printf("Warning: Rdswmin at current temperature = %g is negative. Set to zero.\n",
                   pParam->FEFET1rdswmin);
            pParam->FEFET1rdswmin = 0.0;
        }

        if (pParam->FEFET1pscbe2 <= 0.0)
        {   fprintf(fplog, "Warning: Pscbe2 = %g is not positive.\n",
                    pParam->FEFET1pscbe2);
            printf("Warning: Pscbe2 = %g is not positive.\n", pParam->FEFET1pscbe2);
        }

        if (pParam->FEFET1vsattemp < 1.0e3)
	{   fprintf(fplog, "Warning: Vsat at current temperature = %g may be too small.\n", pParam->FEFET1vsattemp);
	   printf("Warning: Vsat at current temperature = %g may be too small.\n", pParam->FEFET1vsattemp);
	}

      if((model->FEFET1lambdaGiven) && (pParam->FEFET1lambda > 0.0) ) 
      {
        if (pParam->FEFET1lambda > 1.0e-9)
	{   fprintf(fplog, "Warning: Lambda = %g may be too large.\n", pParam->FEFET1lambda);
	   printf("Warning: Lambda = %g may be too large.\n", pParam->FEFET1lambda);
	}
      }

      if((model->FEFET1vtlGiven) && (pParam->FEFET1vtl > 0.0) )
      {  
        if (pParam->FEFET1vtl < 6.0e4)
	{   fprintf(fplog, "Warning: Thermal velocity vtl = %g may be too small.\n", pParam->FEFET1vtl);
	   printf("Warning: Thermal velocity vtl = %g may be too small.\n", pParam->FEFET1vtl);
	}

        if (pParam->FEFET1xn < 3.0)
	{   fprintf(fplog, "Warning: back scattering coeff xn = %g is too small.\n", pParam->FEFET1xn);
	    printf("Warning: back scattering coeff xn = %g is too small. Reset to 3.0 \n", pParam->FEFET1xn);
	    pParam->FEFET1xn = 3.0;
	}

        if (model->FEFET1lc < 0.0)
	{   fprintf(fplog, "Warning: back scattering coeff lc = %g is too small.\n", model->FEFET1lc);
	    printf("Warning: back scattering coeff lc = %g is too small. Reset to 0.0\n", model->FEFET1lc);
	    pParam->FEFET1lc = 0.0;
	}
      }

	if (pParam->FEFET1pdibl1 < 0.0)
	{   fprintf(fplog, "Warning: Pdibl1 = %g is negative.\n",
		    pParam->FEFET1pdibl1);
	    printf("Warning: Pdibl1 = %g is negative.\n", pParam->FEFET1pdibl1);
	}
	if (pParam->FEFET1pdibl2 < 0.0)
	{   fprintf(fplog, "Warning: Pdibl2 = %g is negative.\n",
		    pParam->FEFET1pdibl2);
	    printf("Warning: Pdibl2 = %g is negative.\n", pParam->FEFET1pdibl2);
	}

/* Check stress effect parameters */        
        if((here->FEFET1sa > 0.0) && (here->FEFET1sb > 0.0) && 
       	((here->FEFET1nf == 1.0) || ((here->FEFET1nf > 1.0) && (here->FEFET1sd > 0.0))) )
        {   if (model->FEFET1lodk2 <= 0.0)
            {   fprintf(fplog, "Warning: LODK2 = %g is not positive.\n",model->FEFET1lodk2);
                printf("Warning: LODK2 = %g is not positive.\n",model->FEFET1lodk2);
            }
            if (model->FEFET1lodeta0 <= 0.0)
            {   fprintf(fplog, "Warning: LODETA0 = %g is not positive.\n",model->FEFET1lodeta0);
            	printf("Warning: LODETA0 = %g is not positive.\n",model->FEFET1lodeta0);
            }
 	}

/* Check gate resistance parameters */        
        if (here->FEFET1rgateMod == 1)
        {   if (model->FEFET1rshg <= 0.0)
	        printf("Warning: rshg should be positive for rgateMod = 1.\n");
	}
        else if (here->FEFET1rgateMod == 2)
        {   if (model->FEFET1rshg <= 0.0)
                printf("Warning: rshg <= 0.0 for rgateMod = 2.\n");
            else if (pParam->FEFET1xrcrg1 <= 0.0)
                     printf("Warning: xrcrg1 <= 0.0 for rgateMod = 2.\n");
         }
        if (here->FEFET1rgateMod == 3)
        {   if (model->FEFET1rshg <= 0.0)
                printf("Warning: rshg should be positive for rgateMod = 3.\n");
            else if (pParam->FEFET1xrcrg1 <= 0.0)
                     printf("Warning: xrcrg1 should be positive for rgateMod = 3.\n");
         }
         
/* Check capacitance parameters */
        if (pParam->FEFET1noff < 0.1)
        {   fprintf(fplog, "Warning: Noff = %g is too small.\n",
                    pParam->FEFET1noff);
            printf("Warning: Noff = %g is too small.\n", pParam->FEFET1noff);
        }

        if (pParam->FEFET1voffcv < -0.5)
        {   fprintf(fplog, "Warning: Voffcv = %g is too small.\n",
                    pParam->FEFET1voffcv);
            printf("Warning: Voffcv = %g is too small.\n", pParam->FEFET1voffcv);
        }

        if (pParam->FEFET1moin < 5.0)
        {   fprintf(fplog, "Warning: Moin = %g is too small.\n",
                    pParam->FEFET1moin);
            printf("Warning: Moin = %g is too small.\n", pParam->FEFET1moin);
        }
        if (pParam->FEFET1moin > 25.0)
        {   fprintf(fplog, "Warning: Moin = %g is too large.\n",
                    pParam->FEFET1moin);
            printf("Warning: Moin = %g is too large.\n", pParam->FEFET1moin);
        }
	if(model->FEFET1capMod ==2) {
        	if (pParam->FEFET1acde < 0.1)
        	{   fprintf(fplog, "Warning:  Acde = %g is too small.\n",
                    	pParam->FEFET1acde);
            	printf("Warning: Acde = %g is too small.\n", pParam->FEFET1acde);
        	}
        	if (pParam->FEFET1acde > 1.6)
        	{   fprintf(fplog, "Warning:  Acde = %g is too large.\n",
                    	pParam->FEFET1acde);
            	printf("Warning: Acde = %g is too large.\n", pParam->FEFET1acde);
        	}
	}

/* Check overlap capacitance parameters */
        if (model->FEFET1cgdo < 0.0)
	{   fprintf(fplog, "Warning: cgdo = %g is negative. Set to zero.\n", model->FEFET1cgdo);
	    printf("Warning: cgdo = %g is negative. Set to zero.\n", model->FEFET1cgdo);
	    model->FEFET1cgdo = 0.0;
        }      
        if (model->FEFET1cgso < 0.0)
	{   fprintf(fplog, "Warning: cgso = %g is negative. Set to zero.\n", model->FEFET1cgso);
	    printf("Warning: cgso = %g is negative. Set to zero.\n", model->FEFET1cgso);
	    model->FEFET1cgso = 0.0;
        }      
        if (model->FEFET1cgbo < 0.0)
	{   fprintf(fplog, "Warning: cgbo = %g is negative. Set to zero.\n", model->FEFET1cgbo);
	    printf("Warning: cgbo = %g is negative. Set to zero.\n", model->FEFET1cgbo);
	    model->FEFET1cgbo = 0.0;
        }      
      if (model->FEFET1tnoiMod == 1) { 
        if (model->FEFET1tnoia < 0.0)
        {   fprintf(fplog, "Warning: tnoia = %g is negative. Set to zero.\n", model->FEFET1tnoia);
            printf("Warning: tnoia = %g is negative. Set to zero.\n", model->FEFET1tnoia);
            model->FEFET1tnoia = 0.0;
        }
        if (model->FEFET1tnoib < 0.0)
        {   fprintf(fplog, "Warning: tnoib = %g is negative. Set to zero.\n", model->FEFET1tnoib);
            printf("Warning: tnoib = %g is negative. Set to zero.\n", model->FEFET1tnoib);
            model->FEFET1tnoib = 0.0;
        }

         if (model->FEFET1rnoia < 0.0)
        {   fprintf(fplog, "Warning: rnoia = %g is negative. Set to zero.\n", model->FEFET1rnoia);
            printf("Warning: rnoia = %g is negative. Set to zero.\n", model->FEFET1rnoia);
            model->FEFET1rnoia = 0.0;
        }
        if (model->FEFET1rnoib < 0.0)
        {   fprintf(fplog, "Warning: rnoib = %g is negative. Set to zero.\n", model->FEFET1rnoib);
            printf("Warning: rnoib = %g is negative. Set to zero.\n", model->FEFET1rnoib);
            model->FEFET1rnoib = 0.0;
        }
      }

	if (model->FEFET1SjctEmissionCoeff < 0.0)
	{   fprintf(fplog, "Warning: Njs = %g is negative.\n",
		    model->FEFET1SjctEmissionCoeff);
	    printf("Warning: Njs = %g is negative.\n",
		    model->FEFET1SjctEmissionCoeff);
	}
	if (model->FEFET1DjctEmissionCoeff < 0.0)
	{   fprintf(fplog, "Warning: Njd = %g is negative.\n",
		    model->FEFET1DjctEmissionCoeff);
	    printf("Warning: Njd = %g is negative.\n",
		    model->FEFET1DjctEmissionCoeff);
	}
	if (model->FEFET1njtstemp < 0.0)
	{   fprintf(fplog, "Warning: Njts = %g is negative at temperature = %g.\n",
		    model->FEFET1njtstemp, ckt->CKTtemp);
	    printf("Warning: Njts = %g is negative at temperature = %g.\n",
		    model->FEFET1njtstemp, ckt->CKTtemp);
	}
	if (model->FEFET1njtsswtemp < 0.0)
	{   fprintf(fplog, "Warning: Njtssw = %g is negative at temperature = %g.\n",
		    model->FEFET1njtsswtemp, ckt->CKTtemp);
	    printf("Warning: Njtssw = %g is negative at temperature = %g.\n",
		    model->FEFET1njtsswtemp, ckt->CKTtemp);
	}
	if (model->FEFET1njtsswgtemp < 0.0)
	{   fprintf(fplog, "Warning: Njtsswg = %g is negative at temperature = %g.\n",
		    model->FEFET1njtsswgtemp, ckt->CKTtemp);
	    printf("Warning: Njtsswg = %g is negative at temperature = %g.\n",
		    model->FEFET1njtsswgtemp, ckt->CKTtemp);
	}
       if (model->FEFET1ntnoi < 0.0)
        {   fprintf(fplog, "Warning: ntnoi = %g is negative. Set to zero.\n", model->FEFET1ntnoi);
            printf("Warning: ntnoi = %g is negative. Set to zero.\n", model->FEFET1ntnoi);
            model->FEFET1ntnoi = 0.0;
        }

        /* diode model */
       if (model->FEFET1SbulkJctBotGradingCoeff >= 0.99)
        {   fprintf(fplog, "Warning: MJS = %g is too big. Set to 0.99.\n", model->FEFET1SbulkJctBotGradingCoeff);
            printf("Warning: MJS = %g is too big. Set to 0.99.\n", model->FEFET1SbulkJctBotGradingCoeff);
            model->FEFET1SbulkJctBotGradingCoeff = 0.99;
        }
       if (model->FEFET1SbulkJctSideGradingCoeff >= 0.99)
        {   fprintf(fplog, "Warning: MJSWS = %g is too big. Set to 0.99.\n", model->FEFET1SbulkJctSideGradingCoeff);
            printf("Warning: MJSWS = %g is too big. Set to 0.99.\n", model->FEFET1SbulkJctSideGradingCoeff);
            model->FEFET1SbulkJctSideGradingCoeff = 0.99;
        }
       if (model->FEFET1SbulkJctGateSideGradingCoeff >= 0.99)
        {   fprintf(fplog, "Warning: MJSWGS = %g is too big. Set to 0.99.\n", model->FEFET1SbulkJctGateSideGradingCoeff);
            printf("Warning: MJSWGS = %g is too big. Set to 0.99.\n", model->FEFET1SbulkJctGateSideGradingCoeff);
            model->FEFET1SbulkJctGateSideGradingCoeff = 0.99;
        }

       if (model->FEFET1DbulkJctBotGradingCoeff >= 0.99)
        {   fprintf(fplog, "Warning: MJD = %g is too big. Set to 0.99.\n", model->FEFET1DbulkJctBotGradingCoeff);
            printf("Warning: MJD = %g is too big. Set to 0.99.\n", model->FEFET1DbulkJctBotGradingCoeff);
            model->FEFET1DbulkJctBotGradingCoeff = 0.99;
        }
       if (model->FEFET1DbulkJctSideGradingCoeff >= 0.99)
        {   fprintf(fplog, "Warning: MJSWD = %g is too big. Set to 0.99.\n", model->FEFET1DbulkJctSideGradingCoeff);
            printf("Warning: MJSWD = %g is too big. Set to 0.99.\n", model->FEFET1DbulkJctSideGradingCoeff);
            model->FEFET1DbulkJctSideGradingCoeff = 0.99;
        }
       if (model->FEFET1DbulkJctGateSideGradingCoeff >= 0.99)
        {   fprintf(fplog, "Warning: MJSWGD = %g is too big. Set to 0.99.\n", model->FEFET1DbulkJctGateSideGradingCoeff);
            printf("Warning: MJSWGD = %g is too big. Set to 0.99.\n", model->FEFET1DbulkJctGateSideGradingCoeff);
            model->FEFET1DbulkJctGateSideGradingCoeff = 0.99;
        }
	if (model->FEFET1wpemod == 1)
	{
		if (model->FEFET1scref <= 0.0)
		{   fprintf(fplog, "Warning: SCREF = %g is not positive. Set to 1e-6.\n", model->FEFET1scref);
		    printf("Warning: SCREF = %g is not positive. Set to 1e-6.\n", model->FEFET1scref);
		    model->FEFET1scref = 1e-6;
		}
	        if (here->FEFET1sca < 0.0)
		{   fprintf(fplog, "Warning: SCA = %g is negative. Set to 0.0.\n", here->FEFET1sca);
                    printf("Warning: SCA = %g is negative. Set to 0.0.\n", here->FEFET1sca);
                    here->FEFET1sca = 0.0;
                }
                if (here->FEFET1scb < 0.0)
                {   fprintf(fplog, "Warning: SCB = %g is negative. Set to 0.0.\n", here->FEFET1scb);
                    printf("Warning: SCB = %g is negative. Set to 0.0.\n", here->FEFET1scb);
                    here->FEFET1scb = 0.0;
                }
                if (here->FEFET1scc < 0.0)
                {   fprintf(fplog, "Warning: SCC = %g is negative. Set to 0.0.\n", here->FEFET1scc);
                    printf("Warning: SCC = %g is negative. Set to 0.0.\n", here->FEFET1scc);
                    here->FEFET1scc = 0.0;
                }
                if (here->FEFET1sc < 0.0)
                {   fprintf(fplog, "Warning: SC = %g is negative. Set to 0.0.\n", here->FEFET1sc);
                    printf("Warning: SC = %g is negative. Set to 0.0.\n", here->FEFET1sc);
                    here->FEFET1sc = 0.0;
                }
	}
     }/* loop for the parameter check for warning messages */      
	fclose(fplog);
    }
    else
    {   fprintf(stderr, "Warning: Can't open log file. Parameter checking skipped.\n");
    }

    return(Fatal_Flag);
}

