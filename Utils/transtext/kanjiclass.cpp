/*=========================================================================

	KANJICLASS.CPP	

	Author:  Gary Liddon @
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 G R Liddon

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "kanjiclass.h"

/*	Std Lib
	------- */
#include <iostream>

/*	Glib
	---- */
#include <frame.hpp>
#include <misc.hpp>

/*	Local
	----- */

using namespace std;

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
#include	"fontdata/got11j0b.h"
#include	"fontdata/got11j1b.h"
#include	"fontdata/got13j0b.h"
#include	"fontdata/got13j1b.h"
#include	"fontdata/got15j0b.h"
#include	"fontdata/got15j1b.h"
#include	"fontdata/got15gai.h"

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */
static int const kanjiAddr0[3]=
{
	u32(got11j0b),
	u32(got13j0b),
	u32(got15j0b),
};

static int const kanjiAddr1[3]=
{
	u32(got11j1b),
	u32(got13j1b),
	u32(got15j1b),
};

static int const kanjiAddrG[3]=
{
	u32(0),
	u32(0),
	u32(got15gai),
};

static int const dotSize[3]={11,13,15};

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */
struct SjisS
{
	char 			Ascii;
	unsigned char 	Num;
	unsigned short 	Sjis;
	char const *	name;
};

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */
static void dekan(u8 *ptr,u8 *buf,int dot);
static long kanjiTest( u8 const *sjis,KanjiClass::KanjiFontSize k = KanjiClass::FONT_SIZE_11);
static int kanjiTrans(u16 scode);
static void ascii2SJIS(u8 *dest,u8 const *src);
static void ascii2SJIS(u8 *dest,u8 Ascii);
static GString makeName(char const * dir,int frameNumber);



/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Data
	---- */

static SjisS SjisTable[]=
{
	{'0',10, 0x824f,"num" },{'A',26, 0x8260,"upper_case" },{'a',26, 0x8281,"lower_case" },
	{' ', 1,0x8140,"space"},{'!', 1,0x8149,"exclamation",},{'"', 1,0x8168,"double_quote"},{'#', 1,0x8194,"hash",},{'$', 1,0x8190,"dollar"},
	{'%', 1,0x8193,"percent"},{'&', 1,0x8195,"and"},{'\'',1,0x8166,"single_quote"},{'(', 1,0x8169,"open_bracket"},{')', 1,0x816a,"close_bracket"},
	{'*', 1,0x8196,"star"},{'+', 1,0x817b,"plus",},{',', 1,0x8143,"comma",},{'-', 1,0x817c,"minus"},{'.', 1,0x8144,"period"},
	{'/', 1,0x815e,"forward_slash"},{':', 1,0x8146,"colon"},{';', 1,0x8147,"semi_colon"},{'<', 1,0x8171,"less_than"},{'=', 1,0x8181,"equals"},
	{'>', 1,0x8172,"greater_than"},{'?', 1,0x8148,"question_mark"},{'@', 1,0x8197,"ampersand"},{'[', 1,0x816d,"open_square_brace"},{'\\',1,0x818f,"back_slash"},
	{']', 1,0x816e,"close_square_brace"},{'^', 1,0x814f,"carrat"},{'_', 1,0x8151,"underscore"},{'`', 1,0x8165,"other_quote"},{'{', 1,0x816f,"open_brace"},
	{'|', 1,0x8162,"or"},{'}', 1,0x8170,"close_brace"},{'~', 1,0x8150,"tilde"},{0,0,0,NULL}
};

/* 
 * sjis code to access number transfer table (non-Kanji)
 */
static unsigned short kanji_0_table[19][2] = {
	{0x8140, 0}, 
	{0x8180, 63}, 
	{0x81b8, 108},
	{0x81c8, 116},
	{0x81da, 123},
	{0x81f0, 138},
	{0x81fc, 146},

	{0x824f, 147},
	{0x8260, 157},
	{0x8281, 183},
	{0x829f, 209},
	{0x8340, 292},
	{0x8380, 355},

	{0x839f, 378},
	{0x83bf, 402},
	{0x8440, 426},
	{0x8470, 459},
	{0x8480, 474},
	{0x849f, 492}
};

