/*=========================================================================

	DPANM.HPP	

	Author:  Gary Liddon @ Fareham
	Created:
	Project: 
	Purpose:

	Copyright (c) 1997 Gary Liddon

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
//#include <stdlib.h>
#include <minmax.h>
#include <fstream>
#include <iostream>

/*	Glib
	---- */

/*	Local
	----- */
#include "dpanim.hpp"
//#include "ilbm.hpp"

/*	Name space
	---------- */
using namespace std;

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
typedef UWORD LP_TABLE_ELEMENT;

#define MAX_NUMBER_CYCLES				16

#define BMBODY_RUNSKIPDUMP				1
#define MAX_COLORS						256
#define PALETTE_SIZE					(MAX_COLORS * sizeof(long))
#define MAX_RECORDS						0xffffu
#define MAX_LARGE_PAGE					256

#define LARGE_PAGE_SIZE					0x10000L

#define LPF_HEADER_HEAD_SIZE_IN_FILE	256
#define LPF_HEADER_SIZE_IN_FILE			((sizeof(AnimHdr)) + (8 * 16) + (4 * 256) + (sizeof(LpTable[MAX_LARGE_PAGE])))
#define LP_FILE_OFFSET(nLp)				((nLp) * LARGE_PAGE_SIZE + LPF_HEADER_SIZE_IN_FILE)

#define MakeID(d,c,b,a) (((ULONG)(a)<<24l) | ((ULONG)(b)<<16l) | ((ULONG)(c)<<8l) | ((ULONG)(d)))

#define LARGE_PAGE_FILE_ID		MakeID('L','P','F',' ')
#define ANIM_CONTENTS_ID		MakeID('A','N','I','M')

#define FIRST_FRAME_N	1
#define FIRST_RECORD_N	0					// Records #d from 0.



/*----------------------------------------------------------------------
	Typedefs
	-------- */
class LpObj;


/*----------------------------------------------------------------------
	Class Defintions
	---------------- */

/*	Compressed Frame
	---------------- */
class CompFrame : public GObject
{
public:
	CompFrame(void);
	CompFrame(CompFrame const & C);

	~CompFrame(void);

	void operator=(CompFrame const &);

	/* Make Vectorable */

	bool operator==(CompFrame const &) const;
	bool operator<(CompFrame const &) const;

	void WriteByte(u8 Byte);
	void WriteWord(u16 Word);
	void WriteData(u8 const * Src,int Amount);
	void Write(ofstream & Out) const;

	friend	ostream & operator<<(ostream & str,CompFrame & Fr);

	u8 const *	SeeData(void)	const {return(BinData);}
	int			GetSize() const {return(Len);}

protected:
	void	Dump();
	void	Init();
	void	Resize(int NewLen);
	void	Increase(int NewLen) {Resize(ActualLen+NewLen);}

	u8 *	BinData;
	int		Len;

	int		ActualLen;

	enum
		{
		RESIZE_CHUNKS=1000,
		};
};


typedef std::vector<CompFrame>	CompFrameVec;

/*	Large Frame Object
	------------------ */
class LpObj : public GObject
{
public:	
	LpObj(void);

	bool	AddFrame(CompFrame const & C );
	void	SetBaseRecord(int f){BaseRecord=f;}
	int		NumOfFrames(void)		{	return(MyFrames.size());}

	void	Write(std::ofstream & Out) const;
	void	WriteHdr(std::ofstream & Out) const;

	/* Make Vectorable */

	void operator=(LpObj const &);
	bool operator==(LpObj const &) const;
	bool operator<(LpObj const &) const;


protected:
	int				GetSizeInAll(void) const;
	int				BaseRecord;
	CompFrameVec	MyFrames;
};



/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */
static void		GenerateLpTable(LpObjVec & LpTable,CompFrameVec & CompFrames);
static int		GetMaxRecsPerLp(LpObjVec & LpTable);
static void		WriteWord(std::ofstream & Out,u16 Word);



/*----------------------------------------------------------------------
	Vars
	---- */
//static FilterDetails	MyDetails(DpAnimFilter(),".anm");

/*----------------------------------------------------------------------
	Data
	---- */

