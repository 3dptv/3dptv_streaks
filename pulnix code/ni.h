#define CANVASTOP  90     // top of the display area
#define CANVASLEFT 25     // left of the display area

/*
typedef struct Connection_IDs{
	SESSION_ID   Sid;
	BUFLIST_ID   Bid;
	INTERFACE_ID Iid;
} ;
*/

struct Frame{
	Int32		 AcqWinTop;
	Int32        AcqWinLeft;
	Int32        AcqWinWidth;
	Int32        AcqWinHeight;
};
