/******************/
/*** Prim Stuff ***/
/******************/

#ifndef __PRIM_HEADER__
#define __PRIM_HEADER__

#ifndef		_GLOBAL_HEADER_
#include	"system\global.h"
#endif
#ifndef		__TPAGE_H__
#include	"gfx\tpage.h"
#endif
#ifndef		__PRIMPLUS_HEADER__
#include	"gfx\primplus.h"
#endif

#define	MAX_OT				(16)
#define	MAX_PRIMS			(1024*2)

//#define	USE_NTAGS			1


/************************************************************************************/
#define	GPU_PolyF3Tag		(4)
#define	GPU_PolyF3Code		(0x20)
#define	GPU_PolyF4Tag		(5)
#define	GPU_PolyF4Code		(0x28)
#define	GPU_PolyFT3Tag		(7)
#define	GPU_PolyFT3Code		(0x24)
#define	GPU_PolyFT4Tag		(9)
#define	GPU_PolyFT4Code		(0x2c)
#define	GPU_PolyG4Tag		(8)
#define	GPU_PolyG4Code		(0x38)
#define	GPU_PolyGT3Tag		(9)
#define	GPU_PolyGT3Code		(0x34)
#define	GPU_PolyGT4Tag		(12)
#define	GPU_PolyGT4Code		(0x3c)

#define	GPUCode_ShadeTex	(1<<0)	// Setting this **DISABLES** texture shading.
#define	GPUCode_SemiTrans	(1<<1)	// Setting this enables semi-transparent mode
#define	GPUCode_Textured	(1<<2)
#define	GPUCode_Quad		(1<<3)
#define	GPUCode_Gouraud		(1<<4)
#define	GPUCode_Global		(1<<5)

/******************************************************************************/
#define	GetPrimSpace(primtype,Ptr)		((primtype *)Ptr); Ptr+=sizeof(primtype);

#define setSemiTransPolyF3(p)	setlen(p, 4),  setcode(p, 0x20|GPUCode_SemiTrans)
#define setSemiTransPolyFT3(p)	setlen(p, 7),  setcode(p, 0x24|GPUCode_SemiTrans)
#define setSemiTransPolyG3(p)	setlen(p, 6),  setcode(p, 0x30|GPUCode_SemiTrans)
#define setSemiTransPolyGT3(p)	setlen(p, 9),  setcode(p, 0x34|GPUCode_SemiTrans)
#define setSemiTransPolyF4(p)	setlen(p, 5),  setcode(p, 0x28|GPUCode_SemiTrans)
#define setSemiTransPolyFT4(p)	setlen(p, 9),  setcode(p, 0x2c|GPUCode_SemiTrans)
#define setSemiTransPolyG4(p)	setlen(p, 8),  setcode(p, 0x38|GPUCode_SemiTrans)
#define setSemiTransPolyGT4(p)	setlen(p, 12), setcode(p, 0x3c|GPUCode_SemiTrans)

#define setShadeTexPolyFT3(p)	setlen(p, 7),  setcode(p, 0x24|GPUCode_ShadeTex)
#define setShadeTexPolyFT4(p)	setlen(p, 9),  setcode(p, 0x2c|GPUCode_ShadeTex)
#define setShadeTexPolyGT3(p)	setlen(p, 9),  setcode(p, 0x34|GPUCode_ShadeTex)
#define setShadeTexPolyGT4(p)	setlen(p, 12), setcode(p, 0x3c|GPUCode_ShadeTex)