/*----------------------------------------------------------------------
	Function:		Main
  ---------------------------------------------------------------------- */
DpAnimFilter::DpAnimFilter()
{
	InitVars();
}

DpAnimFilter::DpAnimFilter(char const * FName) : GAnimFilter(FName)
{
	InitVars();

	loaded_LP=0xffff;
}

DpAnimFilter::~DpAnimFilter(void)
{
	DeleteVars();
}

bool DpAnimFilter::Load(GAnim & Anm)
{
	int			Frames;
	Palette		MyPal;

	MakeVars();

	anm_init2(FileName);

	GetPal(MyPal);

	anm_read_first_frame();
	Frames=0;

	do
		{
		Frame *	NewFr;

		NewFr=&Anm.GetNewFrame();

		NewFr->SetFrame(dst_buffer,320,200,MyPal);

		Frames++;
		}
	while(anm_read_next_frame());

	fclose(inANM);

	DeleteVars();

	return(true);
}

void DpAnimFilter::GetPal(Palette & MyPal)
{
	fseek(inANM,sizeof(AnimHdr)+sizeof(ColCycles[MAX_NUMBER_CYCLES]),SEEK_SET);

	for (int f=0;f<256;f++)
		{
		MyPal[f].SetB(fgetc(inANM));
		MyPal[f].SetG(fgetc(inANM));
		MyPal[f].SetR(fgetc(inANM));
		fgetc(inANM);
		}
}

void DpAnimFilter::InitVars(void)
{
	lpfTable=NULL;
	current_LP=NULL;
	dst_buffer=NULL;

}

void DpAnimFilter::MakeVars(void)
{
	DeleteVars();

	if (!(lpfTable = new LpTable[MAX_LARGE_PAGE]))
		Error(ERM_OUTOFMEM);

	if (!(current_LP = new LpTableMem))
		Error(ERM_OUTOFMEM);

	if (!(dst_buffer=new u8[128960]))
		Error(ERM_OUTOFMEM);

}

void DpAnimFilter::DeleteVars(void)
{
	if (lpfTable)
		delete[] lpfTable;

	if (current_LP)
		delete current_LP;

	if (dst_buffer)
		delete dst_buffer;

	InitVars();
}



bool DpAnimFilter::Save(GAnim & Anm)
{

	MakeVars();

	lpfHdr.InitHdr();

	ofstream	Out;

	Out.open(FileName,ios::trunc|ios::out|ios::binary);

	if (!Out)
		Error(ERR_FATAL,"Can't open %s for output",(char const *)FileName);
	else
		{
		int				NumOfFrames;
		CompFrameVec	CompFrames;
		LpObjVec		MyLpTable;

		NumOfFrames=Anm.NumOfFrames();

		if (NumOfFrames)
			{
			CompFrames.reserve(NumOfFrames);
			CompFrames.resize(NumOfFrames);

			for (int f=0;f<NumOfFrames;f++)
				Encode(Anm[f],CompFrames[f]);

			GenerateLpTable(MyLpTable,CompFrames);

			AnimHdr	MyHdr;
			MyHdr.nLps=MyLpTable.size();
			MyHdr.lRecords=NumOfFrames;
			MyHdr.maxRecsPerLp=GetMaxRecsPerLp(MyLpTable);
			MyHdr.nFrames=NumOfFrames;

			WriteHdr(Out,MyHdr);
			WriteCycs(Out);

			WritePal(Out,Anm[0].GetPal());

			WriteLpTable(Out,MyLpTable);
			WriteLps(Out,MyLpTable);
			DiscardAnim();
			}
		}

	Out.close();

	return(true);
}


ULONG DpAnimFilter::anm_init2(char const * filename)
{
	FILE *	Fp;

	if(!(Fp=fopen(filename,"rb")))
		Error(ERR_FATAL,"can't open %s",filename);


	return(anm_init(Fp));
}


/*----------------------------------------------------------------------
	Setup buffer for playback and buffer for LP.
  ---------------------------------------------------------------------- */
