/*=========================================================================

	paul.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */


#include "except.h"

#ifdef __INSTALL_EXCEPTION_HANDLER__

#ifndef _GLOBAL_HEADER_
#include "global.h"
#endif

#ifndef	__GFX_FONT_H__
#include "gfx\font.h"
#endif

#ifndef	__VID_HEADER_
#include "system\vid.h"
#endif

#ifndef __SYSTEM_LNKOPT_H__
#include "system\lnkopt.h"
#endif

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif


/*	Std Lib
	------- */
#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>


/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

// Offsets in the register dump list
enum {
	OFS_AT,OFS_V0,OFS_V1,OFS_A0,OFS_A1,OFS_A2,OFS_A3,OFS_T0,OFS_T1,OFS_T2,OFS_T3,
	OFS_T4,OFS_T5,OFS_T6,OFS_T7,OFS_S0,OFS_S1,OFS_S2,OFS_S3,OFS_S4,OFS_S5,OFS_S6,
	OFS_S7,OFS_T8,OFS_T9,OFS_GP,OFS_SP,OFS_FP,OFS_RA,OFS_HI,OFS_LO,OFS_SR,OFS_CA,
	OFS_EPC
};

// Pages
enum
{
	PAGE_REGS,
	PAGE_DUMP,
	PAGE_VALUES,

	NUM_PAGES
};


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

// Used by displayRegisters to define page layout
typedef struct
{
	char	*m_name;
	int		m_offset;
} RegDef;

  
/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

static void displayTitle();
static void displayCause();
static void displayRegs();
static void displayDump();
static void displayValues();
static void displayKeys();

static void	cls();
static void print(char *_string);
static void printAdr(int *_adr);



/*----------------------------------------------------------------------
	Vars
	---- */

// Exception description strings
static char *s_exceptionText[]={
	"external interrupt",			"tlb modification exception",			"tlb miss (load or fetch)",
	"tlb miss (store)",				"address error (load or fetch)",		"address error (store)",
	"bus error (fetch)",			"bus error (load or store)",			"syscall",
	"break",						"reserved instruction",					"coprocessor unusable",
	"arithmetic overflow",			"unknown exception",					"unknown exception",
	"unknown exception"
};
static char *s_breakText[]={
	"6 (overflow)",					"7 (div by zero)"
};

// Font stuff
static FontBank *excFont;
static int fontx;
static int fonty;
static const int LEFTMARGIN=20;
static const int TOPMARGIN=20;
static const int CHARWIDTH=11;
static const int CHARHEIGHT=10;

static int	currentPage=PAGE_REGS;


// Layout for displayRegs()
static RegDef	s_regDefs[]=
{
	{"at",OFS_AT},{"v0",OFS_V0},{"v1",OFS_V1},{NULL,0},
	{"a0",OFS_A0},{"a1",OFS_A1},{"a2",OFS_A2},{NULL,0},
	{"a3",OFS_A3},{"t0",OFS_T0},{"t1",OFS_T1},{NULL,0},
	{"t2",OFS_T2},{"t3",OFS_T3},{"t4",OFS_T4},{NULL,0},
	{"t5",OFS_T5},{"t6",OFS_T6},{"t7",OFS_T7},{NULL,0},
	{"s0",OFS_S0},{"s1",OFS_S1},{"s2",OFS_S2},{NULL,0},
	{"s3",OFS_S3},{"s4",OFS_S4},{"s5",OFS_S5},{NULL,0},
	{"s6",OFS_S6},{"s7",OFS_S7},{"t8",OFS_T8},{NULL,0},
	{"t9",OFS_T9},{"gp",OFS_GP},{"sp",OFS_SP},{NULL,0},
	{"fp",OFS_FP},{"ra",OFS_RA},{"hi",OFS_HI},{NULL,0},
	{"lo",OFS_LO},{"sr",OFS_SR},{"ca",OFS_CA},{NULL,0},
	{"epc",OFS_EPC},
};
static int	s_numRegs=sizeof(s_regDefs)/sizeof(RegDef);

// Pointers for displayDump();
static int *s_viewSp;
static int *s_viewPc;

