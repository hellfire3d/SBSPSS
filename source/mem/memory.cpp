/**************************/
/*** Memory Alloc Stuff ***/
/**************************/

#include	"system\global.h"
#include	"mem\memory.h"
#include	"fileio/fileio.h"	// Poo, need this for Databank stuff

#if defined(__USER_daveo__)
#define	MemPrint
#endif

/*****************************************************************************/
void	MemAddNode(sLList *LList,u_long Len,char *Addr);
void	MemRemoveNode(sLList *LList,u_short Node);

/*****************************************************************************/
sLList		MainRam;
int			MemNodeCount=0;

#ifdef __VERSION_DEBUG__
#define	USE_MEM_GUARDS
#endif

#ifdef	USE_MEM_GUARDS
static const unsigned int	HEAD_GUARD_FILL_PATTERN	=0x3e3e3e3e;
static const unsigned int	MEM_FILL_PATTERN		=0x3d3d3d3d;
static const unsigned int	TAIL_GUARD_FILL_PATTERN	=0x3c3c3c3c;
static const unsigned int	NUM_MEM_GUARDS=2;
static const unsigned int	MEM_GUARD_SIZE=sizeof(int)*NUM_MEM_GUARDS;
#endif	/* USE_MEM_GUARDS */


/*****************************************************************************/

#ifdef __DEBUG_MEM__

#ifndef __PRIM_HEADER__
#include "gfx/prim.h"
#endif

#ifndef __PAD_PADS_H__
#include "pad/pads.h"
#endif

#ifndef __VID_HEADER_
#include "system/vid.h"
#endif

#ifndef __SYSTEM_GSTATE_H__
#include "system/gstate.h"
#endif

#ifndef __GFX_FONT_H__
#include "gfx/font.h"
#endif


#define	MAX_MEM_DUMP	1024

enum MEM_ID
{
	MEM_BACKEND,
	MEM_FRONTEND,
	MEM_GAME,
	MEM_GAMEOPTIONS,
	MEM_SYSTEM,
	MEM_FMA,
	MEM_MAP,
	MEM_SHOP,
	MEM_PARTY,

	MEM_UNKNOWN,

	MEM_ID_MAX
};


struct MEM_PART
{
	void *	addr;
	MEM_ID	id;
	const char *	name;
	const char *	file;
	int		line;
	u32		frameTime;
};


static MEM_PART	memDump[ MAX_MEM_DUMP ];

static bool	s_dumpMem = false;


static const int		s_shadeX = 20;
static const int		s_shadeY = 100;
static const int		s_shadeW = 572;
static const int		s_shadeH = 102;
static const CVECTOR	s_shadeCol = { 0, 0, 0 };

static const int		s_dumpTextX = s_shadeX+12;
static const int		s_dumpTextY = s_shadeY;

static const int		s_dumpX = s_shadeX;
static const int		s_dumpY = s_shadeY+s_shadeH+5;
static const int		s_dumpWidth = s_shadeW;
static const int		s_dumpHeight = 16;
static const int		s_dumpScale = ONE;
static const int		s_dumpShift = 20;



static int		s_currentMemPart = 0;

#ifdef __DEBUG_MEM__
static const CVECTOR	s_colors[ MEM_ID_MAX ] =
{
	{ 255,   0,   0 },	// MEM_BACKEND
	{   0, 255,   0 },	// MEM_FRONTEND
	{   0,   0, 255 },	// MEM_GAME
	{ 255,   0, 255 },	// MEM_GAMEOPTIONS
	{ 255, 255, 255 },	// MEM_SYSTEM
	{ 128, 128, 255 },	// MEM_FMA,
	{ 255, 128, 128 },	// MEM_MAP,
	{ 128, 255, 128 },	// MEM_SHOP,
	{ 128, 255, 255 },	// MEM_PARTY,

	{  64,  64,  64 },	// MEM_UNKNOWN
};

static const char *	s_sceneNames[] =
{
	"BackEnd",
	"FrontEnd",
	"Game",
	"GameOptions",
	"System",
	"FMA",
	"Map",
	"Shop",
	"Party",
	"UNKNOWN",
};

static const int s_nbSceneNames = sizeof(s_sceneNames) / sizeof(char *);