/* 
 * sjis code to access number transfer table (user defined characters)
 */
static unsigned short kanji_g_table[19][2] = {
	{0x8540, 0},
	{0x8546, 5},
	{0x8548, 6},
	{0x854b, 8},
	{0x8550, 9},

	{0x85a0, 21},
	{0x85b0, 34},

	{0x8640, 47},
	{0x8643, 49},
	{0x8645, 50},
	{0x8647, 51},
	{0x864a, 53},
	{0x864e, 56},
	{0x8660, 58},

	{0x869f, 74},
	{0x86b8, 98},
	{0x8740, 160},
	{0x875c, 187},
	{0x8764, 194},
};

/* 
 * sjis code to access number transfer table (First level Kanji)
 */
static unsigned short kanji_1_table[32][2] = {
	{0x889f, 0},
	{0x8940, 94},
	{0x899f, 188},
	{0x8a40, 282},
	{0x8a9f, 376},
	{0x8b40, 470},
	{0x8b9f, 564},
	{0x8c40, 658},
	{0x8c9f, 752},
	{0x8d40, 846},
	{0x8d9f, 940},
	{0x8e40, 1034},
	{0x8e9f, 1128},
	{0x8f40, 1222},
	{0x8f9f, 1316},
	{0x9040, 1410},
	{0x909f, 1504},
	{0x9140, 1598},
	{0x919f, 1692},
	{0x9240, 1786},
	{0x929f, 1880},
	{0x9340, 1974},
	{0x939f, 2068},
	{0x9440, 2162},
	{0x949f, 2256},
	{0x9540, 2350},
	{0x959f, 2444},
	{0x9640, 2538},
	{0x969f, 2632},
	{0x9740, 2726},
	{0x979f, 2820},
	{0x9840, 2914}
};

/* 
 * sjis code to access number transfer table (Second level Kanji)
 */
static unsigned short kanji_2_table[37][2] = {
	{0x989f, 0},
	{0x9940, 94},
	{0x999f, 188},
	{0x9a40, 282},
	{0x9a9f, 376},
	{0x9b40, 470},
	{0x9b9f, 564},
	{0x9c40, 658},
	{0x9c9f, 752},
	{0x9d40, 846},
	{0x9d9f, 940},
	{0x9e40, 1034},
	{0x9e9f, 1128},
	{0x9f40, 1222},
	{0x9f9f, 1316},
	{0xe040, 1410},
	{0xe09f, 1504},
	{0xe140, 1598},
	{0xe19f, 1692},
	{0xe240, 1786},
	{0xe29f, 1880},
	{0xe340, 1974},
	{0xe39f, 2068},
	{0xe440, 2162},
	{0xe49f, 2256},
	{0xe540, 2350},
	{0xe59f, 2444},
	{0xe640, 2538},
	{0xe69f, 2632},
	{0xe740, 2726},
	{0xe79f, 2820},
	{0xe840, 2914},
	{0xe89f, 3008},
	{0xe940, 3102},
	{0xe99f, 3196},
	{0xea40, 3290},
	{0xea9f, 3384}
};

/*
 * sjis code to access number transfer table (Vertical writing)
 */
static unsigned short kanji_t_table[2][2] = {
	{0xeb40, 0},
	{0xeb42, 4}
};



