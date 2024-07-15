/**********
Copyright 2024 Xidian University.	All rights reserved.
 Author: Bo Li	at Hangzhou Institute of Technology	 
 Modified: 2020/09/09  Bo Li
 Refered to NgSPICE Res/Cap related file
**********/


#ifndef BSIM4V5
#define BSIM4V5

#include "ngspice/ifsim.h"
#include "ngspice/gendefs.h"
#include "ngspice/cktdefs.h"
#include "ngspice/complex.h"
#include "ngspice/noisedef.h"         

typedef struct sFEFET1instance
{

    struct GENinstance gen;

#define FEFET1modPtr(inst) ((struct sFEFET1model *)((inst)->gen.GENmodPtr))
#define FEFET1nextInstance(inst) ((struct sFEFET1instance *)((inst)->gen.GENnextInstance))
#define FEFET1name gen.GENname
#define FEFET1states gen.GENstate

/**add fecap part**/
    const int FECAPposNode; /* number of positive node of FECAP1acitor */
    double FECAPcapac;    /* capacitance */
	double FECAPcapacPseudo; 
    double FECAPinitCondP; /* initial capacitor P if specified */
	double FECAP1_Area;


	int FECAP1Ibranch;
	int FECAP1Pbranch;


	double FECAP1_dF1dP;
	double FECAP1_dF2dV;
	double FECAP1_dF2dP;
	

    double *FECAP1posPosPtr;    /* pointer to sparse matrix diagonal at  * (positive,positive) */
    double *FECAP1posNegPtr;    /* pointer to sparse matrix offdiagonal at  * (positive,negative) */				  	
    double *FECAP1posIbrPtr;    /* pointer to sparse matrix offdiagonal at  * (positive,negative) */

    double *FECAP1negPosPtr;    /* pointer to sparse matrix offdiagonal at  * (negative,positive) */
    double *FECAP1negNegPtr;    /* pointer to sparse matrix diagonal at  * (negative,negative) */
    double *FECAP1negIbrPtr;    /* pointer to sparse matrix offdiagonal at  * (negative,positive) */

    double *FECAP1ibrPosPtr; /* pointer to sparse matrix offdiagonal at (added current i branch,added current i branch) */
    double *FECAP1ibrNegPtr; /*pointer to sparse matrix offdiagonal at (added current i branch,added w row) */
    double *FECAP1ibrIbrPtr; /* pointer to sparse matrix offdiagonal at (added current i branch,added current i branch) */
    double *FECAP1ibrPbrPtr; /*pointer to sparse matrix offdiagonal at (added current i branch,added w row) */


	double *FECAP1pbrPosPtr; /* pointer to sparse matrix offdiagonal at (added current w branch,positive) */
    double *FECAP1pbrNegPtr; /* pointer to sparse matrix offdiagonal at (added current w branch,negative) */
    double *FECAP1pbrIbrPtr; /* pointer to sparse matrix offdiagonal at (added p row,added current i branch) */
    double *FECAP1pbrPbrPtr; /* pointer to sparse matrix offdiagonal at (added p row, added w row */
							  
    unsigned FECAP1_AreaGiven    : 1;   /* flag to indicate area was specified */
    unsigned FECAP1_icGiven     : 1;   /* flag to indicate init. cond. was specified */


    const int FEFET1dNode;
    const int FEFET1gNodeExt;
    const int FEFET1sNode;
    const int FEFET1bNode;
    int FEFET1dNodePrime;
    int FEFET1gNodePrime;
    int FEFET1gNodeMid;
    int FEFET1sNodePrime;
    int FEFET1bNodePrime;
    int FEFET1dbNode;
    int FEFET1sbNode;
    int FEFET1qNode;

    double FEFET1ueff;
    double FEFET1thetavth; 
    double FEFET1von;
    double FEFET1vdsat;
    double FEFET1cgdo;
    double FEFET1qgdo;
    double FEFET1cgso;
    double FEFET1qgso;
    double FEFET1grbsb;
    double FEFET1grbdb;
    double FEFET1grbpb;
    double FEFET1grbps;
    double FEFET1grbpd;

    double FEFET1vjsmFwd;
    double FEFET1vjsmRev;
    double FEFET1vjdmFwd;
    double FEFET1vjdmRev;
    double FEFET1XExpBVS;
    double FEFET1XExpBVD;
    double FEFET1SslpFwd;
    double FEFET1SslpRev;
    double FEFET1DslpFwd;
    double FEFET1DslpRev;
    double FEFET1IVjsmFwd;
    double FEFET1IVjsmRev;
    double FEFET1IVjdmFwd;
    double FEFET1IVjdmRev;

    double FEFET1grgeltd;
    double FEFET1Pseff;
    double FEFET1Pdeff;
    double FEFET1Aseff;
    double FEFET1Adeff;

    double FEFET1l;
    double FEFET1w;
    double FEFET1drainArea;
    double FEFET1sourceArea;
    double FEFET1drainSquares;
    double FEFET1sourceSquares;
    double FEFET1drainPerimeter;
    double FEFET1sourcePerimeter;
    double FEFET1sourceConductance;
    double FEFET1drainConductance;
     /* stress effect instance param */
    double FEFET1sa;
    double FEFET1sb;
    double FEFET1sd;
    double FEFET1sca;
    double FEFET1scb;
    double FEFET1scc;
    double FEFET1sc;

    double FEFET1rbdb;
    double FEFET1rbsb;
    double FEFET1rbpb;
    double FEFET1rbps;
    double FEFET1rbpd;
    
    double FEFET1delvto;
    double FEFET1xgw;
    double FEFET1ngcon;

     /* added here to account stress effect instance dependence */
    double FEFET1u0temp;
    double FEFET1vsattemp;
    double FEFET1vth0;
    double FEFET1vfb;
    double FEFET1vfbzb;
    double FEFET1vtfbphi1;
    double FEFET1vtfbphi2;
    double FEFET1k2;
    double FEFET1vbsc;
    double FEFET1k2ox;
    double FEFET1eta0;

    double FEFET1icVDS;
    double FEFET1icVGS;
    double FEFET1icVBS;
    double FEFET1nf;
    double FEFET1m;
    int FEFET1off;
    int FEFET1mode;
    int FEFET1trnqsMod;
    int FEFET1acnqsMod;
    int FEFET1rbodyMod;
    int FEFET1rgateMod;
    int FEFET1geoMod;
    int FEFET1rgeoMod;
    int FEFET1min;


    /* OP point */
    double FEFET1Vgsteff;
    double FEFET1vgs_eff;
    double FEFET1vgd_eff;
    double FEFET1dvgs_eff_dvg;
    double FEFET1dvgd_eff_dvg;
    double FEFET1Vdseff;
    double FEFET1nstar;
    double FEFET1Abulk;
    double FEFET1EsatL;
    double FEFET1AbovVgst2Vtm;
    double FEFET1qinv;
    double FEFET1cd;
    double FEFET1cbs;
    double FEFET1cbd;
    double FEFET1csub;
    double FEFET1Igidl;
    double FEFET1Igisl;
    double FEFET1gm;
    double FEFET1gds;
    double FEFET1gmbs;
    double FEFET1gbd;
    double FEFET1gbs;

    double FEFET1gbbs;
    double FEFET1gbgs;
    double FEFET1gbds;
    double FEFET1ggidld;
    double FEFET1ggidlg;
    double FEFET1ggidls;
    double FEFET1ggidlb;
    double FEFET1ggisld;
    double FEFET1ggislg;
    double FEFET1ggisls;
    double FEFET1ggislb;

    double FEFET1Igcs;
    double FEFET1gIgcsg;
    double FEFET1gIgcsd;
    double FEFET1gIgcss;
    double FEFET1gIgcsb;
    double FEFET1Igcd;
    double FEFET1gIgcdg;
    double FEFET1gIgcdd;
    double FEFET1gIgcds;
    double FEFET1gIgcdb;

    double FEFET1Igs;
    double FEFET1gIgsg;
    double FEFET1gIgss;
    double FEFET1Igd;
    double FEFET1gIgdg;
    double FEFET1gIgdd;

    double FEFET1Igb;
    double FEFET1gIgbg;
    double FEFET1gIgbd;
    double FEFET1gIgbs;
    double FEFET1gIgbb;

    double FEFET1grdsw;
    double FEFET1IdovVds;
    double FEFET1gcrg;
    double FEFET1gcrgd;
    double FEFET1gcrgg;
    double FEFET1gcrgs;
    double FEFET1gcrgb;

    double FEFET1gstot;
    double FEFET1gstotd;
    double FEFET1gstotg;
    double FEFET1gstots;
    double FEFET1gstotb;

    double FEFET1gdtot;
    double FEFET1gdtotd;
    double FEFET1gdtotg;
    double FEFET1gdtots;
    double FEFET1gdtotb;

    double FEFET1cggb;
    double FEFET1cgdb;
    double FEFET1cgsb;
    double FEFET1cbgb;
    double FEFET1cbdb;
    double FEFET1cbsb;
    double FEFET1cdgb;
    double FEFET1cddb;
    double FEFET1cdsb;
    double FEFET1csgb;
    double FEFET1csdb;
    double FEFET1cssb;
    double FEFET1cgbb;
    double FEFET1cdbb;
    double FEFET1csbb;
    double FEFET1cbbb;
    double FEFET1capbd;
    double FEFET1capbs;

    double FEFET1cqgb;
    double FEFET1cqdb;
    double FEFET1cqsb;
    double FEFET1cqbb;

    double FEFET1qgate;
    double FEFET1qbulk;
    double FEFET1qdrn;
    double FEFET1qsrc;
    double FEFET1qdef;

    double FEFET1qchqs;
    double FEFET1taunet;
    double FEFET1gtau;
    double FEFET1gtg;
    double FEFET1gtd;
    double FEFET1gts;
    double FEFET1gtb;
    double FEFET1SjctTempRevSatCur;
    double FEFET1DjctTempRevSatCur;
    double FEFET1SswTempRevSatCur;
    double FEFET1DswTempRevSatCur;
    double FEFET1SswgTempRevSatCur;
    double FEFET1DswgTempRevSatCur;

    struct fefet1SizeDependParam  *pParam;

    unsigned FEFET1lGiven :1;
    unsigned FEFET1wGiven :1;
    unsigned FEFET1mGiven :1;
    unsigned FEFET1nfGiven :1;
    unsigned FEFET1minGiven :1;
    unsigned FEFET1drainAreaGiven :1;
    unsigned FEFET1sourceAreaGiven    :1;
    unsigned FEFET1drainSquaresGiven  :1;
    unsigned FEFET1sourceSquaresGiven :1;
    unsigned FEFET1drainPerimeterGiven    :1;
    unsigned FEFET1sourcePerimeterGiven   :1;
    unsigned FEFET1saGiven :1;
    unsigned FEFET1sbGiven :1;
    unsigned FEFET1sdGiven :1;
    unsigned FEFET1scaGiven :1;
    unsigned FEFET1scbGiven :1;
    unsigned FEFET1sccGiven :1;
    unsigned FEFET1scGiven :1;
    unsigned FEFET1rbdbGiven   :1;
    unsigned FEFET1rbsbGiven   :1;
    unsigned FEFET1rbpbGiven   :1;
    unsigned FEFET1rbpdGiven   :1;
    unsigned FEFET1rbpsGiven   :1;
    unsigned FEFET1delvtoGiven   :1;
    unsigned FEFET1xgwGiven   :1;
    unsigned FEFET1ngconGiven   :1;
    unsigned FEFET1icVDSGiven :1;
    unsigned FEFET1icVGSGiven :1;
    unsigned FEFET1icVBSGiven :1;
    unsigned FEFET1trnqsModGiven :1;
    unsigned FEFET1acnqsModGiven :1;
    unsigned FEFET1rbodyModGiven :1;
    unsigned FEFET1rgateModGiven :1;
    unsigned FEFET1geoModGiven :1;
    unsigned FEFET1rgeoModGiven :1;


    double *FEFET1DPdPtr;
    double *FEFET1DPdpPtr;
    double *FEFET1DPgpPtr;
    double *FEFET1DPgmPtr;
    double *FEFET1DPspPtr;
    double *FEFET1DPbpPtr;
    double *FEFET1DPdbPtr;

    double *FEFET1DdPtr;
    double *FEFET1DdpPtr;

    double *FEFET1GPdpPtr;
    double *FEFET1GPgpPtr;
    double *FEFET1GPgmPtr;
    double *FEFET1GPgePtr;
    double *FEFET1GPspPtr;
    double *FEFET1GPbpPtr;

    double *FEFET1GMdpPtr;
    double *FEFET1GMgpPtr;
    double *FEFET1GMgmPtr;
    double *FEFET1GMgePtr;
    double *FEFET1GMspPtr;
    double *FEFET1GMbpPtr;

    double *FEFET1GEdpPtr;
    double *FEFET1GEgpPtr;
    double *FEFET1GEgmPtr;
    double *FEFET1GEgePtr;
    double *FEFET1GEspPtr;
    double *FEFET1GEbpPtr;

    double *FEFET1SPdpPtr;
    double *FEFET1SPgpPtr;
    double *FEFET1SPgmPtr;
    double *FEFET1SPsPtr;
    double *FEFET1SPspPtr;
    double *FEFET1SPbpPtr;
    double *FEFET1SPsbPtr;

    double *FEFET1SspPtr;
    double *FEFET1SsPtr;

    double *FEFET1BPdpPtr;
    double *FEFET1BPgpPtr;
    double *FEFET1BPgmPtr;
    double *FEFET1BPspPtr;
    double *FEFET1BPdbPtr;
    double *FEFET1BPbPtr;
    double *FEFET1BPsbPtr;
    double *FEFET1BPbpPtr;

    double *FEFET1DBdpPtr;
    double *FEFET1DBdbPtr;
    double *FEFET1DBbpPtr;
    double *FEFET1DBbPtr;

    double *FEFET1SBspPtr;
    double *FEFET1SBbpPtr;
    double *FEFET1SBbPtr;
    double *FEFET1SBsbPtr;

    double *FEFET1BdbPtr;
    double *FEFET1BbpPtr;
    double *FEFET1BsbPtr;
    double *FEFET1BbPtr;

    double *FEFET1DgpPtr;
    double *FEFET1DspPtr;
    double *FEFET1DbpPtr;
    double *FEFET1SdpPtr;
    double *FEFET1SgpPtr;
    double *FEFET1SbpPtr;

    double *FEFET1QdpPtr;
    double *FEFET1QgpPtr;
    double *FEFET1QspPtr;
    double *FEFET1QbpPtr;
    double *FEFET1QqPtr;
    double *FEFET1DPqPtr;
    double *FEFET1GPqPtr;
    double *FEFET1SPqPtr;

#ifdef USE_OMP
    /* per instance storage of results, to update matrix at a later stge */
    double FEFET1rhsdPrime;
    double FEFET1rhsgPrime;
    double FEFET1rhsgExt;
    double FEFET1grhsMid;
    double FEFET1rhsbPrime;
    double FEFET1rhssPrime;
    double FEFET1rhsdb;
    double FEFET1rhssb;
    double FEFET1rhsd;
    double FEFET1rhss;
    double FEFET1rhsq;

    double FEFET1_1;
    double FEFET1_2;
    double FEFET1_3;
    double FEFET1_4;
    double FEFET1_5;
    double FEFET1_6;
    double FEFET1_7;
    double FEFET1_8;
    double FEFET1_9;
    double FEFET1_10;
    double FEFET1_11;
    double FEFET1_12;
    double FEFET1_13;
    double FEFET1_14;
    double FEFET1_15;
    double FEFET1_16;
    double FEFET1_17;
    double FEFET1_18;
    double FEFET1_19;
    double FEFET1_20;
    double FEFET1_21;
    double FEFET1_22;
    double FEFET1_23;
    double FEFET1_24;
    double FEFET1_25;
    double FEFET1_26;
    double FEFET1_27;
    double FEFET1_28;
    double FEFET1_29;
    double FEFET1_30;
    double FEFET1_31;
    double FEFET1_32;
    double FEFET1_33;
    double FEFET1_34;
    double FEFET1_35;
    double FEFET1_36;
    double FEFET1_37;
    double FEFET1_38;
    double FEFET1_39;
    double FEFET1_40;
    double FEFET1_41;
    double FEFET1_42;
    double FEFET1_43;
    double FEFET1_44;
    double FEFET1_45;
    double FEFET1_46;
    double FEFET1_47;
    double FEFET1_48;
    double FEFET1_49;
    double FEFET1_50;
    double FEFET1_51;
    double FEFET1_52;
    double FEFET1_53;
    double FEFET1_54;
    double FEFET1_55;
    double FEFET1_56;
    double FEFET1_57;
    double FEFET1_58;
    double FEFET1_59;
    double FEFET1_60;
    double FEFET1_61;
    double FEFET1_62;
    double FEFET1_63;
    double FEFET1_64;
    double FEFET1_65;
    double FEFET1_66;
    double FEFET1_67;
    double FEFET1_68;
    double FEFET1_69;
    double FEFET1_70;
    double FEFET1_71;
    double FEFET1_72;
    double FEFET1_73;
    double FEFET1_74;
    double FEFET1_75;
    double FEFET1_76;
    double FEFET1_77;
    double FEFET1_78;
    double FEFET1_79;
    double FEFET1_80;
    double FEFET1_81;
    double FEFET1_82;
    double FEFET1_83;
    double FEFET1_84;
    double FEFET1_85;
    double FEFET1_86;
    double FEFET1_87;
    double FEFET1_88;
    double FEFET1_89;
    double FEFET1_90;
    double FEFET1_91;
    double FEFET1_92;
    double FEFET1_93;
    double FEFET1_94;
    double FEFET1_95;
    double FEFET1_96;
    double FEFET1_97;
    double FEFET1_98;
    double FEFET1_99;
    double FEFET1_100;
    double FEFET1_101;
    double FEFET1_102;
    double FEFET1_103;

#endif

#define FEFET1vbd FEFET1states+ 0
#define FEFET1vbs FEFET1states+ 1
#define FEFET1vgs FEFET1states+ 2
#define FEFET1vds FEFET1states+ 3
#define FEFET1vdbs FEFET1states+ 4
#define FEFET1vdbd FEFET1states+ 5
#define FEFET1vsbs FEFET1states+ 6
#define FEFET1vges FEFET1states+ 7
#define FEFET1vgms FEFET1states+ 8
#define FEFET1vses FEFET1states+ 9
#define FEFET1vdes FEFET1states+ 10

#define FEFET1qb FEFET1states+ 11
#define FEFET1cqb FEFET1states+ 12
#define FEFET1qg FEFET1states+ 13
#define FEFET1cqg FEFET1states+ 14
#define FEFET1qd FEFET1states+ 15
#define FEFET1cqd FEFET1states+ 16
#define FEFET1qgmid FEFET1states+ 17
#define FEFET1cqgmid FEFET1states+ 18

#define FEFET1qbs  FEFET1states+ 19
#define FEFET1cqbs  FEFET1states+ 20
#define FEFET1qbd  FEFET1states+ 21
#define FEFET1cqbd  FEFET1states+ 22

#define FEFET1qcheq FEFET1states+ 23
#define FEFET1cqcheq FEFET1states+ 24
#define FEFET1qcdump FEFET1states+ 25
#define FEFET1cqcdump FEFET1states+ 26
#define FEFET1qdef FEFET1states+ 27
#define FEFET1qs FEFET1states+ 28

#define FECAP1qcap FEFET1state+29    /* charge on the capacitor */
#define FECAP1ccap FEFET1state+30  /* current through the capacitor */

#define FECAP1pcap FEFET1state+31
#define FECAP1pccap FEFET1state+32


#define FEFET1numStates 33


/* indices to the array of FEFET1 NOISE SOURCES */

#define FEFET1RDNOIZ       0
#define FEFET1RSNOIZ       1
#define FEFET1RGNOIZ       2
#define FEFET1RBPSNOIZ     3
#define FEFET1RBPDNOIZ     4
#define FEFET1RBPBNOIZ     5
#define FEFET1RBSBNOIZ     6
#define FEFET1RBDBNOIZ     7
#define FEFET1IDNOIZ       8
#define FEFET1FLNOIZ       9
#define FEFET1IGSNOIZ      10
#define FEFET1IGDNOIZ      11
#define FEFET1IGBNOIZ      12
#define FEFET1TOTNOIZ      13

#define FEFET1NSRCS        14  /* Number of FEFET1 noise sources */

#ifndef NONOISE
    double FEFET1nVar[NSTATVARS][FEFET1NSRCS];
#else /* NONOISE */
        double **FEFET1nVar;
#endif /* NONOISE */

} FEFET1instance ;

