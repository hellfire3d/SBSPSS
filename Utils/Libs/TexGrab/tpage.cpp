/*=========================================================================

	FILENAME.CPP	

	Author:  Gary Liddon @
	Created:
	Project:
	Purpose:

	Copyright (c) 1997 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <algorithm>
#include "conio.h"

/*	Glib
	---- */
#include "gutils.h"

/*	Local
	----- */
#include "tpage.h"
#include "grect.h"

/*	Graphics
	-------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

using namespace std;


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
struct TPInfo
{
	int		XAlign;
	int		TpWidth;
	int		BorderX;
	int		BorderY;
};

/*	Information about where I can place this frame in the Tpage
	----------------------------------------------------------- */
/*----------------------------------------------------------------------
	Vars
	---- */
static TPInfo const	InfStruct[TP_NUM_OF_TP_TYPES]=
{
	{1,256,1,1},			/* TP_4 */
	{2,512,2,1},			/* TP_8 */
	{4,256,4,1},			/* TP_16 */
	{0,0,0,0},				/* TP_SCREEN */
	{16*4,1024*4,0,0},		/* TP_PAL */
};

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */
int	TPRect::W2Alloc=1;
int	TPRect::H2Alloc=1;

/*----------------------------------------------------------------------
	Data
	---- */

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
VRAM::VRAM(int nTpWidth,int nTpHeightInPixels)
{
	Big2Little=false;
	RotateEveryEmpty=false;


	TpWidth=-1;
	TpHeightInPixels=-1;

	Clear(nTpWidth,nTpHeightInPixels);
}

void VRAM::Clear(int nTpWidth,int nTpHeightInPixels)
{
	if (nTpWidth >= 0 )
		TpWidth=nTpWidth;

	if (nTpHeightInPixels >= 0)
		TpHeightInPixels=nTpHeightInPixels;

	InitEmpty(0,0,TpWidth*256,TpHeightInPixels);
}


