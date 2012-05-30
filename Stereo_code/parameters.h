#ifndef _PARAMETERS_H_INCLUDED_
#define _PARAMETERS_H_INCLUDED_

#define CANVASLEFT1 10
#define CANVASTOP1  10
#define CANVASLEFT2 10
#define CANVASTOP2  500

//Error codes
#define NO_EPT -3;

//Macros
#define SWAP_INT(A,B) {\
	A^=B;\
	B^=A;\
	A^=B;\
	}


#endif /* _PARAMETERS_H_INCLUDED_ */