#include <sys/types.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include "system\global.h"
#include "except.h"
#include "pad\pads.h"

#define	NTSC_PAL	1

enum {
OFS_AT,OFS_V0,OFS_V1,OFS_A0,OFS_A1,OFS_A2,OFS_A3,OFS_T0,OFS_T1,OFS_T2,OFS_T3,
OFS_T4,OFS_T5,OFS_T6,OFS_T7,OFS_S0,OFS_S1,OFS_S2,OFS_S3,OFS_S4,OFS_S5,OFS_S6,
OFS_S7,OFS_T8,OFS_T9,OFS_GP,OFS_SP,OFS_FP,OFS_RA,OFS_HI,OFS_LO,OFS_SR,OFS_CA,
OFS_EPC};

typedef struct {
	DRAWENV	draw;
	DISPENV	disp;
} EXC_DB;

enum
{
	PAGE_REGS,
	PAGE_DUMP,
	PAGE_VALUES,

	NUM_PAGES
};

static void exc_swap(void);
static void print_adr(int *adr);

static void	cls();
static void print(char *_string);

static void displayTitle();
static void displayCause();
static void displayRegs();



static EXC_DB exc_db[2];
static int padWait;
int number=0;
static int mem_size;
static int exc_id;

static char *exc_txt[]={
	"external interrupt",
	"tlb modification exception",
	"tlb miss (load or fetch)",
	"tlb miss (store)",
	"address error (load or fetch)",
	"address error (store)",
	"bus error (fetch)",
	"bus error (load or store)",
	"syscall",
	"break",
	"reserved instruction",
	"coprocessor unusable",
	"arithmetic overflow",
	"unknown exception",
	"unknown exception",
	"unknown exception"};

static char *break_txt[]={
	"6 (overflow)",
	"7 (div by zero)"};

static char str_reg[1200];
static char str_dump[1200];
static char *p_sr;
static char *p_sd;



static int	currentPage=PAGE_REGS;

#include "gfx\font.h"
FontBank *excFont;
extern void installExceptionHandler()
{
	install_exc();
	excFont=new ("ExcFont") FontBank();
	excFont->initialise( &standardFont );
	excFont->setColour( 255, 255 , 0 );
}