/*----------------------------------------------------------------------
	Function:	void VRAM::InitEmpty(void)
	Purpose:	Initialise the VRAM as empty
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void VRAM::InitEmpty(int vX,int vY,int nW,int nH)
{
	const int	Width16=256;
	const int	Height16=256;
	
	W=nW;
	H=nH;
	X=vX;
	Y=vY;

	VRAMWidthPages=W/Width16;
	VRAMHeightPages=H/Height16;

	if (!VRAMHeightPages)
	{
		VRAMHeightPages = 1;
	}

	NumOfTPages=VRAMWidthPages*VRAMHeightPages;

	VecOfPages.resize(NumOfTPages,TPRectList());

	Unused.reserve(4000);
	Used.reserve(2000);

	Unused.resize(0);
	Used.resize(0);

	for (int f=0;f<NumOfTPages;f++)
		VecOfPages[f].resize(0);

	for (int y=0;y<VRAMHeightPages;y++)
		for (int x=0;x<VRAMWidthPages;x++)
			AddEmpty((x*Width16)+vX,(y*Height16)+vY,Width16,Height16);
}

/*----------------------------------------------------------------------
	Function:	bool TryNFit(TPRectList & TpList)
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool VRAM::TryNFit(TPRectList & TpList,TPRect & ThisRect)
{
	bool		Done=false;

	TPRectList::iterator	f;


	for (f=TpList.begin();f!=TpList.end();f++)
		{
		TPRect &	BlankRect = *f;

		if (TryRect(BlankRect,ThisRect))
			return(true);
		else
			{
			if (RotateEveryEmpty && CanRotate(ThisRect))
				{
				ThisRect.SetRotate(!ThisRect.GetRotate());
		
				if (TryRect(BlankRect,ThisRect))
					return(true);
				else
					ThisRect.SetRotate(!ThisRect.GetRotate());
				}
			}
		}
		
	return(false);
}

void VRAM::PosFromTPrect(TPRect & ThisRect,POS_INFO & Pi)
{
	TPInfo const *		Tpi;
	int					WorkingWidth;

	Tpi=&InfStruct[ThisRect.TypeOfPage];
	Pi.XAlign=Tpi->XAlign;

	WorkingWidth=GU_AlignVal(ThisRect.W,Pi.XAlign);

	Pi.MinX=Tpi->BorderX;
	Pi.MinY=Tpi->BorderY;
	Pi.MaxX=W-Tpi->BorderX-WorkingWidth;
	Pi.MaxY=H-Tpi->BorderY-ThisRect.H;

	Pi.TpWidthPix=Tpi->TpWidth;

	Pi.MinXTp=Tpi->BorderX;
	Pi.MaxXTp=Tpi->TpWidth-Tpi->BorderX-WorkingWidth;

	Pi.MinYTp=Tpi->BorderY;
	Pi.MaxYTp=(256-Tpi->BorderY)-ThisRect.H;
}

bool VRAM::TryRect(TPRect & BlankRect,TPRect & ThisRect)
{
	POS_INFO		Pi;
	PosFromTPrect(ThisRect,Pi);

	int	MinTpX=Pi.MinXTp+(BlankRect.X/Pi.TpWidthPix)*Pi.TpWidthPix;
	int	MaxTpX=Pi.MaxXTp+(BlankRect.X/Pi.TpWidthPix)*Pi.TpWidthPix;
	int	MinTpY=Pi.MinYTp+(BlankRect.Y/256)*256;
	int	MaxTpY=Pi.MaxYTp+(BlankRect.Y/256)*256;

	/* Move to avoid edges of the map */
	int		MinX = MinTpX > Pi.MinX ? MinTpX : Pi.MinX;
	int		MaxX = MaxTpX < Pi.MaxX ? MaxTpX : Pi.MaxX;
	int		MinY = MinTpY > Pi.MinY ? MinTpY : Pi.MinY;
	int		MaxY = MaxTpY < Pi.MaxY ? MaxTpY : Pi.MaxY;

	int ThisX=GU_AlignVal(BlankRect.X,Pi.XAlign);
	int ThisY=BlankRect.Y;

	if (ThisX<MinX)
		ThisX=MinX;

	if (ThisX>MaxX)
		return(false);

	if (ThisY<MinY)
		ThisY=MinY;

	if (ThisY>MaxY)
		return(false);

	ThisRect.SetXY(ThisX,ThisY);

	if (!InColisionWithUsed(ThisRect))
		{
		RemovedFromUnused(ThisRect);
		AddToUsed(ThisRect);
		return(true);
		}

	return(false);
}