ULONG DpAnimFilter::anm_init(FILE * filename)
{
	inANM = filename;

	fseek(inANM,0,SEEK_SET);
	fread(&lpfHdr,sizeof(AnimHdr),1,inANM);

	if ((lpfHdr.id != LARGE_PAGE_FILE_ID) || (lpfHdr.contentType != ANIM_CONTENTS_ID))
		Error(ERR_FATAL,"Input file: %s is not an ANM file.",filename);

	fseek(inANM,lpfHdr.lpfTableOffset,SEEK_SET);
	fread(lpfTable,sizeof(LpTable[MAX_LARGE_PAGE]),1,inANM);
	loaded_LP = 0xffff;

	return(((lpfHdr.hasLastDelta) && (lpfHdr.lastDeltaValid)) ? (lpfHdr.nFrames - 1) : (lpfHdr.nFrames));
}


/*----------------------------------------------------------------------
	Read LP off disk into memory for playback.
  ---------------------------------------------------------------------- */
void DpAnimFilter::read_LP(UWORD LP_to_load)
{
	if (LP_to_load == loaded_LP)
		return;

	loaded_LP = LP_to_load;
	fseek(inANM,LP_FILE_OFFSET(LP_to_load),0);

	fread(current_LP,(ULONG) (8l + lpfTable[LP_to_load].nRecords * 2l),1,inANM);

	fread(lp_buffer,(ULONG) lpfTable[LP_to_load].nBytes,1,inANM);
}


/*----------------------------------------------------------------------
	Read next frame from anm file.
  ---------------------------------------------------------------------- */
int	DpAnimFilter::anm_read_next_frame(void)
{
	int loop;
	UWORD load_lp;
	UWORD base;
	ULONG lRecords;


	/* next frame */
	current_Frame++;


	lRecords = lpfHdr.lRecords;

	if (lpfHdr.hasLastDelta && lpfHdr.lastDeltaValid)
		lRecords--;

	if (current_Frame == lRecords)
		return(0);

	load_lp = 0;

	while ((current_Frame >= (lpfTable[load_lp].baseRecord + lpfTable[load_lp].nRecords)) || (current_Frame < lpfTable[load_lp].baseRecord))
		{
		load_lp++;
		}


	read_LP(load_lp);
	base = 0;					/* Set start to beginning of buffer */

	/* Find start of Frame by adding number of bytes of previous frames. */

	for (loop = 0;loop <= (current_Frame - current_LP->baseRecord);loop++)
		base += current_LP->size[loop];


	if (lp_buffer[base] != 'B')
		Error(ERR_FATAL,"Corrupt Anim File or Non Bitmapped image.	base=%d,lp=%d in %s\n",base,load_lp,GetName());

	if ((UWORD) lp_buffer[base + 2] != BMBODY_RUNSKIPDUMP)
		Error(ERR_FATAL,"Compression scheme for frames is unkown.");

	PlayRunSkipDump(&lp_buffer[base+4],dst_buffer);

	return(1);
}


/*----------------------------------------------------------------------
	Read first frame of anm file.
  ---------------------------------------------------------------------- */
void DpAnimFilter::anm_read_first_frame(void)
{
	lp_buffer = &dst_buffer[320u * 200u];

	current_Frame = FIRST_FRAME_N;

	read_LP(loaded_LP);

	current_Frame = 0xffff;

	anm_read_next_frame();
}


/*----------------------------------------------------------------------
	Encode a frame with the DP anim codec and bung it into
	a comp frame
  ---------------------------------------------------------------------- */
void DpAnimFilter::Encode(Frame const & Fr,CompFrame & Cfr)
{
	Frame		NewFrame;
	u8 const *	Src;
	int			BytesLeft,Index;
	int	const	px=320;
	int	const	py=200;
	int const	Size=px*py;

	NewFrame=Fr;
	NewFrame.Crop(Rect(0,0,px,py));

	Src=NewFrame.SeeData();

	BytesLeft=px*py;
	Cfr.WriteByte('B');
	Cfr.WriteByte(0);
	Cfr.WriteWord(BMBODY_RUNSKIPDUMP);

	while (BytesLeft)
		{
		int		Length;

		Index=(px*py)-BytesLeft;

		Length=GetRunLength(&Src[Index],BytesLeft);

		if (Length > 3)
			WriteRun(Src[Index],Length,Cfr);
		else
			{
			Length=GetDataLength(&Src[Index],BytesLeft);
			WriteDataRun(&Src[Index],Length,Cfr);
			}

		BytesLeft-=Length;
		}

	WriteEnd(Cfr);
}