struct fefet1SizeDependParam
{
    double Width;
    double Length;
    double NFinger;

    double FEFET1cdsc;           
    double FEFET1cdscb;    
    double FEFET1cdscd;       
    double FEFET1cit;           
    double FEFET1nfactor;      
    double FEFET1xj;
    double FEFET1vsat;         
    double FEFET1at;         
    double FEFET1a0;   
    double FEFET1ags;      
    double FEFET1a1;         
    double FEFET1a2;         
    double FEFET1keta;     
    double FEFET1nsub;
    double FEFET1ndep;        
    double FEFET1nsd;
    double FEFET1phin;
    double FEFET1ngate;        
    double FEFET1gamma1;      
    double FEFET1gamma2;     
    double FEFET1vbx;      
    double FEFET1vbi;       
    double FEFET1vbm;       
    double FEFET1xt;       
    double FEFET1phi;
    double FEFET1litl;
    double FEFET1k1;
    double FEFET1kt1;
    double FEFET1kt1l;
    double FEFET1kt2;
    double FEFET1k2;
    double FEFET1k3;
    double FEFET1k3b;
    double FEFET1w0;
    double FEFET1dvtp0;
    double FEFET1dvtp1;
    double FEFET1lpe0;
    double FEFET1lpeb;
    double FEFET1dvt0;      
    double FEFET1dvt1;      
    double FEFET1dvt2;      
    double FEFET1dvt0w;      
    double FEFET1dvt1w;      
    double FEFET1dvt2w;      
    double FEFET1drout;      
    double FEFET1dsub;      
    double FEFET1vth0;
    double FEFET1ua;
    double FEFET1ua1;
    double FEFET1ub;
    double FEFET1ub1;
    double FEFET1uc;
    double FEFET1uc1;
    double FEFET1ud;
    double FEFET1ud1;
    double FEFET1up;
    double FEFET1lp;
    double FEFET1u0;
    double FEFET1eu;
    double FEFET1ute;
    double FEFET1voff;
    double FEFET1tvoff;
    double FEFET1minv;
    double FEFET1vfb;
    double FEFET1delta;
    double FEFET1rdsw;       
    double FEFET1rds0;       
    double FEFET1rs0;
    double FEFET1rd0;
    double FEFET1rsw;
    double FEFET1rdw;
    double FEFET1prwg;       
    double FEFET1prwb;       
    double FEFET1prt;       
    double FEFET1eta0;         
    double FEFET1etab;         
    double FEFET1pclm;      
    double FEFET1pdibl1;      
    double FEFET1pdibl2;      
    double FEFET1pdiblb;      
    double FEFET1fprout;
    double FEFET1pdits;
    double FEFET1pditsd;
    double FEFET1pscbe1;       
    double FEFET1pscbe2;       
    double FEFET1pvag;       
    double FEFET1wr;
    double FEFET1dwg;
    double FEFET1dwb;
    double FEFET1b0;
    double FEFET1b1;
    double FEFET1alpha0;
    double FEFET1alpha1;
    double FEFET1beta0;
    double FEFET1agidl;
    double FEFET1bgidl;
    double FEFET1cgidl;
    double FEFET1egidl;
    double FEFET1aigc;
    double FEFET1bigc;
    double FEFET1cigc;
    double FEFET1aigsd;
    double FEFET1bigsd;
    double FEFET1cigsd;
    double FEFET1aigbacc;
    double FEFET1bigbacc;
    double FEFET1cigbacc;
    double FEFET1aigbinv;
    double FEFET1bigbinv;
    double FEFET1cigbinv;
    double FEFET1nigc;
    double FEFET1nigbacc;
    double FEFET1nigbinv;
    double FEFET1ntox;
    double FEFET1eigbinv;
    double FEFET1pigcd;
    double FEFET1poxedge;
    double FEFET1xrcrg1;
    double FEFET1xrcrg2;
    double FEFET1lambda; /* overshoot */
    double FEFET1vtl; /* thermal velocity limit */
    double FEFET1xn; /* back scattering parameter */
    double FEFET1lc; /* back scattering parameter */
    double FEFET1tfactor;  /* ballistic transportation factor  */
    double FEFET1vfbsdoff;  /* S/D flatband offset voltage  */
    double FEFET1tvfbsdoff;  

/* added for stress effect */
    double FEFET1ku0;
    double FEFET1kvth0;
    double FEFET1ku0temp;
    double FEFET1rho_ref;
    double FEFET1inv_od_ref;
/* added for well proximity effect */
    double FEFET1kvth0we;
    double FEFET1k2we;
    double FEFET1ku0we;

    /* CV model */
    double FEFET1cgsl;
    double FEFET1cgdl;
    double FEFET1ckappas;
    double FEFET1ckappad;
    double FEFET1cf;
    double FEFET1clc;
    double FEFET1cle;
    double FEFET1vfbcv;
    double FEFET1noff;
    double FEFET1voffcv;
    double FEFET1acde;
    double FEFET1moin;

/* Pre-calculated constants */

    double FEFET1dw;
    double FEFET1dl;
    double FEFET1leff;
    double FEFET1weff;

    double FEFET1dwc;
    double FEFET1dlc;
    double FEFET1dlcig;
    double FEFET1dwj;
    double FEFET1leffCV;
    double FEFET1weffCV;
    double FEFET1weffCJ;
    double FEFET1abulkCVfactor;
    double FEFET1cgso;
    double FEFET1cgdo;
    double FEFET1cgbo;

    double FEFET1u0temp;       
    double FEFET1vsattemp;   
    double FEFET1sqrtPhi;   
    double FEFET1phis3;   
    double FEFET1Xdep0;          
    double FEFET1sqrtXdep0;          
    double FEFET1theta0vb0;
    double FEFET1thetaRout; 
    double FEFET1mstar;
    double FEFET1voffcbn;
    double FEFET1rdswmin;
    double FEFET1rdwmin;
    double FEFET1rswmin;
    double FEFET1vfbsd;

    double FEFET1cof1;
    double FEFET1cof2;
    double FEFET1cof3;
    double FEFET1cof4;
    double FEFET1cdep0;
    double FEFET1ToxRatio;
    double FEFET1Aechvb;
    double FEFET1Bechvb;
    double FEFET1ToxRatioEdge;
    double FEFET1AechvbEdge;
    double FEFET1BechvbEdge;
    double FEFET1ldeb;
    double FEFET1k1ox;
    double FEFET1k2ox;
    double FEFET1vfbzbfactor;


    struct fefet1SizeDependParam  *pNext;
};


