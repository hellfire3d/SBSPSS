/******************/
/*** Prim Stuff ***/
/******************/

#ifndef __PRIM_HEADER__
#define __PRIM_HEADER__

#ifndef _GLOBAL_HEADER_
#include	"system\global.h"
#endif
#include	"gfx\gpugte.h"
#include	"gfx\tpage.h"

#define	MAX_OT				(2048)
#define	MAX_PRIMS			(2048+512)

#define	USE_NTAGS			1

/*** Fast Replacements *********************************************************************************/
#undef	setaddr
#undef	getaddr
#define set3(r0,r1) 		({ __asm__ ( "swl %1, 2( %0 )" : : "r"( r0 ), "r"( r1 ) : "memory" ); })
#define get3(r0) 			({ unsigned long t; __asm__ ( "lwl %0, 2( %1 )" : "=r"(t) : "r"( r0) : "memory" ); t; })
#define setaddr(_p0,_p1)	set3((_p0), ((u32) (_p1)) << 8)
#define getaddr(_p)			(get3(_p) >> 8)

#undef	catPrim
#define	CatPrim	catPrim
#define	catPrim( r0, r1 ) __asm__ volatile (	\
	"sll	$12, %1, 8;"						\
	"swl	$12, 2( %0 )"						\
	:											\
	: "r"( r0 ), "r"( r1 )						\
	: "$12", "memory" )


#ifdef	USE_NTAGS
/*** NTag Stuff **************************************************************************************/
struct sOT
{
	u32	*FirstPrim,*LastPrim;
};

#undef	addPrim
#define	AddPrim		addPrim
#define addPrim(OT,Prim)						\
{												\
   if ((OT)->FirstPrim)							\
      setaddr(((u32*)Prim), (OT)->FirstPrim);	\
   else											\
      (OT)->LastPrim = (u32*)(Prim);			\
   (OT)->FirstPrim = (u32*)(Prim);				\
}

#define NTAG_addPrims(_nt,_ps,_pe) 				\
	{											\
	   if ((_nt)->FirstPrim)					\
		  setaddr((_pe), (_nt)->FirstPrim);	\
	   else										\
		  (_nt)->LastPrim = (_pe); 				\
	   (_nt)->FirstPrim = (_ps);				\
	}


void 	ClearNTag(sOT *Ptr, long Count);
void 	UnlinkNTag(sOT *Ptr, long Count, u32 *FirstNT);
void 	UnlinkNTagR(sOT *Ptr, long Count, u32 *FirstNT);

inline void UnlinkNTagtoNTag(sOT *to, sOT *from, long count)
{
   sOT *ptr = from;
   for (; count; ptr++,count--)			// for all NTAGs...
   {
	  u_long *tag = ptr->FirstPrim;

	  if (tag == NULL)	// if it's blank...
		 continue;				// ...skip past it...

	  NTAG_addPrims(to, ptr->FirstPrim, ptr->LastPrim);

	  ptr->FirstPrim = NULL;
   }
}


#define	InitOTag(Ot, Count)				ClearNTag(Ot,Count);
#define	InitOTagR(Ot, Count)			ClearNTag(Ot,Count);
#define	ResetOTag(Ot, Count)			;
#define	ResetOTagR(Ot, Count)			;
#define	UnlinkOTag(Ot, Count, Dma)		UnlinkNTag(Ot, Count, Dma);
#define	UnlinkOTagR(Ot, Count, Dma)		UnlinkNTagR(Ot, Count, Dma);


#else
/*** OTag Stuff **************************************************************************************/
typedef	u32	sOT;

#undef	addPrim
#define	AddPrim		addPrim
#define	addPrim( r0, r1 ) __asm__  (			\
	"lwl	$12, 2( %0 );"						\
	"sll	$13, %1, 8;"						\
	"swl	$13, 2( %0 );"						\
	"swl	$12, 2( %1 )"						\
	:											\
	: "r"( r0 ), "r"( r1 )						\
	: "$12", "$13", "memory" )

#undef	addPrims
#define	AddPrims	addPrims
#define	addPrims( r0, r1, r2 ) __asm__  (		\
	"lwl	$12, 2( %0 );"						\
	"sll	$13, %1, 8;"						\
	"swl	$13, 2( %0 );"						\
	"swl	$12, 2( %2 )"						\
	:											\
	: "r"( r0 ), "r"( r1 ), "r"( r2 )			\
	: "$12", "$13", "memory" )

#define	InitOTag(Ot, Count)						ClearOTag(Ot,Count);
#define	InitOTagR(Ot, Count)					ClearOTagR(Ot,Count);
#define	ResetOTag(Ot, Count)					InitOTag(Ot,Count);
#define	ResetOTagR(Ot, Count)					InitOTagR(Ot,Count);
#define	UnlinkOTag(OtPtr, MAX_OT, Dma)			;
#define	UnlinkOTagR(OtPtr, MAX_OT, Dma)			;


#endif
/*** Main Prim Stuff **********************************************************************************/
#define GetPrim(Ptype) 		GetPrimSpace(Ptype,CurrPrim);