static FontBank s_debugFont;

void dumpDebugMem()
{
	if (s_dumpMem)
	{
		s32		x, y;
		u16		padh, padd;
		u32		freeMem;
		u32		memBase;
		u32		byteWidth;
		MEM_PART *	mem;
		CVECTOR	black = {0, 0, 0};
		int		dir=-1;

		padd = PadGetRepeat( 0 );
		padh = PadGetHeld( 0 );

		if (padh & PAD_CIRCLE)
		{
			if (padd & PAD_LEFT)
			{
				dir = -1;
				s_currentMemPart--;
			}
			if (padd & PAD_RIGHT)
			{
				dir = +1;
				s_currentMemPart++;
			}

			if (s_currentMemPart < 0)				s_currentMemPart = MAX_MEM_DUMP - 2;
			if (s_currentMemPart >= MAX_MEM_DUMP-1)	s_currentMemPart = 0;
		}
		while( !memDump[s_currentMemPart].addr )
		{
			s_currentMemPart += dir;
			if (s_currentMemPart < 0)				s_currentMemPart = MAX_MEM_DUMP - 2;
			if (s_currentMemPart >= MAX_MEM_DUMP-1)	s_currentMemPart = 0;
		}
		/*
		else if(padh&PAD_TRIANGLE)
		{
			if (padh & PAD_UP)		s_dumpScale += 64;
			if (padh & PAD_DOWN)	s_dumpScale -= 64;

			if (padh & PAD_LEFT)	s_dumpX -= (2 * s_dumpScale) >> 12;
			if (padh & PAD_RIGHT)	s_dumpX += (2 * s_dumpScale) >> 12;
		}
		*/

		memBase = (u32)OPT_LinkerOpts.FreeMemAddress;
		freeMem = OPT_LinkerOpts.FreeMemSize - 4;
		if (freeMem > 6*1024*1024)	freeMem -= 6*1024*1024;
		byteWidth = ((s_dumpWidth << s_dumpShift) / freeMem);

		x = s_dumpX;
		y = s_dumpY;

		mem = memDump;
		
		for (int i=0;i<MAX_MEM_DUMP;i++)
		{
			if (mem->addr)
			{
				u32			len;
				u32 *		addr;
				POLY_F4 *	F4;
				const CVECTOR *	col;

				addr = (u32 *)mem->addr;

				x = (((u32)addr) - ((u32)memBase));
				x *= s_dumpScale;
				x >>= 12;
				x *= byteWidth;
				x >>= s_dumpShift;
				x += s_dumpX;

#ifdef	USE_MEM_GUARDS
				len = *(addr - (NUM_MEM_GUARDS+1));
#else
				len = *(addr - 1);
#endif
				len = (((u32)addr) - ((u32)memBase))+len;
				len *= s_dumpScale;
				len >>= 12;
				len *= byteWidth;
				len >>= s_dumpShift;
				len += s_dumpX;
				len=len-x;
				if(len==0)len=1;

				col = &s_colors[ mem->id ];

				F4 = GetPrimF4();
				setPolyF4( F4 );
				setXYWH(F4,x,y,len,s_dumpHeight);
				setRGB0( F4, col->r, col->g, col->b );
				if(i!=s_currentMemPart)
				{
					AddPrimToList( F4, 1 );
				}
				else
				{
					AddPrimToList( F4, 0 );

					F4 = GetPrimF4();
					setPolyF4( F4 );
					setXYWH(F4,x-1,y-1,len+2,s_dumpHeight+2);
					setRGB0( F4, 0,0,0 );
					AddPrimToList( F4, 0 );
				}
			}
			mem++;
		}

		int			len;
		char		Text[ 2048 ];
		char *		name;
		char *		file;
		POLY_F4 *	F4;

		F4 = GetPrimF4();
		setPolyF4( F4 );
		setXYWH( F4, s_shadeX, s_shadeY, s_shadeW, s_shadeH );
		setSemiTrans( F4, 1 );
		setShadeTex( F4, 0 );
		setRGB0( F4, s_shadeCol.r, s_shadeCol.g, s_shadeCol.b );
		AddPrimToList( F4, 1 );

		mem = &memDump[ s_currentMemPart ];
		if (mem->addr)
#ifdef	USE_MEM_GUARDS
			len = *(((u32 *)mem->addr) - (NUM_MEM_GUARDS+1));
#else
			len = *(((u32 *)mem->addr) - 1);
#endif
		else
			len = 0;

		sprintf( Text, "%d\n", s_currentMemPart );
		if (mem->addr)
		{
			sprintf( Text, "%sAddr    - %X   Len - %d\n", Text, (int)mem->addr, len );
			sprintf( Text, "%sAddrEnd - %X\n", Text, (int)mem->addr+len );

			if (mem->name)	sprintf( Text, "%sName - %s\n", Text, mem->name );
			else			sprintf( Text, "%sName - Undefined", Text );

			sprintf( Text, "%sFile - %s, Line - %d\n", Text, mem->file, mem->line );
			sprintf( Text, "%sScene - %s\n", Text, s_sceneNames[ mem->id ] );
			sprintf( Text, "%sTime - %lu", Text, mem->frameTime );
		}
		else
		{
			sprintf( Text, "%sAddr - NULL\nLen - 0\nName - Undefined\nFile - Undefined, Line - 0", Text );
		}

		s_debugFont.print( s_dumpTextX, s_dumpTextY, Text );
	  }
}
#endif	/* __DEBUG_MEM__ */