typedef struct sFEFET1model 
{

    struct GENmodel gen;

#define FEFET1modType gen.GENmodType
#define FEFET1nextModel(inst) ((struct sFEFET1model *)((inst)->gen.GENnextModel))
#define FEFET1instances(inst) ((FEFET1instance *)((inst)->gen.GENinstances))
#define FEFET1modName gen.GENmodName

/* add fecap model part */
    double FECAP1_Alpha;         /* Relative dielectric constant */
	double FECAP1_Beta;
	double FECAP1_Gamma;
	double FECAP1_Rho;
	double FECAP1_Thickness;      /* Insulator thickness */
	double FECAP1_Ec;
	double FECAP1_Ps;

    unsigned FECAP1_AlphaGiven      : 1;    /* flag indicates default capacitance given */
    unsigned FECAP1_BetaGiven        : 1;    /* Unit Area Capacitance ( F/ M**2 ) */
    unsigned FECAP1_GammaGiven      : 1;    /* Unit Length Sidewall Capacitance( F/M )*/
    unsigned FECAP1_RhoGiven  : 1;    /* flag indicates default width given*/
    unsigned FECAP1_ThicknessGiven : 1;    /* flag indicates deafult lenght given */
    unsigned FECAP1_EcGiven  : 1;  
    unsigned FECAP1_PsGiven  : 1;  

    int FECAP1type;
    unsigned FECAP1typeGiven : 1;


    int FEFET1type;

    int    FEFET1mobMod;
    int    FEFET1capMod;
    int    FEFET1dioMod;
    int    FEFET1trnqsMod;
    int    FEFET1acnqsMod;
    int    FEFET1fnoiMod;
    int    FEFET1tnoiMod;
    int    FEFET1rdsMod;
    int    FEFET1rbodyMod;
    int    FEFET1rgateMod;
    int    FEFET1perMod;
    int    FEFET1geoMod;
    int    FEFET1rgeoMod;
    int    FEFET1igcMod;
    int    FEFET1igbMod;
    int    FEFET1tempMod;
    int    FEFET1binUnit;
    int    FEFET1paramChk;
    char   *FEFET1version;             
    double FEFET1toxe;             
    double FEFET1toxp;
    double FEFET1toxm;
    double FEFET1dtox;
    double FEFET1epsrox;
    double FEFET1cdsc;           
    double FEFET1cdscb; 
    double FEFET1cdscd;          
    double FEFET1cit;           
    double FEFET1nfactor;      
    double FEFET1xj;
    double FEFET1vsat;         
    double FEFET1at;         
    double FEFET1a0;   
    double FEFET1ags;      
    double FEFET1a1;         
    double FEFET1a2;         
    double FEFET1keta;     
    double FEFET1nsub;
    double FEFET1ndep;        
    double FEFET1nsd;
    double FEFET1phin;
    double FEFET1ngate;        
    double FEFET1gamma1;      
    double FEFET1gamma2;     
    double FEFET1vbx;      
    double FEFET1vbm;       
    double FEFET1xt;       
    double FEFET1k1;
    double FEFET1kt1;
    double FEFET1kt1l;
    double FEFET1kt2;
    double FEFET1k2;
    double FEFET1k3;
    double FEFET1k3b;
    double FEFET1w0;
    double FEFET1dvtp0;
    double FEFET1dvtp1;
    double FEFET1lpe0;
    double FEFET1lpeb;
    double FEFET1dvt0;      
    double FEFET1dvt1;      
    double FEFET1dvt2;      
    double FEFET1dvt0w;      
    double FEFET1dvt1w;      
    double FEFET1dvt2w;      
    double FEFET1drout;      
    double FEFET1dsub;      
    double FEFET1vth0;
    double FEFET1eu;
    double FEFET1ua;
    double FEFET1ua1;
    double FEFET1ub;
    double FEFET1ub1;
    double FEFET1uc;
    double FEFET1uc1;
    double FEFET1ud;
    double FEFET1ud1;
    double FEFET1up;
    double FEFET1lp;
    double FEFET1u0;
    double FEFET1ute;
    double FEFET1voff;
    double FEFET1tvoff;
    double FEFET1minv;
    double FEFET1voffl;
    double FEFET1delta;
    double FEFET1rdsw;       
    double FEFET1rdswmin;
    double FEFET1rdwmin;
    double FEFET1rswmin;
    double FEFET1rsw;
    double FEFET1rdw;
    double FEFET1prwg;
    double FEFET1prwb;
    double FEFET1prt;       
    double FEFET1eta0;         
    double FEFET1etab;         
    double FEFET1pclm;      
    double FEFET1pdibl1;      
    double FEFET1pdibl2;      
    double FEFET1pdiblb;
    double FEFET1fprout;
    double FEFET1pdits;
    double FEFET1pditsd;
    double FEFET1pditsl;
    double FEFET1pscbe1;       
    double FEFET1pscbe2;       
    double FEFET1pvag;       
    double FEFET1wr;
    double FEFET1dwg;
    double FEFET1dwb;
    double FEFET1b0;
    double FEFET1b1;
    double FEFET1alpha0;
    double FEFET1alpha1;
    double FEFET1beta0;
    double FEFET1agidl;
    double FEFET1bgidl;
    double FEFET1cgidl;
    double FEFET1egidl;
    double FEFET1aigc;
    double FEFET1bigc;
    double FEFET1cigc;
    double FEFET1aigsd;
    double FEFET1bigsd;
    double FEFET1cigsd;
    double FEFET1aigbacc;
    double FEFET1bigbacc;
    double FEFET1cigbacc;
    double FEFET1aigbinv;
    double FEFET1bigbinv;
    double FEFET1cigbinv;
    double FEFET1nigc;
    double FEFET1nigbacc;
    double FEFET1nigbinv;
    double FEFET1ntox;
    double FEFET1eigbinv;
    double FEFET1pigcd;
    double FEFET1poxedge;
    double FEFET1toxref;
    double FEFET1ijthdfwd;
    double FEFET1ijthsfwd;
    double FEFET1ijthdrev;
    double FEFET1ijthsrev;
    double FEFET1xjbvd;
    double FEFET1xjbvs;
    double FEFET1bvd;
    double FEFET1bvs;

    double FEFET1jtss;
    double FEFET1jtsd;
    double FEFET1jtssws;
    double FEFET1jtsswd;
    double FEFET1jtsswgs;
    double FEFET1jtsswgd;
    double FEFET1njts;
    double FEFET1njtssw;
    double FEFET1njtsswg;
    double FEFET1xtss;
    double FEFET1xtsd;
    double FEFET1xtssws;
    double FEFET1xtsswd;
    double FEFET1xtsswgs;
    double FEFET1xtsswgd;
    double FEFET1tnjts;
    double FEFET1tnjtssw;
    double FEFET1tnjtsswg;
    double FEFET1vtss;
    double FEFET1vtsd;
    double FEFET1vtssws;
    double FEFET1vtsswd;
    double FEFET1vtsswgs;
    double FEFET1vtsswgd;

    double FEFET1xrcrg1;
    double FEFET1xrcrg2;
    double FEFET1lambda;
    double FEFET1vtl; 
    double FEFET1lc; 
    double FEFET1xn; 
    double FEFET1vfbsdoff;  /* S/D flatband offset voltage  */
    double FEFET1lintnoi;  /* lint offset for noise calculation  */
    double FEFET1tvfbsdoff;  

    double FEFET1vfb;
    double FEFET1gbmin;
    double FEFET1rbdb;
    double FEFET1rbsb;
    double FEFET1rbpb;
    double FEFET1rbps;
    double FEFET1rbpd;

    double FEFET1rbps0;
    double FEFET1rbpsl;
    double FEFET1rbpsw;
    double FEFET1rbpsnf;

    double FEFET1rbpd0;
    double FEFET1rbpdl;
    double FEFET1rbpdw;
    double FEFET1rbpdnf;

    double FEFET1rbpbx0;
    double FEFET1rbpbxl;
    double FEFET1rbpbxw;
    double FEFET1rbpbxnf;
    double FEFET1rbpby0;
    double FEFET1rbpbyl;
    double FEFET1rbpbyw;
    double FEFET1rbpbynf;

    double FEFET1rbsbx0;
    double FEFET1rbsby0;
    double FEFET1rbdbx0;
    double FEFET1rbdby0;

    double FEFET1rbsdbxl;
    double FEFET1rbsdbxw;
    double FEFET1rbsdbxnf;
    double FEFET1rbsdbyl;
    double FEFET1rbsdbyw;
    double FEFET1rbsdbynf;

    double FEFET1tnoia;
    double FEFET1tnoib;
    double FEFET1rnoia;
    double FEFET1rnoib;
    double FEFET1ntnoi;

    /* CV model and Parasitics */
    double FEFET1cgsl;
    double FEFET1cgdl;
    double FEFET1ckappas;
    double FEFET1ckappad;
    double FEFET1cf;
    double FEFET1vfbcv;
    double FEFET1clc;
    double FEFET1cle;
    double FEFET1dwc;
    double FEFET1dlc;
    double FEFET1xw;
    double FEFET1xl;
    double FEFET1dlcig;
    double FEFET1dwj;
    double FEFET1noff;
    double FEFET1voffcv;
    double FEFET1acde;
    double FEFET1moin;
    double FEFET1tcj;
    double FEFET1tcjsw;
    double FEFET1tcjswg;
    double FEFET1tpb;
    double FEFET1tpbsw;
    double FEFET1tpbswg;
    double FEFET1dmcg;
    double FEFET1dmci;
    double FEFET1dmdg;
    double FEFET1dmcgt;
    double FEFET1xgw;
    double FEFET1xgl;
    double FEFET1rshg;
    double FEFET1ngcon;

    /* Length Dependence */
    double FEFET1lcdsc;           
    double FEFET1lcdscb; 
    double FEFET1lcdscd;          
    double FEFET1lcit;           
    double FEFET1lnfactor;      
    double FEFET1lxj;
    double FEFET1lvsat;         
    double FEFET1lat;         
    double FEFET1la0;   
    double FEFET1lags;      
    double FEFET1la1;         
    double FEFET1la2;         
    double FEFET1lketa;     
    double FEFET1lnsub;
    double FEFET1lndep;        
    double FEFET1lnsd;
    double FEFET1lphin;
    double FEFET1lngate;        
    double FEFET1lgamma1;      
    double FEFET1lgamma2;     
    double FEFET1lvbx;      
    double FEFET1lvbm;       
    double FEFET1lxt;       
    double FEFET1lk1;
    double FEFET1lkt1;
    double FEFET1lkt1l;
    double FEFET1lkt2;
    double FEFET1lk2;
    double FEFET1lk3;
    double FEFET1lk3b;
    double FEFET1lw0;
    double FEFET1ldvtp0;
    double FEFET1ldvtp1;
    double FEFET1llpe0;
    double FEFET1llpeb;
    double FEFET1ldvt0;      
    double FEFET1ldvt1;      
    double FEFET1ldvt2;      
    double FEFET1ldvt0w;      
    double FEFET1ldvt1w;      
    double FEFET1ldvt2w;      
    double FEFET1ldrout;      
    double FEFET1ldsub;      
    double FEFET1lvth0;
    double FEFET1lua;
    double FEFET1lua1;
    double FEFET1lub;
    double FEFET1lub1;
    double FEFET1luc;
    double FEFET1luc1;
    double FEFET1lud;
    double FEFET1lud1;
    double FEFET1lup;
    double FEFET1llp;
    double FEFET1lu0;
    double FEFET1leu;
    double FEFET1lute;
    double FEFET1lvoff;
    double FEFET1ltvoff;
    double FEFET1lminv;
    double FEFET1ldelta;
    double FEFET1lrdsw;       
    double FEFET1lrsw;
    double FEFET1lrdw;
    double FEFET1lprwg;
    double FEFET1lprwb;
    double FEFET1lprt;       
    double FEFET1leta0;         
    double FEFET1letab;         
    double FEFET1lpclm;      
    double FEFET1lpdibl1;      
    double FEFET1lpdibl2;      
    double FEFET1lpdiblb;
    double FEFET1lfprout;
    double FEFET1lpdits;
    double FEFET1lpditsd;
    double FEFET1lpscbe1;       
    double FEFET1lpscbe2;       
    double FEFET1lpvag;       
    double FEFET1lwr;
    double FEFET1ldwg;
    double FEFET1ldwb;
    double FEFET1lb0;
    double FEFET1lb1;
    double FEFET1lalpha0;
    double FEFET1lalpha1;
    double FEFET1lbeta0;
    double FEFET1lvfb;
    double FEFET1lagidl;
    double FEFET1lbgidl;
    double FEFET1lcgidl;
    double FEFET1legidl;
    double FEFET1laigc;
    double FEFET1lbigc;
    double FEFET1lcigc;
    double FEFET1laigsd;
    double FEFET1lbigsd;
    double FEFET1lcigsd;
    double FEFET1laigbacc;
    double FEFET1lbigbacc;
    double FEFET1lcigbacc;
    double FEFET1laigbinv;
    double FEFET1lbigbinv;
    double FEFET1lcigbinv;
    double FEFET1lnigc;
    double FEFET1lnigbacc;
    double FEFET1lnigbinv;
    double FEFET1lntox;
    double FEFET1leigbinv;
    double FEFET1lpigcd;
    double FEFET1lpoxedge;
    double FEFET1lxrcrg1;
    double FEFET1lxrcrg2;
    double FEFET1llambda;
    double FEFET1lvtl; 
    double FEFET1lxn; 
    double FEFET1lvfbsdoff; 
    double FEFET1ltvfbsdoff; 

    /* CV model */
    double FEFET1lcgsl;
    double FEFET1lcgdl;
    double FEFET1lckappas;
    double FEFET1lckappad;
    double FEFET1lcf;
    double FEFET1lclc;
    double FEFET1lcle;
    double FEFET1lvfbcv;
    double FEFET1lnoff;
    double FEFET1lvoffcv;
    double FEFET1lacde;
    double FEFET1lmoin;

    /* Width Dependence */
    double FEFET1wcdsc;           
    double FEFET1wcdscb; 
    double FEFET1wcdscd;          
    double FEFET1wcit;           
    double FEFET1wnfactor;      
    double FEFET1wxj;
    double FEFET1wvsat;         
    double FEFET1wat;         
    double FEFET1wa0;   
    double FEFET1wags;      
    double FEFET1wa1;         
    double FEFET1wa2;         
    double FEFET1wketa;     
    double FEFET1wnsub;
    double FEFET1wndep;        
    double FEFET1wnsd;
    double FEFET1wphin;
    double FEFET1wngate;        
    double FEFET1wgamma1;      
    double FEFET1wgamma2;     
    double FEFET1wvbx;      
    double FEFET1wvbm;       
    double FEFET1wxt;       
    double FEFET1wk1;
    double FEFET1wkt1;
    double FEFET1wkt1l;
    double FEFET1wkt2;
    double FEFET1wk2;
    double FEFET1wk3;
    double FEFET1wk3b;
    double FEFET1ww0;
    double FEFET1wdvtp0;
    double FEFET1wdvtp1;
    double FEFET1wlpe0;
    double FEFET1wlpeb;
    double FEFET1wdvt0;      
    double FEFET1wdvt1;      
    double FEFET1wdvt2;      
    double FEFET1wdvt0w;      
    double FEFET1wdvt1w;      
    double FEFET1wdvt2w;      
    double FEFET1wdrout;      
    double FEFET1wdsub;      
    double FEFET1wvth0;
    double FEFET1wua;
    double FEFET1wua1;
    double FEFET1wub;
    double FEFET1wub1;
    double FEFET1wuc;
    double FEFET1wuc1;
    double FEFET1wud;
    double FEFET1wud1;
    double FEFET1wup;
    double FEFET1wlp;
    double FEFET1wu0;
    double FEFET1weu;
    double FEFET1wute;
    double FEFET1wvoff;
    double FEFET1wtvoff;
    double FEFET1wminv;
    double FEFET1wdelta;
    double FEFET1wrdsw;       
    double FEFET1wrsw;
    double FEFET1wrdw;
    double FEFET1wprwg;
    double FEFET1wprwb;
    double FEFET1wprt;       
    double FEFET1weta0;         
    double FEFET1wetab;         
    double FEFET1wpclm;      
    double FEFET1wpdibl1;      
    double FEFET1wpdibl2;      
    double FEFET1wpdiblb;
    double FEFET1wfprout;
    double FEFET1wpdits;
    double FEFET1wpditsd;
    double FEFET1wpscbe1;       
    double FEFET1wpscbe2;       
    double FEFET1wpvag;       
    double FEFET1wwr;
    double FEFET1wdwg;
    double FEFET1wdwb;
    double FEFET1wb0;
    double FEFET1wb1;
    double FEFET1walpha0;
    double FEFET1walpha1;
    double FEFET1wbeta0;
    double FEFET1wvfb;
    double FEFET1wagidl;
    double FEFET1wbgidl;
    double FEFET1wcgidl;
    double FEFET1wegidl;
    double FEFET1waigc;
    double FEFET1wbigc;
    double FEFET1wcigc;
    double FEFET1waigsd;
    double FEFET1wbigsd;
    double FEFET1wcigsd;
    double FEFET1waigbacc;
    double FEFET1wbigbacc;
    double FEFET1wcigbacc;
    double FEFET1waigbinv;
    double FEFET1wbigbinv;
    double FEFET1wcigbinv;
    double FEFET1wnigc;
    double FEFET1wnigbacc;
    double FEFET1wnigbinv;
    double FEFET1wntox;
    double FEFET1weigbinv;
    double FEFET1wpigcd;
    double FEFET1wpoxedge;
    double FEFET1wxrcrg1;
    double FEFET1wxrcrg2;
    double FEFET1wlambda;
    double FEFET1wvtl; 
    double FEFET1wxn; 
    double FEFET1wvfbsdoff;  
    double FEFET1wtvfbsdoff;  

    /* CV model */
    double FEFET1wcgsl;
    double FEFET1wcgdl;
    double FEFET1wckappas;
    double FEFET1wckappad;
    double FEFET1wcf;
    double FEFET1wclc;
    double FEFET1wcle;
    double FEFET1wvfbcv;
    double FEFET1wnoff;
    double FEFET1wvoffcv;
    double FEFET1wacde;
    double FEFET1wmoin;

    /* Cross-term Dependence */
    double FEFET1pcdsc;           
    double FEFET1pcdscb; 
    double FEFET1pcdscd;          
    double FEFET1pcit;           
    double FEFET1pnfactor;      
    double FEFET1pxj;
    double FEFET1pvsat;         
    double FEFET1pat;         
    double FEFET1pa0;   
    double FEFET1pags;      
    double FEFET1pa1;         
    double FEFET1pa2;         
    double FEFET1pketa;     
    double FEFET1pnsub;
    double FEFET1pndep;        
    double FEFET1pnsd;
    double FEFET1pphin;
    double FEFET1pngate;        
    double FEFET1pgamma1;      
    double FEFET1pgamma2;     
    double FEFET1pvbx;      
    double FEFET1pvbm;       
    double FEFET1pxt;       
    double FEFET1pk1;
    double FEFET1pkt1;
    double FEFET1pkt1l;
    double FEFET1pkt2;
    double FEFET1pk2;
    double FEFET1pk3;
    double FEFET1pk3b;
    double FEFET1pw0;
    double FEFET1pdvtp0;
    double FEFET1pdvtp1;
    double FEFET1plpe0;
    double FEFET1plpeb;
    double FEFET1pdvt0;      
    double FEFET1pdvt1;      
    double FEFET1pdvt2;      
    double FEFET1pdvt0w;      
    double FEFET1pdvt1w;      
    double FEFET1pdvt2w;      
    double FEFET1pdrout;      
    double FEFET1pdsub;      
    double FEFET1pvth0;
    double FEFET1pua;
    double FEFET1pua1;
    double FEFET1pub;
    double FEFET1pub1;
    double FEFET1puc;
    double FEFET1puc1;
    double FEFET1pud;
    double FEFET1pud1;
    double FEFET1pup;
    double FEFET1plp;
    double FEFET1pu0;
    double FEFET1peu;
    double FEFET1pute;
    double FEFET1pvoff;
    double FEFET1ptvoff;
    double FEFET1pminv;
    double FEFET1pdelta;
    double FEFET1prdsw;
    double FEFET1prsw;
    double FEFET1prdw;
    double FEFET1pprwg;
    double FEFET1pprwb;
    double FEFET1pprt;       
    double FEFET1peta0;         
    double FEFET1petab;         
    double FEFET1ppclm;      
    double FEFET1ppdibl1;      
    double FEFET1ppdibl2;      
    double FEFET1ppdiblb;
    double FEFET1pfprout;
    double FEFET1ppdits;
    double FEFET1ppditsd;
    double FEFET1ppscbe1;       
    double FEFET1ppscbe2;       
    double FEFET1ppvag;       
    double FEFET1pwr;
    double FEFET1pdwg;
    double FEFET1pdwb;
    double FEFET1pb0;
    double FEFET1pb1;
    double FEFET1palpha0;
    double FEFET1palpha1;
    double FEFET1pbeta0;
    double FEFET1pvfb;
    double FEFET1pagidl;
    double FEFET1pbgidl;
    double FEFET1pcgidl;
    double FEFET1pegidl;
    double FEFET1paigc;
    double FEFET1pbigc;
    double FEFET1pcigc;
    double FEFET1paigsd;
    double FEFET1pbigsd;
    double FEFET1pcigsd;
    double FEFET1paigbacc;
    double FEFET1pbigbacc;
    double FEFET1pcigbacc;
    double FEFET1paigbinv;
    double FEFET1pbigbinv;
    double FEFET1pcigbinv;
    double FEFET1pnigc;
    double FEFET1pnigbacc;
    double FEFET1pnigbinv;
    double FEFET1pntox;
    double FEFET1peigbinv;
    double FEFET1ppigcd;
    double FEFET1ppoxedge;
    double FEFET1pxrcrg1;
    double FEFET1pxrcrg2;
    double FEFET1plambda;
    double FEFET1pvtl;
    double FEFET1pxn; 
    double FEFET1pvfbsdoff;  
    double FEFET1ptvfbsdoff;  

    /* CV model */
    double FEFET1pcgsl;
    double FEFET1pcgdl;
    double FEFET1pckappas;
    double FEFET1pckappad;
    double FEFET1pcf;
    double FEFET1pclc;
    double FEFET1pcle;
    double FEFET1pvfbcv;
    double FEFET1pnoff;
    double FEFET1pvoffcv;
    double FEFET1pacde;
    double FEFET1pmoin;

    double FEFET1tnom;
    double FEFET1cgso;
    double FEFET1cgdo;
    double FEFET1cgbo;
    double FEFET1xpart;
    double FEFET1cFringOut;
    double FEFET1cFringMax;

    double FEFET1sheetResistance;
    double FEFET1SjctSatCurDensity;
    double FEFET1DjctSatCurDensity;
    double FEFET1SjctSidewallSatCurDensity;
    double FEFET1DjctSidewallSatCurDensity;
    double FEFET1SjctGateSidewallSatCurDensity;
    double FEFET1DjctGateSidewallSatCurDensity;
    double FEFET1SbulkJctPotential;
    double FEFET1DbulkJctPotential;
    double FEFET1SbulkJctBotGradingCoeff;
    double FEFET1DbulkJctBotGradingCoeff;
    double FEFET1SbulkJctSideGradingCoeff;
    double FEFET1DbulkJctSideGradingCoeff;
    double FEFET1SbulkJctGateSideGradingCoeff;
    double FEFET1DbulkJctGateSideGradingCoeff;
    double FEFET1SsidewallJctPotential;
    double FEFET1DsidewallJctPotential;
    double FEFET1SGatesidewallJctPotential;
    double FEFET1DGatesidewallJctPotential;
    double FEFET1SunitAreaJctCap;
    double FEFET1DunitAreaJctCap;
    double FEFET1SunitLengthSidewallJctCap;
    double FEFET1DunitLengthSidewallJctCap;
    double FEFET1SunitLengthGateSidewallJctCap;
    double FEFET1DunitLengthGateSidewallJctCap;
    double FEFET1SjctEmissionCoeff;
    double FEFET1DjctEmissionCoeff;
    double FEFET1SjctTempExponent;
    double FEFET1DjctTempExponent;
    double FEFET1njtstemp;
    double FEFET1njtsswtemp;
    double FEFET1njtsswgtemp;

    double FEFET1Lint;
    double FEFET1Ll;
    double FEFET1Llc;
    double FEFET1Lln;
    double FEFET1Lw;
    double FEFET1Lwc;
    double FEFET1Lwn;
    double FEFET1Lwl;
    double FEFET1Lwlc;
    double FEFET1Lmin;
    double FEFET1Lmax;

    double FEFET1Wint;
    double FEFET1Wl;
    double FEFET1Wlc;
    double FEFET1Wln;
    double FEFET1Ww;
    double FEFET1Wwc;
    double FEFET1Wwn;
    double FEFET1Wwl;
    double FEFET1Wwlc;
    double FEFET1Wmin;
    double FEFET1Wmax;

    /* added for stress effect */
    double FEFET1saref;
    double FEFET1sbref;
    double FEFET1wlod;
    double FEFET1ku0;
    double FEFET1kvsat;
    double FEFET1kvth0;
    double FEFET1tku0;
    double FEFET1llodku0;
    double FEFET1wlodku0;
    double FEFET1llodvth;
    double FEFET1wlodvth;
    double FEFET1lku0;
    double FEFET1wku0;
    double FEFET1pku0;
    double FEFET1lkvth0;
    double FEFET1wkvth0;
    double FEFET1pkvth0;
    double FEFET1stk2;
    double FEFET1lodk2;
    double FEFET1steta0;
    double FEFET1lodeta0;

    double FEFET1web; 
    double FEFET1wec;
    double FEFET1kvth0we; 
    double FEFET1k2we; 
    double FEFET1ku0we; 
    double FEFET1scref; 
    double FEFET1wpemod; 
    double FEFET1lkvth0we;
    double FEFET1lk2we;
    double FEFET1lku0we;
    double FEFET1wkvth0we;
    double FEFET1wk2we;
    double FEFET1wku0we;
    double FEFET1pkvth0we;
    double FEFET1pk2we;
    double FEFET1pku0we;

/* Pre-calculated constants
 * move to size-dependent param */
    double FEFET1vtm;   
    double FEFET1vtm0;   
    double FEFET1coxe;
    double FEFET1coxp;
    double FEFET1cof1;
    double FEFET1cof2;
    double FEFET1cof3;
    double FEFET1cof4;
    double FEFET1vcrit;
    double FEFET1factor1;
    double FEFET1PhiBS;
    double FEFET1PhiBSWS;
    double FEFET1PhiBSWGS;
    double FEFET1SjctTempSatCurDensity;
    double FEFET1SjctSidewallTempSatCurDensity;
    double FEFET1SjctGateSidewallTempSatCurDensity;
    double FEFET1PhiBD;
    double FEFET1PhiBSWD;
    double FEFET1PhiBSWGD;
    double FEFET1DjctTempSatCurDensity;
    double FEFET1DjctSidewallTempSatCurDensity;
    double FEFET1DjctGateSidewallTempSatCurDensity;
    double FEFET1SunitAreaTempJctCap;
    double FEFET1DunitAreaTempJctCap;
    double FEFET1SunitLengthSidewallTempJctCap;
    double FEFET1DunitLengthSidewallTempJctCap;
    double FEFET1SunitLengthGateSidewallTempJctCap;
    double FEFET1DunitLengthGateSidewallTempJctCap;

    double FEFET1oxideTrapDensityA;      
    double FEFET1oxideTrapDensityB;     
    double FEFET1oxideTrapDensityC;  
    double FEFET1em;  
    double FEFET1ef;  
    double FEFET1af;  
    double FEFET1kf;  

    double FEFET1vgsMax;
    double FEFET1vgdMax;
    double FEFET1vgbMax;
    double FEFET1vdsMax;
    double FEFET1vbsMax;
    double FEFET1vbdMax;
    double FEFET1vgsrMax;
    double FEFET1vgdrMax;
    double FEFET1vgbrMax;
    double FEFET1vbsrMax;
    double FEFET1vbdrMax;

    struct fefet1SizeDependParam *pSizeDependParamKnot;

#ifdef USE_OMP
    int FEFET1InstCount;
    struct sFEFET1instance **FEFET1InstanceArray;
#endif

    /* Flags */
    unsigned FEFET1rgeomodGiven :1;
    unsigned FEFET1stimodGiven :1;
    unsigned FEFET1sa0Given :1;
    unsigned FEFET1sb0Given :1;

    unsigned  FEFET1mobModGiven :1;
    unsigned  FEFET1binUnitGiven :1;
    unsigned  FEFET1capModGiven :1;
    unsigned  FEFET1dioModGiven :1;
    unsigned  FEFET1rdsModGiven :1;
    unsigned  FEFET1rbodyModGiven :1;
    unsigned  FEFET1rgateModGiven :1;
    unsigned  FEFET1perModGiven :1;
    unsigned  FEFET1geoModGiven :1;
    unsigned  FEFET1rgeoModGiven :1;
    unsigned  FEFET1paramChkGiven :1;
    unsigned  FEFET1trnqsModGiven :1;
    unsigned  FEFET1acnqsModGiven :1;
    unsigned  FEFET1fnoiModGiven :1;
    unsigned  FEFET1tnoiModGiven :1;
    unsigned  FEFET1igcModGiven :1;
    unsigned  FEFET1igbModGiven :1;
    unsigned  FEFET1tempModGiven :1;
    unsigned  FEFET1typeGiven   :1;
    unsigned  FEFET1toxrefGiven   :1;
    unsigned  FEFET1toxeGiven   :1;
    unsigned  FEFET1toxpGiven   :1;
    unsigned  FEFET1toxmGiven   :1;
    unsigned  FEFET1dtoxGiven   :1;
    unsigned  FEFET1epsroxGiven   :1;
    unsigned  FEFET1versionGiven   :1;
    unsigned  FEFET1cdscGiven   :1;
    unsigned  FEFET1cdscbGiven   :1;
    unsigned  FEFET1cdscdGiven   :1;
    unsigned  FEFET1citGiven   :1;
    unsigned  FEFET1nfactorGiven   :1;
    unsigned  FEFET1xjGiven   :1;
    unsigned  FEFET1vsatGiven   :1;
    unsigned  FEFET1atGiven   :1;
    unsigned  FEFET1a0Given   :1;
    unsigned  FEFET1agsGiven   :1;
    unsigned  FEFET1a1Given   :1;
    unsigned  FEFET1a2Given   :1;
    unsigned  FEFET1ketaGiven   :1;    
    unsigned  FEFET1nsubGiven   :1;
    unsigned  FEFET1ndepGiven   :1;
    unsigned  FEFET1nsdGiven    :1;
    unsigned  FEFET1phinGiven   :1;
    unsigned  FEFET1ngateGiven   :1;
    unsigned  FEFET1gamma1Given   :1;
    unsigned  FEFET1gamma2Given   :1;
    unsigned  FEFET1vbxGiven   :1;
    unsigned  FEFET1vbmGiven   :1;
    unsigned  FEFET1xtGiven   :1;
    unsigned  FEFET1k1Given   :1;
    unsigned  FEFET1kt1Given   :1;
    unsigned  FEFET1kt1lGiven   :1;
    unsigned  FEFET1kt2Given   :1;
    unsigned  FEFET1k2Given   :1;
    unsigned  FEFET1k3Given   :1;
    unsigned  FEFET1k3bGiven   :1;
    unsigned  FEFET1w0Given   :1;
    unsigned  FEFET1dvtp0Given :1;
    unsigned  FEFET1dvtp1Given :1;
    unsigned  FEFET1lpe0Given   :1;
    unsigned  FEFET1lpebGiven   :1;
    unsigned  FEFET1dvt0Given   :1;   
    unsigned  FEFET1dvt1Given   :1;     
    unsigned  FEFET1dvt2Given   :1;     
    unsigned  FEFET1dvt0wGiven   :1;   
    unsigned  FEFET1dvt1wGiven   :1;     
    unsigned  FEFET1dvt2wGiven   :1;     
    unsigned  FEFET1droutGiven   :1;     
    unsigned  FEFET1dsubGiven   :1;     
    unsigned  FEFET1vth0Given   :1;
    unsigned  FEFET1euGiven   :1;
    unsigned  FEFET1uaGiven   :1;
    unsigned  FEFET1ua1Given   :1;
    unsigned  FEFET1ubGiven   :1;
    unsigned  FEFET1ub1Given   :1;
    unsigned  FEFET1ucGiven   :1;
    unsigned  FEFET1uc1Given   :1;
    unsigned  FEFET1udGiven     :1;
    unsigned  FEFET1ud1Given     :1;
    unsigned  FEFET1upGiven     :1;
    unsigned  FEFET1lpGiven     :1;
    unsigned  FEFET1u0Given   :1;
    unsigned  FEFET1uteGiven   :1;
    unsigned  FEFET1voffGiven   :1;
    unsigned  FEFET1tvoffGiven   :1;
    unsigned  FEFET1vofflGiven  :1;
    unsigned  FEFET1minvGiven   :1;
    unsigned  FEFET1rdswGiven   :1;      
    unsigned  FEFET1rdswminGiven :1;
    unsigned  FEFET1rdwminGiven :1;
    unsigned  FEFET1rswminGiven :1;
    unsigned  FEFET1rswGiven   :1;
    unsigned  FEFET1rdwGiven   :1;
    unsigned  FEFET1prwgGiven   :1;      
    unsigned  FEFET1prwbGiven   :1;      
    unsigned  FEFET1prtGiven   :1;      
    unsigned  FEFET1eta0Given   :1;    
    unsigned  FEFET1etabGiven   :1;    
    unsigned  FEFET1pclmGiven   :1;   
    unsigned  FEFET1pdibl1Given   :1;   
    unsigned  FEFET1pdibl2Given   :1;  
    unsigned  FEFET1pdiblbGiven   :1;  
    unsigned  FEFET1fproutGiven   :1;
    unsigned  FEFET1pditsGiven    :1;
    unsigned  FEFET1pditsdGiven    :1;
    unsigned  FEFET1pditslGiven    :1;
    unsigned  FEFET1pscbe1Given   :1;    
    unsigned  FEFET1pscbe2Given   :1;    
    unsigned  FEFET1pvagGiven   :1;    
    unsigned  FEFET1deltaGiven  :1;     
    unsigned  FEFET1wrGiven   :1;
    unsigned  FEFET1dwgGiven   :1;
    unsigned  FEFET1dwbGiven   :1;
    unsigned  FEFET1b0Given   :1;
    unsigned  FEFET1b1Given   :1;
    unsigned  FEFET1alpha0Given   :1;
    unsigned  FEFET1alpha1Given   :1;
    unsigned  FEFET1beta0Given   :1;
    unsigned  FEFET1agidlGiven   :1;
    unsigned  FEFET1bgidlGiven   :1;
    unsigned  FEFET1cgidlGiven   :1;
    unsigned  FEFET1egidlGiven   :1;
    unsigned  FEFET1aigcGiven   :1;
    unsigned  FEFET1bigcGiven   :1;
    unsigned  FEFET1cigcGiven   :1;
    unsigned  FEFET1aigsdGiven   :1;
    unsigned  FEFET1bigsdGiven   :1;
    unsigned  FEFET1cigsdGiven   :1;
    unsigned  FEFET1aigbaccGiven   :1;
    unsigned  FEFET1bigbaccGiven   :1;
    unsigned  FEFET1cigbaccGiven   :1;
    unsigned  FEFET1aigbinvGiven   :1;
    unsigned  FEFET1bigbinvGiven   :1;
    unsigned  FEFET1cigbinvGiven   :1;
    unsigned  FEFET1nigcGiven   :1;
    unsigned  FEFET1nigbinvGiven   :1;
    unsigned  FEFET1nigbaccGiven   :1;
    unsigned  FEFET1ntoxGiven   :1;
    unsigned  FEFET1eigbinvGiven   :1;
    unsigned  FEFET1pigcdGiven   :1;
    unsigned  FEFET1poxedgeGiven   :1;
    unsigned  FEFET1ijthdfwdGiven  :1;
    unsigned  FEFET1ijthsfwdGiven  :1;
    unsigned  FEFET1ijthdrevGiven  :1;
    unsigned  FEFET1ijthsrevGiven  :1;
    unsigned  FEFET1xjbvdGiven   :1;
    unsigned  FEFET1xjbvsGiven   :1;
    unsigned  FEFET1bvdGiven   :1;
    unsigned  FEFET1bvsGiven   :1;

    unsigned  FEFET1jtssGiven   :1;
    unsigned  FEFET1jtsdGiven   :1;
    unsigned  FEFET1jtsswsGiven   :1;
    unsigned  FEFET1jtsswdGiven   :1;
    unsigned  FEFET1jtsswgsGiven   :1;
    unsigned  FEFET1jtsswgdGiven   :1;
    unsigned  FEFET1njtsGiven   :1;
    unsigned  FEFET1njtsswGiven   :1;
    unsigned  FEFET1njtsswgGiven   :1;
    unsigned  FEFET1xtssGiven   :1;
    unsigned  FEFET1xtsdGiven   :1;
    unsigned  FEFET1xtsswsGiven   :1;
    unsigned  FEFET1xtsswdGiven   :1;
    unsigned  FEFET1xtsswgsGiven   :1;
    unsigned  FEFET1xtsswgdGiven   :1;
    unsigned  FEFET1tnjtsGiven   :1;
    unsigned  FEFET1tnjtsswGiven   :1;
    unsigned  FEFET1tnjtsswgGiven   :1;
    unsigned  FEFET1vtssGiven   :1;
    unsigned  FEFET1vtsdGiven   :1;
    unsigned  FEFET1vtsswsGiven   :1;
    unsigned  FEFET1vtsswdGiven   :1;
    unsigned  FEFET1vtsswgsGiven   :1;
    unsigned  FEFET1vtsswgdGiven   :1;

    unsigned  FEFET1vfbGiven   :1;
    unsigned  FEFET1gbminGiven :1;
    unsigned  FEFET1rbdbGiven :1;
    unsigned  FEFET1rbsbGiven :1;
    unsigned  FEFET1rbpsGiven :1;
    unsigned  FEFET1rbpdGiven :1;
    unsigned  FEFET1rbpbGiven :1;

    unsigned FEFET1rbps0Given :1;
    unsigned FEFET1rbpslGiven :1;
    unsigned FEFET1rbpswGiven :1;
    unsigned FEFET1rbpsnfGiven :1;

    unsigned FEFET1rbpd0Given :1;
    unsigned FEFET1rbpdlGiven :1;
    unsigned FEFET1rbpdwGiven :1;
    unsigned FEFET1rbpdnfGiven :1;

    unsigned FEFET1rbpbx0Given :1;
    unsigned FEFET1rbpbxlGiven :1;
    unsigned FEFET1rbpbxwGiven :1;
    unsigned FEFET1rbpbxnfGiven :1;
    unsigned FEFET1rbpby0Given :1;
    unsigned FEFET1rbpbylGiven :1;
    unsigned FEFET1rbpbywGiven :1;
    unsigned FEFET1rbpbynfGiven :1;

    unsigned FEFET1rbsbx0Given :1;
    unsigned FEFET1rbsby0Given :1;
    unsigned FEFET1rbdbx0Given :1;
    unsigned FEFET1rbdby0Given :1;

    unsigned FEFET1rbsdbxlGiven :1;
    unsigned FEFET1rbsdbxwGiven :1;
    unsigned FEFET1rbsdbxnfGiven :1;
    unsigned FEFET1rbsdbylGiven :1;
    unsigned FEFET1rbsdbywGiven :1;
    unsigned FEFET1rbsdbynfGiven :1;

    unsigned  FEFET1xrcrg1Given   :1;
    unsigned  FEFET1xrcrg2Given   :1;
    unsigned  FEFET1tnoiaGiven    :1;
    unsigned  FEFET1tnoibGiven    :1;
    unsigned  FEFET1rnoiaGiven    :1;
    unsigned  FEFET1rnoibGiven    :1;
    unsigned  FEFET1ntnoiGiven    :1;

    unsigned  FEFET1lambdaGiven    :1;
    unsigned  FEFET1vtlGiven    :1;
    unsigned  FEFET1lcGiven    :1;
    unsigned  FEFET1xnGiven    :1;
    unsigned  FEFET1vfbsdoffGiven    :1;
    unsigned  FEFET1lintnoiGiven    :1;
    unsigned  FEFET1tvfbsdoffGiven    :1;

    /* CV model and parasitics */
    unsigned  FEFET1cgslGiven   :1;
    unsigned  FEFET1cgdlGiven   :1;
    unsigned  FEFET1ckappasGiven   :1;
    unsigned  FEFET1ckappadGiven   :1;
    unsigned  FEFET1cfGiven   :1;
    unsigned  FEFET1vfbcvGiven   :1;
    unsigned  FEFET1clcGiven   :1;
    unsigned  FEFET1cleGiven   :1;
    unsigned  FEFET1dwcGiven   :1;
    unsigned  FEFET1dlcGiven   :1;
    unsigned  FEFET1xwGiven    :1;
    unsigned  FEFET1xlGiven    :1;
    unsigned  FEFET1dlcigGiven   :1;
    unsigned  FEFET1dwjGiven   :1;
    unsigned  FEFET1noffGiven  :1;
    unsigned  FEFET1voffcvGiven :1;
    unsigned  FEFET1acdeGiven  :1;
    unsigned  FEFET1moinGiven  :1;
    unsigned  FEFET1tcjGiven   :1;
    unsigned  FEFET1tcjswGiven :1;
    unsigned  FEFET1tcjswgGiven :1;
    unsigned  FEFET1tpbGiven    :1;
    unsigned  FEFET1tpbswGiven  :1;
    unsigned  FEFET1tpbswgGiven :1;
    unsigned  FEFET1dmcgGiven :1;
    unsigned  FEFET1dmciGiven :1;
    unsigned  FEFET1dmdgGiven :1;
    unsigned  FEFET1dmcgtGiven :1;
    unsigned  FEFET1xgwGiven :1;
    unsigned  FEFET1xglGiven :1;
    unsigned  FEFET1rshgGiven :1;
    unsigned  FEFET1ngconGiven :1;


    /* Length dependence */
    unsigned  FEFET1lcdscGiven   :1;
    unsigned  FEFET1lcdscbGiven   :1;
    unsigned  FEFET1lcdscdGiven   :1;
    unsigned  FEFET1lcitGiven   :1;
    unsigned  FEFET1lnfactorGiven   :1;
    unsigned  FEFET1lxjGiven   :1;
    unsigned  FEFET1lvsatGiven   :1;
    unsigned  FEFET1latGiven   :1;
    unsigned  FEFET1la0Given   :1;
    unsigned  FEFET1lagsGiven   :1;
    unsigned  FEFET1la1Given   :1;
    unsigned  FEFET1la2Given   :1;
    unsigned  FEFET1lketaGiven   :1;    
    unsigned  FEFET1lnsubGiven   :1;
    unsigned  FEFET1lndepGiven   :1;
    unsigned  FEFET1lnsdGiven    :1;
    unsigned  FEFET1lphinGiven   :1;
    unsigned  FEFET1lngateGiven   :1;
    unsigned  FEFET1lgamma1Given   :1;
    unsigned  FEFET1lgamma2Given   :1;
    unsigned  FEFET1lvbxGiven   :1;
    unsigned  FEFET1lvbmGiven   :1;
    unsigned  FEFET1lxtGiven   :1;
    unsigned  FEFET1lk1Given   :1;
    unsigned  FEFET1lkt1Given   :1;
    unsigned  FEFET1lkt1lGiven   :1;
    unsigned  FEFET1lkt2Given   :1;
    unsigned  FEFET1lk2Given   :1;
    unsigned  FEFET1lk3Given   :1;
    unsigned  FEFET1lk3bGiven   :1;
    unsigned  FEFET1lw0Given   :1;
    unsigned  FEFET1ldvtp0Given :1;
    unsigned  FEFET1ldvtp1Given :1;
    unsigned  FEFET1llpe0Given   :1;
    unsigned  FEFET1llpebGiven   :1;
    unsigned  FEFET1ldvt0Given   :1;   
    unsigned  FEFET1ldvt1Given   :1;     
    unsigned  FEFET1ldvt2Given   :1;     
    unsigned  FEFET1ldvt0wGiven   :1;   
    unsigned  FEFET1ldvt1wGiven   :1;     
    unsigned  FEFET1ldvt2wGiven   :1;     
    unsigned  FEFET1ldroutGiven   :1;     
    unsigned  FEFET1ldsubGiven   :1;     
    unsigned  FEFET1lvth0Given   :1;
    unsigned  FEFET1luaGiven   :1;
    unsigned  FEFET1lua1Given   :1;
    unsigned  FEFET1lubGiven   :1;
    unsigned  FEFET1lub1Given   :1;
    unsigned  FEFET1lucGiven   :1;
    unsigned  FEFET1luc1Given   :1;
    unsigned  FEFET1ludGiven     :1;
    unsigned  FEFET1lud1Given     :1;
    unsigned  FEFET1lupGiven     :1;
    unsigned  FEFET1llpGiven     :1;
    unsigned  FEFET1lu0Given   :1;
    unsigned  FEFET1leuGiven   :1;
    unsigned  FEFET1luteGiven   :1;
    unsigned  FEFET1lvoffGiven   :1;
    unsigned  FEFET1ltvoffGiven   :1;
    unsigned  FEFET1lminvGiven   :1;
    unsigned  FEFET1lrdswGiven   :1;      
    unsigned  FEFET1lrswGiven   :1;
    unsigned  FEFET1lrdwGiven   :1;
    unsigned  FEFET1lprwgGiven   :1;      
    unsigned  FEFET1lprwbGiven   :1;      
    unsigned  FEFET1lprtGiven   :1;      
    unsigned  FEFET1leta0Given   :1;    
    unsigned  FEFET1letabGiven   :1;    
    unsigned  FEFET1lpclmGiven   :1;   
    unsigned  FEFET1lpdibl1Given   :1;   
    unsigned  FEFET1lpdibl2Given   :1;  
    unsigned  FEFET1lpdiblbGiven   :1;  
    unsigned  FEFET1lfproutGiven   :1;
    unsigned  FEFET1lpditsGiven    :1;
    unsigned  FEFET1lpditsdGiven    :1;
    unsigned  FEFET1lpscbe1Given   :1;    
    unsigned  FEFET1lpscbe2Given   :1;    
    unsigned  FEFET1lpvagGiven   :1;    
    unsigned  FEFET1ldeltaGiven  :1;     
    unsigned  FEFET1lwrGiven   :1;
    unsigned  FEFET1ldwgGiven   :1;
    unsigned  FEFET1ldwbGiven   :1;
    unsigned  FEFET1lb0Given   :1;
    unsigned  FEFET1lb1Given   :1;
    unsigned  FEFET1lalpha0Given   :1;
    unsigned  FEFET1lalpha1Given   :1;
    unsigned  FEFET1lbeta0Given   :1;
    unsigned  FEFET1lvfbGiven   :1;
    unsigned  FEFET1lagidlGiven   :1;
    unsigned  FEFET1lbgidlGiven   :1;
    unsigned  FEFET1lcgidlGiven   :1;
    unsigned  FEFET1legidlGiven   :1;
    unsigned  FEFET1laigcGiven   :1;
    unsigned  FEFET1lbigcGiven   :1;
    unsigned  FEFET1lcigcGiven   :1;
    unsigned  FEFET1laigsdGiven   :1;
    unsigned  FEFET1lbigsdGiven   :1;
    unsigned  FEFET1lcigsdGiven   :1;
    unsigned  FEFET1laigbaccGiven   :1;
    unsigned  FEFET1lbigbaccGiven   :1;
    unsigned  FEFET1lcigbaccGiven   :1;
    unsigned  FEFET1laigbinvGiven   :1;
    unsigned  FEFET1lbigbinvGiven   :1;
    unsigned  FEFET1lcigbinvGiven   :1;
    unsigned  FEFET1lnigcGiven   :1;
    unsigned  FEFET1lnigbinvGiven   :1;
    unsigned  FEFET1lnigbaccGiven   :1;
    unsigned  FEFET1lntoxGiven   :1;
    unsigned  FEFET1leigbinvGiven   :1;
    unsigned  FEFET1lpigcdGiven   :1;
    unsigned  FEFET1lpoxedgeGiven   :1;
    unsigned  FEFET1lxrcrg1Given   :1;
    unsigned  FEFET1lxrcrg2Given   :1;
    unsigned  FEFET1llambdaGiven    :1;
    unsigned  FEFET1lvtlGiven    :1;
    unsigned  FEFET1lxnGiven    :1;
    unsigned  FEFET1lvfbsdoffGiven    :1;
    unsigned  FEFET1ltvfbsdoffGiven    :1;

    /* CV model */
    unsigned  FEFET1lcgslGiven   :1;
    unsigned  FEFET1lcgdlGiven   :1;
    unsigned  FEFET1lckappasGiven   :1;
    unsigned  FEFET1lckappadGiven   :1;
    unsigned  FEFET1lcfGiven   :1;
    unsigned  FEFET1lclcGiven   :1;
    unsigned  FEFET1lcleGiven   :1;
    unsigned  FEFET1lvfbcvGiven   :1;
    unsigned  FEFET1lnoffGiven   :1;
    unsigned  FEFET1lvoffcvGiven :1;
    unsigned  FEFET1lacdeGiven   :1;
    unsigned  FEFET1lmoinGiven   :1;

    /* Width dependence */
    unsigned  FEFET1wcdscGiven   :1;
    unsigned  FEFET1wcdscbGiven   :1;
    unsigned  FEFET1wcdscdGiven   :1;
    unsigned  FEFET1wcitGiven   :1;
    unsigned  FEFET1wnfactorGiven   :1;
    unsigned  FEFET1wxjGiven   :1;
    unsigned  FEFET1wvsatGiven   :1;
    unsigned  FEFET1watGiven   :1;
    unsigned  FEFET1wa0Given   :1;
    unsigned  FEFET1wagsGiven   :1;
    unsigned  FEFET1wa1Given   :1;
    unsigned  FEFET1wa2Given   :1;
    unsigned  FEFET1wketaGiven   :1;    
    unsigned  FEFET1wnsubGiven   :1;
    unsigned  FEFET1wndepGiven   :1;
    unsigned  FEFET1wnsdGiven    :1;
    unsigned  FEFET1wphinGiven   :1;
    unsigned  FEFET1wngateGiven   :1;
    unsigned  FEFET1wgamma1Given   :1;
    unsigned  FEFET1wgamma2Given   :1;
    unsigned  FEFET1wvbxGiven   :1;
    unsigned  FEFET1wvbmGiven   :1;
    unsigned  FEFET1wxtGiven   :1;
    unsigned  FEFET1wk1Given   :1;
    unsigned  FEFET1wkt1Given   :1;
    unsigned  FEFET1wkt1lGiven   :1;
    unsigned  FEFET1wkt2Given   :1;
    unsigned  FEFET1wk2Given   :1;
    unsigned  FEFET1wk3Given   :1;
    unsigned  FEFET1wk3bGiven   :1;
    unsigned  FEFET1ww0Given   :1;
    unsigned  FEFET1wdvtp0Given :1;
    unsigned  FEFET1wdvtp1Given :1;
    unsigned  FEFET1wlpe0Given   :1;
    unsigned  FEFET1wlpebGiven   :1;
    unsigned  FEFET1wdvt0Given   :1;   
    unsigned  FEFET1wdvt1Given   :1;     
    unsigned  FEFET1wdvt2Given   :1;     
    unsigned  FEFET1wdvt0wGiven   :1;   
    unsigned  FEFET1wdvt1wGiven   :1;     
    unsigned  FEFET1wdvt2wGiven   :1;     
    unsigned  FEFET1wdroutGiven   :1;     
    unsigned  FEFET1wdsubGiven   :1;     
    unsigned  FEFET1wvth0Given   :1;
    unsigned  FEFET1wuaGiven   :1;
    unsigned  FEFET1wua1Given   :1;
    unsigned  FEFET1wubGiven   :1;
    unsigned  FEFET1wub1Given   :1;
    unsigned  FEFET1wucGiven   :1;
    unsigned  FEFET1wuc1Given   :1;
    unsigned  FEFET1wudGiven     :1;
    unsigned  FEFET1wud1Given     :1;
    unsigned  FEFET1wupGiven     :1;
    unsigned  FEFET1wlpGiven     :1;
    unsigned  FEFET1wu0Given   :1;
    unsigned  FEFET1weuGiven   :1;
    unsigned  FEFET1wuteGiven   :1;
    unsigned  FEFET1wvoffGiven   :1;
    unsigned  FEFET1wtvoffGiven   :1;
    unsigned  FEFET1wminvGiven   :1;
    unsigned  FEFET1wrdswGiven   :1;      
    unsigned  FEFET1wrswGiven   :1;
    unsigned  FEFET1wrdwGiven   :1;
    unsigned  FEFET1wprwgGiven   :1;      
    unsigned  FEFET1wprwbGiven   :1;      
    unsigned  FEFET1wprtGiven   :1;      
    unsigned  FEFET1weta0Given   :1;    
    unsigned  FEFET1wetabGiven   :1;    
    unsigned  FEFET1wpclmGiven   :1;   
    unsigned  FEFET1wpdibl1Given   :1;   
    unsigned  FEFET1wpdibl2Given   :1;  
    unsigned  FEFET1wpdiblbGiven   :1;  
    unsigned  FEFET1wfproutGiven   :1;
    unsigned  FEFET1wpditsGiven    :1;
    unsigned  FEFET1wpditsdGiven    :1;
    unsigned  FEFET1wpscbe1Given   :1;    
    unsigned  FEFET1wpscbe2Given   :1;    
    unsigned  FEFET1wpvagGiven   :1;    
    unsigned  FEFET1wdeltaGiven  :1;     
    unsigned  FEFET1wwrGiven   :1;
    unsigned  FEFET1wdwgGiven   :1;
    unsigned  FEFET1wdwbGiven   :1;
    unsigned  FEFET1wb0Given   :1;
    unsigned  FEFET1wb1Given   :1;
    unsigned  FEFET1walpha0Given   :1;
    unsigned  FEFET1walpha1Given   :1;
    unsigned  FEFET1wbeta0Given   :1;
    unsigned  FEFET1wvfbGiven   :1;
    unsigned  FEFET1wagidlGiven   :1;
    unsigned  FEFET1wbgidlGiven   :1;
    unsigned  FEFET1wcgidlGiven   :1;
    unsigned  FEFET1wegidlGiven   :1;
    unsigned  FEFET1waigcGiven   :1;
    unsigned  FEFET1wbigcGiven   :1;
    unsigned  FEFET1wcigcGiven   :1;
    unsigned  FEFET1waigsdGiven   :1;
    unsigned  FEFET1wbigsdGiven   :1;
    unsigned  FEFET1wcigsdGiven   :1;
    unsigned  FEFET1waigbaccGiven   :1;
    unsigned  FEFET1wbigbaccGiven   :1;
    unsigned  FEFET1wcigbaccGiven   :1;
    unsigned  FEFET1waigbinvGiven   :1;
    unsigned  FEFET1wbigbinvGiven   :1;
    unsigned  FEFET1wcigbinvGiven   :1;
    unsigned  FEFET1wnigcGiven   :1;
    unsigned  FEFET1wnigbinvGiven   :1;
    unsigned  FEFET1wnigbaccGiven   :1;
    unsigned  FEFET1wntoxGiven   :1;
    unsigned  FEFET1weigbinvGiven   :1;
    unsigned  FEFET1wpigcdGiven   :1;
    unsigned  FEFET1wpoxedgeGiven   :1;
    unsigned  FEFET1wxrcrg1Given   :1;
    unsigned  FEFET1wxrcrg2Given   :1;
    unsigned  FEFET1wlambdaGiven    :1;
    unsigned  FEFET1wvtlGiven    :1;
    unsigned  FEFET1wxnGiven    :1;
    unsigned  FEFET1wvfbsdoffGiven    :1;
    unsigned  FEFET1wtvfbsdoffGiven    :1;

    /* CV model */
    unsigned  FEFET1wcgslGiven   :1;
    unsigned  FEFET1wcgdlGiven   :1;
    unsigned  FEFET1wckappasGiven   :1;
    unsigned  FEFET1wckappadGiven   :1;
    unsigned  FEFET1wcfGiven   :1;
    unsigned  FEFET1wclcGiven   :1;
    unsigned  FEFET1wcleGiven   :1;
    unsigned  FEFET1wvfbcvGiven   :1;
    unsigned  FEFET1wnoffGiven   :1;
    unsigned  FEFET1wvoffcvGiven :1;
    unsigned  FEFET1wacdeGiven   :1;
    unsigned  FEFET1wmoinGiven   :1;

    /* Cross-term dependence */
    unsigned  FEFET1pcdscGiven   :1;
    unsigned  FEFET1pcdscbGiven   :1;
    unsigned  FEFET1pcdscdGiven   :1;
    unsigned  FEFET1pcitGiven   :1;
    unsigned  FEFET1pnfactorGiven   :1;
    unsigned  FEFET1pxjGiven   :1;
    unsigned  FEFET1pvsatGiven   :1;
    unsigned  FEFET1patGiven   :1;
    unsigned  FEFET1pa0Given   :1;
    unsigned  FEFET1pagsGiven   :1;
    unsigned  FEFET1pa1Given   :1;
    unsigned  FEFET1pa2Given   :1;
    unsigned  FEFET1pketaGiven   :1;    
    unsigned  FEFET1pnsubGiven   :1;
    unsigned  FEFET1pndepGiven   :1;
    unsigned  FEFET1pnsdGiven    :1;
    unsigned  FEFET1pphinGiven   :1;
    unsigned  FEFET1pngateGiven   :1;
    unsigned  FEFET1pgamma1Given   :1;
    unsigned  FEFET1pgamma2Given   :1;
    unsigned  FEFET1pvbxGiven   :1;
    unsigned  FEFET1pvbmGiven   :1;
    unsigned  FEFET1pxtGiven   :1;
    unsigned  FEFET1pk1Given   :1;
    unsigned  FEFET1pkt1Given   :1;
    unsigned  FEFET1pkt1lGiven   :1;
    unsigned  FEFET1pkt2Given   :1;
    unsigned  FEFET1pk2Given   :1;
    unsigned  FEFET1pk3Given   :1;
    unsigned  FEFET1pk3bGiven   :1;
    unsigned  FEFET1pw0Given   :1;
    unsigned  FEFET1pdvtp0Given :1;
    unsigned  FEFET1pdvtp1Given :1;
    unsigned  FEFET1plpe0Given   :1;
    unsigned  FEFET1plpebGiven   :1;
    unsigned  FEFET1pdvt0Given   :1;   
    unsigned  FEFET1pdvt1Given   :1;     
    unsigned  FEFET1pdvt2Given   :1;     
    unsigned  FEFET1pdvt0wGiven   :1;   
    unsigned  FEFET1pdvt1wGiven   :1;     
    unsigned  FEFET1pdvt2wGiven   :1;     
    unsigned  FEFET1pdroutGiven   :1;     
    unsigned  FEFET1pdsubGiven   :1;     
    unsigned  FEFET1pvth0Given   :1;
    unsigned  FEFET1puaGiven   :1;
    unsigned  FEFET1pua1Given   :1;
    unsigned  FEFET1pubGiven   :1;
    unsigned  FEFET1pub1Given   :1;
    unsigned  FEFET1pucGiven   :1;
    unsigned  FEFET1puc1Given   :1;
    unsigned  FEFET1pudGiven     :1;
    unsigned  FEFET1pud1Given     :1;
    unsigned  FEFET1pupGiven     :1;
    unsigned  FEFET1plpGiven     :1;
    unsigned  FEFET1pu0Given   :1;
    unsigned  FEFET1peuGiven   :1;
    unsigned  FEFET1puteGiven   :1;
    unsigned  FEFET1pvoffGiven   :1;
    unsigned  FEFET1ptvoffGiven   :1;
    unsigned  FEFET1pminvGiven   :1;
    unsigned  FEFET1prdswGiven   :1;      
    unsigned  FEFET1prswGiven   :1;
    unsigned  FEFET1prdwGiven   :1;
    unsigned  FEFET1pprwgGiven   :1;      
    unsigned  FEFET1pprwbGiven   :1;      
    unsigned  FEFET1pprtGiven   :1;      
    unsigned  FEFET1peta0Given   :1;    
    unsigned  FEFET1petabGiven   :1;    
    unsigned  FEFET1ppclmGiven   :1;   
    unsigned  FEFET1ppdibl1Given   :1;   
    unsigned  FEFET1ppdibl2Given   :1;  
    unsigned  FEFET1ppdiblbGiven   :1;  
    unsigned  FEFET1pfproutGiven   :1;
    unsigned  FEFET1ppditsGiven    :1;
    unsigned  FEFET1ppditsdGiven    :1;
    unsigned  FEFET1ppscbe1Given   :1;    
    unsigned  FEFET1ppscbe2Given   :1;    
    unsigned  FEFET1ppvagGiven   :1;    
    unsigned  FEFET1pdeltaGiven  :1;     
    unsigned  FEFET1pwrGiven   :1;
    unsigned  FEFET1pdwgGiven   :1;
    unsigned  FEFET1pdwbGiven   :1;
    unsigned  FEFET1pb0Given   :1;
    unsigned  FEFET1pb1Given   :1;
    unsigned  FEFET1palpha0Given   :1;
    unsigned  FEFET1palpha1Given   :1;
    unsigned  FEFET1pbeta0Given   :1;
    unsigned  FEFET1pvfbGiven   :1;
    unsigned  FEFET1pagidlGiven   :1;
    unsigned  FEFET1pbgidlGiven   :1;
    unsigned  FEFET1pcgidlGiven   :1;
    unsigned  FEFET1pegidlGiven   :1;
    unsigned  FEFET1paigcGiven   :1;
    unsigned  FEFET1pbigcGiven   :1;
    unsigned  FEFET1pcigcGiven   :1;
    unsigned  FEFET1paigsdGiven   :1;
    unsigned  FEFET1pbigsdGiven   :1;
    unsigned  FEFET1pcigsdGiven   :1;
    unsigned  FEFET1paigbaccGiven   :1;
    unsigned  FEFET1pbigbaccGiven   :1;
    unsigned  FEFET1pcigbaccGiven   :1;
    unsigned  FEFET1paigbinvGiven   :1;
    unsigned  FEFET1pbigbinvGiven   :1;
    unsigned  FEFET1pcigbinvGiven   :1;
    unsigned  FEFET1pnigcGiven   :1;
    unsigned  FEFET1pnigbinvGiven   :1;
    unsigned  FEFET1pnigbaccGiven   :1;
    unsigned  FEFET1pntoxGiven   :1;
    unsigned  FEFET1peigbinvGiven   :1;
    unsigned  FEFET1ppigcdGiven   :1;
    unsigned  FEFET1ppoxedgeGiven   :1;
    unsigned  FEFET1pxrcrg1Given   :1;
    unsigned  FEFET1pxrcrg2Given   :1;
    unsigned  FEFET1plambdaGiven    :1;
    unsigned  FEFET1pvtlGiven    :1;
    unsigned  FEFET1pxnGiven    :1;
    unsigned  FEFET1pvfbsdoffGiven    :1;
    unsigned  FEFET1ptvfbsdoffGiven    :1;

    /* CV model */
    unsigned  FEFET1pcgslGiven   :1;
    unsigned  FEFET1pcgdlGiven   :1;
    unsigned  FEFET1pckappasGiven   :1;
    unsigned  FEFET1pckappadGiven   :1;
    unsigned  FEFET1pcfGiven   :1;
    unsigned  FEFET1pclcGiven   :1;
    unsigned  FEFET1pcleGiven   :1;
    unsigned  FEFET1pvfbcvGiven   :1;
    unsigned  FEFET1pnoffGiven   :1;
    unsigned  FEFET1pvoffcvGiven :1;
    unsigned  FEFET1pacdeGiven   :1;
    unsigned  FEFET1pmoinGiven   :1;

    unsigned  FEFET1useFringeGiven   :1;

    unsigned  FEFET1tnomGiven   :1;
    unsigned  FEFET1cgsoGiven   :1;
    unsigned  FEFET1cgdoGiven   :1;
    unsigned  FEFET1cgboGiven   :1;
    unsigned  FEFET1xpartGiven   :1;
    unsigned  FEFET1sheetResistanceGiven   :1;

    unsigned  FEFET1SjctSatCurDensityGiven   :1;
    unsigned  FEFET1SjctSidewallSatCurDensityGiven   :1;
    unsigned  FEFET1SjctGateSidewallSatCurDensityGiven   :1;
    unsigned  FEFET1SbulkJctPotentialGiven   :1;
    unsigned  FEFET1SbulkJctBotGradingCoeffGiven   :1;
    unsigned  FEFET1SsidewallJctPotentialGiven   :1;
    unsigned  FEFET1SGatesidewallJctPotentialGiven   :1;
    unsigned  FEFET1SbulkJctSideGradingCoeffGiven   :1;
    unsigned  FEFET1SunitAreaJctCapGiven   :1;
    unsigned  FEFET1SunitLengthSidewallJctCapGiven   :1;
    unsigned  FEFET1SbulkJctGateSideGradingCoeffGiven   :1;
    unsigned  FEFET1SunitLengthGateSidewallJctCapGiven   :1;
    unsigned  FEFET1SjctEmissionCoeffGiven :1;
    unsigned  FEFET1SjctTempExponentGiven	:1;

    unsigned  FEFET1DjctSatCurDensityGiven   :1;
    unsigned  FEFET1DjctSidewallSatCurDensityGiven   :1;
    unsigned  FEFET1DjctGateSidewallSatCurDensityGiven   :1;
    unsigned  FEFET1DbulkJctPotentialGiven   :1;
    unsigned  FEFET1DbulkJctBotGradingCoeffGiven   :1;
    unsigned  FEFET1DsidewallJctPotentialGiven   :1;
    unsigned  FEFET1DGatesidewallJctPotentialGiven   :1;
    unsigned  FEFET1DbulkJctSideGradingCoeffGiven   :1;
    unsigned  FEFET1DunitAreaJctCapGiven   :1;
    unsigned  FEFET1DunitLengthSidewallJctCapGiven   :1;
    unsigned  FEFET1DbulkJctGateSideGradingCoeffGiven   :1;
    unsigned  FEFET1DunitLengthGateSidewallJctCapGiven   :1;
    unsigned  FEFET1DjctEmissionCoeffGiven :1;
    unsigned  FEFET1DjctTempExponentGiven :1;

    unsigned  FEFET1oxideTrapDensityAGiven  :1;         
    unsigned  FEFET1oxideTrapDensityBGiven  :1;        
    unsigned  FEFET1oxideTrapDensityCGiven  :1;     
    unsigned  FEFET1emGiven  :1;     
    unsigned  FEFET1efGiven  :1;     
    unsigned  FEFET1afGiven  :1;     
    unsigned  FEFET1kfGiven  :1;     

    unsigned  FEFET1vgsMaxGiven  :1;
    unsigned  FEFET1vgdMaxGiven  :1;
    unsigned  FEFET1vgbMaxGiven  :1;
    unsigned  FEFET1vdsMaxGiven  :1;
    unsigned  FEFET1vbsMaxGiven  :1;
    unsigned  FEFET1vbdMaxGiven  :1;
    unsigned  FEFET1vgsrMaxGiven  :1;
    unsigned  FEFET1vgdrMaxGiven  :1;
    unsigned  FEFET1vgbrMaxGiven  :1;
    unsigned  FEFET1vbsrMaxGiven  :1;
    unsigned  FEFET1vbdrMaxGiven  :1;

    unsigned  FEFET1LintGiven   :1;
    unsigned  FEFET1LlGiven   :1;
    unsigned  FEFET1LlcGiven   :1;
    unsigned  FEFET1LlnGiven   :1;
    unsigned  FEFET1LwGiven   :1;
    unsigned  FEFET1LwcGiven   :1;
    unsigned  FEFET1LwnGiven   :1;
    unsigned  FEFET1LwlGiven   :1;
    unsigned  FEFET1LwlcGiven   :1;
    unsigned  FEFET1LminGiven   :1;
    unsigned  FEFET1LmaxGiven   :1;

    unsigned  FEFET1WintGiven   :1;
    unsigned  FEFET1WlGiven   :1;
    unsigned  FEFET1WlcGiven   :1;
    unsigned  FEFET1WlnGiven   :1;
    unsigned  FEFET1WwGiven   :1;
    unsigned  FEFET1WwcGiven   :1;
    unsigned  FEFET1WwnGiven   :1;
    unsigned  FEFET1WwlGiven   :1;
    unsigned  FEFET1WwlcGiven   :1;
    unsigned  FEFET1WminGiven   :1;
    unsigned  FEFET1WmaxGiven   :1;

    /* added for stress effect */
    unsigned  FEFET1sarefGiven   :1;
    unsigned  FEFET1sbrefGiven   :1;
    unsigned  FEFET1wlodGiven  :1;
    unsigned  FEFET1ku0Given   :1;
    unsigned  FEFET1kvsatGiven  :1;
    unsigned  FEFET1kvth0Given  :1;
    unsigned  FEFET1tku0Given   :1;
    unsigned  FEFET1llodku0Given   :1;
    unsigned  FEFET1wlodku0Given   :1;
    unsigned  FEFET1llodvthGiven   :1;
    unsigned  FEFET1wlodvthGiven   :1;
    unsigned  FEFET1lku0Given   :1;
    unsigned  FEFET1wku0Given   :1;
    unsigned  FEFET1pku0Given   :1;
    unsigned  FEFET1lkvth0Given   :1;
    unsigned  FEFET1wkvth0Given   :1;
    unsigned  FEFET1pkvth0Given   :1;
    unsigned  FEFET1stk2Given   :1;
    unsigned  FEFET1lodk2Given  :1;
    unsigned  FEFET1steta0Given :1;
    unsigned  FEFET1lodeta0Given :1;

    unsigned  FEFET1webGiven   :1;
    unsigned  FEFET1wecGiven   :1;
    unsigned  FEFET1kvth0weGiven   :1;
    unsigned  FEFET1k2weGiven   :1;
    unsigned  FEFET1ku0weGiven   :1;
    unsigned  FEFET1screfGiven   :1;
    unsigned  FEFET1wpemodGiven   :1;
    unsigned  FEFET1lkvth0weGiven   :1;
    unsigned  FEFET1lk2weGiven   :1;
    unsigned  FEFET1lku0weGiven   :1;
    unsigned  FEFET1wkvth0weGiven   :1;
    unsigned  FEFET1wk2weGiven   :1;
    unsigned  FEFET1wku0weGiven   :1;
    unsigned  FEFET1pkvth0weGiven   :1;
    unsigned  FEFET1pk2weGiven   :1;
    unsigned  FEFET1pku0weGiven   :1;


} FEFET1model;