/*----------------------------------------------------------------------
	Function:	TPRect VRAM::AllocVRAM(TPageType nType,int nW,int nH,bool Rotated)
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

class Predicate
{
public:
	bool operator()(TPRect const & R1,TPRect const & R2) const
		{
		u32 R1Val=(R1.H<<16)|(R1.W);
		u32 R2Val=(R2.H<<16)|(R2.W);
		return (R1Val<R2Val);
		}
};

class Predicate2
{
public:
	bool operator()(TPRect const & R1,TPRect const & R2) const
		{
		u32 R1Val=(R1.H<<16)|(R1.W);
		u32 R2Val=(R2.H<<16)|(R2.W);
		return (R1Val>R2Val);
		}
};

/*----------------------------------------------------------------------
	Function:	TPRect VRAM::AllocVRAM(TpRectVec & RectVec)
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool VRAM::AllocVRAM(TPRectVec & RectVec,bool nRotateEveryEmpty,bool nBig2Little,bool nWiderThanHigher)
{
	int			f;

	RotateEveryEmpty=nRotateEveryEmpty;
	Big2Little=nBig2Little;
	WiderThanHigher=nWiderThanHigher;

	if (WiderThanHigher)
		{
		for (f=0;f<RectVec.size();f++)
			{
			if ((RectVec[f].H > RectVec[f].W) && CanRotate(RectVec[f]))
				RectVec[f].SetRotate(!RectVec[f].GetRotate());
			}
		}

	if (!Big2Little)
		std::sort(RectVec.begin(),RectVec.end(),Predicate());
	else
		std::sort(RectVec.begin(),RectVec.end(),Predicate2());

	bool	AllocedEverything=true;

	for (f=0;f<RectVec.size();f++)
		{
		if (!AllocVRAM(RectVec[f]))
			{
			AllocedEverything=false;
			cout<<"Couldn't alloc "<<RectVec[f].W<<","<<RectVec[f].H<<endl;
			}
		}

	return(AllocedEverything);
}

/*----------------------------------------------------------------------
	Function:	void VRAM::AllocVRAM(TPageType nType,int nW,int nH)
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

bool VRAM::AllocVRAM(TPRect & OriginalRect)
{
	bool	Done;
	TPRect	ThisRect;

	ThisRect=OriginalRect;

	Done=false;

	for (int Page=0;Page<NumOfTPages && !Done;Page++)
		{
		VecOfPages[Page].sort();

		TPRect::SetAllocWH(ThisRect.W,ThisRect.H);

		if (!TryNFit(VecOfPages[Page],ThisRect))
			{
			if (CanRotate(ThisRect))
				{
				ThisRect.SetRotate(!ThisRect.GetRotate());

				VecOfPages[Page].sort();
				TPRect::SetAllocWH(ThisRect.W,ThisRect.H);

				if (!TryNFit(VecOfPages[Page],ThisRect))
					ThisRect.SetRotate(!ThisRect.GetRotate());
				else
					Done=true;
				}
			}
		else
			Done=true;
		}


	if (Done)
		{
		OriginalRect=ThisRect;
		OriginalRect.SetAlloced(true);
		return(true);
		}
	else
		{
		OriginalRect.SetAlloced(false);
		return(false);
		}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool VRAM::CanRotate(TPRect & ThisRect)
{
	if (ThisRect.GetDontRotate())
		return(false);

	switch (ThisRect.TypeOfPage)
		{
		case TP_PAL:
			return(false);

		case TP_4:
		case TP_8:
		case TP_16:
			return(true);

		case TP_SCREEN:
		case TP_NUM_OF_TP_TYPES:
		default:
			Error(ERR_FATAL,"Not catered for");
			return(false);
		}
}

/*----------------------------------------------------------------------
	Function:	void VRAM::AllocVRAM(TPageType nType,int nW,int nH)
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void VRAM::RectFromTpRect(Rect & R,TPRect const & ThisRect)
{
	R.W=ThisRect.W;
	R.H=ThisRect.H;
	R.X=ThisRect.X;
	R.Y=ThisRect.Y;
}

void VRAM::RemovedFromUnused(TPRect const & ThisRect)
{
	RectVec		Result;
	Rect		NewRect;
	int			f;

	RectFromTpRect(NewRect,ThisRect);

	int		ColidedWith=0;

	for ( f=0;f<Unused.size();f++)
		{
		Rect	UnusedRect;
		
		RectFromTpRect(UnusedRect,Unused[f]);

		if (UnusedRect.IsColiding(ThisRect) )
			{
			CutRect(UnusedRect,ThisRect,Result);
			ColidedWith++;
			}	
		else
			Result.push_back(UnusedRect);
		}

	Unused.resize(0);

	for (f=0;f<NumOfTPages;f++)
		VecOfPages[f].resize(0);
	
	for (f=0;f<Result.size();f++)
		AddEmpty(Result[f].X,Result[f].Y,Result[f].W,Result[f].H);
}

void VRAM::AddEmpty(int x,int y,int w,int h)
{
	Unused.resize(Unused.size()+1);

	TPRect & ThisRect=(Unused[Unused.size()-1]);

	ThisRect.TypeOfPage=TP_4;
	ThisRect.W=w;
	ThisRect.H=h;
	ThisRect.X=x;
	ThisRect.Y=y;

	int	TpNum=(x/256)+((y/256)*VRAMWidthPages);



	VecOfPages[TpNum].insert(VecOfPages[TpNum].end(),ThisRect);
}

void VRAM::AddToUsed(TPRect const & ThisRect)
{

	Used.push_back(ThisRect);
}


bool VRAM::InColisionWithUsed(TPRect const & R)
{
	for (int f=0;f<Used.size();f++)
		{
		if (R.IsColiding(Used[f]))
			return(true);
		}

	return(false);
}

/*----------------------------------------------------------------------
	Function:	bool VRAM::CheckValidAlloc(TPageType nType,int nW,int nH)
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool VRAM::CheckValidAlloc(TPageType nType,int nW,int nH)
{
	return(true);
}

/*----------------------------------------------------------------------
	Function:	int VRAM::GetMaxWidthTp(TPageType nType)
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int VRAM::GetMaxWidthTp(TPageType nType)
{
	int		RetVal;

	switch (nType)
		{
		case TP_4:
			RetVal=1;
			break;

		case TP_8:
			RetVal=2;
			break;

		case TP_16:
			RetVal=4;
			break;

		case TP_SCREEN:
			RetVal=256;
			break;

		default:
			RetVal=-1;
			Error(ERR_FATAL,"Illegal tpage type");
			break;
		}

	return(RetVal);
}

/*----------------------------------------------------------------------
	Function:	int VRAM::GetXAlign(TPageType nType)
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int VRAM::GetXAlign(TPageType nType)
{
	int		RetVal;

	switch (nType)
		{
		case TP_4:
			RetVal=1;
			break;

		case TP_8:
			RetVal=2;
			break;

		case TP_16:
			RetVal=4;
			break;
		
		case TP_PAL:
			RetVal=16*4;
			break;

		case TP_SCREEN:
			RetVal=4;
			break;
		
		default:
			RetVal=-1;
			Error(ERR_FATAL,"Illegal tpage type");
			break;
		}

	return(RetVal);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
TPRect & VRAM::NewItem(TPRectVec & TPRects)
{
	TPRects.resize(TPRects.size()+1);
	return(TPRects[TPRects.size()-1]);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
TPRect::TPRect(void)
{	
	InitRect();
}

void TPRect::InitRect(void)
{
	X=0;
	Y=0;
	W=0;
	H=0;
	TypeOfPage=TP_4;
	Rotated=false;
	Alloced=false;
	DontRotate=false;
}

TPRect::TPRect(TPageType nType,int nW,int nH)
{
	int WidthMul;

	InitRect();

	WidthMul=GetWidthMul(nType);

	W=nW*WidthMul;
	H=nH;
	TypeOfPage=nType;
}

int TPRect::GetWidthMul(TPageType nType)
{
	int WidthMul;

	switch (nType)
		{
		case TP_4:
			WidthMul=1;
			break;

		case TP_8:
			WidthMul=2;
			break;

		case TP_16:
		case TP_SCREEN:
		case TP_PAL:
			WidthMul=4;
			break;

		case TP_NUM_OF_TP_TYPES:
		default:
			Error(ERR_FATAL,"Can't alloc type TP_NUM_OF_TP_TYPES");
			break;
		}
	return (WidthMul);
}

void TPRect::Set(TPageType nType,int nX,int nY,int nW,int nH)
{
	TypeOfPage=nType;
	X=nX;
	Y=nY;
	W=nW*GetWidthMul(nType);
	H=nH;
}

void TPRect::SetXY(int nX,int nY)
{
	X=nX;
	Y=nY;
}

u32	TPRect::GetId(void) const
{
	int Colis=0;

	int	PageNum=(X/256)+((Y/256)*16);

	if ((X/256) != ((X+W2Alloc-1)/256))
		Colis|=1;

	if ((Y/256) != ((Y+H2Alloc-1)/256))
		Colis|=2;
				
	return ((Colis<<24)|(PageNum<<16)|((Y&0xff)<<8)|(X&0xff));
}

bool TPRect::SetRotate(bool Rot)
{
	bool	RetBool=Rotated;

	if (Rot != RetBool)
		{
		W/=GetWidthMul(TypeOfPage);
		int	Temp=W;
		W=H;
		H=Temp;
		W*=GetWidthMul(TypeOfPage);
		}

	Rotated=Rot;
	return(RetBool);
}


void VRAM::getUnusedSpace(TPRectVec & unusedBoxes)
{
	int			vX=X;
	int			vY=Y;
	const int	Width16=256;
	const int	Height16=256;

	for (int y=0;y<VRAMHeightPages;y++)
		for (int x=0;x<VRAMWidthPages;x++)
			{
			TPRect		thisRect;
			bool		foundRect;

			foundRect=false;

			for (int f=1;f<255 && !foundRect;f++)
				{
				int		boxBaseY;
				int		boxHeight;

				boxBaseY=f;
				boxHeight=255-f;

				thisRect.X=(x*Width16)+vX;
				thisRect.Y=(y*Height16)+vY+boxBaseY;
				thisRect.W=Width16;
				thisRect.H=boxHeight;

				if (!InColisionWithUsed(thisRect))
					foundRect=true;
				}
			
			if (foundRect)
				unusedBoxes.push_back(thisRect);
			}
}


int VRAM::GetNumOfUsedPages()
{
	int			Used=0;
	int			vX=X;
	int			vY=Y;
	const int	Width16=256;
	const int	Height16=256;

	for (int y=0;y<VRAMHeightPages;y++)
		for (int x=0;x<VRAMWidthPages;x++)
			{
			TPRect		ThisRect;
			ThisRect.X=(x*Width16)+vX;
			ThisRect.Y=(y*Height16)+vY;
			ThisRect.W=Width16;
			ThisRect.H=Height16;

			if (InColisionWithUsed(ThisRect))
				Used++;
			}
	return(Used);
}


bool TPRect::IsAlloced(void) const
{
	return(Alloced);
}

void TPRect::SetAlloced(bool nAlloced)
{
	Alloced=nAlloced;
}

int VRAM::GetNumOfItems()
{
	return(Used.size());
}

void TPRect::InitFromFrame(Frame const & Fr)
{
	/* !!!!! Make this support other bit widths */

	W=Fr.GetWidth();
	H=Fr.GetHeight();
	
	if (Fr.GetNumOfCols() <= 16)
		TypeOfPage=TP_4;
	else if (Fr.GetNumOfCols() <= 256)
		TypeOfPage=TP_8;
	else
		Error(ERR_FATAL,"Only 4 & 8 bit supported right now");

	W*=GetWidthMul(TypeOfPage);
}


TPageType TPRect::convertTo16Bit(void)
{
	TPageType		oldType;

	switch (TypeOfPage)
		{
		case TP_4:
//			W=GU_AlignVal(W,4)/4;
			break;

		case TP_8:
//			W=GU_AlignVal(W,2)/2;
			break;
		default:
			Error(ERR_FATAL,"Only 4 & 8 bit supported right now");
			break;
		}

	oldType=TypeOfPage;
	TypeOfPage=TP_16;

	return(oldType);
}

void TPRect::convertFrom16Bit(TPageType newType)
{
	switch (newType)
		{
		case TP_4:
//			W=W*4;
			break;

		case TP_8:
//			W=W*2;
			break;
		default:
			Error(ERR_FATAL,"Only 4 & 8 bit supported right now");
			break;
		}

	TypeOfPage=newType;
}

/*===========================================================================
 end */