#define GetPrimF3() 		GetPrim(POLY_F3);\
				   			setPolyF3(CurrPrim-sizeof(POLY_F3))

#define GetPrimF4() 		GetPrim(POLY_F4);\
				   			setPolyF4(CurrPrim-sizeof(POLY_F4))

#define GetPrimFT3() 		GetPrim(POLY_FT3);\
							setPolyFT3(CurrPrim-sizeof(POLY_FT3))

#define GetPrimFT4() 		GetPrim(POLY_FT4);\
							setPolyFT4(CurrPrim-sizeof(POLY_FT4))

#define GetPrimG3()			GetPrim(POLY_G3);\
				   			setPolyG3(CurrPrim-sizeof(POLY_G3))

#define GetPrimG4() 		GetPrim(POLY_G4);\
				   			setPolyG4(CurrPrim-sizeof(POLY_G4))

#define GetPrimGT3() 		GetPrim(POLY_GT3);\
							setPolyGT3(CurrPrim-sizeof(POLY_GT3))

#define GetPrimGT4() 		GetPrim(POLY_GT4);\
							setPolyGT4(CurrPrim-sizeof(POLY_GT4))

#define GetPrimLF2() 		GetPrim(LINE_F2);  \
							setLineF2(CurrPrim-sizeof(LINE_F2))

#define GetPrimLF3() 		GetPrim(LINE_F3);\
							setLineF3(CurrPrim-sizeof(LINE_F3))

#define GetPrimLF4() 		GetPrim(LINE_F4);\
							setLineF4((LINE_F4*)CurrPrim-1)

#define GetPrimLG2() 		GetPrim(LINE_G2);\
							setLineG2(CurrPrim-sizeof(LINE_G2))

#define GetPrimLG3() 		GetPrim(LINE_G3);\
							setLineG3(CurrPrim-sizeof(LINE_G3))

#define GetPrimLG4() 		GetPrim(LINE_G4);\
							setLineG4(CurrPrim-sizeof(LINE_G4))

/********************************************************************************************************/
#define	OtInRange			(MAX_OT-1)
#define	OtOutRange			(0xffffffff-OtInRange)
#define	OTCheck(x)			(x&OtOutRange)
#define	IsInOTRange(x)		(!OTCheck(x))

/********************************************************************************************************/
#define	MAX_PRIM_SIZE		(sizeof(POLY_FT4))
#define	PRIMPOOL_SIZE		(MAX_PRIMS*MAX_PRIM_SIZE)
#define	OTLIST_SIZE			(MAX_OT*sizeof(sOT))

/********************************************************************************************************/
extern sOT 	*OtPtr;
extern u8 	*CurrPrim,*EndPrim;
extern u8	*PrimListStart,*PrimListEnd;
extern int	PrimFlipFlag;

/********************************************************************************************************/
const u32 PrimSXY_ClipCode = (256 << 16) | 512;

/********************************************************************************************************/
void 		PrimInit();

void 		PrimDisplay();
void 		PrimClip(RECT *r, u32 Depth);
void 		PrimFullScreen(int Depth);
inline	u8			*GetPrimPtr()			{return(CurrPrim);}
inline	void		SetPrimPtr(u8 *Ptr)		{CurrPrim=Ptr;}

void		SetPrimCheck();

LINE_F2		*DrawLine(int _x0,int _y0,int _x1,int _y1,int _r,int _g,int _b,int _ot);
LINE_G2		*DrawGLine(int _x0,int _y0,int _x1,int _y1,int _r1,int _g1,int _b1,int _r2,int _g2,int _b2,int _ot);

/********************************************************************************************************/
/*** Inlines ********************************************************************************************/
/********************************************************************************************************/
inline	void 		AddPrimToList(void *Prim,u32 Depth)
{
			ASSERT(Depth<MAX_OT);
			addPrim(OtPtr+Depth,(u32*)Prim);
}

/*-----------------------------------------------------------------------------------------------------*/
inline	void		GetFrameUV(sFrameHdr *Fr, u8 *U,u8 *V)
{
			*U=Fr->U;
			*V=Fr->V;
}

/*-----------------------------------------------------------------------------------------------------*/
inline	void		GetFrameUVWH(sFrameHdr *Fr,u8 *U,u8 *V,u8 *W,u8 *H)
{
			*U=Fr->U;
			*V=Fr->V;
			*W=Fr->W;
			*H=Fr->H;
}

/*-----------------------------------------------------------------------------------------------------*/
inline	void		GetFrameWH(sFrameHdr *Fr,u8 *W,u8 *H)
{
			*W=Fr->W;
			*H=Fr->H;
}

/*-----------------------------------------------------------------------------------------------------*/
inline	int			GetFrameClut(sFrameHdr *Fr)
{
			return(Fr->Clut);
}


/*-----------------------------------------------------------------------------------------------------*/
inline	int			GetFrameTPage(sFrameHdr *Fr)
{
			return(Fr->TPage);
}

#endif



