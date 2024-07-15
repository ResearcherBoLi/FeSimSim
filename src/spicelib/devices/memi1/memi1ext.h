
/**********
Comment by Bo Li
this header file is called by memi1defs.h, modified by resext.h
need further modify the parser interface 
---20191024 

**********/



extern int MEMI1ask(CKTcircuit*, GENinstance*, int, IFvalue*, IFvalue*);
extern int MEMI1load(GENmodel*, CKTcircuit*);
//extern int MEMI1acload(GENmodel*, CKTcircuit*);
extern int MEMI1modAsk(CKTcircuit*, GENmodel*, int, IFvalue*);
extern int MEMI1mParam(int,IFvalue*, GENmodel*);
extern int MEMI1param(int,IFvalue*, GENinstance*, IFvalue*);
//extern int MEMI1pzLoad(GENmodel*, CKTcircuit*, SPcomplex*);
//extern int  MEMI1sAcLoad(GENmodel*, CKTcircuit*);
//extern int  MEMI1sLoad(GENmodel*, CKTcircuit*);
//extern int MEMI1sSetup(SENstruct*, GENmodel*);
//extern void MEMI1sPrint(GENmodel*, CKTcircuit*);
extern int MEMI1setup(SMPmatrix*, GENmodel*, CKTcircuit*, int*);
extern int MEMI1temp(GENmodel*, CKTcircuit*);
extern int MEMI1trunc(GENmodel*, CKTcircuit*, double *);
//extern int MEMI1noise(int, int, GENmodel*, CKTcircuit*, Ndata*, double*);
//extern int MEMI1soaCheck(CKTcircuit*, GENmodel*);
extern int MEMI1findBr(CKTcircuit*, GENmodel*, IFuid);
extern int MEMI1getic(GENmodel*, CKTcircuit*);
extern int MEMI1convTest(GENmodel*, CKTcircuit*);