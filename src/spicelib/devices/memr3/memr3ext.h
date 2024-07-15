/**********
Copyright 1990 Regents of the University of California.  All rights reserved.
Author: 1985 Thomas L. Quarles
**********/
/**********
Comment by Bo Li
this header file is called by memdefs.h, modified by resext.h
need further modify the parser interface 
---20210520 

**********/
/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2021/05/20  Bo Li
Refered to NgSPICE Res/Cap related file
**********/



extern int MEMR3ask(CKTcircuit*, GENinstance*, int, IFvalue*, IFvalue*);
extern int MEMR3load(GENmodel*, CKTcircuit*);
//extern int MEMR3acload(GENmodel*, CKTcircuit*);
extern int MEMR3modAsk(CKTcircuit*, GENmodel*, int, IFvalue*);
extern int MEMR3mParam(int,IFvalue*, GENmodel*);
extern int MEMR3param(int,IFvalue*, GENinstance*, IFvalue*);
//extern int MEMR3pzLoad(GENmodel*, CKTcircuit*, SPcomplex*);
//extern int  MEMR3sAcLoad(GENmodel*, CKTcircuit*);
//extern int  MEMR3sLoad(GENmodel*, CKTcircuit*);
//extern int MEMR3sSetup(SENstruct*, GENmodel*);
//extern void MEMR3sPrint(GENmodel*, CKTcircuit*);
extern int MEMR3setup(SMPmatrix*, GENmodel*, CKTcircuit*, int*);
extern int MEMR3temp(GENmodel*, CKTcircuit*);
extern int MEMR3trunc(GENmodel*, CKTcircuit*, double *);
//extern int MEMR3noise(int, int, GENmodel*, CKTcircuit*, Ndata*, double*);
//extern int MEMR3soaCheck(CKTcircuit*, GENmodel*);
extern int MEMR3findBr(CKTcircuit*, GENmodel*, IFuid);
extern int MEMR3getic(GENmodel*, CKTcircuit*);
extern int MEMR3convTest(GENmodel*, CKTcircuit*);