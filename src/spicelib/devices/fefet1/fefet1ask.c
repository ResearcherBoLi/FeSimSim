/**********
Copyright 2024 Xidian University
Author: 2024 Bo Li
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/

#include "ngspice/ngspice.h"
#include "ngspice/ifsim.h"
#include "ngspice/cktdefs.h"
#include "ngspice/devdefs.h"
#include "fefet1def.h"
#include "ngspice/sperror.h"
#include "ngspice/suffix.h"

int
FEFET1ask(
CKTcircuit *ckt,
GENinstance *inst,
int which,
IFvalue *value,
IFvalue *select)
{
FEFET1instance *here = (FEFET1instance*)inst;

    NG_IGNORE(select);

    switch(which) 
    {   
        case FECAP1_IC:
            value->rValue = here->FECAP1initCondP;
            return(OK);	
        case FECAP1_AREA:
            value->rValue=here->FECAP1_Area;
            return(OK);               
        
        case FEFET1_L:
            value->rValue = here->FEFET1l;
            return(OK);
        case FEFET1_W:
            value->rValue = here->FEFET1w;
            return(OK);
        case FEFET1_M:
            value->rValue = here->FEFET1m;
            return(OK);
        case FEFET1_NF:
            value->rValue = here->FEFET1nf;
            return(OK);
        case FEFET1_MIN:
            value->iValue = here->FEFET1min;
            return(OK);
        case FEFET1_AS:
            value->rValue = here->FEFET1sourceArea;
            return(OK);
        case FEFET1_AD:
            value->rValue = here->FEFET1drainArea;
            return(OK);
        case FEFET1_PS:
            value->rValue = here->FEFET1sourcePerimeter;
            return(OK);
        case FEFET1_PD:
            value->rValue = here->FEFET1drainPerimeter;
            return(OK);
        case FEFET1_NRS:
            value->rValue = here->FEFET1sourceSquares;
            return(OK);
        case FEFET1_NRD:
            value->rValue = here->FEFET1drainSquares;
            return(OK);
        case FEFET1_OFF:
            value->rValue = here->FEFET1off;
            return(OK);
        case FEFET1_SA:
            value->rValue = here->FEFET1sa ;
            return(OK);
        case FEFET1_SB:
            value->rValue = here->FEFET1sb ;
            return(OK);
        case FEFET1_SD:
            value->rValue = here->FEFET1sd ;
            return(OK);
	case FEFET1_SCA:
            value->rValue = here->FEFET1sca ;
            return(OK);
	case FEFET1_SCB:
            value->rValue = here->FEFET1scb ;
            return(OK);
	case FEFET1_SCC:
            value->rValue = here->FEFET1scc ;
            return(OK);
	case FEFET1_SC:
            value->rValue = here->FEFET1sc ;
            return(OK);

        case FEFET1_RBSB:
            value->rValue = here->FEFET1rbsb;
            return(OK);
        case FEFET1_RBDB:
            value->rValue = here->FEFET1rbdb;
            return(OK);
        case FEFET1_RBPB:
            value->rValue = here->FEFET1rbpb;
            return(OK);
        case FEFET1_RBPS:
            value->rValue = here->FEFET1rbps;
            return(OK);
        case FEFET1_RBPD:
            value->rValue = here->FEFET1rbpd;
            return(OK);
        case FEFET1_DELVTO:
            value->rValue = here->FEFET1delvto;
            return(OK);
        case FEFET1_XGW:
            value->rValue = here->FEFET1xgw;
            return(OK);
        case FEFET1_NGCON:
            value->rValue = here->FEFET1ngcon;
            return(OK);
        case FEFET1_TRNQSMOD:
            value->iValue = here->FEFET1trnqsMod;
            return(OK);
        case FEFET1_ACNQSMOD:
            value->iValue = here->FEFET1acnqsMod;
            return(OK);
        case FEFET1_RBODYMOD:
            value->iValue = here->FEFET1rbodyMod;
            return(OK);
        case FEFET1_RGATEMOD:
            value->iValue = here->FEFET1rgateMod;
            return(OK);
        case FEFET1_GEOMOD:
            value->iValue = here->FEFET1geoMod;
            return(OK);
        case FEFET1_RGEOMOD:
            value->iValue = here->FEFET1rgeoMod;
            return(OK);
        case FEFET1_IC_VDS:
            value->rValue = here->FEFET1icVDS;
            return(OK);
        case FEFET1_IC_VGS:
            value->rValue = here->FEFET1icVGS;
            return(OK);
        case FEFET1_IC_VBS:
            value->rValue = here->FEFET1icVBS;
            return(OK);
        case FEFET1_DNODE:
            value->iValue = here->FEFET1dNode;
            return(OK);
        case FEFET1_GNODEEXT:
            value->iValue = here->FEFET1gNodeExt;
            return(OK);
        case FEFET1_SNODE:
            value->iValue = here->FEFET1sNode;
            return(OK);
        case FEFET1_BNODE:
            value->iValue = here->FEFET1bNode;
            return(OK);
        case FEFET1_DNODEPRIME:
            value->iValue = here->FEFET1dNodePrime;
            return(OK);
        case FEFET1_GNODEPRIME:
            value->iValue = here->FEFET1gNodePrime;
            return(OK);
        case FEFET1_GNODEMID:
            value->iValue = here->FEFET1gNodeMid;
            return(OK);
        case FEFET1_SNODEPRIME:
            value->iValue = here->FEFET1sNodePrime;
            return(OK);
        case FEFET1_DBNODE:
            value->iValue = here->FEFET1dbNode;
            return(OK);
        case FEFET1_BNODEPRIME:
            value->iValue = here->FEFET1bNodePrime;
            return(OK);
        case FEFET1_SBNODE:
            value->iValue = here->FEFET1sbNode;
            return(OK);
        case FEFET1_SOURCECONDUCT:
            value->rValue = here->FEFET1sourceConductance;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_DRAINCONDUCT:
            value->rValue = here->FEFET1drainConductance;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_VBD:
            value->rValue = *(ckt->CKTstate0 + here->FEFET1vbd);
            return(OK);
        case FEFET1_VBS:
            value->rValue = *(ckt->CKTstate0 + here->FEFET1vbs);
            return(OK);
        case FEFET1_VGS:
            value->rValue = *(ckt->CKTstate0 + here->FEFET1vgs);
            return(OK);
        case FEFET1_VDS:
            value->rValue = *(ckt->CKTstate0 + here->FEFET1vds);
            return(OK);
        case FEFET1_CD:
            value->rValue = here->FEFET1cd; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CBS:
            value->rValue = here->FEFET1cbs; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CBD:
            value->rValue = here->FEFET1cbd; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CSUB:
            value->rValue = here->FEFET1csub; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_QINV:
            value->rValue = here-> FEFET1qinv; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_IGIDL:
            value->rValue = here->FEFET1Igidl; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_IGISL:
            value->rValue = here->FEFET1Igisl; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_IGS:
            value->rValue = here->FEFET1Igs; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_IGD:
            value->rValue = here->FEFET1Igd; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_IGB:
            value->rValue = here->FEFET1Igb; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_IGCS:
            value->rValue = here->FEFET1Igcs; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_IGCD:
            value->rValue = here->FEFET1Igcd; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_GM:
            value->rValue = here->FEFET1gm; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_GDS:
            value->rValue = here->FEFET1gds; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_GMBS:
            value->rValue = here->FEFET1gmbs; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_GBD:
            value->rValue = here->FEFET1gbd; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_GBS:
            value->rValue = here->FEFET1gbs; 
            value->rValue *= here->FEFET1m;
            return(OK);
/*        case FEFET1_QB:
            value->rValue = *(ckt->CKTstate0 + here->FEFET1qb); 
            return(OK); */
        case FEFET1_CQB:
            value->rValue = *(ckt->CKTstate0 + here->FEFET1cqb); 
            return(OK);