#ifndef NMOS
#define NMOS 1
#define PMOS -1
#endif /*NMOS*/


/* Instance parameters */
#define FEFET1_W                   1
#define FEFET1_L                   2
#define FEFET1_AS                  3
#define FEFET1_AD                  4
#define FEFET1_PS                  5
#define FEFET1_PD                  6
#define FEFET1_NRS                 7
#define FEFET1_NRD                 8
#define FEFET1_OFF                 9
#define FEFET1_IC                  10
#define FEFET1_IC_VDS              11
#define FEFET1_IC_VGS              12
#define FEFET1_IC_VBS              13
#define FEFET1_TRNQSMOD            14
#define FEFET1_RBODYMOD            15
#define FEFET1_RGATEMOD            16
#define FEFET1_GEOMOD              17
#define FEFET1_RGEOMOD             18
#define FEFET1_NF                  19
#define FEFET1_MIN                 20 
#define FEFET1_ACNQSMOD            22
#define FEFET1_RBDB                23
#define FEFET1_RBSB                24
#define FEFET1_RBPB                25
#define FEFET1_RBPS                26
#define FEFET1_RBPD                27
#define FEFET1_SA                  28
#define FEFET1_SB                  29
#define FEFET1_SD                  30
#define FEFET1_DELVTO              31
#define FEFET1_XGW                 32
#define FEFET1_NGCON               33
#define FEFET1_SCA                 34
#define FEFET1_SCB                 35
#define FEFET1_SCC                 36 
#define FEFET1_SC                  37
#define FEFET1_M                   38