// Various externs for displayValues()
extern sLList		MainRam;
extern int			MemNodeCount;



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern void installExceptionHandler()
{
	install_exc();
	excFont=new ("ExcFont") FontBank();
	excFont->initialise( &standardFont );
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void exc_c(void)
{
	static int	s_padDelay=50;

	PadInit(0);
	s_viewSp=(int *)reg_lst[OFS_SP];
	s_viewPc=(int *)reg_lst[OFS_EPC];

	while(1)
	{
		// Think
		if(s_padDelay)
		{
			s_padDelay--;
		}
		else
		{
			int	pad=PadRead(1);
			
			if(pad&PADRdown)
			{
				if(++currentPage==NUM_PAGES)currentPage=0;
				s_padDelay=15;
			}
			if(currentPage==PAGE_DUMP)
			{
				if(pad&PADL1)	s_viewSp-=8;
				if(pad&PADL2)	s_viewSp+=8;
				if(pad&PADR1)	s_viewPc-=8;
				if(pad&PADR2)	s_viewPc+=8;
				if(pad&PADstart)
				{
					s_viewSp=(int *)reg_lst[OFS_SP];
					s_viewPc=(int *)reg_lst[OFS_EPC];
				}
			}
		}


		// Draw
		cls();
		displayTitle();
		displayCause();
		switch(currentPage)
		{
			case PAGE_REGS:
				displayRegs();
				break;
			case PAGE_DUMP:
				displayDump();
				break;
			case PAGE_VALUES:
				displayValues();
				break;
		}
		displayKeys();

		// Swap
		while(DrawSync(1));
		VidSwapDraw();
		PrimDisplay();
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void	cls()
{
	fontx=LEFTMARGIN;
	fonty=TOPMARGIN;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void print(char *_string)
{
	char	text[2]="?";
	while(*_string)
	{
		char	_next=*_string;
		if(_next=='\n')
		{
			fontx=LEFTMARGIN;
			fonty+=CHARHEIGHT;
		}
		else
		{
			text[0]=*_string;
			excFont->print(fontx+((CHARWIDTH-excFont->getCharWidth(text[0]))/2),fonty,text);
			fontx+=CHARWIDTH;
		}
		_string++;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void printAdr(int *_adr)
{
	int		chk,dummy_mem,error;
	char	textBuf[64];

	chk=(int)_adr;
	dummy_mem=chk&0xf0000000;
	chk&=0x0ffffffc;
	error=1;
	if(chk<(int)OPT_LinkerOpts.RamSize)
		error=0;
	if(chk>0x1f800000&&chk<0x1f800400)
		error=0;
	_adr=(int *)(chk|dummy_mem);
	if(error==0)
		sprintf(textBuf,"%08x=%08x",(int)(_adr),*_adr);
	else
		sprintf(textBuf,"%08x=XXXXXXXX",(int)(_adr));
	print(textBuf);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void displayTitle()
{
	static int	r=0,g=0,b=0;
	char		textBuf[10];

	excFont->setColour(r,g,b);
	print("[");
	excFont->setColour(255,255,255);
	switch(currentPage)
	{
		case PAGE_REGS:
			print("Registers");
			break;
		case PAGE_DUMP:
			print("Dumps");
			break;
		case PAGE_VALUES:
			print("Values");
			break;
	}
	excFont->setColour(r,g,b);
	print("]\n\n");

	r+=8;
	g+=16;
	b+=32;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void displayCause()
{
	char		textBuf[100];
	int			*exc;

	excFont->setColour(255,0,0);

	exc=(int*)reg_lst[OFS_EPC];
	sprintf(textBuf,"%s",s_exceptionText[reg_lst[OFS_CA]>>2&0x1f]);
	print(textBuf);

	if((reg_lst[OFS_CA]>>2&0x1f)==9)
	{
		int i=(*exc>>16)-6;
		if(i==0||i==1)
			sprintf(textBuf," %s",s_breakText[i]);
			print(textBuf);
	}
	
	sprintf(textBuf,"\nPC=%08x",(int)exc);
	print(textBuf);
	
	if((reg_lst[OFS_CA]&0x80000000)==0x80000000)
		print(" in branch delay slot");
	print("\n\n");
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void displayRegs()
{
	char		textBuf[20];

	excFont->setColour(255,255,255);
	RegDef	*regs=s_regDefs;
	for(int i=0;i<s_numRegs;i++)
	{
		if(regs->m_name)
		{
			sprintf(textBuf,"%s=%08x  ",regs->m_name,reg_lst[regs->m_offset]);
		}
		else
		{
			sprintf(textBuf,"\n");
		}
		print(textBuf);
		regs++;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void displayDump()
{
	char	textBuf[100];
	int		*sp,*pc;

	excFont->setColour(255,255,255);

	sp=s_viewSp;
	pc=s_viewPc;
	print("SP                    PC\n");
	for(int i=0;i<10;i++)
	{
		// SP
		if(sp==(int*)reg_lst[OFS_SP])
		{
			print(">");
		}
		else
		{
			print(" ");
		}
		printAdr(sp);
		print("    ");
		sp+=8;

		// PC
		if(pc==(int*)reg_lst[OFS_EPC])
		{
			print(">");
		}
		else
		{
			print(" ");
		}
		printAdr(pc);
		print("\n");
		pc+=8;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void displayValues()
{
	char		textBuf[256];
	CScene		*scene;

	excFont->setColour(255,255,255);

	print("Memory:\n");
	sprintf(textBuf," %ld used in %d nodes (%ld available)\n",MainRam.RamUsed,MemNodeCount,MainRam.TotalRam);
	print(textBuf);
	sprintf(textBuf," %dk ram size set in link options\n",(int)(OPT_LinkerOpts.RamSize/1024));
	print(textBuf);

	print("Scene:\n");
	scene=GameState::getCurrentScene();
	sprintf(textBuf," Current scene: %s\n",scene?scene->getSceneName():"None!?!?");
	print(textBuf);
	scene=GameState::getPendingScene();
	sprintf(textBuf," Pending scene: %s\n",scene?scene->getSceneName():"None");
	print(textBuf);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
static void displayKeys()
{
	char		textBuf[256];

	excFont->setColour(64,32,128);
	
	sprintf(textBuf,"\n\nX=Next page\n");
	print(textBuf);

	textBuf[0]='\0';
	switch(currentPage)
	{
		case PAGE_REGS:
			break;
		case PAGE_DUMP:
			sprintf(textBuf,"L1/L2=Scroll SP, R1/R2=Scroll PC\nSTART=Reset");
			break;
		case PAGE_VALUES:
			break;
	}
	print(textBuf);
}


#else

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern void installExceptionHandler()
{
}
extern void exc_c()
{
}

#endif	/* __INSTALL_EXCEPTION_HANDLER__ */

/*===========================================================================
 end */