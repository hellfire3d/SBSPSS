/**********************/
/*** PSX Vram Stuff ***/
/**********************/

#include 	"System\global.h"
#include 	"Gfx\TPage.h"
#include 	"Gfx\AnimTex.h"
#include 	"Gfx\Prim.h"
#include 	"System\vid.h"
#include 	"FileIO/FileIO.h"
#include	"Mem\Memory.h"
#include	"utils\utils.h"

enum TPAGE_ENUMS
{
TPAGE_HALF0	=		0,
TPAGE_HALF1	=		1,
TPAGE_FULL	=		0,
TPAGE_DYN_START =	(24),
TPAGE_DYN_END =		(26),
MAX_TPAGES	=		32,
};

//#define	INVALIDATE_CACHE	1

const	int			TPRawW=64;
const	int			TPRawH=256;

static	sTPageCache	s_TPCache[MAX_TPAGES];

// Theory!!
// Filenames stored so IF tpage is set to load, AND is found within table (free or not) then use that
// Will only work if people are good with the tpage allocation (due to no checks on width)
// Will write a InvalidDate routine

/*****************************************************************************/
void	TPInit()
{
// Likes to be a multiple of 65536, dunno why, Im shit like that

	for (int i=0; i<MAX_TPAGES ;i++) 
		{
		s_TPCache[i].Info[0].RefCount=s_TPCache[i].Info[1].RefCount=0;
		s_TPCache[i].Info[0].TPageName=s_TPCache[i].Info[1].TPageName=(FileEquate)0;
		s_TPCache[i].Info[0].AnimTexCount=s_TPCache[i].Info[1].AnimTexCount=0;
		}
}

/*****************************************************************************/
void TPFree( const TPAGE_DESC & desc,int Invalidate)
{
#if		defined(INVALIDATE_CACHE)
RECT	R;
		setRECT(&R,(desc.tpage&0xf)*TPRawW,(desc.tpage>15)*TPRawH,64,128);
		Invalidate=1;
#endif
		s_TPCache[desc.tpage].Info[desc.Half].RefCount--;
		if (!s_TPCache[desc.tpage].Info[desc.Half].RefCount)
			{
			if (Invalidate)	s_TPCache[desc.tpage].Info[desc.Half].TPageName=(FileEquate)0;
			if (desc.Half==0 && s_TPCache[desc.tpage].Info[1].RefCount==-1)	// Check for full tpage
				{
				s_TPCache[desc.tpage].Info[1].RefCount=0;
#if		defined(INVALIDATE_CACHE)
				R.h=256;
#endif
				}
			CAnimTex::DumpThisTPage(s_TPCache[desc.tpage].Info[desc.Half].TPageName);
#if		defined(INVALIDATE_CACHE)
			R.y+=desc.Half*128;
			ClearImage(&R,0,255,0);
#endif
			}
}

/*****************************************************************************/
void	IsTPageInCache(FileEquate Filename,int &TPage,int &Half)
{
	TPage=0;
	for (int Loop=0; Loop<MAX_TPAGES && !TPage; Loop++)
		{
		if (s_TPCache[Loop].Info[0].TPageName==Filename) 
			{
			Half=0;
			TPage=Loop;
			}
		else
		if (s_TPCache[Loop].Info[1].TPageName==Filename) 
			{
			Half=1;
			TPage=Loop;
			}
		}
}

/*****************************************************************************/
sTPageInfo	*FindSpareTPage(FileEquate Filename, int &TPage,int &Half,sTPageHdr *TPHdr)
{
	TPage=TPHdr->TPageStart;
	Half=0;
	if (TPage>=TPAGE_DYN_START && TPage<=TPAGE_DYN_END)	// Its a dynamic TPage!
		{
// Find spare VRam
		if (TPHdr->TPageHeightInPixels <= 128)	// Half TPage
			{
			// Find spare half
			while(s_TPCache[TPage].Info[0].RefCount && s_TPCache[TPage].Info[1].RefCount)
				{
				TPage++; 
				ASSERT(TPage <= TPAGE_DYN_END);// ASSERT(!"VRAM FULL");
				}

			if (!s_TPCache[TPage].Info[0].RefCount)
				Half = TPAGE_HALF0;
			else
				Half=TPAGE_HALF1;
			}
		else
			{	// Full TPage
			while(s_TPCache[TPage].Info[0].RefCount || s_TPCache[TPage].Info[1].RefCount)
				{
				TPage++;
				if (TPage >= MAX_TPAGES) ASSERT(!"VRAM FULL");
				}
			Half = TPAGE_FULL;
			s_TPCache[TPage].Info[1].TPageName=(FileEquate)0;
			s_TPCache[TPage].Info[1].RefCount=-1;	// lock 2nd half
			}
		}

sTPageInfo	*Cache=&s_TPCache[TPage].Info[Half];
		Cache->RefCount++;
		Cache->TPageName=Filename;
		Cache->XOfs=TPage-TPHdr->TPageStart;
		Cache->YOfs=Half*128;
		Cache->AnimTexCount=0;
		return(Cache);
}

/*****************************************************************************/
void	TPLoadVRam(sTPageHdr *TPHdr, int TPage, int Half, u32 *VRamData)
{
RECT		Rect;

// Read and DMA TP to VRam
		Rect.x=(TPage%16)*TPRawW;
		Rect.y=((TPage>>4)*TPRawH)+(Half*128);
		Rect.w=TPHdr->TPageWidth*TPRawW;
		Rect.h=TPHdr->TPageHeightInPixels;

		DrawSync( 0 );
		LoadImage( &Rect, VRamData);
}

