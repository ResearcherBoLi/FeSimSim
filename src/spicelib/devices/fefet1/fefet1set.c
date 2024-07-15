/**********
Copyright 2024 Xidian University
Author: 2024 Bo Li
Modified: 2024/01/27  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/jobdefs.h"
#include "ngspice/ftedefs.h"
#include "ngspice/smpdefs.h"
#include "ngspice/cktdefs.h"
#include "fefet1def.h"
#include "ngspice/const.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

#ifdef USE_OMP
#include "ngspice/cpextern.h"
#endif

#define MAX_EXP 5.834617425e14
#define MIN_EXP 1.713908431e-15
#define EXP_THRESHOLD 34.0
#define EPS0 8.85418e-12
#define EPSSI 1.03594e-10
#define PI 3.141592654
#define Charge_q 1.60219e-19
#define K 8.987551787e9
#define EPSF 29 * (8.854e-12)

int
FEFET1setup(SMPmatrix *matrix,GENmodel *inModel,CKTcircuit *ckt,int *states)
{
    FEFET1model *model = (FEFET1model*)inModel;
    FEFET1instance *here;
    int error;
    CKTnode *tmp;
    int    noiseAnalGiven = 0, createNode;  /* Criteria for new node creation */
    double Rtot, DMCGeff, DMCIeff, DMDGeff;
    JOB   *job;

    #ifdef USE_OMP
    int idx, InstCount;
    FEFET1instance **InstArray;
    #endif

        /* Search for a noise analysis request */
        for (job = ft_curckt->ci_curTask->jobs; job; job = job->JOBnextJob) {
            if(strcmp(job->JOBname,"Noise Analysis")==0) {
                noiseAnalGiven = 1;
                break;
            }
        }

        /*  loop through all the FEFET1 device models */
        for( ; model != NULL; model = FEFET1nextModel(model))
        {   /* process defaults of model parameters */
            if (!model->FEFET1typeGiven)
                model->FEFET1type = NMOS;     

            if (!model->FECAP1_ThicknessGiven) model->FECAP1_Thickness = 1e-9;
                    
            if (!model->FECAP1_PsGiven) model->FECAP1_Ps = 27.54;	
            
            if (!model->FECAP1_EcGiven) model->FECAP1_Ec = 1.175;

            if (!model->FECAP1_AlphaGiven) model->FECAP1_Alpha =18;

            if (!model->FECAP1_BetaGiven) model->FECAP1_Beta = 0.0;
            
            if (!model->FECAP1_GammaGiven) model->FECAP1_Gamma = 0.0;

            if (!model->FECAP1_RhoGiven) model->FECAP1_Rho = 0.0;                

            if (!model->FEFET1mobModGiven) 
                model->FEFET1mobMod = 0;
            else if ((model->FEFET1mobMod != 0) && (model->FEFET1mobMod != 1)
                    && (model->FEFET1mobMod != 2))
            {   model->FEFET1mobMod = 0;
                    printf("Warning: mobMod has been set to its default value: 0.\n");
            }

            if (!model->FEFET1binUnitGiven) 
                model->FEFET1binUnit = 1;
            if (!model->FEFET1paramChkGiven) 
                model->FEFET1paramChk = 1;

            if (!model->FEFET1dioModGiven)
                model->FEFET1dioMod = 1;
            else if ((model->FEFET1dioMod != 0) && (model->FEFET1dioMod != 1)
                && (model->FEFET1dioMod != 2))
            {   model->FEFET1dioMod = 1;
                printf("Warning: dioMod has been set to its default value: 1.\n");
            }

            if (!model->FEFET1capModGiven) 
                model->FEFET1capMod = 2;
            else if ((model->FEFET1capMod != 0) && (model->FEFET1capMod != 1)
                && (model->FEFET1capMod != 2))
            {   model->FEFET1capMod = 2;
                printf("Warning: capMod has been set to its default value: 2.\n");
            }

            if (!model->FEFET1rdsModGiven)
                model->FEFET1rdsMod = 0;
            else if ((model->FEFET1rdsMod != 0) && (model->FEFET1rdsMod != 1))
                {   model->FEFET1rdsMod = 0;
                printf("Warning: rdsMod has been set to its default value: 0.\n");
            }
            if (!model->FEFET1rbodyModGiven)
                model->FEFET1rbodyMod = 0;
            else if ((model->FEFET1rbodyMod != 0) && (model->FEFET1rbodyMod != 1) && (model->FEFET1rbodyMod != 2))
            {   model->FEFET1rbodyMod = 0;
                printf("Warning: rbodyMod has been set to its default value: 0.\n");
            }

            if (!model->FEFET1rgateModGiven)
                model->FEFET1rgateMod = 0;
            else if ((model->FEFET1rgateMod != 0) && (model->FEFET1rgateMod != 1)
                && (model->FEFET1rgateMod != 2) && (model->FEFET1rgateMod != 3))
            {   model->FEFET1rgateMod = 0;
                printf("Warning: rgateMod has been set to its default value: 0.\n");
            }

            if (!model->FEFET1perModGiven)
                model->FEFET1perMod = 1;
            else if ((model->FEFET1perMod != 0) && (model->FEFET1perMod != 1))
            {   model->FEFET1perMod = 1;
                printf("Warning: perMod has been set to its default value: 1.\n");
            }

            if (!model->FEFET1geoModGiven)
                model->FEFET1geoMod = 0;

            if (!model->FEFET1rgeoModGiven)
                model->FEFET1rgeoMod = 0;
            else if ((model->FEFET1rgeoMod != 0) && (model->FEFET1rgeoMod != 1))
            {   model->FEFET1rgeoMod = 1;
                printf("Warning: rgeoMod has been set to its default value: 1.\n");
            }

            if (!model->FEFET1fnoiModGiven) 
                model->FEFET1fnoiMod = 1;
            else if ((model->FEFET1fnoiMod != 0) && (model->FEFET1fnoiMod != 1))
            {   model->FEFET1fnoiMod = 1;
                printf("Warning: fnoiMod has been set to its default value: 1.\n");
            }
            if (!model->FEFET1tnoiModGiven)
                model->FEFET1tnoiMod = 0; /* WDLiu: tnoiMod=1 needs to set internal S/D nodes */
            else if ((model->FEFET1tnoiMod != 0) && (model->FEFET1tnoiMod != 1))
            {   model->FEFET1tnoiMod = 0;
                printf("Warning: tnoiMod has been set to its default value: 0.\n");
            }

            if (!model->FEFET1trnqsModGiven)
                model->FEFET1trnqsMod = 0; 
            else if ((model->FEFET1trnqsMod != 0) && (model->FEFET1trnqsMod != 1))
            {   model->FEFET1trnqsMod = 0;
                printf("Warning: trnqsMod has been set to its default value: 0.\n");
            }
            if (!model->FEFET1acnqsModGiven)
                model->FEFET1acnqsMod = 0;
            else if ((model->FEFET1acnqsMod != 0) && (model->FEFET1acnqsMod != 1))
            {   model->FEFET1acnqsMod = 0;
                printf("Warning: acnqsMod has been set to its default value: 0.\n");
            }

            if (!model->FEFET1igcModGiven)
                model->FEFET1igcMod = 0;
            else if ((model->FEFET1igcMod != 0) && (model->FEFET1igcMod != 1)
            && (model->FEFET1igcMod != 2))
            {   model->FEFET1igcMod = 0;
                printf("Warning: igcMod has been set to its default value: 0.\n");
            }
            if (!model->FEFET1igbModGiven)
                model->FEFET1igbMod = 0;
            else if ((model->FEFET1igbMod != 0) && (model->FEFET1igbMod != 1))
            {   model->FEFET1igbMod = 0;
                printf("Warning: igbMod has been set to its default value: 0.\n");
            }
            if (!model->FEFET1tempModGiven)
                model->FEFET1tempMod = 0;
            else if ((model->FEFET1tempMod != 0) && (model->FEFET1tempMod != 1) 
            && (model->FEFET1tempMod != 2))
            {   model->FEFET1tempMod = 0;
                printf("Warning: tempMod has been set to its default value: 0.\n");
            }

            if (!model->FEFET1versionGiven) 
                model->FEFET1version = "4.5.0";
            if (!model->FEFET1toxrefGiven)
                model->FEFET1toxref = 30.0e-10;
            if (!model->FEFET1toxeGiven)
                model->FEFET1toxe = 30.0e-10;
            if (!model->FEFET1toxpGiven)
                model->FEFET1toxp = model->FEFET1toxe;
            if (!model->FEFET1toxmGiven)
                model->FEFET1toxm = model->FEFET1toxe;
            if (!model->FEFET1dtoxGiven)
                model->FEFET1dtox = 0.0;
            if (!model->FEFET1epsroxGiven)
                model->FEFET1epsrox = 3.9;

            if (!model->FEFET1cdscGiven)
            model->FEFET1cdsc = 2.4e-4;   /* unit Q/V/m^2  */
            if (!model->FEFET1cdscbGiven)
            model->FEFET1cdscb = 0.0;   /* unit Q/V/m^2  */    
            if (!model->FEFET1cdscdGiven)
            model->FEFET1cdscd = 0.0;   /* unit Q/V/m^2  */
            if (!model->FEFET1citGiven)
            model->FEFET1cit = 0.0;   /* unit Q/V/m^2  */
            if (!model->FEFET1nfactorGiven)
            model->FEFET1nfactor = 1.0;
            if (!model->FEFET1xjGiven)
                model->FEFET1xj = .15e-6;
            if (!model->FEFET1vsatGiven)
                model->FEFET1vsat = 8.0e4;    /* unit m/s */ 
            if (!model->FEFET1atGiven)
                model->FEFET1at = 3.3e4;    /* unit m/s */ 
            if (!model->FEFET1a0Given)
                model->FEFET1a0 = 1.0;  
            if (!model->FEFET1agsGiven)
                model->FEFET1ags = 0.0;
            if (!model->FEFET1a1Given)
                model->FEFET1a1 = 0.0;
            if (!model->FEFET1a2Given)
                model->FEFET1a2 = 1.0;
            if (!model->FEFET1ketaGiven)
                model->FEFET1keta = -0.047;    /* unit  / V */
            if (!model->FEFET1nsubGiven)
                model->FEFET1nsub = 6.0e16;   /* unit 1/cm3 */
            if (!model->FEFET1ndepGiven)
                model->FEFET1ndep = 1.7e17;   /* unit 1/cm3 */
            if (!model->FEFET1nsdGiven)
                model->FEFET1nsd = 1.0e20;   /* unit 1/cm3 */
            if (!model->FEFET1phinGiven)
                model->FEFET1phin = 0.0; /* unit V */
            if (!model->FEFET1ngateGiven)
                model->FEFET1ngate = 0;   /* unit 1/cm3 */
            if (!model->FEFET1vbmGiven)
            model->FEFET1vbm = -3.0;
            if (!model->FEFET1xtGiven)
            model->FEFET1xt = 1.55e-7;
            if (!model->FEFET1kt1Given)
                model->FEFET1kt1 = -0.11;      /* unit V */
            if (!model->FEFET1kt1lGiven)
                model->FEFET1kt1l = 0.0;      /* unit V*m */
            if (!model->FEFET1kt2Given)
                model->FEFET1kt2 = 0.022;      /* No unit */
            if (!model->FEFET1k3Given)
                model->FEFET1k3 = 80.0;      
            if (!model->FEFET1k3bGiven)
                model->FEFET1k3b = 0.0;      
            if (!model->FEFET1w0Given)
                model->FEFET1w0 = 2.5e-6;    
            if (!model->FEFET1lpe0Given)
                model->FEFET1lpe0 = 1.74e-7;     
            if (!model->FEFET1lpebGiven)
                model->FEFET1lpeb = 0.0;
            if (!model->FEFET1dvtp0Given)
                model->FEFET1dvtp0 = 0.0;
            if (!model->FEFET1dvtp1Given)
                model->FEFET1dvtp1 = 0.0;
            if (!model->FEFET1dvt0Given)
                model->FEFET1dvt0 = 2.2;    
            if (!model->FEFET1dvt1Given)
                model->FEFET1dvt1 = 0.53;      
            if (!model->FEFET1dvt2Given)
                model->FEFET1dvt2 = -0.032;   /* unit 1 / V */     

            if (!model->FEFET1dvt0wGiven)
                model->FEFET1dvt0w = 0.0;    
            if (!model->FEFET1dvt1wGiven)
                model->FEFET1dvt1w = 5.3e6;    
            if (!model->FEFET1dvt2wGiven)
                model->FEFET1dvt2w = -0.032;   

            if (!model->FEFET1droutGiven)
                model->FEFET1drout = 0.56;     
            if (!model->FEFET1dsubGiven)
                model->FEFET1dsub = model->FEFET1drout;     
            if (!model->FEFET1vth0Given)
                model->FEFET1vth0 = (model->FEFET1type == NMOS) ? 0.7 : -0.7;
            if (!model->FEFET1euGiven)
                model->FEFET1eu = (model->FEFET1type == NMOS) ? 1.67 : 1.0;;
            if (!model->FEFET1uaGiven)
                model->FEFET1ua = (model->FEFET1mobMod == 2) ? 1.0e-15 : 1.0e-9; /* unit m/V */
            if (!model->FEFET1ua1Given)
                model->FEFET1ua1 = 1.0e-9;      /* unit m/V */
            if (!model->FEFET1ubGiven)
                model->FEFET1ub = 1.0e-19;     /* unit (m/V)**2 */
            if (!model->FEFET1ub1Given)
                model->FEFET1ub1 = -1.0e-18;     /* unit (m/V)**2 */
            if (!model->FEFET1ucGiven)
                model->FEFET1uc = (model->FEFET1mobMod == 1) ? -0.0465 : -0.0465e-9;   
            if (!model->FEFET1uc1Given)
                model->FEFET1uc1 = (model->FEFET1mobMod == 1) ? -0.056 : -0.056e-9;   
            if (!model->FEFET1udGiven)
                model->FEFET1ud = 1.0e14;     /* unit m**(-2) */
            if (!model->FEFET1ud1Given)
                model->FEFET1ud1 = 0.0;     
            if (!model->FEFET1upGiven)
                model->FEFET1up = 0.0;     
            if (!model->FEFET1lpGiven)
                model->FEFET1lp = 1.0e-8;     
            if (!model->FEFET1u0Given)
                model->FEFET1u0 = (model->FEFET1type == NMOS) ? 0.067 : 0.025;
            if (!model->FEFET1uteGiven)
            model->FEFET1ute = -1.5;    
            if (!model->FEFET1voffGiven)
            model->FEFET1voff = -0.08;
            if (!model->FEFET1vofflGiven)
                model->FEFET1voffl = 0.0;
            if (!model->FEFET1minvGiven)
                model->FEFET1minv = 0.0;
            if (!model->FEFET1fproutGiven)
                model->FEFET1fprout = 0.0;
            if (!model->FEFET1pditsGiven)
                model->FEFET1pdits = 0.0;
            if (!model->FEFET1pditsdGiven)
                model->FEFET1pditsd = 0.0;
            if (!model->FEFET1pditslGiven)
                model->FEFET1pditsl = 0.0;
            if (!model->FEFET1deltaGiven)  
            model->FEFET1delta = 0.01;
            if (!model->FEFET1rdswminGiven)
                model->FEFET1rdswmin = 0.0;
            if (!model->FEFET1rdwminGiven)
                model->FEFET1rdwmin = 0.0;
            if (!model->FEFET1rswminGiven)
                model->FEFET1rswmin = 0.0;
            if (!model->FEFET1rdswGiven)
            model->FEFET1rdsw = 200.0; /* in ohm*um */     
            if (!model->FEFET1rdwGiven)
                model->FEFET1rdw = 100.0;
            if (!model->FEFET1rswGiven)
                model->FEFET1rsw = 100.0;
            if (!model->FEFET1prwgGiven)
                model->FEFET1prwg = 1.0; /* in 1/V */
            if (!model->FEFET1prwbGiven)
                model->FEFET1prwb = 0.0;      
            if (!model->FEFET1prtGiven)
                model->FEFET1prt = 0.0;      
            if (!model->FEFET1eta0Given)
                model->FEFET1eta0 = 0.08;      /* no unit  */ 
            if (!model->FEFET1etabGiven)
                model->FEFET1etab = -0.07;      /* unit  1/V */ 
            if (!model->FEFET1pclmGiven)
                model->FEFET1pclm = 1.3;      /* no unit  */ 
            if (!model->FEFET1pdibl1Given)
                model->FEFET1pdibl1 = 0.39;    /* no unit  */
            if (!model->FEFET1pdibl2Given)
                model->FEFET1pdibl2 = 0.0086;    /* no unit  */ 
            if (!model->FEFET1pdiblbGiven)
                model->FEFET1pdiblb = 0.0;    /* 1/V  */ 
            if (!model->FEFET1pscbe1Given)
                model->FEFET1pscbe1 = 4.24e8;     
            if (!model->FEFET1pscbe2Given)
                model->FEFET1pscbe2 = 1.0e-5;    
            if (!model->FEFET1pvagGiven)
                model->FEFET1pvag = 0.0;     
            if (!model->FEFET1wrGiven)  
                model->FEFET1wr = 1.0;
            if (!model->FEFET1dwgGiven)  
                model->FEFET1dwg = 0.0;
            if (!model->FEFET1dwbGiven)  
                model->FEFET1dwb = 0.0;
            if (!model->FEFET1b0Given)
                model->FEFET1b0 = 0.0;
            if (!model->FEFET1b1Given)  
                model->FEFET1b1 = 0.0;
            if (!model->FEFET1alpha0Given)  
                model->FEFET1alpha0 = 0.0;
            if (!model->FEFET1alpha1Given)
                model->FEFET1alpha1 = 0.0;
            if (!model->FEFET1beta0Given)  
                model->FEFET1beta0 = 0.0;
            if (!model->FEFET1agidlGiven)
                model->FEFET1agidl = 0.0;
            if (!model->FEFET1bgidlGiven)
                model->FEFET1bgidl = 2.3e9; /* V/m */
            if (!model->FEFET1cgidlGiven)
                model->FEFET1cgidl = 0.5; /* V^3 */
            if (!model->FEFET1egidlGiven)
                model->FEFET1egidl = 0.8; /* V */
            if (!model->FEFET1aigcGiven)
                model->FEFET1aigc = (model->FEFET1type == NMOS) ? 1.36e-2 : 9.80e-3;
            if (!model->FEFET1bigcGiven)
                model->FEFET1bigc = (model->FEFET1type == NMOS) ? 1.71e-3 : 7.59e-4;
            if (!model->FEFET1cigcGiven)
                model->FEFET1cigc = (model->FEFET1type == NMOS) ? 0.075 : 0.03;
            if (!model->FEFET1aigsdGiven)
                model->FEFET1aigsd = (model->FEFET1type == NMOS) ? 1.36e-2 : 9.80e-3;
            if (!model->FEFET1bigsdGiven)
                model->FEFET1bigsd = (model->FEFET1type == NMOS) ? 1.71e-3 : 7.59e-4;
            if (!model->FEFET1cigsdGiven)
                model->FEFET1cigsd = (model->FEFET1type == NMOS) ? 0.075 : 0.03;
            if (!model->FEFET1aigbaccGiven)
                model->FEFET1aigbacc = 1.36e-2;
            if (!model->FEFET1bigbaccGiven)
                model->FEFET1bigbacc = 1.71e-3;
            if (!model->FEFET1cigbaccGiven)
                model->FEFET1cigbacc = 0.075;
            if (!model->FEFET1aigbinvGiven)
                model->FEFET1aigbinv = 1.11e-2;
            if (!model->FEFET1bigbinvGiven)
                model->FEFET1bigbinv = 9.49e-4;
            if (!model->FEFET1cigbinvGiven)
                model->FEFET1cigbinv = 0.006;
            if (!model->FEFET1nigcGiven)
                model->FEFET1nigc = 1.0;
            if (!model->FEFET1nigbinvGiven)
                model->FEFET1nigbinv = 3.0;
            if (!model->FEFET1nigbaccGiven)
                model->FEFET1nigbacc = 1.0;
            if (!model->FEFET1ntoxGiven)
                model->FEFET1ntox = 1.0;
            if (!model->FEFET1eigbinvGiven)
                model->FEFET1eigbinv = 1.1;
            if (!model->FEFET1pigcdGiven)
                model->FEFET1pigcd = 1.0;
            if (!model->FEFET1poxedgeGiven)
                model->FEFET1poxedge = 1.0;
            if (!model->FEFET1xrcrg1Given)
                model->FEFET1xrcrg1 = 12.0;
            if (!model->FEFET1xrcrg2Given)
                model->FEFET1xrcrg2 = 1.0;
            if (!model->FEFET1ijthsfwdGiven)
                model->FEFET1ijthsfwd = 0.1; /* unit A */
            if (!model->FEFET1ijthdfwdGiven)
                model->FEFET1ijthdfwd = model->FEFET1ijthsfwd;
            if (!model->FEFET1ijthsrevGiven)
                model->FEFET1ijthsrev = 0.1; /* unit A */
            if (!model->FEFET1ijthdrevGiven)
                model->FEFET1ijthdrev = model->FEFET1ijthsrev;
            if (!model->FEFET1tnoiaGiven)
                model->FEFET1tnoia = 1.5;
            if (!model->FEFET1tnoibGiven)
                model->FEFET1tnoib = 3.5;
            if (!model->FEFET1rnoiaGiven)
                model->FEFET1rnoia = 0.577;
            if (!model->FEFET1rnoibGiven)
                model->FEFET1rnoib = 0.5164;
            if (!model->FEFET1ntnoiGiven)
                model->FEFET1ntnoi = 1.0;
            if (!model->FEFET1lambdaGiven)
                model->FEFET1lambda = 0.0;
            if (!model->FEFET1vtlGiven)
                model->FEFET1vtl = 2.0e5;    /* unit m/s */ 
            if (!model->FEFET1xnGiven)
                model->FEFET1xn = 3.0;   
            if (!model->FEFET1lcGiven)
                model->FEFET1lc = 5.0e-9;   
            if (!model->FEFET1vfbsdoffGiven)  
                model->FEFET1vfbsdoff = 0.0;  /* unit v */  
            if (!model->FEFET1tvfbsdoffGiven)
                model->FEFET1tvfbsdoff = 0.0;  
            if (!model->FEFET1tvoffGiven)
                model->FEFET1tvoff = 0.0;  
    
            if (!model->FEFET1lintnoiGiven)
                model->FEFET1lintnoi = 0.0;  /* unit m */  

            if (!model->FEFET1xjbvsGiven)
                model->FEFET1xjbvs = 1.0; /* no unit */
            if (!model->FEFET1xjbvdGiven)
                model->FEFET1xjbvd = model->FEFET1xjbvs;
            if (!model->FEFET1bvsGiven)
                model->FEFET1bvs = 10.0; /* V */
            if (!model->FEFET1bvdGiven)
                model->FEFET1bvd = model->FEFET1bvs;

            if (!model->FEFET1gbminGiven)
                model->FEFET1gbmin = 1.0e-12; /* in mho */
            if (!model->FEFET1rbdbGiven)
                model->FEFET1rbdb = 50.0; /* in ohm */
            if (!model->FEFET1rbpbGiven)
                model->FEFET1rbpb = 50.0;
            if (!model->FEFET1rbsbGiven)
                model->FEFET1rbsb = 50.0;
            if (!model->FEFET1rbpsGiven)
                model->FEFET1rbps = 50.0;
            if (!model->FEFET1rbpdGiven)
                model->FEFET1rbpd = 50.0;

            if (!model->FEFET1rbps0Given)
                model->FEFET1rbps0 = 50.0;
            if (!model->FEFET1rbpslGiven)
                model->FEFET1rbpsl = 0.0;
            if (!model->FEFET1rbpswGiven)
                model->FEFET1rbpsw = 0.0;
            if (!model->FEFET1rbpsnfGiven)
                model->FEFET1rbpsnf = 0.0;

            if (!model->FEFET1rbpd0Given)
                model->FEFET1rbpd0 = 50.0;
            if (!model->FEFET1rbpdlGiven)
                model->FEFET1rbpdl = 0.0;
            if (!model->FEFET1rbpdwGiven)
                model->FEFET1rbpdw = 0.0;
            if (!model->FEFET1rbpdnfGiven)
                model->FEFET1rbpdnf = 0.0;

            if (!model->FEFET1rbpbx0Given)
                model->FEFET1rbpbx0 = 100.0;
            if (!model->FEFET1rbpbxlGiven)
                model->FEFET1rbpbxl = 0.0;
            if (!model->FEFET1rbpbxwGiven)
                model->FEFET1rbpbxw = 0.0;
            if (!model->FEFET1rbpbxnfGiven)
                model->FEFET1rbpbxnf = 0.0;
            if (!model->FEFET1rbpby0Given)
                model->FEFET1rbpby0 = 100.0;
            if (!model->FEFET1rbpbylGiven)
                model->FEFET1rbpbyl = 0.0;
            if (!model->FEFET1rbpbywGiven)
                model->FEFET1rbpbyw = 0.0;
            if (!model->FEFET1rbpbynfGiven)
                model->FEFET1rbpbynf = 0.0;


            if (!model->FEFET1rbsbx0Given)
                model->FEFET1rbsbx0 = 100.0;
            if (!model->FEFET1rbsby0Given)
                model->FEFET1rbsby0 = 100.0;
            if (!model->FEFET1rbdbx0Given)
                model->FEFET1rbdbx0 = 100.0;
            if (!model->FEFET1rbdby0Given)
                model->FEFET1rbdby0 = 100.0;


            if (!model->FEFET1rbsdbxlGiven)
                model->FEFET1rbsdbxl = 0.0;
            if (!model->FEFET1rbsdbxwGiven)
                model->FEFET1rbsdbxw = 0.0;
            if (!model->FEFET1rbsdbxnfGiven)
                model->FEFET1rbsdbxnf = 0.0;
            if (!model->FEFET1rbsdbylGiven)
                model->FEFET1rbsdbyl = 0.0;
            if (!model->FEFET1rbsdbywGiven)
                model->FEFET1rbsdbyw = 0.0;
            if (!model->FEFET1rbsdbynfGiven)
                model->FEFET1rbsdbynf = 0.0;

            if (!model->FEFET1cgslGiven)  
                model->FEFET1cgsl = 0.0;
            if (!model->FEFET1cgdlGiven)  
                model->FEFET1cgdl = 0.0;
            if (!model->FEFET1ckappasGiven)  
                model->FEFET1ckappas = 0.6;
            if (!model->FEFET1ckappadGiven)
                model->FEFET1ckappad = model->FEFET1ckappas;
            if (!model->FEFET1clcGiven)  
                model->FEFET1clc = 0.1e-6;
            if (!model->FEFET1cleGiven)  
                model->FEFET1cle = 0.6;
            if (!model->FEFET1vfbcvGiven)  
                model->FEFET1vfbcv = -1.0;
            if (!model->FEFET1acdeGiven)
                model->FEFET1acde = 1.0;
            if (!model->FEFET1moinGiven)
                model->FEFET1moin = 15.0;
            if (!model->FEFET1noffGiven)
                model->FEFET1noff = 1.0;
            if (!model->FEFET1voffcvGiven)
                model->FEFET1voffcv = 0.0;
            if (!model->FEFET1dmcgGiven)
                model->FEFET1dmcg = 0.0;
            if (!model->FEFET1dmciGiven)
                model->FEFET1dmci = model->FEFET1dmcg;
            if (!model->FEFET1dmdgGiven)
                model->FEFET1dmdg = 0.0;
            if (!model->FEFET1dmcgtGiven)
                model->FEFET1dmcgt = 0.0;
            if (!model->FEFET1xgwGiven)
                model->FEFET1xgw = 0.0;
            if (!model->FEFET1xglGiven)
                model->FEFET1xgl = 0.0;
            if (!model->FEFET1rshgGiven)
                model->FEFET1rshg = 0.1;
            if (!model->FEFET1ngconGiven)
                model->FEFET1ngcon = 1.0;
            if (!model->FEFET1tcjGiven)
                model->FEFET1tcj = 0.0;
            if (!model->FEFET1tpbGiven)
                model->FEFET1tpb = 0.0;
            if (!model->FEFET1tcjswGiven)
                model->FEFET1tcjsw = 0.0;
            if (!model->FEFET1tpbswGiven)
                model->FEFET1tpbsw = 0.0;
            if (!model->FEFET1tcjswgGiven)
                model->FEFET1tcjswg = 0.0;
            if (!model->FEFET1tpbswgGiven)
                model->FEFET1tpbswg = 0.0;

        /* Length dependence */
            if (!model->FEFET1lcdscGiven)
            model->FEFET1lcdsc = 0.0;
            if (!model->FEFET1lcdscbGiven)
            model->FEFET1lcdscb = 0.0;
            if (!model->FEFET1lcdscdGiven)
            model->FEFET1lcdscd = 0.0;
            if (!model->FEFET1lcitGiven)
            model->FEFET1lcit = 0.0;
            if (!model->FEFET1lnfactorGiven)
            model->FEFET1lnfactor = 0.0;
            if (!model->FEFET1lxjGiven)
                model->FEFET1lxj = 0.0;
            if (!model->FEFET1lvsatGiven)
                model->FEFET1lvsat = 0.0;
            if (!model->FEFET1latGiven)
                model->FEFET1lat = 0.0;
            if (!model->FEFET1la0Given)
                model->FEFET1la0 = 0.0; 
            if (!model->FEFET1lagsGiven)
                model->FEFET1lags = 0.0;
            if (!model->FEFET1la1Given)
                model->FEFET1la1 = 0.0;
            if (!model->FEFET1la2Given)
                model->FEFET1la2 = 0.0;
            if (!model->FEFET1lketaGiven)
                model->FEFET1lketa = 0.0;
            if (!model->FEFET1lnsubGiven)
                model->FEFET1lnsub = 0.0;
            if (!model->FEFET1lndepGiven)
                model->FEFET1lndep = 0.0;
            if (!model->FEFET1lnsdGiven)
                model->FEFET1lnsd = 0.0;
            if (!model->FEFET1lphinGiven)
                model->FEFET1lphin = 0.0;
            if (!model->FEFET1lngateGiven)
                model->FEFET1lngate = 0.0;
            if (!model->FEFET1lvbmGiven)
            model->FEFET1lvbm = 0.0;
            if (!model->FEFET1lxtGiven)
            model->FEFET1lxt = 0.0;
            if (!model->FEFET1lkt1Given)
                model->FEFET1lkt1 = 0.0; 
            if (!model->FEFET1lkt1lGiven)
                model->FEFET1lkt1l = 0.0;
            if (!model->FEFET1lkt2Given)
                model->FEFET1lkt2 = 0.0;
            if (!model->FEFET1lk3Given)
                model->FEFET1lk3 = 0.0;      
            if (!model->FEFET1lk3bGiven)
                model->FEFET1lk3b = 0.0;      
            if (!model->FEFET1lw0Given)
                model->FEFET1lw0 = 0.0;    
            if (!model->FEFET1llpe0Given)
                model->FEFET1llpe0 = 0.0;
            if (!model->FEFET1llpebGiven)
                model->FEFET1llpeb = 0.0; 
            if (!model->FEFET1ldvtp0Given)
                model->FEFET1ldvtp0 = 0.0;
            if (!model->FEFET1ldvtp1Given)
                model->FEFET1ldvtp1 = 0.0;
            if (!model->FEFET1ldvt0Given)
                model->FEFET1ldvt0 = 0.0;    
            if (!model->FEFET1ldvt1Given)
                model->FEFET1ldvt1 = 0.0;      
            if (!model->FEFET1ldvt2Given)
                model->FEFET1ldvt2 = 0.0;
            if (!model->FEFET1ldvt0wGiven)
                model->FEFET1ldvt0w = 0.0;    
            if (!model->FEFET1ldvt1wGiven)
                model->FEFET1ldvt1w = 0.0;      
            if (!model->FEFET1ldvt2wGiven)
                model->FEFET1ldvt2w = 0.0;
            if (!model->FEFET1ldroutGiven)
                model->FEFET1ldrout = 0.0;     
            if (!model->FEFET1ldsubGiven)
                model->FEFET1ldsub = 0.0;
            if (!model->FEFET1lvth0Given)
            model->FEFET1lvth0 = 0.0;
            if (!model->FEFET1luaGiven)
                model->FEFET1lua = 0.0;
            if (!model->FEFET1lua1Given)
                model->FEFET1lua1 = 0.0;
            if (!model->FEFET1lubGiven)
                model->FEFET1lub = 0.0;
            if (!model->FEFET1lub1Given)
                model->FEFET1lub1 = 0.0;
            if (!model->FEFET1lucGiven)
                model->FEFET1luc = 0.0;
            if (!model->FEFET1luc1Given)
                model->FEFET1luc1 = 0.0;
            if (!model->FEFET1ludGiven)
                model->FEFET1lud = 0.0;
            if (!model->FEFET1lud1Given)
                model->FEFET1lud1 = 0.0;
            if (!model->FEFET1lupGiven)
                model->FEFET1lup = 0.0;
            if (!model->FEFET1llpGiven)
                model->FEFET1llp = 0.0;
            if (!model->FEFET1lu0Given)
                model->FEFET1lu0 = 0.0;
            if (!model->FEFET1luteGiven)
            model->FEFET1lute = 0.0;    
            if (!model->FEFET1lvoffGiven)
            model->FEFET1lvoff = 0.0;
            if (!model->FEFET1lminvGiven)
                model->FEFET1lminv = 0.0;
            if (!model->FEFET1lfproutGiven)
                model->FEFET1lfprout = 0.0;
            if (!model->FEFET1lpditsGiven)
                model->FEFET1lpdits = 0.0;
            if (!model->FEFET1lpditsdGiven)
                model->FEFET1lpditsd = 0.0;
            if (!model->FEFET1ldeltaGiven)  
                model->FEFET1ldelta = 0.0;
            if (!model->FEFET1lrdswGiven)
                model->FEFET1lrdsw = 0.0;
            if (!model->FEFET1lrdwGiven)
                model->FEFET1lrdw = 0.0;
            if (!model->FEFET1lrswGiven)
                model->FEFET1lrsw = 0.0;
            if (!model->FEFET1lprwbGiven)
                model->FEFET1lprwb = 0.0;
            if (!model->FEFET1lprwgGiven)
                model->FEFET1lprwg = 0.0;
            if (!model->FEFET1lprtGiven)
                model->FEFET1lprt = 0.0;
            if (!model->FEFET1leta0Given)
                model->FEFET1leta0 = 0.0;
            if (!model->FEFET1letabGiven)
                model->FEFET1letab = -0.0;
            if (!model->FEFET1lpclmGiven)
                model->FEFET1lpclm = 0.0; 
            if (!model->FEFET1lpdibl1Given)
                model->FEFET1lpdibl1 = 0.0;
            if (!model->FEFET1lpdibl2Given)
                model->FEFET1lpdibl2 = 0.0;
            if (!model->FEFET1lpdiblbGiven)
                model->FEFET1lpdiblb = 0.0;
            if (!model->FEFET1lpscbe1Given)
                model->FEFET1lpscbe1 = 0.0;
            if (!model->FEFET1lpscbe2Given)
                model->FEFET1lpscbe2 = 0.0;
            if (!model->FEFET1lpvagGiven)
                model->FEFET1lpvag = 0.0;     
            if (!model->FEFET1lwrGiven)  
                model->FEFET1lwr = 0.0;
            if (!model->FEFET1ldwgGiven)  
                model->FEFET1ldwg = 0.0;
            if (!model->FEFET1ldwbGiven)  
                model->FEFET1ldwb = 0.0;
            if (!model->FEFET1lb0Given)
                model->FEFET1lb0 = 0.0;
            if (!model->FEFET1lb1Given)  
                model->FEFET1lb1 = 0.0;
            if (!model->FEFET1lalpha0Given)  
                model->FEFET1lalpha0 = 0.0;
            if (!model->FEFET1lalpha1Given)
                model->FEFET1lalpha1 = 0.0;
            if (!model->FEFET1lbeta0Given)  
                model->FEFET1lbeta0 = 0.0;
            if (!model->FEFET1lagidlGiven)
                model->FEFET1lagidl = 0.0;
            if (!model->FEFET1lbgidlGiven)
                model->FEFET1lbgidl = 0.0;
            if (!model->FEFET1lcgidlGiven)
                model->FEFET1lcgidl = 0.0;
            if (!model->FEFET1legidlGiven)
                model->FEFET1legidl = 0.0;
            if (!model->FEFET1laigcGiven)
                model->FEFET1laigc = 0.0;
            if (!model->FEFET1lbigcGiven)
                model->FEFET1lbigc = 0.0;
            if (!model->FEFET1lcigcGiven)
                model->FEFET1lcigc = 0.0;
            if (!model->FEFET1laigsdGiven)
                model->FEFET1laigsd = 0.0;
            if (!model->FEFET1lbigsdGiven)
                model->FEFET1lbigsd = 0.0;
            if (!model->FEFET1lcigsdGiven)
                model->FEFET1lcigsd = 0.0;
            if (!model->FEFET1laigbaccGiven)
                model->FEFET1laigbacc = 0.0;
            if (!model->FEFET1lbigbaccGiven)
                model->FEFET1lbigbacc = 0.0;
            if (!model->FEFET1lcigbaccGiven)
                model->FEFET1lcigbacc = 0.0;
            if (!model->FEFET1laigbinvGiven)
                model->FEFET1laigbinv = 0.0;
            if (!model->FEFET1lbigbinvGiven)
                model->FEFET1lbigbinv = 0.0;
            if (!model->FEFET1lcigbinvGiven)
                model->FEFET1lcigbinv = 0.0;
            if (!model->FEFET1lnigcGiven)
                model->FEFET1lnigc = 0.0;
            if (!model->FEFET1lnigbinvGiven)
                model->FEFET1lnigbinv = 0.0;
            if (!model->FEFET1lnigbaccGiven)
                model->FEFET1lnigbacc = 0.0;
            if (!model->FEFET1lntoxGiven)
                model->FEFET1lntox = 0.0;
            if (!model->FEFET1leigbinvGiven)
                model->FEFET1leigbinv = 0.0;
            if (!model->FEFET1lpigcdGiven)
                model->FEFET1lpigcd = 0.0;
            if (!model->FEFET1lpoxedgeGiven)
                model->FEFET1lpoxedge = 0.0;
            if (!model->FEFET1lxrcrg1Given)
                model->FEFET1lxrcrg1 = 0.0;
            if (!model->FEFET1lxrcrg2Given)
                model->FEFET1lxrcrg2 = 0.0;
            if (!model->FEFET1leuGiven)
                model->FEFET1leu = 0.0;
            if (!model->FEFET1lvfbGiven)
                model->FEFET1lvfb = 0.0;
            if (!model->FEFET1llambdaGiven)
                model->FEFET1llambda = 0.0;
            if (!model->FEFET1lvtlGiven)
                model->FEFET1lvtl = 0.0;  
            if (!model->FEFET1lxnGiven)
                model->FEFET1lxn = 0.0;  
            if (!model->FEFET1lvfbsdoffGiven)
                model->FEFET1lvfbsdoff = 0.0;   
            if (!model->FEFET1ltvfbsdoffGiven)
                model->FEFET1ltvfbsdoff = 0.0;  
            if (!model->FEFET1ltvoffGiven)
                model->FEFET1ltvoff = 0.0;  


            if (!model->FEFET1lcgslGiven)  
                model->FEFET1lcgsl = 0.0;
            if (!model->FEFET1lcgdlGiven)  
                model->FEFET1lcgdl = 0.0;
            if (!model->FEFET1lckappasGiven)  
                model->FEFET1lckappas = 0.0;
            if (!model->FEFET1lckappadGiven)
                model->FEFET1lckappad = 0.0;
            if (!model->FEFET1lclcGiven)  
                model->FEFET1lclc = 0.0;
            if (!model->FEFET1lcleGiven)  
                model->FEFET1lcle = 0.0;
            if (!model->FEFET1lcfGiven)  
                model->FEFET1lcf = 0.0;
            if (!model->FEFET1lvfbcvGiven)  
                model->FEFET1lvfbcv = 0.0;
            if (!model->FEFET1lacdeGiven)
                model->FEFET1lacde = 0.0;
            if (!model->FEFET1lmoinGiven)
                model->FEFET1lmoin = 0.0;
            if (!model->FEFET1lnoffGiven)
                model->FEFET1lnoff = 0.0;
            if (!model->FEFET1lvoffcvGiven)
                model->FEFET1lvoffcv = 0.0;

        /* Width dependence */
            if (!model->FEFET1wcdscGiven)
            model->FEFET1wcdsc = 0.0;
            if (!model->FEFET1wcdscbGiven)
            model->FEFET1wcdscb = 0.0;  
            if (!model->FEFET1wcdscdGiven)
            model->FEFET1wcdscd = 0.0;
            if (!model->FEFET1wcitGiven)
            model->FEFET1wcit = 0.0;
            if (!model->FEFET1wnfactorGiven)
            model->FEFET1wnfactor = 0.0;
            if (!model->FEFET1wxjGiven)
                model->FEFET1wxj = 0.0;
            if (!model->FEFET1wvsatGiven)
                model->FEFET1wvsat = 0.0;
            if (!model->FEFET1watGiven)
                model->FEFET1wat = 0.0;
            if (!model->FEFET1wa0Given)
                model->FEFET1wa0 = 0.0; 
            if (!model->FEFET1wagsGiven)
                model->FEFET1wags = 0.0;
            if (!model->FEFET1wa1Given)
                model->FEFET1wa1 = 0.0;
            if (!model->FEFET1wa2Given)
                model->FEFET1wa2 = 0.0;
            if (!model->FEFET1wketaGiven)
                model->FEFET1wketa = 0.0;
            if (!model->FEFET1wnsubGiven)
                model->FEFET1wnsub = 0.0;
            if (!model->FEFET1wndepGiven)
                model->FEFET1wndep = 0.0;
            if (!model->FEFET1wnsdGiven)
                model->FEFET1wnsd = 0.0;
            if (!model->FEFET1wphinGiven)
                model->FEFET1wphin = 0.0;
            if (!model->FEFET1wngateGiven)
                model->FEFET1wngate = 0.0;
            if (!model->FEFET1wvbmGiven)
            model->FEFET1wvbm = 0.0;
            if (!model->FEFET1wxtGiven)
            model->FEFET1wxt = 0.0;
            if (!model->FEFET1wkt1Given)
                model->FEFET1wkt1 = 0.0; 
            if (!model->FEFET1wkt1lGiven)
                model->FEFET1wkt1l = 0.0;
            if (!model->FEFET1wkt2Given)
                model->FEFET1wkt2 = 0.0;
            if (!model->FEFET1wk3Given)
                model->FEFET1wk3 = 0.0;      
            if (!model->FEFET1wk3bGiven)
                model->FEFET1wk3b = 0.0;      
            if (!model->FEFET1ww0Given)
                model->FEFET1ww0 = 0.0;    
            if (!model->FEFET1wlpe0Given)
                model->FEFET1wlpe0 = 0.0;
            if (!model->FEFET1wlpebGiven)
                model->FEFET1wlpeb = 0.0; 
            if (!model->FEFET1wdvtp0Given)
                model->FEFET1wdvtp0 = 0.0;
            if (!model->FEFET1wdvtp1Given)
                model->FEFET1wdvtp1 = 0.0;
            if (!model->FEFET1wdvt0Given)
                model->FEFET1wdvt0 = 0.0;    
            if (!model->FEFET1wdvt1Given)
                model->FEFET1wdvt1 = 0.0;      
            if (!model->FEFET1wdvt2Given)
                model->FEFET1wdvt2 = 0.0;
            if (!model->FEFET1wdvt0wGiven)
                model->FEFET1wdvt0w = 0.0;    
            if (!model->FEFET1wdvt1wGiven)
                model->FEFET1wdvt1w = 0.0;      
            if (!model->FEFET1wdvt2wGiven)
                model->FEFET1wdvt2w = 0.0;
            if (!model->FEFET1wdroutGiven)
                model->FEFET1wdrout = 0.0;     
            if (!model->FEFET1wdsubGiven)
                model->FEFET1wdsub = 0.0;
            if (!model->FEFET1wvth0Given)
            model->FEFET1wvth0 = 0.0;
            if (!model->FEFET1wuaGiven)
                model->FEFET1wua = 0.0;
            if (!model->FEFET1wua1Given)
                model->FEFET1wua1 = 0.0;
            if (!model->FEFET1wubGiven)
                model->FEFET1wub = 0.0;
            if (!model->FEFET1wub1Given)
                model->FEFET1wub1 = 0.0;
            if (!model->FEFET1wucGiven)
                model->FEFET1wuc = 0.0;
            if (!model->FEFET1wuc1Given)
                model->FEFET1wuc1 = 0.0;
            if (!model->FEFET1wudGiven)
                model->FEFET1wud = 0.0;
            if (!model->FEFET1wud1Given)
                model->FEFET1wud1 = 0.0;
            if (!model->FEFET1wupGiven)
                model->FEFET1wup = 0.0;
            if (!model->FEFET1wlpGiven)
                model->FEFET1wlp = 0.0;
            if (!model->FEFET1wu0Given)
                model->FEFET1wu0 = 0.0;
            if (!model->FEFET1wuteGiven)
            model->FEFET1wute = 0.0;    
            if (!model->FEFET1wvoffGiven)
            model->FEFET1wvoff = 0.0;
            if (!model->FEFET1wminvGiven)
                model->FEFET1wminv = 0.0;
            if (!model->FEFET1wfproutGiven)
                model->FEFET1wfprout = 0.0;
            if (!model->FEFET1wpditsGiven)
                model->FEFET1wpdits = 0.0;
            if (!model->FEFET1wpditsdGiven)
                model->FEFET1wpditsd = 0.0;
            if (!model->FEFET1wdeltaGiven)  
                model->FEFET1wdelta = 0.0;
            if (!model->FEFET1wrdswGiven)
                model->FEFET1wrdsw = 0.0;
            if (!model->FEFET1wrdwGiven)
                model->FEFET1wrdw = 0.0;
            if (!model->FEFET1wrswGiven)
                model->FEFET1wrsw = 0.0;
            if (!model->FEFET1wprwbGiven)
                model->FEFET1wprwb = 0.0;
            if (!model->FEFET1wprwgGiven)
                model->FEFET1wprwg = 0.0;
            if (!model->FEFET1wprtGiven)
                model->FEFET1wprt = 0.0;
            if (!model->FEFET1weta0Given)
                model->FEFET1weta0 = 0.0;
            if (!model->FEFET1wetabGiven)
                model->FEFET1wetab = 0.0;
            if (!model->FEFET1wpclmGiven)
                model->FEFET1wpclm = 0.0; 
            if (!model->FEFET1wpdibl1Given)
                model->FEFET1wpdibl1 = 0.0;
            if (!model->FEFET1wpdibl2Given)
                model->FEFET1wpdibl2 = 0.0;
            if (!model->FEFET1wpdiblbGiven)
                model->FEFET1wpdiblb = 0.0;
            if (!model->FEFET1wpscbe1Given)
                model->FEFET1wpscbe1 = 0.0;
            if (!model->FEFET1wpscbe2Given)
                model->FEFET1wpscbe2 = 0.0;
            if (!model->FEFET1wpvagGiven)
                model->FEFET1wpvag = 0.0;     
            if (!model->FEFET1wwrGiven)  
                model->FEFET1wwr = 0.0;
            if (!model->FEFET1wdwgGiven)  
                model->FEFET1wdwg = 0.0;
            if (!model->FEFET1wdwbGiven)  
                model->FEFET1wdwb = 0.0;
            if (!model->FEFET1wb0Given)
                model->FEFET1wb0 = 0.0;
            if (!model->FEFET1wb1Given)  
                model->FEFET1wb1 = 0.0;
            if (!model->FEFET1walpha0Given)  
                model->FEFET1walpha0 = 0.0;
            if (!model->FEFET1walpha1Given)
                model->FEFET1walpha1 = 0.0;
            if (!model->FEFET1wbeta0Given)  
                model->FEFET1wbeta0 = 0.0;
            if (!model->FEFET1wagidlGiven)
                model->FEFET1wagidl = 0.0;
            if (!model->FEFET1wbgidlGiven)
                model->FEFET1wbgidl = 0.0;
            if (!model->FEFET1wcgidlGiven)
                model->FEFET1wcgidl = 0.0;
            if (!model->FEFET1wegidlGiven)
                model->FEFET1wegidl = 0.0;
            if (!model->FEFET1waigcGiven)
                model->FEFET1waigc = 0.0;
            if (!model->FEFET1wbigcGiven)
                model->FEFET1wbigc = 0.0;
            if (!model->FEFET1wcigcGiven)
                model->FEFET1wcigc = 0.0;
            if (!model->FEFET1waigsdGiven)
                model->FEFET1waigsd = 0.0;
            if (!model->FEFET1wbigsdGiven)
                model->FEFET1wbigsd = 0.0;
            if (!model->FEFET1wcigsdGiven)
                model->FEFET1wcigsd = 0.0;
            if (!model->FEFET1waigbaccGiven)
                model->FEFET1waigbacc = 0.0;
            if (!model->FEFET1wbigbaccGiven)
                model->FEFET1wbigbacc = 0.0;
            if (!model->FEFET1wcigbaccGiven)
                model->FEFET1wcigbacc = 0.0;
            if (!model->FEFET1waigbinvGiven)
                model->FEFET1waigbinv = 0.0;
            if (!model->FEFET1wbigbinvGiven)
                model->FEFET1wbigbinv = 0.0;
            if (!model->FEFET1wcigbinvGiven)
                model->FEFET1wcigbinv = 0.0;
            if (!model->FEFET1wnigcGiven)
                model->FEFET1wnigc = 0.0;
            if (!model->FEFET1wnigbinvGiven)
                model->FEFET1wnigbinv = 0.0;
            if (!model->FEFET1wnigbaccGiven)
                model->FEFET1wnigbacc = 0.0;
            if (!model->FEFET1wntoxGiven)
                model->FEFET1wntox = 0.0;
            if (!model->FEFET1weigbinvGiven)
                model->FEFET1weigbinv = 0.0;
            if (!model->FEFET1wpigcdGiven)
                model->FEFET1wpigcd = 0.0;
            if (!model->FEFET1wpoxedgeGiven)
                model->FEFET1wpoxedge = 0.0;
            if (!model->FEFET1wxrcrg1Given)
                model->FEFET1wxrcrg1 = 0.0;
            if (!model->FEFET1wxrcrg2Given)
                model->FEFET1wxrcrg2 = 0.0;
            if (!model->FEFET1weuGiven)
                model->FEFET1weu = 0.0;
            if (!model->FEFET1wvfbGiven)
                model->FEFET1wvfb = 0.0;
            if (!model->FEFET1wlambdaGiven)
                model->FEFET1wlambda = 0.0;
            if (!model->FEFET1wvtlGiven)
                model->FEFET1wvtl = 0.0;  
            if (!model->FEFET1wxnGiven)
                model->FEFET1wxn = 0.0;  
            if (!model->FEFET1wvfbsdoffGiven)
                model->FEFET1wvfbsdoff = 0.0;   
            if (!model->FEFET1wtvfbsdoffGiven)
                model->FEFET1wtvfbsdoff = 0.0;  
            if (!model->FEFET1wtvoffGiven)
                model->FEFET1wtvoff = 0.0;  

            if (!model->FEFET1wcgslGiven)  
                model->FEFET1wcgsl = 0.0;
            if (!model->FEFET1wcgdlGiven)  
                model->FEFET1wcgdl = 0.0;
            if (!model->FEFET1wckappasGiven)  
                model->FEFET1wckappas = 0.0;
            if (!model->FEFET1wckappadGiven)
                model->FEFET1wckappad = 0.0;
            if (!model->FEFET1wcfGiven)  
                model->FEFET1wcf = 0.0;
            if (!model->FEFET1wclcGiven)  
                model->FEFET1wclc = 0.0;
            if (!model->FEFET1wcleGiven)  
                model->FEFET1wcle = 0.0;
            if (!model->FEFET1wvfbcvGiven)  
                model->FEFET1wvfbcv = 0.0;
            if (!model->FEFET1wacdeGiven)
                model->FEFET1wacde = 0.0;
            if (!model->FEFET1wmoinGiven)
                model->FEFET1wmoin = 0.0;
            if (!model->FEFET1wnoffGiven)
                model->FEFET1wnoff = 0.0;
            if (!model->FEFET1wvoffcvGiven)
                model->FEFET1wvoffcv = 0.0;

        /* Cross-term dependence */
            if (!model->FEFET1pcdscGiven)
            model->FEFET1pcdsc = 0.0;
            if (!model->FEFET1pcdscbGiven)
            model->FEFET1pcdscb = 0.0;   
            if (!model->FEFET1pcdscdGiven)
            model->FEFET1pcdscd = 0.0;
            if (!model->FEFET1pcitGiven)
            model->FEFET1pcit = 0.0;
            if (!model->FEFET1pnfactorGiven)
            model->FEFET1pnfactor = 0.0;
            if (!model->FEFET1pxjGiven)
                model->FEFET1pxj = 0.0;
            if (!model->FEFET1pvsatGiven)
                model->FEFET1pvsat = 0.0;
            if (!model->FEFET1patGiven)
                model->FEFET1pat = 0.0;
            if (!model->FEFET1pa0Given)
                model->FEFET1pa0 = 0.0; 
                
            if (!model->FEFET1pagsGiven)
                model->FEFET1pags = 0.0;
            if (!model->FEFET1pa1Given)
                model->FEFET1pa1 = 0.0;
            if (!model->FEFET1pa2Given)
                model->FEFET1pa2 = 0.0;
            if (!model->FEFET1pketaGiven)
                model->FEFET1pketa = 0.0;
            if (!model->FEFET1pnsubGiven)
                model->FEFET1pnsub = 0.0;
            if (!model->FEFET1pndepGiven)
                model->FEFET1pndep = 0.0;
            if (!model->FEFET1pnsdGiven)
                model->FEFET1pnsd = 0.0;
            if (!model->FEFET1pphinGiven)
                model->FEFET1pphin = 0.0;
            if (!model->FEFET1pngateGiven)
                model->FEFET1pngate = 0.0;
            if (!model->FEFET1pvbmGiven)
            model->FEFET1pvbm = 0.0;
            if (!model->FEFET1pxtGiven)
            model->FEFET1pxt = 0.0;
            if (!model->FEFET1pkt1Given)
                model->FEFET1pkt1 = 0.0; 
            if (!model->FEFET1pkt1lGiven)
                model->FEFET1pkt1l = 0.0;
            if (!model->FEFET1pkt2Given)
                model->FEFET1pkt2 = 0.0;
            if (!model->FEFET1pk3Given)
                model->FEFET1pk3 = 0.0;      
            if (!model->FEFET1pk3bGiven)
                model->FEFET1pk3b = 0.0;      
            if (!model->FEFET1pw0Given)
                model->FEFET1pw0 = 0.0;    
            if (!model->FEFET1plpe0Given)
                model->FEFET1plpe0 = 0.0;
            if (!model->FEFET1plpebGiven)
                model->FEFET1plpeb = 0.0;
            if (!model->FEFET1pdvtp0Given)
                model->FEFET1pdvtp0 = 0.0;
            if (!model->FEFET1pdvtp1Given)
                model->FEFET1pdvtp1 = 0.0;
            if (!model->FEFET1pdvt0Given)
                model->FEFET1pdvt0 = 0.0;    
            if (!model->FEFET1pdvt1Given)
                model->FEFET1pdvt1 = 0.0;      
            if (!model->FEFET1pdvt2Given)
                model->FEFET1pdvt2 = 0.0;
            if (!model->FEFET1pdvt0wGiven)
                model->FEFET1pdvt0w = 0.0;    
            if (!model->FEFET1pdvt1wGiven)
                model->FEFET1pdvt1w = 0.0;      
            if (!model->FEFET1pdvt2wGiven)
                model->FEFET1pdvt2w = 0.0;
            if (!model->FEFET1pdroutGiven)
                model->FEFET1pdrout = 0.0;     
            if (!model->FEFET1pdsubGiven)
                model->FEFET1pdsub = 0.0;
            if (!model->FEFET1pvth0Given)
            model->FEFET1pvth0 = 0.0;
            if (!model->FEFET1puaGiven)
                model->FEFET1pua = 0.0;
            if (!model->FEFET1pua1Given)
                model->FEFET1pua1 = 0.0;
            if (!model->FEFET1pubGiven)
                model->FEFET1pub = 0.0;
            if (!model->FEFET1pub1Given)
                model->FEFET1pub1 = 0.0;
            if (!model->FEFET1pucGiven)
                model->FEFET1puc = 0.0;
            if (!model->FEFET1puc1Given)
                model->FEFET1puc1 = 0.0;
            if (!model->FEFET1pudGiven)
                model->FEFET1pud = 0.0;
            if (!model->FEFET1pud1Given)
                model->FEFET1pud1 = 0.0;
            if (!model->FEFET1pupGiven)
                model->FEFET1pup = 0.0;
            if (!model->FEFET1plpGiven)
                model->FEFET1plp = 0.0;
            if (!model->FEFET1pu0Given)
                model->FEFET1pu0 = 0.0;
            if (!model->FEFET1puteGiven)
            model->FEFET1pute = 0.0;    
            if (!model->FEFET1pvoffGiven)
            model->FEFET1pvoff = 0.0;
            if (!model->FEFET1pminvGiven)
                model->FEFET1pminv = 0.0;
            if (!model->FEFET1pfproutGiven)
                model->FEFET1pfprout = 0.0;
            if (!model->FEFET1ppditsGiven)
                model->FEFET1ppdits = 0.0;
            if (!model->FEFET1ppditsdGiven)
                model->FEFET1ppditsd = 0.0;
            if (!model->FEFET1pdeltaGiven)  
                model->FEFET1pdelta = 0.0;
            if (!model->FEFET1prdswGiven)
                model->FEFET1prdsw = 0.0;
            if (!model->FEFET1prdwGiven)
                model->FEFET1prdw = 0.0;
            if (!model->FEFET1prswGiven)
                model->FEFET1prsw = 0.0;
            if (!model->FEFET1pprwbGiven)
                model->FEFET1pprwb = 0.0;
            if (!model->FEFET1pprwgGiven)
                model->FEFET1pprwg = 0.0;
            if (!model->FEFET1pprtGiven)
                model->FEFET1pprt = 0.0;
            if (!model->FEFET1peta0Given)
                model->FEFET1peta0 = 0.0;
            if (!model->FEFET1petabGiven)
                model->FEFET1petab = 0.0;
            if (!model->FEFET1ppclmGiven)
                model->FEFET1ppclm = 0.0; 
            if (!model->FEFET1ppdibl1Given)
                model->FEFET1ppdibl1 = 0.0;
            if (!model->FEFET1ppdibl2Given)
                model->FEFET1ppdibl2 = 0.0;
            if (!model->FEFET1ppdiblbGiven)
                model->FEFET1ppdiblb = 0.0;
            if (!model->FEFET1ppscbe1Given)
                model->FEFET1ppscbe1 = 0.0;
            if (!model->FEFET1ppscbe2Given)
                model->FEFET1ppscbe2 = 0.0;
            if (!model->FEFET1ppvagGiven)
                model->FEFET1ppvag = 0.0;     
            if (!model->FEFET1pwrGiven)  
                model->FEFET1pwr = 0.0;
            if (!model->FEFET1pdwgGiven)  
                model->FEFET1pdwg = 0.0;
            if (!model->FEFET1pdwbGiven)  
                model->FEFET1pdwb = 0.0;
            if (!model->FEFET1pb0Given)
                model->FEFET1pb0 = 0.0;
            if (!model->FEFET1pb1Given)  
                model->FEFET1pb1 = 0.0;
            if (!model->FEFET1palpha0Given)  
                model->FEFET1palpha0 = 0.0;
            if (!model->FEFET1palpha1Given)
                model->FEFET1palpha1 = 0.0;
            if (!model->FEFET1pbeta0Given)  
                model->FEFET1pbeta0 = 0.0;
            if (!model->FEFET1pagidlGiven)
                model->FEFET1pagidl = 0.0;
            if (!model->FEFET1pbgidlGiven)
                model->FEFET1pbgidl = 0.0;
            if (!model->FEFET1pcgidlGiven)
                model->FEFET1pcgidl = 0.0;
            if (!model->FEFET1pegidlGiven)
                model->FEFET1pegidl = 0.0;
            if (!model->FEFET1paigcGiven)
                model->FEFET1paigc = 0.0;
            if (!model->FEFET1pbigcGiven)
                model->FEFET1pbigc = 0.0;
            if (!model->FEFET1pcigcGiven)
                model->FEFET1pcigc = 0.0;
            if (!model->FEFET1paigsdGiven)
                model->FEFET1paigsd = 0.0;
            if (!model->FEFET1pbigsdGiven)
                model->FEFET1pbigsd = 0.0;
            if (!model->FEFET1pcigsdGiven)
                model->FEFET1pcigsd = 0.0;
            if (!model->FEFET1paigbaccGiven)
                model->FEFET1paigbacc = 0.0;
            if (!model->FEFET1pbigbaccGiven)
                model->FEFET1pbigbacc = 0.0;
            if (!model->FEFET1pcigbaccGiven)
                model->FEFET1pcigbacc = 0.0;
            if (!model->FEFET1paigbinvGiven)
                model->FEFET1paigbinv = 0.0;
            if (!model->FEFET1pbigbinvGiven)
                model->FEFET1pbigbinv = 0.0;
            if (!model->FEFET1pcigbinvGiven)
                model->FEFET1pcigbinv = 0.0;
            if (!model->FEFET1pnigcGiven)
                model->FEFET1pnigc = 0.0;
            if (!model->FEFET1pnigbinvGiven)
                model->FEFET1pnigbinv = 0.0;
            if (!model->FEFET1pnigbaccGiven)
                model->FEFET1pnigbacc = 0.0;
            if (!model->FEFET1pntoxGiven)
                model->FEFET1pntox = 0.0;
            if (!model->FEFET1peigbinvGiven)
                model->FEFET1peigbinv = 0.0;
            if (!model->FEFET1ppigcdGiven)
                model->FEFET1ppigcd = 0.0;
            if (!model->FEFET1ppoxedgeGiven)
                model->FEFET1ppoxedge = 0.0;
            if (!model->FEFET1pxrcrg1Given)
                model->FEFET1pxrcrg1 = 0.0;
            if (!model->FEFET1pxrcrg2Given)
                model->FEFET1pxrcrg2 = 0.0;
            if (!model->FEFET1peuGiven)
                model->FEFET1peu = 0.0;
            if (!model->FEFET1pvfbGiven)
                model->FEFET1pvfb = 0.0;
            if (!model->FEFET1plambdaGiven)
                model->FEFET1plambda = 0.0;
            if (!model->FEFET1pvtlGiven)
                model->FEFET1pvtl = 0.0;  
            if (!model->FEFET1pxnGiven)
                model->FEFET1pxn = 0.0;  
            if (!model->FEFET1pvfbsdoffGiven)
                model->FEFET1pvfbsdoff = 0.0;   
            if (!model->FEFET1ptvfbsdoffGiven)
                model->FEFET1ptvfbsdoff = 0.0;  
            if (!model->FEFET1ptvoffGiven)
                model->FEFET1ptvoff = 0.0;  

            if (!model->FEFET1pcgslGiven)  
                model->FEFET1pcgsl = 0.0;
            if (!model->FEFET1pcgdlGiven)  
                model->FEFET1pcgdl = 0.0;
            if (!model->FEFET1pckappasGiven)  
                model->FEFET1pckappas = 0.0;
            if (!model->FEFET1pckappadGiven)
                model->FEFET1pckappad = 0.0;
            if (!model->FEFET1pcfGiven)  
                model->FEFET1pcf = 0.0;
            if (!model->FEFET1pclcGiven)  
                model->FEFET1pclc = 0.0;
            if (!model->FEFET1pcleGiven)  
                model->FEFET1pcle = 0.0;
            if (!model->FEFET1pvfbcvGiven)  
                model->FEFET1pvfbcv = 0.0;
            if (!model->FEFET1pacdeGiven)
                model->FEFET1pacde = 0.0;
            if (!model->FEFET1pmoinGiven)
                model->FEFET1pmoin = 0.0;
            if (!model->FEFET1pnoffGiven)
                model->FEFET1pnoff = 0.0;
            if (!model->FEFET1pvoffcvGiven)
                model->FEFET1pvoffcv = 0.0;

            if (!model->FEFET1gamma1Given)
                model->FEFET1gamma1 = 0.0;
            if (!model->FEFET1lgamma1Given)
                model->FEFET1lgamma1 = 0.0;
            if (!model->FEFET1wgamma1Given)
                model->FEFET1wgamma1 = 0.0;
            if (!model->FEFET1pgamma1Given)
                model->FEFET1pgamma1 = 0.0;
            if (!model->FEFET1gamma2Given)
                model->FEFET1gamma2 = 0.0;
            if (!model->FEFET1lgamma2Given)
                model->FEFET1lgamma2 = 0.0;
            if (!model->FEFET1wgamma2Given)
                model->FEFET1wgamma2 = 0.0;
            if (!model->FEFET1pgamma2Given)
                model->FEFET1pgamma2 = 0.0;
            if (!model->FEFET1vbxGiven)
                model->FEFET1vbx = 0.0;
            if (!model->FEFET1lvbxGiven)
                model->FEFET1lvbx = 0.0;
            if (!model->FEFET1wvbxGiven)
                model->FEFET1wvbx = 0.0;
            if (!model->FEFET1pvbxGiven)
                model->FEFET1pvbx = 0.0;

            /* unit degree celcius */
            if (!model->FEFET1tnomGiven)  
            model->FEFET1tnom = ckt->CKTnomTemp; 
            if (!model->FEFET1LintGiven)  
            model->FEFET1Lint = 0.0;
            if (!model->FEFET1LlGiven)  
            model->FEFET1Ll = 0.0;
            if (!model->FEFET1LlcGiven)
            model->FEFET1Llc = model->FEFET1Ll;
            if (!model->FEFET1LlnGiven)  
            model->FEFET1Lln = 1.0;
            if (!model->FEFET1LwGiven)  
            model->FEFET1Lw = 0.0;
            if (!model->FEFET1LwcGiven)
            model->FEFET1Lwc = model->FEFET1Lw;
            if (!model->FEFET1LwnGiven)  
            model->FEFET1Lwn = 1.0;
            if (!model->FEFET1LwlGiven)  
            model->FEFET1Lwl = 0.0;
            if (!model->FEFET1LwlcGiven)
            model->FEFET1Lwlc = model->FEFET1Lwl;
            if (!model->FEFET1LminGiven)  
            model->FEFET1Lmin = 0.0;
            if (!model->FEFET1LmaxGiven)  
            model->FEFET1Lmax = 1.0;
            if (!model->FEFET1WintGiven)  
            model->FEFET1Wint = 0.0;
            if (!model->FEFET1WlGiven)  
            model->FEFET1Wl = 0.0;
            if (!model->FEFET1WlcGiven)
            model->FEFET1Wlc = model->FEFET1Wl;
            if (!model->FEFET1WlnGiven)  
            model->FEFET1Wln = 1.0;
            if (!model->FEFET1WwGiven)  
            model->FEFET1Ww = 0.0;
            if (!model->FEFET1WwcGiven)
            model->FEFET1Wwc = model->FEFET1Ww;
            if (!model->FEFET1WwnGiven)  
            model->FEFET1Wwn = 1.0;
            if (!model->FEFET1WwlGiven)  
            model->FEFET1Wwl = 0.0;
            if (!model->FEFET1WwlcGiven)
            model->FEFET1Wwlc = model->FEFET1Wwl;
            if (!model->FEFET1WminGiven)  
            model->FEFET1Wmin = 0.0;
            if (!model->FEFET1WmaxGiven)  
            model->FEFET1Wmax = 1.0;
            if (!model->FEFET1dwcGiven)  
            model->FEFET1dwc = model->FEFET1Wint;
            if (!model->FEFET1dlcGiven)  
            model->FEFET1dlc = model->FEFET1Lint;
            if (!model->FEFET1xlGiven)  
            model->FEFET1xl = 0.0;
            if (!model->FEFET1xwGiven)  
            model->FEFET1xw = 0.0;
            if (!model->FEFET1dlcigGiven)
            model->FEFET1dlcig = model->FEFET1Lint;
            if (!model->FEFET1dwjGiven)
            model->FEFET1dwj = model->FEFET1dwc;
        if (!model->FEFET1cfGiven)
            model->FEFET1cf = 2.0 * model->FEFET1epsrox * EPS0 / PI
                    * log(1.0 + 0.4e-6 / model->FEFET1toxe);

            if (!model->FEFET1xpartGiven)
                model->FEFET1xpart = 0.0;
            if (!model->FEFET1sheetResistanceGiven)
                model->FEFET1sheetResistance = 0.0;

            if (!model->FEFET1SunitAreaJctCapGiven)
                model->FEFET1SunitAreaJctCap = 5.0E-4;
            if (!model->FEFET1DunitAreaJctCapGiven)
                model->FEFET1DunitAreaJctCap = model->FEFET1SunitAreaJctCap;
            if (!model->FEFET1SunitLengthSidewallJctCapGiven)
                model->FEFET1SunitLengthSidewallJctCap = 5.0E-10;
            if (!model->FEFET1DunitLengthSidewallJctCapGiven)
                model->FEFET1DunitLengthSidewallJctCap = model->FEFET1SunitLengthSidewallJctCap;
            if (!model->FEFET1SunitLengthGateSidewallJctCapGiven)
                model->FEFET1SunitLengthGateSidewallJctCap = model->FEFET1SunitLengthSidewallJctCap ;
            if (!model->FEFET1DunitLengthGateSidewallJctCapGiven)
                model->FEFET1DunitLengthGateSidewallJctCap = model->FEFET1SunitLengthGateSidewallJctCap;
            if (!model->FEFET1SjctSatCurDensityGiven)
                model->FEFET1SjctSatCurDensity = 1.0E-4;
            if (!model->FEFET1DjctSatCurDensityGiven)
                model->FEFET1DjctSatCurDensity = model->FEFET1SjctSatCurDensity;
            if (!model->FEFET1SjctSidewallSatCurDensityGiven)
                model->FEFET1SjctSidewallSatCurDensity = 0.0;
            if (!model->FEFET1DjctSidewallSatCurDensityGiven)
                model->FEFET1DjctSidewallSatCurDensity = model->FEFET1SjctSidewallSatCurDensity;
            if (!model->FEFET1SjctGateSidewallSatCurDensityGiven)
                model->FEFET1SjctGateSidewallSatCurDensity = 0.0;
            if (!model->FEFET1DjctGateSidewallSatCurDensityGiven)
                model->FEFET1DjctGateSidewallSatCurDensity = model->FEFET1SjctGateSidewallSatCurDensity;
            if (!model->FEFET1SbulkJctPotentialGiven)
                model->FEFET1SbulkJctPotential = 1.0;
            if (!model->FEFET1DbulkJctPotentialGiven)
                model->FEFET1DbulkJctPotential = model->FEFET1SbulkJctPotential;
            if (!model->FEFET1SsidewallJctPotentialGiven)
                model->FEFET1SsidewallJctPotential = 1.0;
            if (!model->FEFET1DsidewallJctPotentialGiven)
                model->FEFET1DsidewallJctPotential = model->FEFET1SsidewallJctPotential;
            if (!model->FEFET1SGatesidewallJctPotentialGiven)
                model->FEFET1SGatesidewallJctPotential = model->FEFET1SsidewallJctPotential;
            if (!model->FEFET1DGatesidewallJctPotentialGiven)
                model->FEFET1DGatesidewallJctPotential = model->FEFET1SGatesidewallJctPotential;
            if (!model->FEFET1SbulkJctBotGradingCoeffGiven)
                model->FEFET1SbulkJctBotGradingCoeff = 0.5;
            if (!model->FEFET1DbulkJctBotGradingCoeffGiven)
                model->FEFET1DbulkJctBotGradingCoeff = model->FEFET1SbulkJctBotGradingCoeff;
            if (!model->FEFET1SbulkJctSideGradingCoeffGiven)
                model->FEFET1SbulkJctSideGradingCoeff = 0.33;
            if (!model->FEFET1DbulkJctSideGradingCoeffGiven)
                model->FEFET1DbulkJctSideGradingCoeff = model->FEFET1SbulkJctSideGradingCoeff;
            if (!model->FEFET1SbulkJctGateSideGradingCoeffGiven)
                model->FEFET1SbulkJctGateSideGradingCoeff = model->FEFET1SbulkJctSideGradingCoeff;
            if (!model->FEFET1DbulkJctGateSideGradingCoeffGiven)
                model->FEFET1DbulkJctGateSideGradingCoeff = model->FEFET1SbulkJctGateSideGradingCoeff;
            if (!model->FEFET1SjctEmissionCoeffGiven)
                model->FEFET1SjctEmissionCoeff = 1.0;
            if (!model->FEFET1DjctEmissionCoeffGiven)
                model->FEFET1DjctEmissionCoeff = model->FEFET1SjctEmissionCoeff;
            if (!model->FEFET1SjctTempExponentGiven)
                model->FEFET1SjctTempExponent = 3.0;
            if (!model->FEFET1DjctTempExponentGiven)
                model->FEFET1DjctTempExponent = model->FEFET1SjctTempExponent;

            if (!model->FEFET1jtssGiven)
                model->FEFET1jtss = 0.0;
            if (!model->FEFET1jtsdGiven)
                model->FEFET1jtsd = model->FEFET1jtss;
            if (!model->FEFET1jtsswsGiven)
                model->FEFET1jtssws = 0.0;
            if (!model->FEFET1jtsswdGiven)
                model->FEFET1jtsswd = model->FEFET1jtssws;
            if (!model->FEFET1jtsswgsGiven)
                model->FEFET1jtsswgs = 0.0;
            if (!model->FEFET1jtsswgdGiven)
                model->FEFET1jtsswgd = model->FEFET1jtsswgs;
            if (!model->FEFET1njtsGiven)
                model->FEFET1njts = 20.0;
            if (!model->FEFET1njtsswGiven)
                model->FEFET1njtssw = 20.0;
            if (!model->FEFET1njtsswgGiven)
                model->FEFET1njtsswg = 20.0;
            if (!model->FEFET1xtssGiven)
                model->FEFET1xtss = 0.02;
            if (!model->FEFET1xtsdGiven)
                model->FEFET1xtsd = model->FEFET1xtss;
            if (!model->FEFET1xtsswsGiven)
                model->FEFET1xtssws = 0.02;
            if (!model->FEFET1xtsswdGiven)
                model->FEFET1xtsswd = model->FEFET1xtssws;
            if (!model->FEFET1xtsswgsGiven)
                model->FEFET1xtsswgs = 0.02;
            if (!model->FEFET1xtsswgdGiven)
                model->FEFET1xtsswgd = model->FEFET1xtsswgs;
            if (!model->FEFET1tnjtsGiven)
                model->FEFET1tnjts = 0.0;
            if (!model->FEFET1tnjtsswGiven)
                model->FEFET1tnjtssw = 0.0;
            if (!model->FEFET1tnjtsswgGiven)
                model->FEFET1tnjtsswg = 0.0;
            if (!model->FEFET1vtssGiven)
                model->FEFET1vtss = 10.0;
            if (!model->FEFET1vtsdGiven)
                model->FEFET1vtsd = model->FEFET1vtss;
            if (!model->FEFET1vtsswsGiven)
                model->FEFET1vtssws = 10.0;
            if (!model->FEFET1vtsswdGiven)
                model->FEFET1vtsswd = model->FEFET1vtssws;
            if (!model->FEFET1vtsswgsGiven)
                model->FEFET1vtsswgs = 10.0;
            if (!model->FEFET1vtsswgdGiven)
                model->FEFET1vtsswgd = model->FEFET1vtsswgs;

            if (!model->FEFET1oxideTrapDensityAGiven)
        {   if (model->FEFET1type == NMOS)
                    model->FEFET1oxideTrapDensityA = 6.25e41;
                else
                    model->FEFET1oxideTrapDensityA= 6.188e40;
        }
            if (!model->FEFET1oxideTrapDensityBGiven)
        {   if (model->FEFET1type == NMOS)
                    model->FEFET1oxideTrapDensityB = 3.125e26;
                else
                    model->FEFET1oxideTrapDensityB = 1.5e25;
        }
            if (!model->FEFET1oxideTrapDensityCGiven)
                model->FEFET1oxideTrapDensityC = 8.75e9;
            if (!model->FEFET1emGiven)
                model->FEFET1em = 4.1e7; /* V/m */
            if (!model->FEFET1efGiven)
                model->FEFET1ef = 1.0;
            if (!model->FEFET1afGiven)
                model->FEFET1af = 1.0;
            if (!model->FEFET1kfGiven)
                model->FEFET1kf = 0.0;

            if (!model->FEFET1vgsMaxGiven)
                model->FEFET1vgsMax = 1e99;
            if (!model->FEFET1vgdMaxGiven)
                model->FEFET1vgdMax = 1e99;
            if (!model->FEFET1vgbMaxGiven)
                model->FEFET1vgbMax = 1e99;
            if (!model->FEFET1vdsMaxGiven)
                model->FEFET1vdsMax = 1e99;
            if (!model->FEFET1vbsMaxGiven)
                model->FEFET1vbsMax = 1e99;
            if (!model->FEFET1vbdMaxGiven)
                model->FEFET1vbdMax = 1e99;
            if (!model->FEFET1vgsrMaxGiven)
                model->FEFET1vgsrMax = 1e99;
            if (!model->FEFET1vgdrMaxGiven)
                model->FEFET1vgdrMax = 1e99;
            if (!model->FEFET1vgbrMaxGiven)
                model->FEFET1vgbrMax = 1e99;
            if (!model->FEFET1vbsrMaxGiven)
                model->FEFET1vbsrMax = 1e99;
            if (!model->FEFET1vbdrMaxGiven)
                model->FEFET1vbdrMax = 1e99;

            /* stress effect */
            if (!model->FEFET1sarefGiven)
                model->FEFET1saref = 1e-6; /* m */
            if (!model->FEFET1sbrefGiven)
                model->FEFET1sbref = 1e-6;  /* m */
            if (!model->FEFET1wlodGiven)
                model->FEFET1wlod = 0;  /* m */
            if (!model->FEFET1ku0Given)
                model->FEFET1ku0 = 0; /* 1/m */
            if (!model->FEFET1kvsatGiven)
                model->FEFET1kvsat = 0;
            if (!model->FEFET1kvth0Given) /* m */
                model->FEFET1kvth0 = 0;
            if (!model->FEFET1tku0Given)
                model->FEFET1tku0 = 0;
            if (!model->FEFET1llodku0Given)
                model->FEFET1llodku0 = 0;
            if (!model->FEFET1wlodku0Given)
                model->FEFET1wlodku0 = 0;
            if (!model->FEFET1llodvthGiven)
                model->FEFET1llodvth = 0;
            if (!model->FEFET1wlodvthGiven)
                model->FEFET1wlodvth = 0;
            if (!model->FEFET1lku0Given)
                model->FEFET1lku0 = 0;
            if (!model->FEFET1wku0Given)
                model->FEFET1wku0 = 0;
            if (!model->FEFET1pku0Given)
                model->FEFET1pku0 = 0;
            if (!model->FEFET1lkvth0Given)
                model->FEFET1lkvth0 = 0;
            if (!model->FEFET1wkvth0Given)
                model->FEFET1wkvth0 = 0;
            if (!model->FEFET1pkvth0Given)
                model->FEFET1pkvth0 = 0;
            if (!model->FEFET1stk2Given)
                model->FEFET1stk2 = 0;
            if (!model->FEFET1lodk2Given)
                model->FEFET1lodk2 = 1.0;
            if (!model->FEFET1steta0Given)
                model->FEFET1steta0 = 0;
            if (!model->FEFET1lodeta0Given)
                model->FEFET1lodeta0 = 1.0;

        /* Well Proximity Effect  */
            if (!model->FEFET1webGiven)
                model->FEFET1web = 0.0; 
            if (!model->FEFET1wecGiven)
                model->FEFET1wec = 0.0;
            if (!model->FEFET1kvth0weGiven)
                model->FEFET1kvth0we = 0.0; 
            if (!model->FEFET1k2weGiven)
                model->FEFET1k2we = 0.0; 
            if (!model->FEFET1ku0weGiven)
                model->FEFET1ku0we = 0.0; 
            if (!model->FEFET1screfGiven)
                model->FEFET1scref = 1.0E-6; /* m */
            if (!model->FEFET1wpemodGiven)
                model->FEFET1wpemod = 0; 
            else if ((model->FEFET1wpemod != 0) && (model->FEFET1wpemod != 1))
            {   model->FEFET1wpemod = 0;
                printf("Warning: wpemod has been set to its default value: 0.\n");
            }
            if (!model->FEFET1lkvth0weGiven)
                model->FEFET1lkvth0we = 0; 
            if (!model->FEFET1lk2weGiven)
                model->FEFET1lk2we = 0;
            if (!model->FEFET1lku0weGiven)
                model->FEFET1lku0we = 0;
            if (!model->FEFET1wkvth0weGiven)
                model->FEFET1wkvth0we = 0; 
            if (!model->FEFET1wk2weGiven)
                model->FEFET1wk2we = 0;
            if (!model->FEFET1wku0weGiven)
                model->FEFET1wku0we = 0;
            if (!model->FEFET1pkvth0weGiven)
                model->FEFET1pkvth0we = 0; 
            if (!model->FEFET1pk2weGiven)
                model->FEFET1pk2we = 0;
            if (!model->FEFET1pku0weGiven)
                model->FEFET1pku0we = 0;

            DMCGeff = model->FEFET1dmcg - model->FEFET1dmcgt;
            DMCIeff = model->FEFET1dmci;
            DMDGeff = model->FEFET1dmdg - model->FEFET1dmcgt;

        /*
            * End processing models and begin to loop
            * through all the instances of the model
            */

            for (here = FEFET1instances(model); here != NULL ;
                here=FEFET1nextInstance(here)) 
            {

                if (!here->FECAP1_AreaGiven) here->FECAP1_Area = 1e-14;
                if (!here->FECAP1_icGiven) here->FECAP1initCondP = -20e-2; 
                here->FECAP1capac =  EPSF * here->FECAP1_Area/(4*K*PI*model->FECAP1_Thickness);
                here->FECAP1capacPseudo = 1;                
                if(here->FECAP1Ibranch == 0) { 
                    error = CKTmkCur(ckt,&tmp,here->FECAP1name,"ibranch"); 
                    if(error) return(error); 
                    here->FECAP1Ibranch = tmp->number; 
                } 

                if(here->FECAP1Pbranch == 0) { 
                    error = CKTmkCur(ckt,&tmp,here->FECAP1name,"pbranch"); 
                    if(error) return(error); 
                    here->FECAP1Pbranch = tmp->number; 
                } 
                /* allocate a chunk of the state vector */
                here->FEFET1states = *states;
                *states += FEFET1numStates;

                /* perform the parameter defaulting */
                if (!here->FEFET1lGiven)
                    here->FEFET1l = 5.0e-6;
                if (!here->FEFET1wGiven)
                    here->FEFET1w = 5.0e-6;
                if (!here->FEFET1mGiven)
                    here->FEFET1m = 1.0;
                if (!here->FEFET1nfGiven)
                    here->FEFET1nf = 1.0;
                if (!here->FEFET1minGiven)
                    here->FEFET1min = 0; /* integer */
                if (!here->FEFET1icVDSGiven)
                    here->FEFET1icVDS = 0.0;
                if (!here->FEFET1icVGSGiven)
                    here->FEFET1icVGS = 0.0;
                if (!here->FEFET1icVBSGiven)
                    here->FEFET1icVBS = 0.0;
                if (!here->FEFET1drainAreaGiven)
                    here->FEFET1drainArea = 0.0;
                if (!here->FEFET1drainPerimeterGiven)
                    here->FEFET1drainPerimeter = 0.0;
                if (!here->FEFET1drainSquaresGiven)
                    here->FEFET1drainSquares = 1.0;
                if (!here->FEFET1sourceAreaGiven)
                    here->FEFET1sourceArea = 0.0;
                if (!here->FEFET1sourcePerimeterGiven)
                    here->FEFET1sourcePerimeter = 0.0;
                if (!here->FEFET1sourceSquaresGiven)
                    here->FEFET1sourceSquares = 1.0;

                if (!here->FEFET1saGiven)
                    here->FEFET1sa = 0.0;
                if (!here->FEFET1sbGiven)
                    here->FEFET1sb = 0.0;
                if (!here->FEFET1sdGiven)
                    here->FEFET1sd = 0.0;

                if (!here->FEFET1rbdbGiven)
                    here->FEFET1rbdb = model->FEFET1rbdb; /* in ohm */
                if (!here->FEFET1rbsbGiven)
                    here->FEFET1rbsb = model->FEFET1rbsb;
                if (!here->FEFET1rbpbGiven)
                    here->FEFET1rbpb = model->FEFET1rbpb;
                if (!here->FEFET1rbpsGiven)
                    here->FEFET1rbps = model->FEFET1rbps;
                if (!here->FEFET1rbpdGiven)
                    here->FEFET1rbpd = model->FEFET1rbpd;
                if (!here->FEFET1delvtoGiven)
                    here->FEFET1delvto = 0.0;
                if (!here->FEFET1xgwGiven)
                    here->FEFET1xgw = model->FEFET1xgw;
                if (!here->FEFET1ngconGiven)
                    here->FEFET1ngcon = model->FEFET1ngcon;

                        
                /* Process instance model selectors, some
                * may override their global counterparts
            */
                if (!here->FEFET1rbodyModGiven)
                    here->FEFET1rbodyMod = model->FEFET1rbodyMod;
                else if ((here->FEFET1rbodyMod != 0) && (here->FEFET1rbodyMod != 1) && (here->FEFET1rbodyMod != 2))
                {   here->FEFET1rbodyMod = model->FEFET1rbodyMod;
                    printf("Warning: rbodyMod has been set to its global value %d.\n",
                model->FEFET1rbodyMod);
                }

                if (!here->FEFET1rgateModGiven)
                    here->FEFET1rgateMod = model->FEFET1rgateMod;
                else if ((here->FEFET1rgateMod != 0) && (here->FEFET1rgateMod != 1)
                && (here->FEFET1rgateMod != 2) && (here->FEFET1rgateMod != 3))
                {   here->FEFET1rgateMod = model->FEFET1rgateMod;
                    printf("Warning: rgateMod has been set to its global value %d.\n",
                    model->FEFET1rgateMod);
                }

                if (!here->FEFET1geoModGiven)
                    here->FEFET1geoMod = model->FEFET1geoMod;

                if (!here->FEFET1rgeoModGiven)
                    here->FEFET1rgeoMod = model->FEFET1rgeoMod;
                else if ((here->FEFET1rgeoMod != 0) && (here->FEFET1rgeoMod != 1))
                {   here->FEFET1rgeoMod = model->FEFET1rgeoMod;
                    printf("Warning: rgeoMod has been set to its global value %d.\n",
                    model->FEFET1rgeoMod);
                }
                if (!here->FEFET1trnqsModGiven)
                    here->FEFET1trnqsMod = model->FEFET1trnqsMod;
                else if ((here->FEFET1trnqsMod != 0) && (here->FEFET1trnqsMod != 1))
                {   here->FEFET1trnqsMod = model->FEFET1trnqsMod;
                    printf("Warning: trnqsMod has been set to its global value %d.\n",
                    model->FEFET1trnqsMod);
                }

                if (!here->FEFET1acnqsModGiven)
                    here->FEFET1acnqsMod = model->FEFET1acnqsMod;
                else if ((here->FEFET1acnqsMod != 0) && (here->FEFET1acnqsMod != 1))
                {   here->FEFET1acnqsMod = model->FEFET1acnqsMod;
                    printf("Warning: acnqsMod has been set to its global value %d.\n",
                    model->FEFET1acnqsMod);
                }

                /* stress effect */
                if (!here->FEFET1saGiven)
                    here->FEFET1sa = 0.0;
                if (!here->FEFET1sbGiven)
                    here->FEFET1sb = 0.0;
                if (!here->FEFET1sdGiven)
                    here->FEFET1sd = 2 * model->FEFET1dmcg;
            /* Well Proximity Effect  */
            if (!here->FEFET1scaGiven)
                    here->FEFET1sca = 0.0;
            if (!here->FEFET1scbGiven)
                    here->FEFET1scb = 0.0;
            if (!here->FEFET1sccGiven)
                    here->FEFET1scc = 0.0;
            if (!here->FEFET1scGiven)
                    here->FEFET1sc = 0.0; /* m */

                /* process drain series resistance */
                createNode = 0;
                if ( (model->FEFET1rdsMod != 0)
                                || (model->FEFET1tnoiMod != 0 && noiseAnalGiven))
                {
                createNode = 1;
                } else if (model->FEFET1sheetResistance > 0)
                {
                        if (here->FEFET1drainSquaresGiven
                                        && here->FEFET1drainSquares > 0)
                        {
                            createNode = 1;
                        } else if (!here->FEFET1drainSquaresGiven
                                        && (here->FEFET1rgeoMod != 0))
                        {
                            FEFET1RdseffGeo(here->FEFET1nf*here->FEFET1m, here->FEFET1geoMod,
                                    here->FEFET1rgeoMod, here->FEFET1min,
                                    here->FEFET1w, model->FEFET1sheetResistance,
                                    DMCGeff, DMCIeff, DMDGeff, 0, &Rtot);
                            if(Rtot > 0)
                                createNode = 1;
                        }
                }
                if ( createNode != 0 )
                {   if (here->FEFET1dNodePrime == 0)
                {   error = CKTmkVolt(ckt,&tmp,here->FEFET1name,"drain");
                    if(error) return(error);
                    here->FEFET1dNodePrime = tmp->number;

                    if (ckt->CKTcopyNodesets) {
                CKTnode *tmpNode;
            IFuid tmpName;    
                    if (CKTinst2Node(ckt,here,1,&tmpNode,&tmpName)==OK) {
                        if (tmpNode->nsGiven) {
                        tmp->nodeset=tmpNode->nodeset; 
                        tmp->nsGiven=tmpNode->nsGiven; 
                        }
                    }
                
    }

                }
                }
                else
                {   here->FEFET1dNodePrime = here->FEFET1dNode;
                }
                
                /* process source series resistance */
                createNode = 0;
                if ( (model->FEFET1rdsMod != 0)
                                || (model->FEFET1tnoiMod != 0 && noiseAnalGiven))
                {
                createNode = 1;
                } else if (model->FEFET1sheetResistance > 0)
                {
                        if (here->FEFET1sourceSquaresGiven
                                            && here->FEFET1sourceSquares > 0)
                        {
                            createNode = 1;
                        } else if (!here->FEFET1sourceSquaresGiven
                                            && (here->FEFET1rgeoMod != 0))
                        {
                            FEFET1RdseffGeo(here->FEFET1nf*here->FEFET1m, here->FEFET1geoMod,
                                    here->FEFET1rgeoMod, here->FEFET1min,
                                    here->FEFET1w, model->FEFET1sheetResistance,
                                    DMCGeff, DMCIeff, DMDGeff, 1, &Rtot);
                            if(Rtot > 0)
                                createNode = 1;
                        }
                }
                if ( createNode != 0 )
                {   if (here->FEFET1sNodePrime == 0)
                {   error = CKTmkVolt(ckt,&tmp,here->FEFET1name,"source");
                    if(error) return(error);
                    here->FEFET1sNodePrime = tmp->number;
            
            if (ckt->CKTcopyNodesets) {
            CKTnode *tmpNode;
            IFuid tmpName;
                    if (CKTinst2Node(ckt,here,3,&tmpNode,&tmpName)==OK) {
                        if (tmpNode->nsGiven) {
                        tmp->nodeset=tmpNode->nodeset; 
                        tmp->nsGiven=tmpNode->nsGiven; 
                        }
                    }
                    }				
            
                }
                }
                else
                    here->FEFET1sNodePrime = here->FEFET1sNode;

                if (here->FEFET1rgateMod > 0)
                {   if (here->FEFET1gNodePrime == 0)
                {   error = CKTmkVolt(ckt,&tmp,here->FEFET1name,"gate");
                    if(error) return(error);
                    here->FEFET1gNodePrime = tmp->number;
            
            if (ckt->CKTcopyNodesets) {
            CKTnode *tmpNode;
            IFuid tmpName;
                    if (CKTinst2Node(ckt,here,2,&tmpNode,&tmpName)==OK) {
                        if (tmpNode->nsGiven) {
                        tmp->nodeset=tmpNode->nodeset; 
                        tmp->nsGiven=tmpNode->nsGiven; 
                        }
                    }
                    }
                            
                }
                }
                else
                    here->FEFET1gNodePrime = here->FEFET1gNodeExt;

                if (here->FEFET1rgateMod == 3)
                {   if (here->FEFET1gNodeMid == 0)
                {   error = CKTmkVolt(ckt,&tmp,here->FEFET1name,"midgate");
                    if(error) return(error);
                    here->FEFET1gNodeMid = tmp->number;
                }
                }
                else
                    here->FEFET1gNodeMid = here->FEFET1gNodeExt;
                

                /* internal body nodes for body resistance model */
                if ((here->FEFET1rbodyMod ==1) || (here->FEFET1rbodyMod ==2))
                {   if (here->FEFET1dbNode == 0)
            {   error = CKTmkVolt(ckt,&tmp,here->FEFET1name,"dbody");
                        if(error) return(error);
                        here->FEFET1dbNode = tmp->number;
                
                if (ckt->CKTcopyNodesets) {
            CKTnode *tmpNode;
            IFuid tmpName;
                    if (CKTinst2Node(ckt,here,4,&tmpNode,&tmpName)==OK) {
                        if (tmpNode->nsGiven) {
                        tmp->nodeset=tmpNode->nodeset; 
                        tmp->nsGiven=tmpNode->nsGiven; 
                        }
                    }
                    }				
            }
            if (here->FEFET1bNodePrime == 0)
                    {   error = CKTmkVolt(ckt,&tmp,here->FEFET1name,"body");
                        if(error) return(error);
                        here->FEFET1bNodePrime = tmp->number;
                    }
            if (here->FEFET1sbNode == 0)
                    {   error = CKTmkVolt(ckt,&tmp,here->FEFET1name,"sbody");
                        if(error) return(error);
                        here->FEFET1sbNode = tmp->number;
                    }
                }
            else
                here->FEFET1dbNode = here->FEFET1bNodePrime = here->FEFET1sbNode
                    = here->FEFET1bNode;

                /* NQS node */
                if (here->FEFET1trnqsMod)
                {   if (here->FEFET1qNode == 0)
            {   error = CKTmkVolt(ckt,&tmp,here->FEFET1name,"charge");
                    if(error) return(error);
                    here->FEFET1qNode = tmp->number;
            
            if (ckt->CKTcopyNodesets) {
            CKTnode *tmpNode;
            IFuid tmpName;
                    if (CKTinst2Node(ckt,here,5,&tmpNode,&tmpName)==OK) {
                        if (tmpNode->nsGiven) {
                        tmp->nodeset=tmpNode->nodeset; 
                        tmp->nsGiven=tmpNode->nsGiven; 
                        }
                    }
                    }				
                }
                }
            else 
                here->FEFET1qNode = 0;

    /* set Sparse Matrix Pointers 
    * macro to make elements with built-in out-of-memory test */
    #define TSTALLOC(ptr,first,second) \
    do { if((here->ptr = SMPmakeElt(matrix, here->first, here->second)) == NULL){\
        return(E_NOMEM);\
    } } while(0)


                TSTALLOC(FECAP1posPosPtr,FECAP1posNode,FECAP1posNode);
                TSTALLOC(FECAP1posNegPtr,FECAP1posNode,FEFET1gNodeExt);
                TSTALLOC(FECAP1posIbrPtr, FECAP1posNode, FECAP1Ibranch); 

                TSTALLOC(FECAP1negPosPtr,FEFET1gNodeExt,FECAP1posNode);
                TSTALLOC(FECAP1negNegPtr,FEFET1gNodeExt,FEFET1gNodeExt);            
                TSTALLOC(FECAP1negIbrPtr, FEFET1gNodeExt, FECAP1Ibranch); 
                
                TSTALLOC(FECAP1ibrPosPtr, FECAP1Ibranch, FECAP1posNode); 
                TSTALLOC(FECAP1ibrNegPtr, FECAP1Ibranch, FEFET1gNodeExt); 
                TSTALLOC(FECAP1ibrIbrPtr, FECAP1Ibranch, FECAP1Ibranch); 
                TSTALLOC(FECAP1ibrPbrPtr, FECAP1Ibranch, FECAP1Pbranch); 
                
                TSTALLOC(FECAP1pbrPosPtr, FECAP1Pbranch, FECAP1posNode); 
                TSTALLOC(FECAP1pbrNegPtr, FECAP1Pbranch, FEFET1gNodeExt); 
                TSTALLOC(FECAP1pbrIbrPtr, FECAP1Pbranch, FECAP1Ibranch); 
                TSTALLOC(FECAP1pbrPbrPtr, FECAP1Pbranch,FECAP1Pbranch); 

                TSTALLOC(FEFET1DPbpPtr, FEFET1dNodePrime, FEFET1bNodePrime);
                TSTALLOC(FEFET1GPbpPtr, FEFET1gNodePrime, FEFET1bNodePrime);
                TSTALLOC(FEFET1SPbpPtr, FEFET1sNodePrime, FEFET1bNodePrime);

                TSTALLOC(FEFET1BPdpPtr, FEFET1bNodePrime, FEFET1dNodePrime);
                TSTALLOC(FEFET1BPgpPtr, FEFET1bNodePrime, FEFET1gNodePrime);
                TSTALLOC(FEFET1BPspPtr, FEFET1bNodePrime, FEFET1sNodePrime);
                TSTALLOC(FEFET1BPbpPtr, FEFET1bNodePrime, FEFET1bNodePrime);

                TSTALLOC(FEFET1DdPtr, FEFET1dNode, FEFET1dNode);
                TSTALLOC(FEFET1GPgpPtr, FEFET1gNodePrime, FEFET1gNodePrime);
                TSTALLOC(FEFET1SsPtr, FEFET1sNode, FEFET1sNode);
                TSTALLOC(FEFET1DPdpPtr, FEFET1dNodePrime, FEFET1dNodePrime);
                TSTALLOC(FEFET1SPspPtr, FEFET1sNodePrime, FEFET1sNodePrime);
                TSTALLOC(FEFET1DdpPtr, FEFET1dNode, FEFET1dNodePrime);
                TSTALLOC(FEFET1GPdpPtr, FEFET1gNodePrime, FEFET1dNodePrime);
                TSTALLOC(FEFET1GPspPtr, FEFET1gNodePrime, FEFET1sNodePrime);
                TSTALLOC(FEFET1SspPtr, FEFET1sNode, FEFET1sNodePrime);
                TSTALLOC(FEFET1DPspPtr, FEFET1dNodePrime, FEFET1sNodePrime);
                TSTALLOC(FEFET1DPdPtr, FEFET1dNodePrime, FEFET1dNode);
                TSTALLOC(FEFET1DPgpPtr, FEFET1dNodePrime, FEFET1gNodePrime);
                TSTALLOC(FEFET1SPgpPtr, FEFET1sNodePrime, FEFET1gNodePrime);
                TSTALLOC(FEFET1SPsPtr, FEFET1sNodePrime, FEFET1sNode);
                TSTALLOC(FEFET1SPdpPtr, FEFET1sNodePrime, FEFET1dNodePrime);

                TSTALLOC(FEFET1QqPtr, FEFET1qNode, FEFET1qNode);
                TSTALLOC(FEFET1QbpPtr, FEFET1qNode, FEFET1bNodePrime) ;
                TSTALLOC(FEFET1QdpPtr, FEFET1qNode, FEFET1dNodePrime);
                TSTALLOC(FEFET1QspPtr, FEFET1qNode, FEFET1sNodePrime);
                TSTALLOC(FEFET1QgpPtr, FEFET1qNode, FEFET1gNodePrime);
                TSTALLOC(FEFET1DPqPtr, FEFET1dNodePrime, FEFET1qNode);
                TSTALLOC(FEFET1SPqPtr, FEFET1sNodePrime, FEFET1qNode);
                TSTALLOC(FEFET1GPqPtr, FEFET1gNodePrime, FEFET1qNode);

                if (here->FEFET1rgateMod != 0)
                {   TSTALLOC(FEFET1GEgePtr, FEFET1gNodeExt, FEFET1gNodeExt);
                    TSTALLOC(FEFET1GEgpPtr, FEFET1gNodeExt, FEFET1gNodePrime);
                    TSTALLOC(FEFET1GPgePtr, FEFET1gNodePrime, FEFET1gNodeExt);
                    TSTALLOC(FEFET1GEdpPtr, FEFET1gNodeExt, FEFET1dNodePrime);
                    TSTALLOC(FEFET1GEspPtr, FEFET1gNodeExt, FEFET1sNodePrime);
                    TSTALLOC(FEFET1GEbpPtr, FEFET1gNodeExt, FEFET1bNodePrime);

                    TSTALLOC(FEFET1GMdpPtr, FEFET1gNodeMid, FEFET1dNodePrime);
                    TSTALLOC(FEFET1GMgpPtr, FEFET1gNodeMid, FEFET1gNodePrime);
                    TSTALLOC(FEFET1GMgmPtr, FEFET1gNodeMid, FEFET1gNodeMid);
                    TSTALLOC(FEFET1GMgePtr, FEFET1gNodeMid, FEFET1gNodeExt);
                    TSTALLOC(FEFET1GMspPtr, FEFET1gNodeMid, FEFET1sNodePrime);
                    TSTALLOC(FEFET1GMbpPtr, FEFET1gNodeMid, FEFET1bNodePrime);
                    TSTALLOC(FEFET1DPgmPtr, FEFET1dNodePrime, FEFET1gNodeMid);
                    TSTALLOC(FEFET1GPgmPtr, FEFET1gNodePrime, FEFET1gNodeMid);
                    TSTALLOC(FEFET1GEgmPtr, FEFET1gNodeExt, FEFET1gNodeMid);
                    TSTALLOC(FEFET1SPgmPtr, FEFET1sNodePrime, FEFET1gNodeMid);
                    TSTALLOC(FEFET1BPgmPtr, FEFET1bNodePrime, FEFET1gNodeMid);
                }	

                if ((here->FEFET1rbodyMod ==1) || (here->FEFET1rbodyMod ==2))
                {   TSTALLOC(FEFET1DPdbPtr, FEFET1dNodePrime, FEFET1dbNode);
                    TSTALLOC(FEFET1SPsbPtr, FEFET1sNodePrime, FEFET1sbNode);

                    TSTALLOC(FEFET1DBdpPtr, FEFET1dbNode, FEFET1dNodePrime);
                    TSTALLOC(FEFET1DBdbPtr, FEFET1dbNode, FEFET1dbNode);
                    TSTALLOC(FEFET1DBbpPtr, FEFET1dbNode, FEFET1bNodePrime);
                    TSTALLOC(FEFET1DBbPtr, FEFET1dbNode, FEFET1bNode);

                    TSTALLOC(FEFET1BPdbPtr, FEFET1bNodePrime, FEFET1dbNode);
                    TSTALLOC(FEFET1BPbPtr, FEFET1bNodePrime, FEFET1bNode);
                    TSTALLOC(FEFET1BPsbPtr, FEFET1bNodePrime, FEFET1sbNode);

                    TSTALLOC(FEFET1SBspPtr, FEFET1sbNode, FEFET1sNodePrime);
                    TSTALLOC(FEFET1SBbpPtr, FEFET1sbNode, FEFET1bNodePrime);
                    TSTALLOC(FEFET1SBbPtr, FEFET1sbNode, FEFET1bNode);
                    TSTALLOC(FEFET1SBsbPtr, FEFET1sbNode, FEFET1sbNode);

                    TSTALLOC(FEFET1BdbPtr, FEFET1bNode, FEFET1dbNode);
                    TSTALLOC(FEFET1BbpPtr, FEFET1bNode, FEFET1bNodePrime);
                    TSTALLOC(FEFET1BsbPtr, FEFET1bNode, FEFET1sbNode);
                    TSTALLOC(FEFET1BbPtr, FEFET1bNode, FEFET1bNode);
                }

                if (model->FEFET1rdsMod)
                {   TSTALLOC(FEFET1DgpPtr, FEFET1dNode, FEFET1gNodePrime);
                    TSTALLOC(FEFET1DspPtr, FEFET1dNode, FEFET1sNodePrime);
                    TSTALLOC(FEFET1DbpPtr, FEFET1dNode, FEFET1bNodePrime);
                    TSTALLOC(FEFET1SdpPtr, FEFET1sNode, FEFET1dNodePrime);
                    TSTALLOC(FEFET1SgpPtr, FEFET1sNode, FEFET1gNodePrime);
                    TSTALLOC(FEFET1SbpPtr, FEFET1sNode, FEFET1bNodePrime);
                }
            }
        }

    #ifdef USE_OMP
        InstCount = 0;
        model = (FEFET1model*)inModel;
        /* loop through all the BSIM4v6 device models
        to count the number of instances */

        for (; model != NULL; model = FEFET1nextModel(model))
        {
            /* loop through all the instances of the model */
            for (here = FEFET1instances(model); here != NULL;
                here = FEFET1nextInstance(here))
            {
                InstCount++;
            }
            model->FEFET1InstCount = 0;
            model->FEFET1InstanceArray = NULL;
        }
        InstArray = TMALLOC(FEFET1instance*, InstCount);
        model = (FEFET1model*)inModel;
        /* store this in the first model only */
        model->FEFET1InstCount = InstCount;
        model->FEFET1InstanceArray = InstArray;
        idx = 0;
        for (; model != NULL; model = FEFET1nextModel(model))
        {
            /* loop through all the instances of the model */
            for (here = FEFET1instances(model); here != NULL;
                here = FEFET1nextInstance(here))
            {
                InstArray[idx] = here;
                idx++;
            }
        }
    #endif

        return(OK);
    }  

    int
    FEFET1unsetup(
        GENmodel *inModel,
        CKTcircuit *ckt)
    {
    #ifndef HAS_BATCHSIM
        FEFET1model *model;
        FEFET1instance *here;

    #ifdef USE_OMP
        model = (FEFET1model*)inModel;
        tfree(model->FEFET1InstanceArray);
    #endif

        for (model = (FEFET1model *)inModel; model != NULL;
                model = FEFET1nextModel(model))
        {
            for (here = FEFET1instances(model); here != NULL;
                    here=FEFET1nextInstance(here))
            {
                if (here->FEFET1qNode > 0)
                    CKTdltNNum(ckt, here->FEFET1qNode);
                here->FEFET1qNode = 0;

                if (here->FEFET1sbNode > 0 &&
                    here->FEFET1sbNode != here->FEFET1bNode)
                    CKTdltNNum(ckt, here->FEFET1sbNode);
                here->FEFET1sbNode = 0;

                if (here->FEFET1bNodePrime > 0 &&
                    here->FEFET1bNodePrime != here->FEFET1bNode)
                    CKTdltNNum(ckt, here->FEFET1bNodePrime);
                here->FEFET1bNodePrime = 0;

                if (here->FEFET1dbNode > 0 &&
                    here->FEFET1dbNode != here->FEFET1bNode)
                    CKTdltNNum(ckt, here->FEFET1dbNode);
                here->FEFET1dbNode = 0;

                if (here->FEFET1gNodeMid > 0 &&
                    here->FEFET1gNodeMid != here->FEFET1gNodeExt)
                    CKTdltNNum(ckt, here->FEFET1gNodeMid);
                here->FEFET1gNodeMid = 0;

                if (here->FEFET1gNodePrime > 0 &&
                    here->FEFET1gNodePrime != here->FEFET1gNodeExt)
                    CKTdltNNum(ckt, here->FEFET1gNodePrime);
                here->FEFET1gNodePrime = 0;

                if (here->FEFET1sNodePrime > 0
                        && here->FEFET1sNodePrime != here->FEFET1sNode)
                    CKTdltNNum(ckt, here->FEFET1sNodePrime);
                here->FEFET1sNodePrime = 0;

                if (here->FEFET1dNodePrime > 0
                        && here->FEFET1dNodePrime != here->FEFET1dNode)
                    CKTdltNNum(ckt, here->FEFET1dNodePrime);
                here->FEFET1dNodePrime = 0;
            }
        }
    #endif
        return OK;
}
