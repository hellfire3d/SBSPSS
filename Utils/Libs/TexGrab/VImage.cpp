/*=========================================================================

	FILENAME.CPP	

	Author:  Gary Liddon @
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 G R Liddon

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*	Glib
	---- */
#include "pak.h"

/*	Local
	----- */
#include "vimage.h"

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
using namespace std;

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Data
	---- */

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
VRAMImage::VRAMImage(int NewWidthInTpages,int NewHeightInPixels) : VRAMData(NULL)
{
	WidthInTpages=NewWidthInTpages;
	HeightInPixels=NewHeightInPixels;
	HeightInTPages=NewHeightInPixels / 256;

	if (!HeightInTPages)
	{
		aTPageHeight = 128;
		HeightInTPages = 1;
	}
	else
	{
		aTPageHeight = HeightInTPages * 256;
	}
	TPageSizeInBytes = 128 * aTPageHeight;

	WidthInBytes=WidthInTpages*128;

	VramAreaBytes=WidthInBytes * HeightInPixels;

	VRAMData=new u8[VramAreaBytes];
	lbmData = new u8[WidthInTpages*256*HeightInPixels];

	if (!lbmData)
		Error(ERM_OUTOFMEM);

	if (!VRAMData)
		Error(ERM_OUTOFMEM);

	memset(VRAMData,0,VramAreaBytes);
	m_doCompress=false;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
VRAMImage::~VRAMImage(void)
{
	if (lbmData)
		delete lbmData;
	if (VRAMData)
		delete VRAMData;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void VRAMImage::getTpData(unsigned int tp,std::vector<u8> & dest) const
{
	const int TPAGES_IN_ALL =WidthInTpages * HeightInTPages;

	if (tp >= TPAGES_IN_ALL)
		Error(ERR_FATAL,"illegal tpage number");

	int			tpX,tpY;
	u8 const *	srcTpData;
	dest.resize(TPageSizeInBytes);


	tpX=(tp%WidthInTpages)*128;
	tpY=(tp/WidthInTpages)*aTPageHeight;

	srcTpData=&VRAMData[tpX+tpY*WidthInBytes];

	for (int x=0;x<128;x++)
		for (int y=0;y<aTPageHeight;y++)
			dest[x+y*128]=srcTpData[x+y*WidthInBytes];
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CompressMem(char const * CmdLine,std::vector<u8> & Body)
{
	try
		{
		GString		Com;

		char	TmpName[100];
		char	CompTmpName[100];

		tmpnam(TmpName);
		tmpnam(CompTmpName);

		GString	ComStr(CmdLine);
		
		ComStr.Replace("%1",TmpName);
		ComStr.Replace("%2",CompTmpName);

		ofstream Out;
		Out.open(TmpName,ios::binary|ios::trunc);
		
		if (Out)
			{
			int		CompSize;
			Out.write((char const *)&Body[0],Body.size());
			Out.close();

			cout<<(char const *)ComStr<<"from "<<CmdLine<<endl;
			system(ComStr);

			CompSize=FileSize(CompTmpName);

			Body.resize(CompSize);

			ifstream In;
			In.open(CompTmpName,ios::binary);

			if (In)
				{
				In.read((char *)&Body[0],CompSize);
				In.close();
				}
			else
				throw("Can't open compressed out file");

			}
		else
			throw("Can't open uncompressed out file");

		remove(TmpName);
		remove(CompTmpName);
		}

	catch (const char * Err)
		{
		GObject::Error(ERR_FATAL,"%s im CompressMem",Err);
		}
}



void VRAMImage::WriteInTpageChunks(ofstream & str)
{
	const int TPAGES_IN_ALL =WidthInTpages * HeightInTPages;

	vector<u8>	tpageData;
	tpageData.resize(TPageSizeInBytes);

	for (int f=0;f<TPAGES_IN_ALL;f++)
		{
		getTpData(f,tpageData);
		str.write((char*)&tpageData[0],TPageSizeInBytes);
		}
}


void VRAMImage::Write(ofstream & Str)
{
	if (m_doCompress)
		{
		printf("Packing TPage\n");
		const int TPAGES_IN_ALL =WidthInTpages * HeightInTPages;
		const int NUM_OF_WINDOWS = TPAGES_IN_ALL*4;

		vector<u8>	tpageData;
		vector<u8>	dataToWrite;

		tpageData.resize(TPageSizeInBytes);
		dataToWrite.resize(TPageSizeInBytes);

		for (int f=0;f<NUM_OF_WINDOWS;f++)
			{
			int			dataWriteSize;
			const u8 *	srcData;

			if ((f&3) == 0)
				getTpData(f/4,tpageData);

			srcData=&tpageData[(f%4)*128*64];
			
			if (f != 0)
				{
				vector<u8>	myData;

				myData.resize(128*64);

				memcpy(&myData[0],srcData,128*64);

				#ifdef __USE_LZNP__

					CompressMem("lznp %1 %2",myData);
					dataWriteSize=myData.size();
					memcpy(&dataToWrite[0],&myData[0],dataWriteSize);

				#else
					dataWriteSize=PAK_findPakSize(&myData[0],128*64);
					PAK_doPak(&dataToWrite[0],&myData[0],128*64);
				#endif

				}
			else
				{
				dataWriteSize=128*64;
				memcpy(&dataToWrite[0],srcData,128*64);
				}
			for (int a=0;a<16;a++) printf("%i, ",(int)dataToWrite[a]);
			printf("\n");
			Str.write((char *)(&dataToWrite[0]),dataWriteSize);
			}

		}
	else
		Str.write((char *)(VRAMData),VramAreaBytes);
}







/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void VRAMImage::PlotPal(SprPal const & PalToPlot)
{
	TPRect  Tp=PalToPlot.GetTPRect();
	
	int W=WidthInTpages*256;

	W/=2;

	u16 * Addr=(u16 *)&VRAMData[Tp.Y*W+Tp.X/2];

	std::vector<u16> OutWords;

	PalToPlot.MakePSXPal(OutWords);

	int		f;

	for (f=0;f<OutWords.size();f++)
		Addr[f]=OutWords[f];
}

		

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void VRAMImage::PlotFrame(SprFrame const & FrameToPlot)
{
	SprFrame::BDEPTH	ThisDepth;

	ThisDepth=FrameToPlot.GetBitDepth();

	switch(ThisDepth)
		{
		case SprFrame::BITS_4:
			PlotFrame4(FrameToPlot);
			break;

		case SprFrame::BITS_8:
			PlotFrame8(FrameToPlot);
			break;
		};
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void VRAMImage::SaveAs16ColLbm(const char * Name)
{
	Palette	GazPalette;

	GazPalette[0].SetR(0);
	GazPalette[0].SetG(0);
	GazPalette[0].SetB(255);

	int		W=WidthInTpages*256;
	int		H=HeightInPixels;

	Frame	ThisFr;

	for (int f=1;f<15;f++)
		{
		int	Col=f*17;
		GazPalette[1+f].SetR(Col);
		GazPalette[1+f].SetG(Col);
		GazPalette[1+f].SetB(Col);
		}


	for (int y=0;y<H;y++)
		for (int x=0;x<W;x++)
			{
			if ((x&1))
				lbmData[x+y*W]=(VRAMData[ (y*(W/2))+(x/2)]&0xf0) >> 4;
			else
				lbmData[x+y*W]=VRAMData[(y*(W/2))+(x/2)]&0x0f;
			}

	ThisFr.SetFrame(lbmData,W,H,GazPalette);
	ThisFr.SaveLbm(Name);

//	cout<<"Written "<<Name<<endl;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void VRAMImage::PlotFrame4(SprFrame const & Fr)
{
	TPRect  Tp=Fr.GetTPRect();
	
	int W=WidthInTpages*256;

	W=W/2;			/* we're plotting nibbles */

	if (!Tp.GetRotate())
		{
		for (int y=Tp.Y;y<Tp.Y+Tp.H;y++)
			for (int x=Tp.X;x<Tp.X+Tp.W;x++)
			{
			int		FrX=x-Tp.X;
			int		FrY=y-Tp.Y;
			u8		ScrNib=Fr.SeeData()[FrY*Tp.W+FrX]&0xf;
			u8 *	PixAddr=&VRAMData[y*W+x/2];

			if ((x&1))
				{
				*PixAddr&=0x0f;
				*PixAddr|=ScrNib<<4;
				}
			else
				{
				*PixAddr&=0xf0;
				*PixAddr|=ScrNib;
				}
			}
		}
	else
		{
		for (int y=Tp.Y;y<Tp.Y+Tp.H;y++)
			for (int x=Tp.X;x<Tp.X+Tp.W;x++)
			{
			int		FrX=x-Tp.X;
			int		FrY=y-Tp.Y;

			u8		ScrNib=Fr.SeeData()[FrX*Tp.H+(Tp.H-FrY-1)]&0xf;
			u8 *	PixAddr=&VRAMData[y*W+x/2];

			if ((x&1))
				{
				*PixAddr&=0x0f;
				*PixAddr|=ScrNib<<4;
				}
			else
				{
				*PixAddr&=0xf0;
				*PixAddr|=ScrNib;
				
				}
			}
		}

}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void VRAMImage::PlotFrame8(SprFrame const & Fr)
{
	const TPRect & Tp=Fr.GetTPRect();

	int W=WidthInTpages*256;

	W/=2;

	u8 * Addr=&VRAMData[Tp.Y*W+Tp.X/2];

	if (!Fr.IsRotated())
		{
		for (int y=0;y<Fr.GetHeight();y++)
			for (int x=0;x<Fr.GetWidth();x++)
				Addr[y*W+x]=Fr.SeeData()[x+y*Fr.GetWidth()];
		}
	else
		{

		for (int y=0;y<Fr.GetWidth();y++)
			for (int x=0;x<Fr.GetHeight();x++)
				Addr[y*W+x]=Fr.SeeData()[x*Fr.GetWidth()+(Fr.GetWidth()-y-1)];
		}
}


/*===========================================================================
 end */