/* --------------------------------- exc_c ---------------------------------- */
extern void exc_c(void) {

	int i,*p_exc,*cur_exc,*p_sp,*cur_sp;//,pad;

	ResetCallback();
	ResetGraph(0);
	PadInit(0);
	SetGraphDebug(0);
	SetVideoMode(NTSC_PAL==0 ? MODE_NTSC: MODE_PAL);
	SetDispMask(1);

	FntLoad(640,0);
	SetDumpFnt(FntOpen(8,8,320-8,240-8,0,1024));

	SetDefDispEnv(&exc_db[0].disp,  0,  0,512,256);
	SetDefDrawEnv(&exc_db[0].draw,  0,256,512,256);
	SetDefDispEnv(&exc_db[1].disp,  0,256,512,256);
	SetDefDrawEnv(&exc_db[1].draw,  0,  0,512,256);

	exc_db[0].draw.isbg=exc_db[1].draw.isbg=1;
	exc_db[0].disp.screen.x=exc_db[1].disp.screen.x=4;
	exc_db[0].disp.screen.y=exc_db[1].disp.screen.y=24;
	setRGB0(&exc_db[0].draw,104,0,0);
	setRGB0(&exc_db[1].draw,104,0,0);

	if(dev_kit==1)
		mem_size=0x800000;
	else
		mem_size=0x200000;



/*---- cause ----*/
/*
	p_sr=str_reg;

	p_exc=(int *)reg_lst[OFS_EPC];

	p_sr+=sprintf(p_sr," %s",exc_txt[reg_lst[OFS_CA]>>2&0x1f]);

	if((reg_lst[OFS_CA]>>2&0x1f)==9) {
		i=(*p_exc>>16)-6;
		if(i==0||i==1)
			p_sr+=sprintf(p_sr," %s",break_txt[i]);
	}

	p_sr+=sprintf(p_sr,"\n at %08x",(int)p_exc);

	if((reg_lst[OFS_CA]&0x80000000)==0x80000000)
		p_sr+=sprintf(p_sr," in branch delay slot\n\n");
	else
		p_sr+=sprintf(p_sr,"\n\n");

*/
/*---- mem-dump ----*/
/*
#define NR_DUMP		9

	exc_id=1;

	p_sp=(int *)reg_lst[OFS_SP];
	cur_sp=p_sp+(NR_DUMP-1);
	cur_exc=p_exc+(-NR_DUMP/2);
	*/

	padWait=100;

	while(1) {

		while(DrawSync(1));
//		DrawSync(0);	
//		while(DrawSync(1));	
		VSync(0);
		exc_id=exc_id? 0: 1;
		exc_swap();
		cls();

/*
		if(padWait>0)
		{
			padWait--;
		}
		else
		{
			int pad=PadRead(1);
			if(pad&PADRup)currentPage=PAGE_REGS;
			if(pad&PADRdown)currentPage=PAGE_DUMP;
			if(pad&PADRleft)currentPage=PAGE_VALUES;
		}
*/

//		PadUpdate();
/*
		if(wait_pad==0) {
			pad=PadRead(1);

			if(pad&PADRdown&&++currentPage==NUM_PAGES)currentPage=0;
			
#define	STEP_MEM	256

			if(pad&PADL1)			cur_exc+=STEP_MEM;
			if(pad&PADL2)			cur_exc-=STEP_MEM;
			if(pad&PADLup)		cur_exc--;
			if(pad&PADLdown)	cur_exc++;
			if(pad&PADR1)			cur_sp-=STEP_MEM;
			if(pad&PADR2)			cur_sp+=STEP_MEM;
			if(pad&PADRup)		cur_sp++;
			if(pad&PADRdown)	cur_sp--;

			if(pad&PADstart) {
				cur_sp=p_sp+(NR_DUMP-1);
				cur_exc=p_exc+(-NR_DUMP/2);
			}
		}
		else
			wait_pad--;
*/
/*
		p_sd=str_dump;

		p_sd+=sprintf(p_sd,"\n        pc:              stack:\n\n");
		for(i=0;i<NR_DUMP;i++) {
// pc-dump 
			if(cur_exc+i==p_exc)
				p_sd+=sprintf(p_sd,">");
			else
				p_sd+=sprintf(p_sd," ");
			print_adr(cur_exc+i);
			p_sd+=sprintf(p_sd," ");
// stack-dump 
			print_adr(cur_sp-i);
			if(cur_sp-i==p_sp)
				p_sd+=sprintf(p_sd,"<\n");
			else
				p_sd+=sprintf(p_sd," \n");
		}
		*/

		p_sd+=sprintf(p_sd,"\n");


		displayTitle();
		displayCause();
		switch(currentPage)
		{
			case PAGE_REGS:
				displayRegs();
				break;
			case PAGE_DUMP:
				break;
			case PAGE_VALUES:
				break;
		}
		
//int len=0;
//len+=strlen(str_reg);
//len+=strlen(str_dump);
//		FntPrint(str_reg);
//		print(str_reg);
//		FntPrint(str_dump);
//		print(str_dump);
//		if(wait_pad==0)
//			FntPrint("     scroll through mem with pad");
//			print("     scroll through mem with pad");
//		FntFlush(-1);
		PrimDisplay();
	}

}

/* ------------------------------ print_adr ------------------------------- */
static void print_adr(int *adr) {

	int chk,dummy_mem,error;

	chk=(int)adr;

	dummy_mem=chk&0xf0000000;
	chk&=0x0ffffffc;
	error=1;
	if(chk<mem_size)
		error=0;
	if(chk>0x1f800000&&chk<0x1f800400)
		error=0;
	adr=(int *)(chk|dummy_mem);

	if(error==0)
		p_sd+=sprintf(p_sd,"%08x=%08x",(int)(adr),*adr);
	else
		p_sd+=sprintf(p_sd,"%08x=XXXXXXXX",(int)(adr));
}

/* ------------------------------- exc_swap ------------------------------ */
static void exc_swap(void) {

	PutDrawEnv(&exc_db[exc_id].draw);
	PutDispEnv(&exc_db[exc_id].disp);
}






static int fontx;
static int fonty;
static const int LEFTMARGIN=5;
static const int TOPMARGIN=5;
static const int CHARWIDTH=11;
static const int CHARHEIGHT=10;
static void	cls()
{
	fontx=LEFTMARGIN;
	fonty=TOPMARGIN;
}
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










static void displayTitle()
{
	print("[");
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
	print("]\n");
}
static void displayCause()
{
	char	textBuf[100];
	int		*exc;

	exc=(int*)reg_lst[OFS_EPC];
	sprintf(textBuf,"%s",exc_txt[reg_lst[OFS_CA]>>2&0x1f]);
	print(textBuf);

	if((reg_lst[OFS_CA]>>2&0x1f)==9)
	{
		int i=(*exc>>16)-6;
		if(i==0||i==1)
			sprintf(textBuf," %s",break_txt[i]);
			print(textBuf);
	}
	
	sprintf(textBuf,"\nPC=%08x",(int)exc);
	print(textBuf);
	
	if((reg_lst[OFS_CA]&0x80000000)==0x80000000)
		print(" in branch delay slot");
	print("\n\n");

sprintf(textBuf,"(%d)\n",number++);
print(textBuf);
}