void resetDebugMem()
{
	for (int i=0;i<MAX_MEM_DUMP;i++)
	{
		memDump[i].addr = NULL;
	}
}


void addDebugMem( void * addr, const char * name, const char * file, int line )
{
	int			id;
	char *		sname;
	CScene *	scene;

	scene = GameState::getCurrentScene();
	if (scene)
	{
		sname = scene->getSceneName();

		id = MEM_UNKNOWN;
		for (int i=0;i<s_nbSceneNames;i++)
		{
			if (!strcmp( sname, s_sceneNames[i] ))
			{
				id = i;
			}
		}
	}
	else
	{
		id = MEM_SYSTEM;
	}

	MEM_PART	*md=memDump;
	for (int i=0;i<MAX_MEM_DUMP;i++)
	{
		if (!md->addr)
		{
			md->addr = addr;
			md->id = MEM_ID( id );
			md->name = name;
			md->file = file;
			md->line = line;
			md->frameTime = VidGetFrameCount();
			return;
		}
		md++;
	}
	ASSERT( !"Out of debug mem slots" );
}


void freeDebugMem( void * addr )
{
	ASSERT( addr );

	for (int i=0;i<MAX_MEM_DUMP;i++)
	{
		if (memDump[i].addr == addr)
		{
			memDump[i].addr = NULL;
			return;
		}
	}
	ASSERT( !"Can't find debug mem node ( memory already freed? )" );
}

// hmm.. have to alloc this memory before the first scene, but have to wait until all the
// systems are active so can't do it in the standard MemInit() thing
void DebugMemFontInit()
{
	s_debugFont.initialise( &standardFont );
	s_debugFont.setOt( 0 );
}

#endif

/*****************************************************************************/
void MemInit()
{
	u16			stack = LListLen - 1;
	sLList *	mem = &MainRam;
	char *		addr = (char*)OPT_LinkerOpts.FreeMemAddress;
	u32			len = OPT_LinkerOpts.FreeMemSize - 4;

	mem->SP = stack;
	mem->Head = 0xffff;
	mem->Tail = 0xffff;

	for (int i=0;i<LListLen;i++)
		{
		mem->Stack[ i ] = stack--;
		}

	MemAddNode( mem, len, addr );

	mem->RamUsed = 0;
	mem->TotalRam = len;

#ifdef __DEBUG_MEM__
	resetDebugMem();
#endif
}