#define FECAP1_AREA,               39
#define FECAP1_IC,                 40
/*enum {
    FECAP1_AREA,
    FECAP1_IC,
    FECAP1_POWER,
    FECAP1_TEMP,
    FECAP1_M,

};
*/

/* Global parameters */
#define FEFET1_MOD_TEMPMOD         89
#define FEFET1_MOD_IGCMOD          90
#define FEFET1_MOD_IGBMOD          91
#define FEFET1_MOD_ACNQSMOD        92
#define FEFET1_MOD_FNOIMOD         93
#define FEFET1_MOD_RDSMOD          94
#define FEFET1_MOD_DIOMOD          95
#define FEFET1_MOD_PERMOD          96
#define FEFET1_MOD_GEOMOD          97
#define FEFET1_MOD_RGEOMOD         98
#define FEFET1_MOD_RGATEMOD        99
#define FEFET1_MOD_RBODYMOD        100
#define FEFET1_MOD_CAPMOD          101
#define FEFET1_MOD_TRNQSMOD        102
#define FEFET1_MOD_MOBMOD          103    
#define FEFET1_MOD_TNOIMOD         104    
#define FEFET1_MOD_TOXE            105
#define FEFET1_MOD_CDSC            106
#define FEFET1_MOD_CDSCB           107
#define FEFET1_MOD_CIT             108
#define FEFET1_MOD_NFACTOR         109
#define FEFET1_MOD_XJ              110
#define FEFET1_MOD_VSAT            111
#define FEFET1_MOD_AT              112
#define FEFET1_MOD_A0              113
#define FEFET1_MOD_A1              114
#define FEFET1_MOD_A2              115
#define FEFET1_MOD_KETA            116   
#define FEFET1_MOD_NSUB            117
#define FEFET1_MOD_NDEP            118
#define FEFET1_MOD_NGATE           120
#define FEFET1_MOD_GAMMA1          121
#define FEFET1_MOD_GAMMA2          122
#define FEFET1_MOD_VBX             123
#define FEFET1_MOD_BINUNIT         124    
#define FEFET1_MOD_VBM             125
#define FEFET1_MOD_XT              126
#define FEFET1_MOD_K1              129
#define FEFET1_MOD_KT1             130
#define FEFET1_MOD_KT1L            131
#define FEFET1_MOD_K2              132
#define FEFET1_MOD_KT2             133
#define FEFET1_MOD_K3              134
#define FEFET1_MOD_K3B             135
#define FEFET1_MOD_W0              136
#define FEFET1_MOD_LPE0            137
#define FEFET1_MOD_DVT0            138
#define FEFET1_MOD_DVT1            139
#define FEFET1_MOD_DVT2            140
#define FEFET1_MOD_DVT0W           141
#define FEFET1_MOD_DVT1W           142
#define FEFET1_MOD_DVT2W           143
#define FEFET1_MOD_DROUT           144
#define FEFET1_MOD_DSUB            145
#define FEFET1_MOD_VTH0            146
#define FEFET1_MOD_UA              147
#define FEFET1_MOD_UA1             148
#define FEFET1_MOD_UB              149
#define FEFET1_MOD_UB1             150
#define FEFET1_MOD_UC              151
#define FEFET1_MOD_UC1             152
#define FEFET1_MOD_U0              153
#define FEFET1_MOD_UTE             154
#define FEFET1_MOD_VOFF            155
#define FEFET1_MOD_DELTA           156
#define FEFET1_MOD_RDSW            157
#define FEFET1_MOD_PRT             158
#define FEFET1_MOD_LDD             159
#define FEFET1_MOD_ETA             160
#define FEFET1_MOD_ETA0            161
#define FEFET1_MOD_ETAB            162
#define FEFET1_MOD_PCLM            163
#define FEFET1_MOD_PDIBL1          164
#define FEFET1_MOD_PDIBL2          165
#define FEFET1_MOD_PSCBE1          166
#define FEFET1_MOD_PSCBE2          167
#define FEFET1_MOD_PVAG            168
#define FEFET1_MOD_WR              169
#define FEFET1_MOD_DWG             170
#define FEFET1_MOD_DWB             171
#define FEFET1_MOD_B0              172
#define FEFET1_MOD_B1              173
#define FEFET1_MOD_ALPHA0          174
#define FEFET1_MOD_BETA0           175
#define FEFET1_MOD_PDIBLB          178
#define FEFET1_MOD_PRWG            179
#define FEFET1_MOD_PRWB            180
#define FEFET1_MOD_CDSCD           181
#define FEFET1_MOD_AGS             182
#define FEFET1_MOD_FRINGE          184
#define FEFET1_MOD_CGSL            186
#define FEFET1_MOD_CGDL            187
#define FEFET1_MOD_CKAPPAS         188
#define FEFET1_MOD_CF              189
#define FEFET1_MOD_CLC             190
#define FEFET1_MOD_CLE             191
#define FEFET1_MOD_PARAMCHK        192
#define FEFET1_MOD_VERSION         193
#define FEFET1_MOD_VFBCV           194
#define FEFET1_MOD_ACDE            195
#define FEFET1_MOD_MOIN            196
#define FEFET1_MOD_NOFF            197
#define FEFET1_MOD_IJTHDFWD        198
#define FEFET1_MOD_ALPHA1          199
#define FEFET1_MOD_VFB             200
#define FEFET1_MOD_TOXM            201
#define FEFET1_MOD_TCJ             202
#define FEFET1_MOD_TCJSW           203
#define FEFET1_MOD_TCJSWG          204
#define FEFET1_MOD_TPB             205
#define FEFET1_MOD_TPBSW           206
#define FEFET1_MOD_TPBSWG          207
#define FEFET1_MOD_VOFFCV          208
#define FEFET1_MOD_GBMIN           209
#define FEFET1_MOD_RBDB            210
#define FEFET1_MOD_RBSB            211
#define FEFET1_MOD_RBPB            212
#define FEFET1_MOD_RBPS            213
#define FEFET1_MOD_RBPD            214
#define FEFET1_MOD_DMCG            215
#define FEFET1_MOD_DMCI            216
#define FEFET1_MOD_DMDG            217
#define FEFET1_MOD_XGW             218
#define FEFET1_MOD_XGL             219
#define FEFET1_MOD_RSHG            220
#define FEFET1_MOD_NGCON           221
#define FEFET1_MOD_AGIDL           222
#define FEFET1_MOD_BGIDL           223
#define FEFET1_MOD_EGIDL           224
#define FEFET1_MOD_IJTHSFWD        225
#define FEFET1_MOD_XJBVD           226
#define FEFET1_MOD_XJBVS           227
#define FEFET1_MOD_BVD             228
#define FEFET1_MOD_BVS             229
#define FEFET1_MOD_TOXP            230
#define FEFET1_MOD_DTOX            231
#define FEFET1_MOD_XRCRG1          232
#define FEFET1_MOD_XRCRG2          233
#define FEFET1_MOD_EU              234
#define FEFET1_MOD_IJTHSREV        235
#define FEFET1_MOD_IJTHDREV        236
#define FEFET1_MOD_MINV            237
#define FEFET1_MOD_VOFFL           238
#define FEFET1_MOD_PDITS           239
#define FEFET1_MOD_PDITSD          240
#define FEFET1_MOD_PDITSL          241
#define FEFET1_MOD_TNOIA           242
#define FEFET1_MOD_TNOIB           243
#define FEFET1_MOD_NTNOI           244
#define FEFET1_MOD_FPROUT          245
#define FEFET1_MOD_LPEB            246
#define FEFET1_MOD_DVTP0           247
#define FEFET1_MOD_DVTP1           248
#define FEFET1_MOD_CGIDL           249
#define FEFET1_MOD_PHIN            250
#define FEFET1_MOD_RDSWMIN         251
#define FEFET1_MOD_RSW             252
#define FEFET1_MOD_RDW             253
#define FEFET1_MOD_RDWMIN          254
#define FEFET1_MOD_RSWMIN          255
#define FEFET1_MOD_NSD             256
#define FEFET1_MOD_CKAPPAD         257
#define FEFET1_MOD_DMCGT           258
#define FEFET1_MOD_AIGC            259
#define FEFET1_MOD_BIGC            260
#define FEFET1_MOD_CIGC            261
#define FEFET1_MOD_AIGBACC         262
#define FEFET1_MOD_BIGBACC         263
#define FEFET1_MOD_CIGBACC         264            
#define FEFET1_MOD_AIGBINV         265
#define FEFET1_MOD_BIGBINV         266
#define FEFET1_MOD_CIGBINV         267
#define FEFET1_MOD_NIGC            268
#define FEFET1_MOD_NIGBACC         269
#define FEFET1_MOD_NIGBINV         270
#define FEFET1_MOD_NTOX            271
#define FEFET1_MOD_TOXREF          272
#define FEFET1_MOD_EIGBINV         273
#define FEFET1_MOD_PIGCD           274
#define FEFET1_MOD_POXEDGE         275
#define FEFET1_MOD_EPSROX          276
#define FEFET1_MOD_AIGSD           277
#define FEFET1_MOD_BIGSD           278
#define FEFET1_MOD_CIGSD           279
#define FEFET1_MOD_JSWGS           280
#define FEFET1_MOD_JSWGD           281
#define FEFET1_MOD_LAMBDA          282
#define FEFET1_MOD_VTL             283
#define FEFET1_MOD_LC              284
#define FEFET1_MOD_XN              285
#define FEFET1_MOD_RNOIA           286
#define FEFET1_MOD_RNOIB           287
#define FEFET1_MOD_VFBSDOFF        288
#define FEFET1_MOD_LINTNOI         289
#define FEFET1_MOD_UD              290
#define FEFET1_MOD_UD1             291
#define FEFET1_MOD_UP              292
#define FEFET1_MOD_LP              293
#define FEFET1_MOD_TVOFF           294
#define FEFET1_MOD_TVFBSDOFF       295