int DpAnimFilter::GetRunLength(u8 const * Src,int BytesLeft)
{
	int		Index=0;
	int		LastIndex;
	bool	Done=false;
	int		Previous=Src[0];

	for (Index=0;Index<BytesLeft && !Done;Index++)
		{
		if (Src[Index] != Previous)
			Done=true;
		else
			LastIndex=Index;
			
		Previous=Src[Index];
		}

//	cout<<"Run of "<<LastIndex+1<<endl;

	return(LastIndex+1);
}

int DpAnimFilter::GetDataLength(u8 const * Src,int BytesLeft)
{
	int		Index=0;
	int		LastIndex=0;
	bool	Done=false;
	int		Previous=Src[0]+1;
	int		RunLen=0;

	for (Index=0;Index<BytesLeft && !Done;Index++)
		{
		if (Src[Index] == Previous)
			{
			RunLen++;
			}
		else
			{
			LastIndex=Index;
			RunLen=0;
			}

		if (RunLen >= 2)
			Done=true;
		else
			Previous=Src[Index];

		}

//	cout<<"Data Run of "<<LastIndex+1<<endl;

	return(LastIndex+1);
}

void DpAnimFilter::WriteDataRun(u8 const * Data,int Length,CompFrame &Cfr)
{
//	cout<<"*** Writing unique of "<<Length<<endl;

	while (Length)
		{
		int	ToWrite;

		ToWrite=min(MAX_RUN_WRITE,Length);

		if (ToWrite<128)
			Cfr.WriteByte(ToWrite);
		else
			{
			Cfr.WriteByte(0x80);
			Cfr.WriteWord(0x8000|ToWrite);
			}

		Cfr.WriteData(Data,ToWrite);

		Length-=ToWrite;
		}
	
}

void DpAnimFilter::WriteRun(u8 Val,int Length,CompFrame &Cfr)
{
//	cout<<"*** Writing run of "<<Length<<endl;

	while (Length)
		{
		int	ToWrite;

		ToWrite=min(MAX_RUN_WRITE,Length);

		if (ToWrite<128)
			{
			Cfr.WriteByte(0);
			Cfr.WriteByte(ToWrite);
			}
		else
			{
			Cfr.WriteByte(0x80);
			Cfr.WriteWord(0xc000|ToWrite);
			}

		Cfr.WriteByte(Val);

		Length-=ToWrite;
		}
}

void DpAnimFilter::WriteEnd(CompFrame &Cfr)
{
	Cfr.WriteByte(0x80);
	Cfr.WriteWord(0);
}

void DpAnimFilter::PlayRunSkipDump(UBYTE const *src, UBYTE *dst)
{
	BOOL	Done;
	UBYTE *	odst;
	odst=dst;

	Done=FALSE;

	while (!Done)
		{
		u8	count;
		count=*src++;

		if (count == 0)
			{
			u8 clearlength,pixel;
			clearlength=*src++;
			pixel=*src++;
			memset(dst,pixel,clearlength);
			dst+=clearlength;
			}
		else
			{
			if (!(count&0x80))
				{
				memcpy(dst,src,count);
				dst+=count;
				src+=count;
				}
			else
				{
				count&=~0x80;

				if (count==0)
					{
					uint	wordcount;

					wordcount=(*src++);
					wordcount|=(*src++)<<8;

					if (wordcount==0)
						Done=TRUE;
					else
						{
						if (wordcount&0x8000)
							{
							wordcount&=~0x8000;

							if(wordcount&0x4000)
								{
								u8	pixel;
								wordcount&=~0x4000;
								pixel=*src++;
								memset(dst,pixel,wordcount);
								}
							else
								{
								memcpy(dst,src,wordcount);
								src+=wordcount;
								}
							}

						dst+=wordcount;
						}
					}
				else
					dst+=count;
				}
			}

		}
}

/*----------------------------------------------------------------------
	DpAnimFIlter Sub Classes
  ---------------------------------------------------------------------- */