/*****************************************************************************/
// 030700	Dave - Implemented smart allocation
//			It now looks thru the whole node list, and takes from the smallest VALID node
//			I now understand how this memory stuff works, it aint all bad!
char * MemAllocate( u32 TLen, char const *Name, char const * File, int LineNumber )
{
sLList	*mem = &MainRam;
u16		Head = mem->Head;
char	*Addr = (char*)-1;
u32		Len = ((TLen + 3) & 0xfffffffc);
int		BestNode,FirstNode;

		Len += 4;			//add on 4 to store Addr !
#ifdef	USE_MEM_GUARDS
		Len+=(MEM_GUARD_SIZE*2);
#endif	/* USE_MEM_GUARDS */

// Find First (and possably only)
		while (Head != 0xffff && mem->Nodes[ Head ].Len<Len) Head = mem->Nodes[ Head ].Next;
		ASSERT(Head != 0xffff);

		BestNode=FirstNode=Head;

// Check rest of list
		while (Head != 0xffff)
			{
			if (mem->Nodes[Head].Len>=Len)
				{
				if (mem->Nodes[Head].Len<mem->Nodes[BestNode].Len) BestNode=Head;
				}
			Head = mem->Nodes[ Head ].Next;
			}
		
//--------------------
// Alloc it
	if (mem->Nodes[BestNode].Len >= Len)
		{
		Addr = mem->Nodes[BestNode].Addr;
		mem->Nodes[BestNode].Len -= Len;
	 	mem->Nodes[BestNode].Addr += Len;
	 	if (mem->Nodes[BestNode].Len == 0) MemRemoveNode( mem, BestNode);


		*(u32*)Addr = Len;
		Addr += 4;

#ifdef	USE_MEM_GUARDS
		unsigned int	i;
		for(i=0;i<MEM_GUARD_SIZE;i+=sizeof(int))
		{
			*(int*)(Addr+i)=HEAD_GUARD_FILL_PATTERN;
		}
		Addr+=MEM_GUARD_SIZE;

		for(i=0;i<((TLen+3)&0xfffffffc);i+=sizeof(int))
		{
			*(int*)(Addr+i)=MEM_FILL_PATTERN;
		}

		for(i=0;i<MEM_GUARD_SIZE;i+=sizeof(int))
		{
			*(int*)(Addr+((TLen+3)&0xfffffffc)+i)=TAIL_GUARD_FILL_PATTERN;
		}
#endif	/* USE_MEM_GUARDS */

		mem->RamUsed += Len;
		}

#ifdef __DEBUG_MEM__
		addDebugMem( Addr, Name, File, LineNumber );
#endif

	ASSERT( (Addr != (char*)-1) );
	return Addr;
}