/* Length dependence */
#define FEFET1_MOD_LCDSC            301
#define FEFET1_MOD_LCDSCB           302
#define FEFET1_MOD_LCIT             303
#define FEFET1_MOD_LNFACTOR         304
#define FEFET1_MOD_LXJ              305
#define FEFET1_MOD_LVSAT            306
#define FEFET1_MOD_LAT              307
#define FEFET1_MOD_LA0              308
#define FEFET1_MOD_LA1              309
#define FEFET1_MOD_LA2              310
#define FEFET1_MOD_LKETA            311
#define FEFET1_MOD_LNSUB            312
#define FEFET1_MOD_LNDEP            313
#define FEFET1_MOD_LNGATE           315
#define FEFET1_MOD_LGAMMA1          316
#define FEFET1_MOD_LGAMMA2          317
#define FEFET1_MOD_LVBX             318
#define FEFET1_MOD_LVBM             320
#define FEFET1_MOD_LXT              322
#define FEFET1_MOD_LK1              325
#define FEFET1_MOD_LKT1             326
#define FEFET1_MOD_LKT1L            327
#define FEFET1_MOD_LK2              328
#define FEFET1_MOD_LKT2             329
#define FEFET1_MOD_LK3              330
#define FEFET1_MOD_LK3B             331
#define FEFET1_MOD_LW0              332
#define FEFET1_MOD_LLPE0            333
#define FEFET1_MOD_LDVT0            334
#define FEFET1_MOD_LDVT1            335
#define FEFET1_MOD_LDVT2            336
#define FEFET1_MOD_LDVT0W           337
#define FEFET1_MOD_LDVT1W           338
#define FEFET1_MOD_LDVT2W           339
#define FEFET1_MOD_LDROUT           340
#define FEFET1_MOD_LDSUB            341
#define FEFET1_MOD_LVTH0            342
#define FEFET1_MOD_LUA              343
#define FEFET1_MOD_LUA1             344
#define FEFET1_MOD_LUB              345
#define FEFET1_MOD_LUB1             346
#define FEFET1_MOD_LUC              347
#define FEFET1_MOD_LUC1             348
#define FEFET1_MOD_LU0              349
#define FEFET1_MOD_LUTE             350
#define FEFET1_MOD_LVOFF            351
#define FEFET1_MOD_LDELTA           352
#define FEFET1_MOD_LRDSW            353
#define FEFET1_MOD_LPRT             354
#define FEFET1_MOD_LLDD             355
#define FEFET1_MOD_LETA             356
#define FEFET1_MOD_LETA0            357
#define FEFET1_MOD_LETAB            358
#define FEFET1_MOD_LPCLM            359
#define FEFET1_MOD_LPDIBL1          360
#define FEFET1_MOD_LPDIBL2          361
#define FEFET1_MOD_LPSCBE1          362
#define FEFET1_MOD_LPSCBE2          363
#define FEFET1_MOD_LPVAG            364
#define FEFET1_MOD_LWR              365
#define FEFET1_MOD_LDWG             366
#define FEFET1_MOD_LDWB             367
#define FEFET1_MOD_LB0              368
#define FEFET1_MOD_LB1              369
#define FEFET1_MOD_LALPHA0          370
#define FEFET1_MOD_LBETA0           371
#define FEFET1_MOD_LPDIBLB          374
#define FEFET1_MOD_LPRWG            375
#define FEFET1_MOD_LPRWB            376
#define FEFET1_MOD_LCDSCD           377
#define FEFET1_MOD_LAGS             378

