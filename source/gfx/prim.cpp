///**********************/
/*** Psx Prim Stuff ***/
/**********************/

#include "system\global.h"
#include "gfx\TPage.h"
#include "gfx\AnimTex.h"
#include "gfx\Prim.h"
#include "system\vid.h"

/*****************************************************************************/
sOT 		*OtList[2],*OtPtr;
u32			DmaStart[2];
u8			*PrimBuffer[2],*PrimListStart,*PrimListEnd;
u8			*CurrPrim,*EndPrim;
int			PrimFlipFlag;

/*****************************************************************************/
void	PrimInit()
{
// Alloc Lists
	OtList[0]=(sOT*)MemAlloc(MAX_OT*2*sizeof(sOT), "-Ot-");
	OtList[1]=OtList[0]+MAX_OT;
	PrimBuffer[0]=(u8*)MemAlloc(PRIMPOOL_SIZE*2, "Prim");
	PrimBuffer[1]=PrimBuffer[0]+(PRIMPOOL_SIZE);

	PrimFlipFlag=0;
	OtPtr=(sOT*)OtList[PrimFlipFlag];
	CurrPrim=(u8*)PrimBuffer[PrimFlipFlag];
	EndPrim=CurrPrim+(PRIMPOOL_SIZE);

	PrimListStart=PrimBuffer[0];
	PrimListEnd=PrimListStart+(PRIMPOOL_SIZE*2);

	InitOTagR(OtList[0],MAX_OT);
	InitOTagR(OtList[1],MAX_OT);

}


/*****************************************************************************/
void	PrimDisplay()
{
	CAnimTex::AnimateTex();
	UnlinkOTagR(OtPtr, MAX_OT, &DmaStart[PrimFlipFlag]);

#ifdef	USE_NTAGS
	DrawOTag((u32*)&DmaStart[PrimFlipFlag]);
#else
	DrawOTag(OtPtr+(MAX_OT-1));
#endif

	PrimFlipFlag^=1;
	OtPtr=(sOT*)OtList[PrimFlipFlag];
	CurrPrim=(u8*)PrimBuffer[PrimFlipFlag];
	EndPrim=CurrPrim+(PRIMPOOL_SIZE);
	ResetOTagR(OtPtr,MAX_OT);
}

/*** Clipping ****************************************************************/
void	PrimClip(RECT *R, u32 Depth)
{
DR_AREA 	*CPtr=(DR_AREA*)CurrPrim;
RECT 		Real;
sVidScreen	*Scr=VidGetDrawScreen();

	Real=*R;
	CurrPrim+=sizeof(DR_AREA);
	Real.x+=Scr->Draw.clip.x;
	Real.y+=Scr->Draw.clip.y;

	int xLimit=512;
	int yLimit=((FrameFlipFlag^1)+1)*256;
	if(Real.x+Real.w>xLimit)Real.w=xLimit-Real.x;
	if(Real.y+Real.h>yLimit)Real.h=yLimit-Real.y;

	SetDrawArea(CPtr,&Real);
	AddPrimToList(CPtr,Depth);
};

/*---------------------------------------------------------------------------*/
void PrimFullScreen(int Depth)
{
RECT	FullScreenRect={0,0,VidGetScrW(),VidGetScrH()};
		PrimClip(&FullScreenRect,Depth);
}

/*****************************************************************************/
/*** NTag Functions **********************************************************/
/*****************************************************************************/
#ifdef	USE_NTAGS
void 	ClearNTag(sOT *Ptr, long Count)
{
   for (; Count; Ptr++,Count--) Ptr->FirstPrim = NULL;
}

/*****************************************************************************/
void 	UnlinkNTag(sOT *Ptr, long Count, u32 *StartOt)
{
u32 	*Tag;
		for (; Count; Ptr++,Count--)
			{
			if ((Tag = Ptr->FirstPrim) == NULL) continue;
			setaddr(StartOt, Tag);
			StartOt = Ptr->LastPrim;
			Ptr->FirstPrim = NULL;			// Reset while Im at it!
			}
		termPrim(StartOt);
}

/*****************************************************************************/
void 	UnlinkNTagR(sOT *Ptr, long Count, u32 *StartOt)
{
u32 	*Tag;
		Ptr+=Count-1;
		for (; Count; Ptr--,Count--)
			{
			if ((Tag = Ptr->FirstPrim) == NULL) continue;
			setaddr(StartOt, Tag);
			StartOt = Ptr->LastPrim;
			Ptr->FirstPrim = NULL;			// Reset while Im at it!
			}
		termPrim(StartOt);
}

#endif

/*****************************************************************************/
LINE_F2		*DrawLine(int _x0,int _y0,int _x1,int _y1,int _r,int _g,int _b,int _ot)
{
	LINE_F2		*lf2=GetPrimLF2();
	lf2->x0=_x0;
	lf2->y0=_y0;
	lf2->x1=_x1;
	lf2->y1=_y1;
	setRGB0(lf2,_r,_g,_b);
	AddPrimToList(lf2,_ot);
	return lf2;
}

/*****************************************************************************/
LINE_G2		*DrawGLine(int _x0,int _y0,int _x1,int _y1,int _r1,int _g1,int _b1,int _r2,int _g2,int _b2,int _ot)
{
	LINE_G2		*lg2=GetPrimLG2();
	lg2->x0=_x0;
	lg2->y0=_y0;
	lg2->x1=_x1;
	lg2->y1=_y1;
	setRGB0(lg2,_r1,_g1,_b1);
	setRGB1(lg2,_r2,_g2,_b2);
	AddPrimToList(lg2,_ot);
	return lg2;
}
