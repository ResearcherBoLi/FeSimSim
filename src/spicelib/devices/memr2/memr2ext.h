/**********
Comment by Bo Li
this header file is called by memdefs.h, modified by resext.h
need further modify the parser interface 
---20191024 

**********/
/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/


extern int MEMR2ask(CKTcircuit*, GENinstance*, int, IFvalue*, IFvalue*);
extern int MEMR2load(GENmodel*, CKTcircuit*);
extern int MEMR2acLoad(GENmodel*, CKTcircuit*);
extern int MEMR2modAsk(CKTcircuit*, GENmodel*, int, IFvalue*);
extern int MEMR2mParam(int,IFvalue*, GENmodel*);
extern int MEMR2param(int,IFvalue*, GENinstance*, IFvalue*);
//extern int MEM2pzLoad(GENmodel*, CKTcircuit*, SPcomplex*);
//extern int  MEM2sAcLoad(GENmodel*, CKTcircuit*);
//extern int  MEM2sLoad(GENmodel*, CKTcircuit*);
//extern int MEM2sSetup(SENstruct*, GENmodel*);
//extern void MEM2sPrint(GENmodel*, CKTcircuit*);
extern int MEMR2setup(SMPmatrix*, GENmodel*, CKTcircuit*, int*);
extern int MEMR2temp(GENmodel*, CKTcircuit*);
extern int MEMR2trunc(GENmodel*, CKTcircuit*, double *);
extern int MEMR2noise(int, int, GENmodel*, CKTcircuit*, Ndata*, double*);
//extern int MEM2soaCheck(CKTcircuit*, GENmodel*);
extern int MEMR2findBr(CKTcircuit*, GENmodel*, IFuid);
extern int MEMR2getic(GENmodel*, CKTcircuit*);
extern int MEMR2convTest(GENmodel*, CKTcircuit*);