#define FEFET1_MOD_LFRINGE          381
#define FEFET1_MOD_LCGSL            383
#define FEFET1_MOD_LCGDL            384
#define FEFET1_MOD_LCKAPPAS         385
#define FEFET1_MOD_LCF              386
#define FEFET1_MOD_LCLC             387
#define FEFET1_MOD_LCLE             388
#define FEFET1_MOD_LVFBCV           389
#define FEFET1_MOD_LACDE            390
#define FEFET1_MOD_LMOIN            391
#define FEFET1_MOD_LNOFF            392
#define FEFET1_MOD_LALPHA1          394
#define FEFET1_MOD_LVFB             395
#define FEFET1_MOD_LVOFFCV          396
#define FEFET1_MOD_LAGIDL           397
#define FEFET1_MOD_LBGIDL           398
#define FEFET1_MOD_LEGIDL           399
#define FEFET1_MOD_LXRCRG1          400
#define FEFET1_MOD_LXRCRG2          401
#define FEFET1_MOD_LEU              402
#define FEFET1_MOD_LMINV            403
#define FEFET1_MOD_LPDITS           404
#define FEFET1_MOD_LPDITSD          405
#define FEFET1_MOD_LFPROUT          406
#define FEFET1_MOD_LLPEB            407
#define FEFET1_MOD_LDVTP0           408
#define FEFET1_MOD_LDVTP1           409
#define FEFET1_MOD_LCGIDL           410
#define FEFET1_MOD_LPHIN            411
#define FEFET1_MOD_LRSW             412
#define FEFET1_MOD_LRDW             413
#define FEFET1_MOD_LNSD             414
#define FEFET1_MOD_LCKAPPAD         415
#define FEFET1_MOD_LAIGC            416
#define FEFET1_MOD_LBIGC            417
#define FEFET1_MOD_LCIGC            418
#define FEFET1_MOD_LAIGBACC         419            
#define FEFET1_MOD_LBIGBACC         420
#define FEFET1_MOD_LCIGBACC         421
#define FEFET1_MOD_LAIGBINV         422
#define FEFET1_MOD_LBIGBINV         423
#define FEFET1_MOD_LCIGBINV         424
#define FEFET1_MOD_LNIGC            425
#define FEFET1_MOD_LNIGBACC         426
#define FEFET1_MOD_LNIGBINV         427
#define FEFET1_MOD_LNTOX            428
#define FEFET1_MOD_LEIGBINV         429
#define FEFET1_MOD_LPIGCD           430
#define FEFET1_MOD_LPOXEDGE         431
#define FEFET1_MOD_LAIGSD           432
#define FEFET1_MOD_LBIGSD           433
#define FEFET1_MOD_LCIGSD           434

#define FEFET1_MOD_LLAMBDA          435
#define FEFET1_MOD_LVTL             436
#define FEFET1_MOD_LXN              437
#define FEFET1_MOD_LVFBSDOFF        438
#define FEFET1_MOD_LUD              439
#define FEFET1_MOD_LUD1             440
#define FEFET1_MOD_LUP              441
#define FEFET1_MOD_LLP              442

/* Width dependence */
#define FEFET1_MOD_WCDSC            481
#define FEFET1_MOD_WCDSCB           482
#define FEFET1_MOD_WCIT             483
#define FEFET1_MOD_WNFACTOR         484
#define FEFET1_MOD_WXJ              485
#define FEFET1_MOD_WVSAT            486
#define FEFET1_MOD_WAT              487
#define FEFET1_MOD_WA0              488
#define FEFET1_MOD_WA1              489
#define FEFET1_MOD_WA2              490
#define FEFET1_MOD_WKETA            491
#define FEFET1_MOD_WNSUB            492
#define FEFET1_MOD_WNDEP            493
#define FEFET1_MOD_WNGATE           495
#define FEFET1_MOD_WGAMMA1          496
#define FEFET1_MOD_WGAMMA2          497
#define FEFET1_MOD_WVBX             498
#define FEFET1_MOD_WVBM             500
#define FEFET1_MOD_WXT              502
#define FEFET1_MOD_WK1              505
#define FEFET1_MOD_WKT1             506
#define FEFET1_MOD_WKT1L            507
#define FEFET1_MOD_WK2              508
#define FEFET1_MOD_WKT2             509
#define FEFET1_MOD_WK3              510
#define FEFET1_MOD_WK3B             511
#define FEFET1_MOD_WW0              512
#define FEFET1_MOD_WLPE0            513
#define FEFET1_MOD_WDVT0            514
#define FEFET1_MOD_WDVT1            515
#define FEFET1_MOD_WDVT2            516
#define FEFET1_MOD_WDVT0W           517
#define FEFET1_MOD_WDVT1W           518
#define FEFET1_MOD_WDVT2W           519
#define FEFET1_MOD_WDROUT           520
#define FEFET1_MOD_WDSUB            521
#define FEFET1_MOD_WVTH0            522
#define FEFET1_MOD_WUA              523
#define FEFET1_MOD_WUA1             524
#define FEFET1_MOD_WUB              525
#define FEFET1_MOD_WUB1             526
#define FEFET1_MOD_WUC              527
#define FEFET1_MOD_WUC1             528
#define FEFET1_MOD_WU0              529
#define FEFET1_MOD_WUTE             530
#define FEFET1_MOD_WVOFF            531
#define FEFET1_MOD_WDELTA           532
#define FEFET1_MOD_WRDSW            533
#define FEFET1_MOD_WPRT             534
#define FEFET1_MOD_WLDD             535
#define FEFET1_MOD_WETA             536
#define FEFET1_MOD_WETA0            537
#define FEFET1_MOD_WETAB            538
#define FEFET1_MOD_WPCLM            539
#define FEFET1_MOD_WPDIBL1          540
#define FEFET1_MOD_WPDIBL2          541
#define FEFET1_MOD_WPSCBE1          542
#define FEFET1_MOD_WPSCBE2          543
#define FEFET1_MOD_WPVAG            544
#define FEFET1_MOD_WWR              545
#define FEFET1_MOD_WDWG             546
#define FEFET1_MOD_WDWB             547
#define FEFET1_MOD_WB0              548
#define FEFET1_MOD_WB1              549
#define FEFET1_MOD_WALPHA0          550
#define FEFET1_MOD_WBETA0           551
#define FEFET1_MOD_WPDIBLB          554
#define FEFET1_MOD_WPRWG            555
#define FEFET1_MOD_WPRWB            556
#define FEFET1_MOD_WCDSCD           557
#define FEFET1_MOD_WAGS             558

#define FEFET1_MOD_WFRINGE          561
#define FEFET1_MOD_WCGSL            563
#define FEFET1_MOD_WCGDL            564
#define FEFET1_MOD_WCKAPPAS         565
#define FEFET1_MOD_WCF              566
#define FEFET1_MOD_WCLC             567
#define FEFET1_MOD_WCLE             568
#define FEFET1_MOD_WVFBCV           569
#define FEFET1_MOD_WACDE            570
#define FEFET1_MOD_WMOIN            571
#define FEFET1_MOD_WNOFF            572
#define FEFET1_MOD_WALPHA1          574
#define FEFET1_MOD_WVFB             575
#define FEFET1_MOD_WVOFFCV          576
#define FEFET1_MOD_WAGIDL           577
#define FEFET1_MOD_WBGIDL           578
#define FEFET1_MOD_WEGIDL           579
#define FEFET1_MOD_WXRCRG1          580
#define FEFET1_MOD_WXRCRG2          581
#define FEFET1_MOD_WEU              582
#define FEFET1_MOD_WMINV            583
#define FEFET1_MOD_WPDITS           584
#define FEFET1_MOD_WPDITSD          585
#define FEFET1_MOD_WFPROUT          586
#define FEFET1_MOD_WLPEB            587
#define FEFET1_MOD_WDVTP0           588
#define FEFET1_MOD_WDVTP1           589
#define FEFET1_MOD_WCGIDL           590
#define FEFET1_MOD_WPHIN            591
#define FEFET1_MOD_WRSW             592
#define FEFET1_MOD_WRDW             593
#define FEFET1_MOD_WNSD             594
#define FEFET1_MOD_WCKAPPAD         595
#define FEFET1_MOD_WAIGC            596
#define FEFET1_MOD_WBIGC            597
#define FEFET1_MOD_WCIGC            598
#define FEFET1_MOD_WAIGBACC         599            
#define FEFET1_MOD_WBIGBACC         600
#define FEFET1_MOD_WCIGBACC         601
#define FEFET1_MOD_WAIGBINV         602
#define FEFET1_MOD_WBIGBINV         603
#define FEFET1_MOD_WCIGBINV         604
#define FEFET1_MOD_WNIGC            605
#define FEFET1_MOD_WNIGBACC         606
#define FEFET1_MOD_WNIGBINV         607
#define FEFET1_MOD_WNTOX            608
#define FEFET1_MOD_WEIGBINV         609
#define FEFET1_MOD_WPIGCD           610
#define FEFET1_MOD_WPOXEDGE         611
#define FEFET1_MOD_WAIGSD           612
#define FEFET1_MOD_WBIGSD           613
#define FEFET1_MOD_WCIGSD           614
#define FEFET1_MOD_WLAMBDA          615
#define FEFET1_MOD_WVTL             616
#define FEFET1_MOD_WXN              617
#define FEFET1_MOD_WVFBSDOFF        618
#define FEFET1_MOD_WUD              619
#define FEFET1_MOD_WUD1             620
#define FEFET1_MOD_WUP              621
#define FEFET1_MOD_WLP              622

/* Cross-term dependence */
#define FEFET1_MOD_PCDSC            661
#define FEFET1_MOD_PCDSCB           662
#define FEFET1_MOD_PCIT             663
#define FEFET1_MOD_PNFACTOR         664
#define FEFET1_MOD_PXJ              665
#define FEFET1_MOD_PVSAT            666
#define FEFET1_MOD_PAT              667
#define FEFET1_MOD_PA0              668
#define FEFET1_MOD_PA1              669
#define FEFET1_MOD_PA2              670
#define FEFET1_MOD_PKETA            671
#define FEFET1_MOD_PNSUB            672
#define FEFET1_MOD_PNDEP            673
#define FEFET1_MOD_PNGATE           675
#define FEFET1_MOD_PGAMMA1          676
#define FEFET1_MOD_PGAMMA2          677
#define FEFET1_MOD_PVBX             678

#define FEFET1_MOD_PVBM             680

#define FEFET1_MOD_PXT              682
#define FEFET1_MOD_PK1              685
#define FEFET1_MOD_PKT1             686
#define FEFET1_MOD_PKT1L            687
#define FEFET1_MOD_PK2              688
#define FEFET1_MOD_PKT2             689
#define FEFET1_MOD_PK3              690
#define FEFET1_MOD_PK3B             691
#define FEFET1_MOD_PW0              692
#define FEFET1_MOD_PLPE0            693