/*****************************************************************************/
void  	MemFree( void * Address )
{
u32 	Len;
sLLNode *node;
sLList	*mem = &MainRam;
u16		Head = mem->Head;
char	*Addr = (char*)Address;

// If file from Databank, dont try and clear it (simple!!)
		if (CFileIO::IsFromDataBank(Address)) return;
#ifdef	USE_MEM_GUARDS
		Addr-=MEM_GUARD_SIZE;
#endif	/* USE_MEM_GUARDS */
		Addr -= 4;
		Len = *(u32*)Addr;
		
#ifdef	USE_MEM_GUARDS
		// Check that the guards are intact
		unsigned int	i;
		unsigned int	*guardAddr;
		guardAddr=(unsigned int*)(Addr+4);
		for(i=0;i<MEM_GUARD_SIZE;i+=sizeof(unsigned int),guardAddr++)
		{
			if(*guardAddr!=HEAD_GUARD_FILL_PATTERN)
			{
				ASSERT(!"Memory guard trashed (head)");
				break;
			}
		}
		guardAddr=(unsigned int*)(Addr+Len-MEM_GUARD_SIZE);
		for(i=0;i<MEM_GUARD_SIZE;i+=sizeof(unsigned int),guardAddr++)
		{
			if(*guardAddr!=TAIL_GUARD_FILL_PATTERN)
			{
				ASSERT(!"Memory guard trashed (tail)");
				break;
			}
		}
#endif	/* USE_MEM_GUARDS */

		mem->RamUsed -= Len;

		node = &mem->Nodes[ Head ];
		while (Head != 0xffff)
			{
			if (mem->Nodes[ Head ].Addr == Addr + Len)
				{
				Len += mem->Nodes[ Head ].Len;
				node = &mem->Nodes[ Head ];
				MemRemoveNode( mem, Head );
				Head = mem->Head;
				}
			else
				{
				if (mem->Nodes[ Head ].Addr + mem->Nodes[ Head ].Len == Addr)
					{
					node = &mem->Nodes[ Head ];
					Addr = mem->Nodes[ Head ].Addr;
					Len += mem->Nodes[ Head ].Len;
					MemRemoveNode( mem, Head );
					Head = mem->Head;
					}
				else
					{
					Head = mem->Nodes[ Head ].Next;
					}
				}
			}

		MemAddNode( mem, Len, Addr );

#ifdef __DEBUG_MEM__
		freeDebugMem( Address );
#endif
}
/*
void  	MemFree( void * Address )
{
	u32 		Len;
	sLLNode *	node;
	sLList *	mem = &MainRam;
	u16			Head = mem->Head;
	char *		Addr = (char*)Address;

	Addr -= 4;

	Len = *(u32*)Addr;

	mem->RamUsed -= Len;

	node = &mem->Nodes[ Head ];
	while (Head != 0xffff)
		{
		if (mem->Nodes[ Head ].Addr == Addr + Len)
			{
			Len += mem->Nodes[ Head ].Len;
			node = &mem->Nodes[ Head ];
			MemRemoveNode( mem, Head );
			Head = mem->Head;
			}
		else
			{
			if (mem->Nodes[ Head ].Addr + mem->Nodes[ Head ].Len == Addr)
				{
				node = &mem->Nodes[ Head ];
				Addr = mem->Nodes[ Head ].Addr;
				Len += mem->Nodes[ Head ].Len;
				MemRemoveNode( mem, Head );
				Head = mem->Head;
				}
			else
				{
				Head = mem->Nodes[ Head ].Next;
				}
			}
		}

	MemAddNode( mem, Len, Addr );

#ifdef __DEBUG_MEM__

	freeDebugMem( Address );

#endif
}

*/
/*****************************************************************************/
void	MemAddNode( sLList * LList, u32 Len, char * Addr )
{
u16		Head = LList->Head;
u16		Tail = LList->Tail;
u16		Node = LList->Stack[ LList->SP-- ];

		MemNodeCount++;
		ASSERT(MemNodeCount<LListLen);

		if (Head == 0xffff && Tail == 0xffff)
			{
			LList->Head = Node;
			LList->Tail = Node;
			LList->Nodes[ Node ].Len = Len;
			LList->Nodes[ Node ].Addr = Addr;
			LList->Nodes[ Node ].Prev = 0xffff;
			LList->Nodes[ Node ].Next = 0xffff;
			}
		else
			{
			LList->Head = Node;
			LList->Nodes[ Node ].Len = Len;
			LList->Nodes[ Node ].Addr = Addr;
			LList->Nodes[ Head ].Prev = Node;
			LList->Nodes[ Node ].Prev = 0xffff;
			LList->Nodes[ Node ].Next = Head;
			}
}


/*****************************************************************************/
void	MemRemoveNode ( sLList * LList, u16 Node )
{
u16		Head = LList->Head;
u16		Tail = LList->Tail;
u16		Prev = LList->Nodes[ Node ].Prev;
u16		Next = LList->Nodes[ Node ].Next;

	MemNodeCount--;

	if (Head == Tail)
		{
		LList->Head = 0xffff;
		LList->Tail = 0xffff;
		}
	else
		{
		if (Node == Head)
			{
			LList->Head = Next;
			LList->Nodes[ Next ].Prev = 0xffff;
			}
		else if (Node == Tail)
			{
			LList->Tail = Prev;
			LList->Nodes[ Prev ].Next = 0xffff;
			}
		else
			{
			LList->Nodes[ Next ].Prev = Prev;
			LList->Nodes[ Prev ].Next = Next;
			}
		}
	LList->Nodes[Node].Len=0;
	LList->SP++;
	LList->Stack[ LList->SP ] = Node;
}


/*****************************************************************************/
void *operator new(size_t Size, const char * name )
{
		return MemAlloc( Size, name );
}

/*****************************************************************************/
void *operator new[](size_t Size, const char * name)
{
		return MemAlloc( Size, name );
}

/*****************************************************************************/
void operator delete(void *Ptr)
{
		MemFree((char*)Ptr);
}
/*****************************************************************************/
void operator delete[](void *Ptr)
{
		MemFree((char*)Ptr);
}

/*****************************************************************************/