/*        case FEFET1_QG:
            value->rValue = *(ckt->CKTstate0 + here->FEFET1qg); 
            return(OK); */
        case FEFET1_CQG:
            value->rValue = *(ckt->CKTstate0 + here->FEFET1cqg); 
            return(OK);
/*        case FEFET1_QD:
            value->rValue = *(ckt->CKTstate0 + here->FEFET1qd); 
            return(OK); */
        case FEFET1_CQD:
            value->rValue = *(ckt->CKTstate0 + here->FEFET1cqd); 
            return(OK);
/*        case FEFET1_QS:
            value->rValue = *(ckt->CKTstate0 + here->FEFET1qs); 
            return(OK); */
        case FEFET1_QB:
            value->rValue = here->FEFET1qbulk; 
            value->rValue *= here->FEFET1m;
            return(OK); 
        case FEFET1_QG:
            value->rValue = here->FEFET1qgate; 
            value->rValue *= here->FEFET1m;
            return(OK); 
        case FEFET1_QS:
            value->rValue = here->FEFET1qsrc; 
            value->rValue *= here->FEFET1m;
            return(OK); 
        case FEFET1_QD:
            value->rValue = here->FEFET1qdrn; 
            value->rValue *= here->FEFET1m;
            return(OK); 
        case FEFET1_QDEF:
            value->rValue = *(ckt->CKTstate0 + here->FEFET1qdef); 
            return(OK); 
        case FEFET1_GCRG:
            value->rValue = here->FEFET1gcrg;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_GTAU:
            value->rValue = here->FEFET1gtau;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CGGB:
            value->rValue = here->FEFET1cggb; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CGDB:
            value->rValue = here->FEFET1cgdb;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CGSB:
            value->rValue = here->FEFET1cgsb;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CDGB:
            value->rValue = here->FEFET1cdgb; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CDDB:
            value->rValue = here->FEFET1cddb; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CDSB:
            value->rValue = here->FEFET1cdsb; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CBGB:
            value->rValue = here->FEFET1cbgb;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CBDB:
            value->rValue = here->FEFET1cbdb;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CBSB:
            value->rValue = here->FEFET1cbsb;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CSGB:
            value->rValue = here->FEFET1csgb;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CSDB:
            value->rValue = here->FEFET1csdb;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CSSB:
            value->rValue = here->FEFET1cssb;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CGBB:
            value->rValue = here->FEFET1cgbb;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CDBB:
            value->rValue = here->FEFET1cdbb;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CSBB:
            value->rValue = here->FEFET1csbb;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CBBB:
            value->rValue = here->FEFET1cbbb;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CAPBD:
            value->rValue = here->FEFET1capbd; 
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_CAPBS:
            value->rValue = here->FEFET1capbs;
            value->rValue *= here->FEFET1m;
            return(OK);
        case FEFET1_VON:
            value->rValue = here->FEFET1von; 
            return(OK);
        case FEFET1_VDSAT:
            value->rValue = here->FEFET1vdsat; 
            return(OK);
        case FEFET1_QBS:
            value->rValue = *(ckt->CKTstate0 + here->FEFET1qbs); 
            return(OK);
        case FEFET1_QBD:
            value->rValue = *(ckt->CKTstate0 + here->FEFET1qbd); 
            return(OK);
        default:
            return(E_BADPARM);
    }
    /* NOTREACHED */
}