DpAnimFilter::AnimHdr::AnimHdr(void)
{
	InitHdr();

}

void DpAnimFilter::AnimHdr::InitHdr(void)
{
	id=MakeID('L','P','F',' ');
	maxLps=256;
	nLps=0;					/* updated later */
	lRecords=0;           	/* updated later */
	maxRecsPerLp=256;		/* updated later */
	lpfTableOffset=1280;
	contentType=MakeID('A','N','I','M');
	width=320;
	height=200;
	variant=0;
	version=0;
	hasLastDelta=0;
	lastDeltaValid=0;
	pixelType=0;
	highestBBComp=1;
	otherRecordsPerFrame=0;
	bitmapRecordsPerFrame=1;
	nFrames=0;				/* may be updated later */
	framesPerSecond=10;
}

DpAnimFilter::ColCycles::ColCycles(void)
{
	rate=0;
	count=0;
	flags=0;
	low=0x10;
	low=0x1f;
}


int GetMaxRecsPerLp(LpObjVec & LpTable)
{
	int		MaxLps=0;
	int		f;


	for (f=0;f<LpTable.size();f++)
		{
		if (LpTable[f].NumOfFrames() > MaxLps)
			MaxLps=LpTable[f].NumOfFrames();
		}

	return(MaxLps);
}

void GenerateLpTable(LpObjVec & LpTable,CompFrameVec & CompFrames)
{
	LpObj *	ThisObj;
	int		LpIndex;
	int		CompIndex;
	bool	Done;

	ThisObj=NULL;
	LpIndex=0;
	CompIndex=0;
	Done=false;

	while (!Done)
		{
		if (CompIndex==CompFrames.size())
			Done=TRUE;
		else
			{
			if (!ThisObj)
				{
				LpTable.resize(LpTable.size()+1);
				ThisObj=&LpTable[LpTable.size()-1];
				ThisObj->SetBaseRecord(CompIndex);
				}

			if (ThisObj->AddFrame(CompFrames[CompIndex]))
				CompIndex++;
			else
				ThisObj=NULL;
			}
		}
}

/*----------------------------------------------------------------------
	Anim writing stuff
  ---------------------------------------------------------------------- */
void DpAnimFilter::MakeAnim(void)
{
}

void DpAnimFilter::WriteHdr(ofstream & Out,AnimHdr & A)
{
	Out.write((char const *) &A,sizeof(A));
}

void DpAnimFilter::WriteCycs(ofstream & Out)
{
	ColCycles	MyCycles[MAX_NUMBER_CYCLES];
	Out.write((char const *) MyCycles,sizeof(MyCycles));
}

void DpAnimFilter::WritePal(ofstream & Out,Palette const & P)
{
	for (int f=0;f<256;f++)
		{
		Out.put((u8)P[f].GetB());
		Out.put((u8)P[f].GetG());
		Out.put((u8)P[f].GetR());
		Out.put((u8)0);
		}
}

void DpAnimFilter::WriteLpTable(ofstream & Out,LpObjVec & LpTable)
{
	int		WriteFromTable;
	int		WriteDummy;

	int		Val=Out.tellp();

	WriteFromTable=min(MAX_LARGE_PAGE,LpTable.size());

	for (int f=0;f<WriteFromTable;f++)
		LpTable[f].WriteHdr(Out);

	WriteDummy=MAX_LARGE_PAGE-WriteFromTable;

	if (WriteDummy)
		{
		LpObj		Dummy;

		for (int f=0;f<WriteDummy;f++)
			Dummy.WriteHdr(Out);
		}

	int NewVal=Out.tellp();
	Val=NewVal-Val;
}

void DpAnimFilter::WriteLps(ofstream & Out,LpObjVec & LpTable)
{
	for (int f=0;f<LpTable.size();f++)
		{
		int		Written=Out.tellp();

		LpTable[f].Write(Out);

		int CurrentPos;
		CurrentPos=Out.tellp();
		Written=CurrentPos-Written;

		int ToWrite=LARGE_PAGE_SIZE-Written;

		if (ToWrite>0)
			{
			for (int f=0;f<ToWrite;f++)
				Out.put((char)'z');
			}
		}
}