#define setSemiTransShadeTexPolyFT3(p)	setlen(p, 7),  setcode(p, 0x24|GPUCode_SemiTrans|GPUCode_ShadeTex)
#define setSemiTransShadeTexPolyFT4(p)	setlen(p, 9),  setcode(p, 0x2c|GPUCode_SemiTrans|GPUCode_ShadeTex)

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
#define addPrimNoCheck(OT,Prim)					\
{												\
	setaddr(((u32*)Prim), (OT)->FirstPrim);		\
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

#define GetPrimSPRT8() 		GetPrim(SPRT_8);\
							setSprt8(CurrPrim-sizeof(SPRT_8))

#define GetPrimSPRT16() 	GetPrim(SPRT_16);\
							setSprt16(CurrPrim-sizeof(SPRT_16))

#define GetPrimSPRT() 		GetPrim(SPRT);\
							setSprt(CurrPrim-sizeof(SPRT))

#define GetPrimTILE8() 		GetPrim(TILE_8);\
							setTile(CurrPrim-sizeof(TILE_8))

#define GetPrimTILE16() 	GetPrim(TILE_16);\
							setTile(CurrPrim-sizeof(TILE_16))

#define GetPrimTILE() 		GetPrim(TILE);\
							setTile(CurrPrim-sizeof(TILE))

// Extra prims :o)
#define GetPrimTF3() 		GetPrim(TPOLY_F3);\
				   			setTPolyF3(CurrPrim-sizeof(TPOLY_F3))

#define GetPrimTF4() 		GetPrim(TPOLY_F4);\
				   			setTPolyF4(CurrPrim-sizeof(TPOLY_F4))

#define GetPrimTG3()		GetPrim(TPOLY_G3);\
				   			setTPolyG3(CurrPrim-sizeof(TPOLY_G3))

#define GetPrimTG4() 		GetPrim(TPOLY_G4);\
				   			setTPolyG4(CurrPrim-sizeof(TPOLY_G4))

#define GetPrimTLF2() 		GetPrim(TLINE_F2);  \
							setTLineF2(CurrPrim-sizeof(TLINE_F2))

#define GetPrimTLF3() 		GetPrim(TLINE_F3);\
							setTLineF3(CurrPrim-sizeof(TLINE_F3))

#define GetPrimTLF4() 		GetPrim(TLINE_F4);\
							setTLineF4((TLINE_F4*)CurrPrim-1)

#define GetPrimTLG2() 		GetPrim(TLINE_G2);\
							setTLineG2(CurrPrim-sizeof(TLINE_G2))

#define GetPrimTLG3() 		GetPrim(TLINE_G3);\
							setTLineG3(CurrPrim-sizeof(TLINE_G3))

#define GetPrimTLG4() 		GetPrim(TLINE_G4);\
							setTLineG4(CurrPrim-sizeof(TLINE_G4))

#define GetPrimTSPRT8() 	GetPrim(TSPRT8);\
							setTSprt8(CurrPrim-sizeof(TSPRT8))

#define GetPrimTSPRT16() 	GetPrim(TSPRT16);\
							setTSprt16(CurrPrim-sizeof(TSPRT16))

#define GetPrimTSPRT() 		GetPrim(TSPRT);\
							setTSprt(CurrPrim-sizeof(TSPRT))

#define GetPrimTTILE8() 	GetPrim(TTILE8);\
							setTile(CurrPrim-sizeof(TTILE8))

#define GetPrimTTILE16() 	GetPrim(TTILE16);\
							setTile(CurrPrim-sizeof(TTILE16))

#define GetPrimTTILE() 		GetPrim(TTILE);\
							setTile(CurrPrim-sizeof(TTILE))


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
//extern int	PrimFlipFlag;

/********************************************************************************************************/
void 			PrimInit();

void 			PrimDisplay();

void 			PrimClip(RECT *r, u32 Depth);
void 			PrimFullScreen(int Depth);
inline	u8		*GetPrimPtr()			{return(CurrPrim);}
inline	void	SetPrimPtr(u8 *Ptr)		{CurrPrim=Ptr;}

LINE_F2			*DrawLine(int _x0,int _y0,int _x1,int _y1,int _r,int _g,int _b,int _ot);
LINE_G2			*DrawGLine(int _x0,int _y0,int _x1,int _y1,int _r1,int _g1,int _b1,int _r2,int _g2,int _b2,int _ot);

/********************************************************************************************************/
/*** Inlines ********************************************************************************************/
/********************************************************************************************************/
inline	void 		AddPrimToList(void *Prim,u32 Depth)
{
			ASSERT(Depth<MAX_OT);
			addPrim(OtPtr+Depth,(u32*)Prim);
}

/*-----------------------------------------------------------------------------------------------------*/
inline	void	GetFrameUV(sFrameHdr *Fr, u8 *U,u8 *V)	{*U=Fr->U;*V=Fr->V;}
inline	void	GetFrameUVWH(sFrameHdr *Fr,u8 *U,u8 *V,u8 *W,u8 *H)	{*U=Fr->U; *V=Fr->V; *W=Fr->W; *H=Fr->H; }
inline	void	GetFrameWH(sFrameHdr *Fr,u8 *W,u8 *H)				{*W=Fr->W; *H=Fr->H;}
inline	int		GetFrameClut(sFrameHdr *Fr)							{return(Fr->Clut);}
inline	int		GetFrameTPage(sFrameHdr *Fr)						{return(Fr->TPage);}

#endif



