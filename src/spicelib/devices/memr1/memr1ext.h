/**********
Comment by Bo Li
this header file is called by memdefs.h, modified by resext.h
need further modify the parser interface 
---20190610

**********/
	
/**********
Copyright 2020 Shanghai Jiao Tong University.  All rights reserved.
Author: Bo Li  at MSDA Lab, School of Electronic Information and Electrical Engineering 	
Modified: 2020/09/09  Bo Li
Refered to NgSPICE Res/Cap related file
**********/



extern int MEMR1ask(CKTcircuit*,GENinstance*,int,IFvalue*,IFvalue*);
extern int MEMR1load(GENmodel*,CKTcircuit*);
extern int MEMR1acLoad(GENmodel*,CKTcircuit*);
extern int MEMR1modAsk(CKTcircuit*,GENmodel*,int,IFvalue*);
extern int MEMR1mParam(int,IFvalue*,GENmodel*);
extern int MEMR1param(int,IFvalue*,GENinstance*,IFvalue*);

extern int MEMR1setup(SMPmatrix*,GENmodel*,CKTcircuit*,int*);
extern int MEMR1temp(GENmodel*,CKTcircuit*);
extern int MEMR1trunc(GENmodel*, CKTcircuit*, double *);
extern int MEMR1noise(int,int,GENmodel*,CKTcircuit*,Ndata*,double*);
//extern int MEMsoaCheck(CKTcircuit *, GENmodel *);
extern int MEMR1findBr(CKTcircuit*, GENmodel*, IFuid);
extern int MEMR1getic(GENmodel*, CKTcircuit*);
extern int MEMR1convTest(GENmodel *, CKTcircuit *);