/*****************************************************************************/
void	AddAnimTexToList(sTPageInfo	*Cache,sFrameHdr *FramePtr,int TPage,int Half)
{
sFrameHdr	*AT=&Cache->AnimTexFrame[Cache->AnimTexCount];
	
		MCmemcpy(AT,FramePtr,sizeof(sFrameHdr));
		Cache->AnimTexCount++;
}

/*****************************************************************************/
TPAGE_DESC	TPLoadTex(FileEquate Filename)
{
TPAGE_DESC	Desc;
sTPageHdr	*TPHdr;
sFrameHdr	*FramePtr;
u32			*VRAMData;
int			TPage,Half;
sTPageInfo	*Cache;

// Is it already loaded in TCache?
		IsTPageInCache(Filename,TPage,Half);

		if (TPage)	// Is in cache, no need to load it again :o)
			{
//			DBG_MSG2("TPLoadTex Cached (%i,%i)",TPage,Half);
			s_TPCache[TPage].Info[Half].RefCount++;
			Cache=&s_TPCache[TPage].Info[Half];
			FramePtr=&Cache->AnimTexFrame[0];
			}
		else
			{		// Better load it then
//			DBG_MSG0("TPLoadTex");
			TPHdr=(sTPageHdr*)CFileIO::loadFile(Filename,"TPLoadTEX");
			ASSERT(!TPHdr->NumOfSpareBoxes);
			FramePtr=(sFrameHdr*)	MakePtr(TPHdr,sizeof(sTPageHdr));
			VRAMData=(u32*)			MakePtr(FramePtr,TPHdr->NoOfFrames*sizeof(sFrameHdr));
			Cache=FindSpareTPage(Filename,TPage,Half,TPHdr);
			for (int Frm=0;Frm<TPHdr->NoOfFrames; Frm++)	// Add Animated Texture references
				{
				if (FramePtr->Cycle) AddAnimTexToList(Cache,FramePtr++,TPage,Half);
				}
			TPLoadVRam(TPHdr, TPage,Half,VRAMData);
			MemFree(TPHdr);
			}
// If first instance, add animated textures
		if (Cache->RefCount==1)	
			{
			FramePtr=Cache->AnimTexFrame;
			for (int Frm=0; Frm<Cache->AnimTexCount; Frm++) CAnimTex::AddAnimTex(FramePtr++,Filename);
			}
		
		Desc.Half= Half;
		Desc.tpage = TPage;
		Desc.xoffset = Cache->XOfs;
		Desc.yoffset = Cache->YOfs;

		return (Desc);
}


/*****************************************************************************/
// These are NEVER dynamic, but need to store the frame headers
// Plus, this should only happen at system startup now :o)
TPAGE_DESC TPLoadTexWithHeaders(FileEquate Filename, sFrameHdr **hdrs )
{
TPAGE_DESC	Desc;
sTPageHdr	TPHdr;
sFrameHdr	*FramePtr;
u32			*VRAMData;
int			TPage,Half=0;
sTPageInfo	*Cache;
int			ReadLeft;

// Is it already loaded in TCache?
	IsTPageInCache(Filename,TPage,Half);

	if (TPage)	// Found one!!
		{
//		DBG_MSG2("TPLoadTexWithHeaders Cached (%i,%i)",TPage,Half);
		s_TPCache[TPage].Info[Half].RefCount++;
		Cache=&s_TPCache[TPage].Info[Half];
		}
	else
		{
//		DBG_MSG0("TPLoadTexWithHeaders");
		CFileIO::OpenFile(Filename);
// Load Main Header
		CFileIO::ReadFile((void*)&TPHdr,sizeof(sTPageHdr));
		ASSERT(!TPHdr.NumOfSpareBoxes);
		TPage=TPHdr.TPageStart;
// Load Rest
		*hdrs=(sFrameHdr*)MemAlloc(sizeof(sFrameHdr)*TPHdr.NoOfFrames,"TpFh");
		ReadLeft=CFileIO::GetReadLeft();
		FramePtr=(sFrameHdr*)MemAlloc(ReadLeft,"TPLoadTemp");
		CFileIO::ReadFile(FramePtr,ReadLeft);
		CFileIO::CloseFile();
		VRAMData=(u32*)	MakePtr(FramePtr,TPHdr.NoOfFrames*sizeof(sFrameHdr));
		MCmemcpy(*hdrs,FramePtr,TPHdr.NoOfFrames*sizeof(sFrameHdr));	
		TPLoadVRam(&TPHdr, TPage,Half,VRAMData);
		Cache=&s_TPCache[TPage].Info[Half];
		MemFree(FramePtr);
		FramePtr=*hdrs;
		for (int Frm=0;Frm<TPHdr.NoOfFrames; Frm++)	// Add Animated Texture references
			{
			if (FramePtr->Cycle) AddAnimTexToList(Cache,FramePtr++,TPage,Half);
			}

		Cache->RefCount=1;
		Cache->TPageName=Filename;
		Cache->XOfs=0;
		Cache->YOfs=0;
		s_TPCache[TPage].Info[1].RefCount=-1;
		}
	
// If first instance, add animated textures
		if (Cache->RefCount==1)	
			{
			FramePtr=Cache->AnimTexFrame;
			for (int Frm=0; Frm<Cache->AnimTexCount; Frm++) CAnimTex::AddAnimTex(FramePtr++,Filename);
			}

	Desc.Half= Half;
	Desc.tpage = TPage;
	Desc.xoffset = Cache->XOfs;
	Desc.yoffset = Cache->YOfs;

	return (Desc);
}