void DpAnimFilter::DiscardAnim(void)
{
}


/*----------------------------------------------------------------------
	A Compressed Frame
  ---------------------------------------------------------------------- */
CompFrame::CompFrame(void)
{
	Init();
	Resize(RESIZE_CHUNKS);
}

void CompFrame::Resize(int NewLen)
{
	u8 *	NewData;

	if(!(NewData=new u8[NewLen]))
		Error(ERM_OUTOFMEM);

	if (BinData)
		{
		int	ToCopy;
		ToCopy=min(Len,NewLen);
		memcpy(NewData,BinData,ToCopy);
		delete BinData;
		}

	BinData=NewData;
	ActualLen=NewLen;

	Len=min(Len,ActualLen);
}

void CompFrame::Init(void)
{
	Len=0;
	ActualLen=0;
	BinData=NULL;
}

void CompFrame::Dump(void)
{
	if (BinData)
		delete BinData;

	Init();
}

CompFrame::~CompFrame(void)
{
	Dump();
}

void CompFrame::WriteByte(u8 Byte)
{
	if ((Len+1) >= ActualLen)
		Increase(RESIZE_CHUNKS);

	BinData[Len]=Byte;

	Len+=1;
}

void CompFrame::WriteWord(u16 Word)
{
	if ((Len+2) >= ActualLen)
		Increase(RESIZE_CHUNKS);

	BinData[Len]=Word&0xff;
	BinData[Len+1]=Word>>8;

	Len+=2;
}

void CompFrame::WriteData(u8 const  * Src,int Amount)
{
	if ((Len+Amount) >= ActualLen)
		Increase(Amount+RESIZE_CHUNKS);

	memcpy(&BinData[Len],Src,Amount);

	Len+=Amount;

}

void CompFrame::Write(ofstream & Out) const
{
	if (Len)
		{
		char const * Data;
		Data=(char const *)BinData;

		Out.write(Data,Len);
		}
}

ostream & operator<<(ostream & str,CompFrame & Fr)
{
	str<<"size: "<<Fr.GetSize();
	return(str);
}


void CompFrame::operator=(CompFrame const & Fr)
{
	Dump();

	Resize(Fr.Len);

	if (Fr.Len)
		memcpy(BinData,Fr.BinData,Fr.Len);

	Len=Fr.Len;
}

CompFrame::CompFrame(CompFrame const & C)
{
	Init();

	*this=C;
}


/*----------------------------------------------------------------------
	Large Page Shite
  ---------------------------------------------------------------------- */
LpObj::LpObj(void)
{
	BaseRecord=0;
}

void LpObj::WriteHdr(ofstream & Out) const
{
	WriteWord(Out,BaseRecord);
	WriteWord(Out,MyFrames.size());
	WriteWord(Out,GetSizeInAll()+1+1+2);
}

void LpObj::Write(ofstream & Out) const
{
	int		f;

	WriteWord(Out,BaseRecord);
	WriteWord(Out,MyFrames.size());
	WriteWord(Out,GetSizeInAll()+1+1+2);
	WriteWord(Out,0);

	for (f=0;f<MyFrames.size();f++)
		WriteWord(Out,MyFrames[f].GetSize());


	for (f=0;f<MyFrames.size();f++)
		{
		
		MyFrames[f].Write(Out);
		}
}

bool LpObj::AddFrame(CompFrame const & C )
{

	if (((GetSizeInAll()+C.GetSize()) > 64000) && MyFrames.size())
		return(false);
	else
		{
		MyFrames.push_back(C);
		return(true);
		}
}

void LpObj::operator=(LpObj const & No)
{
	BaseRecord=No.BaseRecord;
	MyFrames=No.MyFrames;
}

int LpObj::GetSizeInAll(void) const
{
	int		Total=0;

	for (int f=0;f<MyFrames.size();f++)
		Total+=MyFrames[f].GetSize();

	return(Total);
}


/*----------------------------------------------------------------------
	Helpy Stuff
  ---------------------------------------------------------------------- */
void WriteWord(ofstream & Out,u16 Word)
{
	Out.put((u8)(Word&0xff));
	Out.put((u8)(Word>>8));
}


/*===========================================================================
 end */




