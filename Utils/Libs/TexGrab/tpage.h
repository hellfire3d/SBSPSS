/*=========================================================================

	FILENAME.CPP	

	Author:  Gary Liddon @
	Created:
	Project:
	Purpose:

	Copyright (c) 1997 Climax Development Ltd

===========================================================================*/

#ifndef __TPAGE_HPP__
#define __TPAGE_HPP__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <list>

/*	Glib
	---- */
#include <frame.hpp>

/*	Local
	----- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
struct POS_INFO
{
	int		MinXTp;			/* Minimum x placeable within a Tpage */
	int		MaxXTp;			/* Maximum x placeable within a Tpage */

	int		MinYTp;			/* Minimum y placeable within a Tpage */
	int		MaxYTp;			/* Maximum y placeable within a Tpage */

	int		MinX;			/* Minimum global x */
	int		MaxX;			/* Maximum global x */

	int		MinY;			/* Minimum global y */
	int		MaxY;			/* Maximum global y */

	int		XAlign;		/* X Alignment */
	int		TpWidthPix;		/* Width of each tpages for this rect */
};


/*	Different Tpage Types
	--------------------- */
enum TPageType
{
	TP_4=		0,
	TP_8,
	TP_16,
	TP_SCREEN,
	TP_PAL,
	TP_NUM_OF_TP_TYPES,
};


/*	A Rectangle in the Tpage
	------------------------ */
class TPRect : public Rect
{
public:
			TPRect(void);
			TPRect(TPageType nType,int nW,int nH);
			TPRect(TPRect const & NewR)	{MakeCopy(NewR);}

	void	InitFromFrame(Frame const & Fr);
	void	SetAlloced(bool nAlloced=true);
	bool	SetRotate(bool Rot=true);
	void	operator=(TPRect const & NewR)	{MakeCopy(NewR);}	
	bool	operator< (TPRect const & R) const	{return(GetId()<R.GetId());}
	void	SetWH(int nW,int nH)
		{W=nW;H=nH;}

	void	SetXY(int nX,int nY);
	void	Set(TPageType nType,int nX,int nY,int nW,int nH);
	void	SetTpType(TPageType nType)	{TypeOfPage=nType;}

	bool	GetRotate(void) const {return(Rotated);}
	bool	IsAlloced(void) const;
	int		GetTpageIn(void) const;
	u32		GetId(void) const;
	bool	GetAlloced(void){return(Alloced);}

	static void	SetAllocWH(int nW,int nH)	{W2Alloc=nW; H2Alloc=nH;}


	TPageType	convertTo16Bit(void);
	void		convertFrom16Bit(TPageType newType);


	void SetId(int NewId)
		{Id=NewId;}

	int GetId(void)
		{return(Id);}

	TPageType	TypeOfPage;

	void	SetDontRotate(bool NewVal=true)	{DontRotate=NewVal;}
	bool	GetDontRotate(void)	const		{return(DontRotate);}

protected:
	int		GetWidthMul(TPageType nType);
	void	InitRect(void);

	bool	Rotated;
	int		Id;

	void MakeCopy(TPRect const & NewR)
		{
		W=NewR.W;
		H=NewR.H;
		X=NewR.X;
		Y=NewR.Y;
		TypeOfPage=NewR.TypeOfPage;
		Rotated=NewR.Rotated;
		Id=NewR.Id;
		Alloced=NewR.Alloced;
		DontRotate=NewR.DontRotate;
		}

	static int	W2Alloc;
	static int	H2Alloc;

	bool		Alloced;
	bool		DontRotate;
};




/*	VRAM Alloc handle
	----------------- */
typedef int VHND;
typedef std::vector<TPRect> TPRectVec;

/*	VRAM
	---- */

typedef std::list<TPRect>		TPRectList;
typedef std::vector<TPRectList> TPRectListVec;

class VRAM : public GObject
{
public:
	VRAM(int nTpWidth=16,int nTpHeightInPixels=512);

	int GetNumOfItems(void);
	int GetNumOfUsedPages();

	bool AllocVRAM(TPRect & OriginalRect);
	bool AllocVRAM(TPRectVec & RectVec,bool nRotateEveryEmpty,bool nBig2Little,bool nWiderThanHigher);

	TPRect const &	GetVRAM(VHND hnd) const;

	TPRectVec &	GetUsed(void)
		{return(Used);}

	TPRectVec &	GetUnused(void)
		{return(Unused);}

	void Clear(int nTpWidth=-1,int nTpHeight=-1);

	void SetBig2Little(bool Val=true)
		{ Big2Little=Val;}

	void SetRotateEveryEmpty(bool Val=true)
		{ RotateEveryEmpty=Val;}

	bool CanRotate(TPRect & ThisRect);

	void getUnusedSpace(TPRectVec & unusedBoxes);

protected:
	void	PosFromTPrect(TPRect & ThisRect,POS_INFO & Pi);
	bool	TryRect(TPRect & BlankRect,TPRect & ThisRect);

	TPRectListVec	VecOfPages;

	bool		TryNFit(TPRectList & TpList,TPRect & ThisRect);

	void		AddEmpty(int x,int y,int w,int h);
	bool		InColisionWithUsed(TPRect const & R);
	void		InitEmpty(int vX,int vY,int W,int H);
	bool		CheckValidAlloc(TPageType nType,int nW,int nH);
	int			GetXAlign(TPageType nType);
	int			GetMaxWidthTp(TPageType nType);
	void		RemovedFromUnused(TPRect const & ThisRect);
	void		AddToUsed(TPRect const & ThisRect);
	void		RectFromTpRect(Rect & R,TPRect const & ThisRect);

	TPRect &	NewItem(TPRectVec & TPRects);

	TPRect &	NewUnused(void)
		{ return(NewItem(Unused)); }

	TPRect & NewUsed(void)
		{ return(NewItem(Used)); }


	TPRectVec	Unused;
	TPRectVec	Used;
	TPRect		ErrorRect;

	int		X,Y,W,H;
	int		NumOfTPages;
	int		TpWidth;
	int		TpHeightInPixels;

	int		VRAMWidthPages;
	int		VRAMHeightPages;

	bool	Big2Little;
	bool	RotateEveryEmpty;
	bool	WiderThanHigher;
};

/*	Error codes for VHND
	-------------------- */
enum
{
	VHND_NULL = -1,
};

/*----------------------------------------------------------------------
	Globals
	------- */

/*	Vars
	---- */

/*	Data
	---- */

/*	Functions
	--------- */
void TestTPageStuff(void);

/*---------------------------------------------------------------------- */

#endif	/* __TPAGE_HPP__ */

/*===========================================================================
 end */


