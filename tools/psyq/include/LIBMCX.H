#ifndef _LIBMCX_H_
#define _LIBMCX_H_

/*
 *	File:libmcx.h
 *	Copyright (C) 1998 by Sony Computer Entertainment Inc.
 *			All rights Reserved
 */
/*
 * $PSLibId: Run-time Library Release 4.7$
 */

/* don't change these macros and structures which is referred in mcx code */ 

#ifndef TRUE
#define TRUE		1
#endif
#ifndef FALSE
#define FALSE		0
#endif
#ifndef NULL
#define NULL		0
#endif
#ifndef ERROR
#define ERROR		(-1)
#endif


#define McxFuncGetApl		1
#define McxFuncExecApl		2
#define McxFuncGetTime		3
#define McxFuncGetMem		4
#define McxFuncSetMem		5
#define McxFuncShowTrans	6
#define McxFuncHideTrans	7
#define McxFuncCurrCtrl		8
#define McxFuncSetLED		9
#define McxFuncGetSerial	10
#define McxFuncExecFlag		11
#define McxFuncAllInfo		12
#define McxFuncFlashAcs		13
#define McxFuncReadDev		14
#define McxFuncWriteDev		15
#define McxFuncGetUIFS		16
#define McxFuncSetUIFS		17
#define McxFuncSetTime		18
#define McxFuncCardType		19


#define McxSyncRun	0
#define McxSyncNone	(-1)
#define McxSyncFin	1

#define McxErrSuccess	0
#define McxErrNoCard	1
#define McxErrInvalid	2
#define McxErrNewCard	3



/*
 * Prototypes
 */

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

void McxStartCom(void);
void McxStopCom(void);
int McxSync(int, long *, long *);

int McxGetApl(int , long *);
int McxExecApl(int, int, long);
int McxGetTime(int, unsigned char *);
int McxGetMem(int, unsigned char *, unsigned, unsigned);
int McxSetMem(int, unsigned char *, unsigned, unsigned);
int McxShowTrans(int, int, int);
int McxHideTrans(int);
int McxCurrCtrl(int, int, int, int);
int McxFlashAcs(int, int);
int McxGetSerial(int, unsigned long *);
int McxSetLED(int, int);
int McxAllInfo(int,  unsigned char *);
int McxExecFlag(int, int, int);
int McxReadDev(int, int, unsigned char *, unsigned char *);
int McxWriteDev(int, int, unsigned char *, unsigned char *);
int McxSetTime(int, unsigned char *);
int McxGetUIFS(int, unsigned char *);
int McxSetUIFS(int, unsigned char *);
int McxCardType(int);


#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* _LIBMCX_H_ */

/* don't add stuff after this */