#define FEFET1_MOD_PDVT0            694
#define FEFET1_MOD_PDVT1            695
#define FEFET1_MOD_PDVT2            696

#define FEFET1_MOD_PDVT0W           697
#define FEFET1_MOD_PDVT1W           698
#define FEFET1_MOD_PDVT2W           699

#define FEFET1_MOD_PDROUT           700
#define FEFET1_MOD_PDSUB            701
#define FEFET1_MOD_PVTH0            702
#define FEFET1_MOD_PUA              703
#define FEFET1_MOD_PUA1             704
#define FEFET1_MOD_PUB              705
#define FEFET1_MOD_PUB1             706
#define FEFET1_MOD_PUC              707
#define FEFET1_MOD_PUC1             708
#define FEFET1_MOD_PU0              709
#define FEFET1_MOD_PUTE             710
#define FEFET1_MOD_PVOFF            711
#define FEFET1_MOD_PDELTA           712
#define FEFET1_MOD_PRDSW            713
#define FEFET1_MOD_PPRT             714
#define FEFET1_MOD_PLDD             715
#define FEFET1_MOD_PETA             716
#define FEFET1_MOD_PETA0            717
#define FEFET1_MOD_PETAB            718
#define FEFET1_MOD_PPCLM            719
#define FEFET1_MOD_PPDIBL1          720
#define FEFET1_MOD_PPDIBL2          721
#define FEFET1_MOD_PPSCBE1          722
#define FEFET1_MOD_PPSCBE2          723
#define FEFET1_MOD_PPVAG            724
#define FEFET1_MOD_PWR              725
#define FEFET1_MOD_PDWG             726
#define FEFET1_MOD_PDWB             727
#define FEFET1_MOD_PB0              728
#define FEFET1_MOD_PB1              729
#define FEFET1_MOD_PALPHA0          730
#define FEFET1_MOD_PBETA0           731
#define FEFET1_MOD_PPDIBLB          734

#define FEFET1_MOD_PPRWG            735
#define FEFET1_MOD_PPRWB            736

#define FEFET1_MOD_PCDSCD           737
#define FEFET1_MOD_PAGS             738

#define FEFET1_MOD_PFRINGE          741
#define FEFET1_MOD_PCGSL            743
#define FEFET1_MOD_PCGDL            744
#define FEFET1_MOD_PCKAPPAS         745
#define FEFET1_MOD_PCF              746
#define FEFET1_MOD_PCLC             747
#define FEFET1_MOD_PCLE             748
#define FEFET1_MOD_PVFBCV           749
#define FEFET1_MOD_PACDE            750
#define FEFET1_MOD_PMOIN            751
#define FEFET1_MOD_PNOFF            752
#define FEFET1_MOD_PALPHA1          754
#define FEFET1_MOD_PVFB             755
#define FEFET1_MOD_PVOFFCV          756
#define FEFET1_MOD_PAGIDL           757
#define FEFET1_MOD_PBGIDL           758
#define FEFET1_MOD_PEGIDL           759
#define FEFET1_MOD_PXRCRG1          760
#define FEFET1_MOD_PXRCRG2          761
#define FEFET1_MOD_PEU              762
#define FEFET1_MOD_PMINV            763
#define FEFET1_MOD_PPDITS           764
#define FEFET1_MOD_PPDITSD          765
#define FEFET1_MOD_PFPROUT          766
#define FEFET1_MOD_PLPEB            767
#define FEFET1_MOD_PDVTP0           768
#define FEFET1_MOD_PDVTP1           769
#define FEFET1_MOD_PCGIDL           770
#define FEFET1_MOD_PPHIN            771
#define FEFET1_MOD_PRSW             772
#define FEFET1_MOD_PRDW             773
#define FEFET1_MOD_PNSD             774
#define FEFET1_MOD_PCKAPPAD         775
#define FEFET1_MOD_PAIGC            776
#define FEFET1_MOD_PBIGC            777
#define FEFET1_MOD_PCIGC            778
#define FEFET1_MOD_PAIGBACC         779            
#define FEFET1_MOD_PBIGBACC         780
#define FEFET1_MOD_PCIGBACC         781
#define FEFET1_MOD_PAIGBINV         782
#define FEFET1_MOD_PBIGBINV         783
#define FEFET1_MOD_PCIGBINV         784
#define FEFET1_MOD_PNIGC            785
#define FEFET1_MOD_PNIGBACC         786
#define FEFET1_MOD_PNIGBINV         787
#define FEFET1_MOD_PNTOX            788
#define FEFET1_MOD_PEIGBINV         789
#define FEFET1_MOD_PPIGCD           790
#define FEFET1_MOD_PPOXEDGE         791
#define FEFET1_MOD_PAIGSD           792
#define FEFET1_MOD_PBIGSD           793
#define FEFET1_MOD_PCIGSD           794

#define FEFET1_MOD_SAREF            795
#define FEFET1_MOD_SBREF            796
#define FEFET1_MOD_KU0              797
#define FEFET1_MOD_KVSAT            798
#define FEFET1_MOD_TKU0             799
#define FEFET1_MOD_LLODKU0          800
#define FEFET1_MOD_WLODKU0          801
#define FEFET1_MOD_LLODVTH          802
#define FEFET1_MOD_WLODVTH          803
#define FEFET1_MOD_LKU0             804
#define FEFET1_MOD_WKU0             805
#define FEFET1_MOD_PKU0             806
#define FEFET1_MOD_KVTH0            807
#define FEFET1_MOD_LKVTH0           808
#define FEFET1_MOD_WKVTH0           809
#define FEFET1_MOD_PKVTH0           810
#define FEFET1_MOD_WLOD		   811
#define FEFET1_MOD_STK2		   812
#define FEFET1_MOD_LODK2		   813
#define FEFET1_MOD_STETA0	   814
#define FEFET1_MOD_LODETA0	   815

#define FEFET1_MOD_WEB          816
#define FEFET1_MOD_WEC          817
#define FEFET1_MOD_KVTH0WE          818
#define FEFET1_MOD_K2WE          819
#define FEFET1_MOD_KU0WE          820
#define FEFET1_MOD_SCREF          821
#define FEFET1_MOD_WPEMOD          822

#define FEFET1_MOD_PLAMBDA          825
#define FEFET1_MOD_PVTL             826
#define FEFET1_MOD_PXN              827
#define FEFET1_MOD_PVFBSDOFF        828

#define FEFET1_MOD_TNOM             831
#define FEFET1_MOD_CGSO             832
#define FEFET1_MOD_CGDO             833
#define FEFET1_MOD_CGBO             834
#define FEFET1_MOD_XPART            835
#define FEFET1_MOD_RSH              836
#define FEFET1_MOD_JSS              837
#define FEFET1_MOD_PBS              838
#define FEFET1_MOD_MJS              839
#define FEFET1_MOD_PBSWS            840
#define FEFET1_MOD_MJSWS            841
#define FEFET1_MOD_CJS              842
#define FEFET1_MOD_CJSWS            843
#define FEFET1_MOD_NMOS             844
#define FEFET1_MOD_PMOS             845
#define FEFET1_MOD_NOIA             846
#define FEFET1_MOD_NOIB             847
#define FEFET1_MOD_NOIC             848
#define FEFET1_MOD_LINT             849
#define FEFET1_MOD_LL               850
#define FEFET1_MOD_LLN              851
#define FEFET1_MOD_LW               852
#define FEFET1_MOD_LWN              853
#define FEFET1_MOD_LWL              854
#define FEFET1_MOD_LMIN             855
#define FEFET1_MOD_LMAX             856
#define FEFET1_MOD_WINT             857
#define FEFET1_MOD_WL               858
#define FEFET1_MOD_WLN              859
#define FEFET1_MOD_WW               860
#define FEFET1_MOD_WWN              861
#define FEFET1_MOD_WWL              862
#define FEFET1_MOD_WMIN             863
#define FEFET1_MOD_WMAX             864
#define FEFET1_MOD_DWC              865
#define FEFET1_MOD_DLC              866
#define FEFET1_MOD_XL               867
#define FEFET1_MOD_XW               868
#define FEFET1_MOD_EM               869
#define FEFET1_MOD_EF               870
#define FEFET1_MOD_AF               871
#define FEFET1_MOD_KF               872
#define FEFET1_MOD_NJS              873
#define FEFET1_MOD_XTIS             874
#define FEFET1_MOD_PBSWGS           875
#define FEFET1_MOD_MJSWGS           876
#define FEFET1_MOD_CJSWGS           877
#define FEFET1_MOD_JSWS             878
#define FEFET1_MOD_LLC              879
#define FEFET1_MOD_LWC              880
#define FEFET1_MOD_LWLC             881
#define FEFET1_MOD_WLC              882
#define FEFET1_MOD_WWC              883
#define FEFET1_MOD_WWLC             884
#define FEFET1_MOD_DWJ              885
#define FEFET1_MOD_JSD              886
#define FEFET1_MOD_PBD              887
#define FEFET1_MOD_MJD              888
#define FEFET1_MOD_PBSWD            889
#define FEFET1_MOD_MJSWD            890
#define FEFET1_MOD_CJD              891
#define FEFET1_MOD_CJSWD            892
#define FEFET1_MOD_NJD              893
#define FEFET1_MOD_XTID             894
#define FEFET1_MOD_PBSWGD           895
#define FEFET1_MOD_MJSWGD           896
#define FEFET1_MOD_CJSWGD           897
#define FEFET1_MOD_JSWD             898
#define FEFET1_MOD_DLCIG            899

/* trap-assisted tunneling */

#define FEFET1_MOD_JTSS             900
#define FEFET1_MOD_JTSD		   901
#define FEFET1_MOD_JTSSWS	   902
#define FEFET1_MOD_JTSSWD	   903
#define FEFET1_MOD_JTSSWGS	   904
#define FEFET1_MOD_JTSSWGD	   905
#define FEFET1_MOD_NJTS	 	   906
#define FEFET1_MOD_NJTSSW	   907
#define FEFET1_MOD_NJTSSWG	   908
#define FEFET1_MOD_XTSS		   909
#define FEFET1_MOD_XTSD		   910
#define FEFET1_MOD_XTSSWS	   911
#define FEFET1_MOD_XTSSWD	   912
#define FEFET1_MOD_XTSSWGS	   913
#define FEFET1_MOD_XTSSWGD	   914
#define FEFET1_MOD_TNJTS		   915
#define FEFET1_MOD_TNJTSSW	   916
#define FEFET1_MOD_TNJTSSWG	   917
#define FEFET1_MOD_VTSS             918
#define FEFET1_MOD_VTSD		   919
#define FEFET1_MOD_VTSSWS	   920
#define FEFET1_MOD_VTSSWD	   921
#define FEFET1_MOD_VTSSWGS	   922
#define FEFET1_MOD_VTSSWGD	   923
#define FEFET1_MOD_PUD              924
#define FEFET1_MOD_PUD1             925
#define FEFET1_MOD_PUP              926
#define FEFET1_MOD_PLP              927

/* device questions */
#define FEFET1_DNODE                945
#define FEFET1_GNODEEXT             946
#define FEFET1_SNODE                947
#define FEFET1_BNODE                948
#define FEFET1_DNODEPRIME           949
#define FEFET1_GNODEPRIME           950
#define FEFET1_GNODEMIDE            951
#define FEFET1_GNODEMID             952
#define FEFET1_SNODEPRIME           953
#define FEFET1_BNODEPRIME           954
#define FEFET1_DBNODE               955
#define FEFET1_SBNODE               956
#define FEFET1_VBD                  957
#define FEFET1_VBS                  958
#define FEFET1_VGS                  959
#define FEFET1_VDS                  960
#define FEFET1_CD                   961
#define FEFET1_CBS                  962
#define FEFET1_CBD                  963
#define FEFET1_GM                   964
#define FEFET1_GDS                  965
#define FEFET1_GMBS                 966
#define FEFET1_GBD                  967
#define FEFET1_GBS                  968
#define FEFET1_QB                   969
#define FEFET1_CQB                  970
#define FEFET1_QG                   971
#define FEFET1_CQG                  972
#define FEFET1_QD                   973
#define FEFET1_CQD                  974
#define FEFET1_CGGB                 975
#define FEFET1_CGDB                 976
#define FEFET1_CGSB                 977
#define FEFET1_CBGB                 978
#define FEFET1_CAPBD                979
#define FEFET1_CQBD                 980
#define FEFET1_CAPBS                981
#define FEFET1_CQBS                 982
#define FEFET1_CDGB                 983
#define FEFET1_CDDB                 984
#define FEFET1_CDSB                 985
#define FEFET1_VON                  986
#define FEFET1_VDSAT                987
#define FEFET1_QBS                  988
#define FEFET1_QBD                  989
#define FEFET1_SOURCECONDUCT        990
#define FEFET1_DRAINCONDUCT         991
#define FEFET1_CBDB                 992
#define FEFET1_CBSB                 993
#define FEFET1_CSUB		   994
#define FEFET1_QINV		   995
#define FEFET1_IGIDL		   996
#define FEFET1_CSGB                 997
#define FEFET1_CSDB                 998
#define FEFET1_CSSB                 999
#define FEFET1_CGBB                 1000
#define FEFET1_CDBB                 1001
#define FEFET1_CSBB                 1002
#define FEFET1_CBBB                 1003
#define FEFET1_QS                   1004
#define FEFET1_IGISL		   1005
#define FEFET1_IGS		   1006
#define FEFET1_IGD		   1007
#define FEFET1_IGB		   1008
#define FEFET1_IGCS		   1009
#define FEFET1_IGCD		   1010
#define FEFET1_QDEF		   1011
#define FEFET1_DELVT0		   1012
#define FEFET1_GCRG                 1013
#define FEFET1_GTAU                 1014

#define FEFET1_MOD_LTVOFF           1051
#define FEFET1_MOD_LTVFBSDOFF       1052
#define FEFET1_MOD_WTVOFF           1053
#define FEFET1_MOD_WTVFBSDOFF       1054
#define FEFET1_MOD_PTVOFF           1055
#define FEFET1_MOD_PTVFBSDOFF       1056

#define FEFET1_MOD_LKVTH0WE          1061
#define FEFET1_MOD_LK2WE             1062
#define FEFET1_MOD_LKU0WE		1063
#define FEFET1_MOD_WKVTH0WE          1064
#define FEFET1_MOD_WK2WE             1065
#define FEFET1_MOD_WKU0WE		1066
#define FEFET1_MOD_PKVTH0WE          1067
#define FEFET1_MOD_PK2WE             1068
#define FEFET1_MOD_PKU0WE		1069

#define FEFET1_MOD_RBPS0               1101
#define FEFET1_MOD_RBPSL               1102
#define FEFET1_MOD_RBPSW               1103
#define FEFET1_MOD_RBPSNF              1104
#define FEFET1_MOD_RBPD0               1105
#define FEFET1_MOD_RBPDL               1106
#define FEFET1_MOD_RBPDW               1107
#define FEFET1_MOD_RBPDNF              1108

#define FEFET1_MOD_RBPBX0              1109
#define FEFET1_MOD_RBPBXL              1110
#define FEFET1_MOD_RBPBXW              1111
#define FEFET1_MOD_RBPBXNF             1112
#define FEFET1_MOD_RBPBY0              1113
#define FEFET1_MOD_RBPBYL              1114
#define FEFET1_MOD_RBPBYW              1115
#define FEFET1_MOD_RBPBYNF             1116

#define FEFET1_MOD_RBSBX0              1117
#define FEFET1_MOD_RBSBY0              1118
#define FEFET1_MOD_RBDBX0              1119
#define FEFET1_MOD_RBDBY0              1120

#define FEFET1_MOD_RBSDBXL             1121
#define FEFET1_MOD_RBSDBXW             1122
#define FEFET1_MOD_RBSDBXNF            1123
#define FEFET1_MOD_RBSDBYL             1124
#define FEFET1_MOD_RBSDBYW             1125
#define FEFET1_MOD_RBSDBYNF            1126

#define FEFET1_MOD_VGS_MAX             1201
#define FEFET1_MOD_VGD_MAX             1202
#define FEFET1_MOD_VGB_MAX             1203
#define FEFET1_MOD_VDS_MAX             1204
#define FEFET1_MOD_VBS_MAX             1205
#define FEFET1_MOD_VBD_MAX             1206
#define FEFET1_MOD_VGSR_MAX            1207
#define FEFET1_MOD_VGDR_MAX            1208
#define FEFET1_MOD_VGBR_MAX            1209
#define FEFET1_MOD_VBSR_MAX            1210
#define FEFET1_MOD_VBDR_MAX            1211

#define FECAP1_MOD_FECAP               1212
#define FECAP1_MOD_Thickness           1213
#define FECAP1_MOD_Alpha               1214
#define FECAP1_MOD_Beta                1215
#define FECAP1_MOD_Gamma               1216
#define FECAP1_MOD_Rho                 1217
#define FECAP1_MOD_Ec                  1218
#define FECAP1_MOD_Ps                  1219

#include "fefet1ext.h"

extern void FEFET1evaluate(double,double,double,FEFET1instance*,FEFET1model*,
        double*,double*,double*, double*, double*, double*, double*, 
        double*, double*, double*, double*, double*, double*, double*, 
        double*, double*, double*, double*, CKTcircuit*);
extern int FEFET1debug(FEFET1model*, FEFET1instance*, CKTcircuit*, int);
extern int FEFET1checkModel(FEFET1model*, FEFET1instance*, CKTcircuit*);
extern int FEFET1PAeffGeo(double, int, int, double, double, double, double, double *, double *, double *, double *);
extern int FEFET1RdseffGeo(double, int, int, int, double, double, double, double, double, int, double *);

#endif /*FEFET1*/