/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static GString makeName(char const * dir,int frameNumber)
{
	char	tempBuff[1024];

	sprintf(tempBuff,"%s\\%04d.lbm",dir,frameNumber);
	return(tempBuff);
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void KanjiClass::writeEnglishFontLbms(char const * _dir,KanjiFontSize k,char const * repFile)
{
	SjisS * jisTab;

	jisTab=SjisTable;

	GString		dir;

	dir=_dir;
	dir+="\\";

	vector<GString>		allLbms;

	while (jisTab->name)
		{
		u16			num;
		GString		name;
		GString		fileName;
		
		num=jisTab->Sjis;
		name=jisTab->name;
		fileName=dir+name;
	
		if (jisTab->Num != 1)
			{
			for (int f=0;f<jisTab->Num;f++)
				{
				char		saveName[300];

				sprintf(saveName,"%s_%c.lbm",(char const *)fileName,jisTab->Ascii+f);

				saveKanjiLbm(saveName,k,num);
				allLbms.push_back(saveName);
				num++;
				}
			}
		else
			{
			GString		saveName;
			saveName=fileName;
			saveName+=".lbm";
			saveKanjiLbm(saveName,k,num);
			allLbms.push_back(saveName);
			}

		jisTab++;
		}

	if (repFile)
		{
		ofstream out;

		out.open(repFile,ios::out|ios::trunc);

		if (out)
			{	
			for (int f=0;f<allLbms.size();f++)
				out<<allLbms[f]<<endl;
			
			out.close();
			}
		else
			Error(ERR_FATAL,"error opening %s for output",repFile);
		}

	
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void KanjiClass::saveKanjiLbmNames(char const * dir,char const *name)
{
	int		num;

	num=0;

	ofstream	nameFile;

	nameFile.open(name,ios::trunc);

	if (nameFile)
		{
		for(int l=0;l<256;l++)
			{
			for(int h=0;h<256;h++)
				{
				if(getKanjiHit(h,l))
					{
					GString		name;
					name=makeName(dir,num);
					num++;
					nameFile<<name<<endl;
					}
				}
			}
		}

	nameFile.close();

}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

int KanjiClass::findKanjiIndex(u16 kan) const
{
    if(kan>=0x8140 && kan<=0x84be)
		kan-=0x8140;
	else
		{
        if(kan>=0x8540 && kan<=0x8796)
			{
			kan-=0x8540;
			kan+=((0x84be-0x8140)+1);
			}
		else
			{
            if(kan>=0x889f && kan<=0x9872)
				{
				kan-=0x889f;
				kan+=((0x84be-0x8140)+1);
				kan+=((0x8796-0x8540)+1);
				}
			else
				Error(ERR_FATAL,"illegal kanji char");
			}
		}
	return kan;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void KanjiClass::saveKanjiTable(char const *name)
{
	int		num,highest;

	num=0;
	highest=0;

	vector<int>		kanjiChars;
	kanjiChars.resize(256*256);
	
	for (int f=0;f<256*256;f++)
		kanjiChars[f]=-1;

	for(int l=0;l<256;l++)
		{
		for(int h=0;h<256;h++)
			{
			if(getKanjiHit(h,l))
				{
				u16		kan;
				int		val;


				kan=(h<<8)|l;


				val=findKanjiIndex(kan);
				kanjiChars[val]=num;

				if (val > highest)
					highest=val;

				num++;
				}
			}
		}
	
	cout<<"highest is "<<highest<<endl;

	Gofstream	nameFile(Gofstream::LITTLE_ENDIAN);
	if (nameFile)
		{
		nameFile.open(name,ios::trunc|ios::binary);

		nameFile.Put16(howManyKanjiChars());
		
		for (int f=0;f<highest+1;f++)
			nameFile.Put16(kanjiChars[f]);

		nameFile.close();
		}
	else
		GObject::Error(ERR_FATAL,"error opening %s for writing",name);

}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void KanjiClass::saveKanjiLbm(char const * fileName,KanjiFontSize k,u16 kan)
{
	char buff[32*32];
	long addr;
	char kbuff[2];

	kbuff[0]=kan>>8;
	kbuff[1]=kan&0xff;


	addr=kanjiTest((u8 *)&kbuff,k);

	if(addr!=-1)
		{
		const int PLOT_X = 100;
		const int PLOT_Y = 100;
		int			dot;

		dot=dotSize[k];

		memset(buff,0,sizeof(buff));
		dekan((u8*)addr,(u8*)&buff,dot);

		Frame		myFrame;
		Frame		topFrame;
		Palette		myPal;

		for (int f=0;f<256;f++)
			myPal[f].SetRGB(255,255,0);

		myPal[0].SetRGB(255,0,0);
		myPal[1].SetRGB(255,255,255);
		myPal[2].SetRGB(0,0,0);
		myPal[3].SetRGB(64,64,64);
		myPal[255].SetRGB(255,0,255);

		myFrame.SetFrame((u8*)buff,16,dot,myPal);
		topFrame=myFrame;

		myFrame.Resize(32,dot+2);
		myFrame.Clear(0);


		topFrame.ReplaceColour(1,3);
		topFrame.PlotTrans(myFrame,0,0);
		topFrame.PlotTrans(myFrame,2,0);
		topFrame.PlotTrans(myFrame,0,2);
		topFrame.PlotTrans(myFrame,2,2);

		topFrame.ReplaceColour(3,2);
		topFrame.PlotTrans(myFrame,1,0);
		topFrame.PlotTrans(myFrame,1,2);
		topFrame.PlotTrans(myFrame,0,1);
		topFrame.PlotTrans(myFrame,2,1);
		
		topFrame.ReplaceColour(2,1);
		topFrame.PlotTrans(myFrame,1,1);

		Rect		myRect;


		myRect=topFrame.FindBoundingRect();
	
		topFrame.Resize(256,200);
		topFrame.Clear(0);
		topFrame.DrawBox(Rect(100,0,1,200),255);
		topFrame.DrawBox(Rect(0,100,200,1),255);
		myFrame.PlotTrans(topFrame,100-myRect.X,(100-dot));

		topFrame.SaveLbm(fileName);
	
		cout<<"saved "<<fileName<<endl;
		}
	else
		Error(ERR_FATAL,"Kanji Failed bad style on test of kanji hit");
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void KanjiClass::saveKanjiLbms(char const * dir,KanjiFontSize k)
{
	int num=0;

	for(int l=0;l<256;l++)
		{
		for(int h=0;h<256;h++)
			{
			if(getKanjiHit(h,l))
				{
				GString		name;
				name=makeName(dir,num);
				u16		kan;

				kan=(h<<8)|l;

				saveKanjiLbm(name,k,kan);
				num++;
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
void KanjiClass::addStr(u8 const * newStr)
{
	u8 tempstr[4096];

	u8 const *str=newStr;

	ascii2SJIS(tempstr,str);

	str=tempstr;
	
	while(*str)
		{
		if(kanjiTest(str)!=-1)
			{
			addKanjiHit(str[0],str[1]);
			str++;
			}
		str++;
		}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
KanjiClass::KanjiClass(void)
{
	initialise();
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void KanjiClass::initialise(void)
{
	m_kanjiHit.resize(256*256);

	for (int x=0;x<256;x++)
		for (int y=0;y<256;y++)
			clearKanjiHit(x,y);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int KanjiClass::howManyKanjiChars(void)
{
	int		count;

	count=0;

	for (int x=0;x<256;x++)
		for (int y=0;y<256;y++)
			if (getKanjiHit(x,y) != 0)
				count++;

	return(count);
}
 
/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void checkKanjiCode(uint hi,uint lo)
{
	if(hi>=256)
		GObject::Error(ERR_FATAL,"high char too big");
	if(lo>=256)
		GObject::Error(ERR_FATAL,"low char too big");
}

void KanjiClass::addKanjiHit(uint hi,uint lo)
{
	if(!hi && !lo)
		{
		GObject::Error(ERR_FATAL,"passed a bad kanji code");
		}

	checkKanjiCode(hi,lo);
	m_kanjiHit[hi*256+lo]=1;
}

bool KanjiClass::getKanjiHit(uint hi,uint lo)
{
	checkKanjiCode(hi,lo);
	return(m_kanjiHit[hi*256+lo]!=0);
}

void KanjiClass::clearKanjiHit(uint hi,uint lo)
{
	checkKanjiCode(hi,lo);
	m_kanjiHit[hi*256+lo]=0;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void ascii2SJIS(u8 *dest,u8 const *src)
{
	while (*src)
		{
		if ((src[0])&0x80)
			{
			*dest++=*src++;
			*dest++=*src++;
			}
		else
			{
			ascii2SJIS(dest,*src);
			src++;
			dest+=2;
			}
		}

    *dest++=0;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int kanjiTrans(u16 scode)
{
	int ret = 0;
	u8 ss[2];
	u8 stmp;

	ss[0]=scode&0xff;
	ss[1]=scode>>8&0xff;
	//bcopy(&scode, ss, 2);
	
	switch(ss[1])
	{
	case 0x81:
		if ((ss[0] >= 0x40) && (ss[0] <= 0x7e))
			stmp = 0;
		else
		if ((ss[0] >= 0x80) && (ss[0] <= 0xac))
			stmp = 1;
		else
		if ((ss[0] >= 0xb8) && (ss[0] <= 0xbf))
			stmp = 2;
		else
		if ((ss[0] >= 0xc8) && (ss[0] <= 0xce))
			stmp = 3;
		else
		if ((ss[0] >= 0xda) && (ss[0] <= 0xe8))
			stmp = 4;
		else
		if ((ss[0] >= 0xf0) && (ss[0] <= 0xf7))
			stmp = 5;
		else
		if (ss[0] == 0xfc)
			stmp = 6;
		else {
			ret = -1;
			break;
		}
		ret = scode - kanji_0_table[stmp][0] + kanji_0_table[stmp][1];
		break;

	case 0x82:
		if ((ss[0] >= 0x4f) && (ss[0] <= 0x58))
			stmp = 7;
		else
		if ((ss[0] >= 0x60) && (ss[0] <= 0x79))
			stmp = 8;
		else
		if ((ss[0] >= 0x81) && (ss[0] <= 0x9a))
			stmp = 9;
		else
		if ((ss[0] >= 0x9f) && (ss[0] <= 0xf1))
			stmp = 10;
		else {
			ret = -1;
			break;
		}
		ret = scode - kanji_0_table[stmp][0] + kanji_0_table[stmp][1];
		break;

	case 0x83:
		if ((ss[0] >= 0x40) && (ss[0] <= 0x7e))
			stmp = 11;
		else
		if ((ss[0] >= 0x80) && (ss[0] <= 0x96))
			stmp = 12;
		else
		if ((ss[0] >= 0x9f) && (ss[0] <= 0xb6))
			stmp = 13;
		else
		if ((ss[0] >= 0xbf) && (ss[0] <= 0xd6))
			stmp = 14;
		else {
			ret = -1;
			break;
		}
		ret = scode - kanji_0_table[stmp][0] + kanji_0_table[stmp][1];
		break;

	case 0x84:
		if ((ss[0] >= 0x40) && (ss[0] <= 0x60))
			stmp = 15;
		else
		if ((ss[0] >= 0x70) && (ss[0] <= 0x7e))
			stmp = 16;
		else
		if ((ss[0] >= 0x80) && (ss[0] <= 0x91))
			stmp = 17;
		else
		if ((ss[0] >= 0x9f) && (ss[0] <= 0xbe))
			stmp = 18;
		ret = scode - kanji_0_table[stmp][0] + kanji_0_table[stmp][1];
		break;

	case 0x85:
		if ((ss[0] >= 0x40) && (ss[0] <= 0x44))
			stmp = 0;
		else
		if (ss[0] == 0x46)
			stmp = 1;
		else
		if ((ss[0] >= 0x48) && (ss[0] <= 0x49))
			stmp = 2;
		else
		if (ss[0] == 0x4b)
			stmp = 3;
		else
		if ((ss[0] >= 0x50) && (ss[0] <= 0x5b))
			stmp = 4;
		else
		if ((ss[0] >= 0xa0) && (ss[0] <= 0xac))
			stmp = 5;
		else
		if ((ss[0] >= 0xb0) && (ss[0] <= 0xbc))
			stmp = 6;
		else {
			ret = -1;
			break;
		}
		ret = scode - kanji_g_table[stmp][0] + kanji_g_table[stmp][1];
		break;

	case 0x86:
		if ((ss[0] >= 0x40) && (ss[0] <= 0x41))
			stmp = 7;
		else
		if (ss[0] == 0x43)
			stmp = 8;
		else
		if (ss[0] == 0x45)
			stmp = 9;
		else
		if ((ss[0] >= 0x47) && (ss[0] <= 0x48))
			stmp = 10;
		else
		if ((ss[0] >= 0x4a) && (ss[0] <= 0x4c))
			stmp = 11;
		else
		if ((ss[0] >= 0x4e) && (ss[0] <= 0x4f))
			stmp = 12;
		else
		if ((ss[0] >= 0x60) && (ss[0] <= 0x6f))
			stmp = 13;
		else
		if ((ss[0] >= 0x9f) && (ss[0] <= 0xb6))
			stmp = 14;
		else
		if ((ss[0] >= 0xb8) && (ss[0] <= 0xf5))
			stmp = 15;
		else {
			ret = -1;
			break;
		}
		ret = scode - kanji_g_table[stmp][0] + kanji_g_table[stmp][1];
		break;

	case 0x87:
		if ((ss[0] >= 0x40) && (ss[0] <= 0x5a))
			stmp = 16;
		else
		if ((ss[0] >= 0x5c) && (ss[0] <= 0x62))
			stmp = 17;
		else
		if ((ss[0] >= 0x64) && (ss[0] <= 0x96))
			stmp = 18;
		else {
			ret = -1;
			break;
		}
		ret = scode - kanji_g_table[stmp][0] + kanji_g_table[stmp][1];
		break;

	default:
		if ((ss[1] > 0x98) || ((ss[1] == 0x98) && (ss[0] > 0x72)) ||
		    (ss[1] < 0x88) || ((ss[1] == 0x88) && (ss[0] < 0x9f)) ||
		    (ss[0] > 0xfc) || (ss[0] < 0x40) ||
		    ((ss[0] > 0x7e) && (ss[0] < 0x80))) {
			ret = -1;
			break;
		}
		stmp = ((ss[1] - 0x88)*2) + ((ss[0] < 0x7f) ? 0 : 1) - 1;
		ret = scode - kanji_1_table[stmp][0] + kanji_1_table[stmp][1];
	}
	
	return(ret);
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
  long kanjiTest( u8 const *sjis,KanjiClass::KanjiFontSize k)
{
	int	code;
	u32	kaddr, addr;
	u16 sjiscode;
	int	dot;

	dot=dotSize[k];

	sjiscode = *sjis << 8 | *(sjis+1);

	if ((sjiscode >= 0x8140) && (sjiscode <= 0x84be))
		kaddr = kanjiAddr0[k];
	else if (dot==15 && (sjiscode >= 0x8540) && (sjiscode <= 0x8796))
		kaddr = kanjiAddrG[k];
	else if ((sjiscode >= 0x889f) && (sjiscode <= 0x9872))
		kaddr = kanjiAddr1[k];
    else
		return -1;
  
  code = kanjiTrans(sjiscode);
  addr = kaddr + code * dot * 2;

  return addr;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void dekan(u8 *ptr,u8 *buf,int dot)
{
	int i,j,k;
	char tmp;
	int xchar, ychar;

	xchar = 2;
	ychar = dot;

	for (i=0 ; i < ychar ; i++)
		{
		for (j=0 ; j < xchar ; j++)
			{
			tmp = *(ptr+i*xchar+j);
			for (k=0 ; k < 8 ; k++)
				{
				if (tmp & 0x80)
					*buf++=1;
				else
					*buf++=0;

				tmp *= 2 ;
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
void ascii2SJIS(u8 *dest,u8 Ascii)
{
	u16 kan;
	int done=0;

    struct  SjisS   *Ptr=SjisTable;

     while (!done && Ptr->Ascii)
     {

        if ((Ascii>=Ptr->Ascii) && (Ascii<(Ptr->Ascii+Ptr->Num)))
        {
            /* Found ASCII char, return SJIS equivalent */
            kan= (Ptr->Sjis+(Ascii-Ptr->Ascii));
			done=1;
        }
        Ptr++;
     }

	if(!done)
		kan=0x8148;

	*dest++=(kan>>8);
	*dest++=(kan&0xff);
}

/*===========================================================================
 end */