static void displayRegs()
{
	char	textBuf[100];
	/* ---- reg-dump ---- */
	/*
	zr	t0	s0	t8	hi
	at	t1	s1	t9	lo
	v0	t2	s2	k0
	v1	t3	s3	k1	SR
	a0	t4	s4	gp
	a1	t5	s5	sp	Ca
	a2	t6	s6	fp
	a3	t7	s7	ra
	
	  pc
	*/
	
	sprintf(textBuf,"zr=%08x t0=%08x s0=%08x t8=%08x hi=%08x\n",              0,reg_lst[OFS_T0],reg_lst[OFS_S0],reg_lst[OFS_T8],reg_lst[OFS_HI]);
	print(textBuf);
	sprintf(textBuf,"at=%08x t1=%08x s1=%08x t9=%08x lo=%08x\n",reg_lst[OFS_AT],reg_lst[OFS_T1],reg_lst[OFS_S1],reg_lst[OFS_T9],reg_lst[OFS_LO]);
	print(textBuf);
	sprintf(textBuf,"v0=%08x t2=%08x s2=%08x ko=%08x"          ,reg_lst[OFS_V0],reg_lst[OFS_T2],reg_lst[OFS_S2],              0);
	print(textBuf);
	sprintf(textBuf,"v1=%08x t3=%08x s3=%08x k1=%08x SR=%08x\n",reg_lst[OFS_V1],reg_lst[OFS_T3],reg_lst[OFS_S3],              0,reg_lst[OFS_SR]);
	print(textBuf);
	sprintf(textBuf,"a0=%08x t4=%08x s4=%08x gp=%08x"          ,reg_lst[OFS_A0],reg_lst[OFS_T4],reg_lst[OFS_S4],reg_lst[OFS_GP]);
	print(textBuf);
	sprintf(textBuf,"a1=%08x t5=%08x s5=%08x sp=%08x Ca=%08x\n",reg_lst[OFS_A1],reg_lst[OFS_T5],reg_lst[OFS_S5],reg_lst[OFS_SP],reg_lst[OFS_CA]);
	print(textBuf);
	sprintf(textBuf,"a2=%08x t6=%08x s6=%08x fp=%08x"          ,reg_lst[OFS_A2],reg_lst[OFS_T6],reg_lst[OFS_S6],reg_lst[OFS_FP]);
	print(textBuf);
	sprintf(textBuf,"a3=%08x t7=%08x s7=%08x ra=%08x"          ,reg_lst[OFS_A3],reg_lst[OFS_T7],reg_lst[OFS_S7],reg_lst[OFS_RA]);
	print(textBuf);
	
	/*
	p_sr+=sprintf(p_sr," at=%08x t4=%08x s7=%08x\n",
	reg_lst[OFS_AT],reg_lst[OFS_T4],reg_lst[OFS_S7]);
	p_sr+=sprintf(p_sr," v0=%08x t5=%08x t8=%08x\n",
	reg_lst[OFS_V0],reg_lst[OFS_T5],reg_lst[OFS_T8]);
	p_sr+=sprintf(p_sr," v1=%08x t6=%08x t9=%08x\n",
	reg_lst[OFS_V1],reg_lst[OFS_T6],reg_lst[OFS_T9]);
	p_sr+=sprintf(p_sr," a0=%08x t7=%08x gp=%08x\n",
	reg_lst[OFS_A0],reg_lst[OFS_T7],reg_lst[OFS_GP]);
	p_sr+=sprintf(p_sr," a1=%08x s0=%08x sp=%08x\n",
	reg_lst[OFS_A1],reg_lst[OFS_S0],reg_lst[OFS_SP]);
	p_sr+=sprintf(p_sr," a2=%08x s1=%08x fp=%08x\n",
	reg_lst[OFS_A2],reg_lst[OFS_S1],reg_lst[OFS_FP]);
	p_sr+=sprintf(p_sr," a3=%08x s2=%08x ra=%08x\n",
	reg_lst[OFS_A3],reg_lst[OFS_S2],reg_lst[OFS_RA]);
	p_sr+=sprintf(p_sr," t0=%08x s3=%08x hi=%08x\n",
	reg_lst[OFS_T0],reg_lst[OFS_S3],reg_lst[OFS_HI]);
	p_sr+=sprintf(p_sr," t1=%08x s4=%08x lo=%08x\n",
	reg_lst[OFS_T1],reg_lst[OFS_S4],reg_lst[OFS_LO]);
	p_sr+=sprintf(p_sr," t2=%08x s5=%08x sr=%08x\n",
	reg_lst[OFS_T2],reg_lst[OFS_S5],reg_lst[OFS_SR]);
	p_sr+=sprintf(p_sr," t3=%08x s6=%08x ca=%08x\n",
	reg_lst[OFS_T3],reg_lst[OFS_S6],reg_lst[OFS_CA]);
	*/
}
