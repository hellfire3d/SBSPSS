/* ===========================================================================
	File:		GAL.C

	Notes:		Memory allocation \ deallocation Module

	Author:		G Robert Liddon @ 73b

	Version:	1.1

	Copyright (C) 1996 DCI Ltd All rights reserved. 
  ============================================================================ */

/* ---------------------------------------------------------------------------
	Includes
	-------- */

/*	Standard Library
	---------------- */
#include "stdio.h"

/*	Glib
	---- */
#include "gal.h"
#include "gdebug.h"
#include "tick.h"

/* ---------------------------------------------------------------------------
	Defines
	------- */
#define MAX_MEM_BLOCKS 		400			/* Maximum amount of memory blocks	*/
#define VER_STR 			"1.4"

#define MAX_NAME_SIZE		8

/* ---------------------------------------------------------------------------
	Structure Declarations
	---------------------- */

/*	Description of an area of memory, each free or alloced block has one
	-------------------------------------------------------------------- */
typedef struct MEM_HDR
{
	struct MEM_HDR *	Prev;		/* Previous block in list			*/
	struct MEM_HDR *	Next;		/* Next block in this list			*/

	void *	Mem;					/* Mem owned by this block			*/
	u32		Size;					/* Size of it 						*/

	u16			TimeStamp;			/* A time stamp						*/
	u16			Type;				/* Unique mem type of this block 	*/
	u16			Owners;				/* How many owners this block has	*/

	U16			Handle;				/* Handle for this block			*/

#ifdef __GL_DEBUG__
	u8			Name[MAX_NAME_SIZE];
#endif

} MEM_HDR;


/*	Description of a region of mem
	------------------------------ */
typedef struct MEM_REG
{
	void *	Mem;
	int		Size;
	
} MEM_REG;

/* ---------------------------------------------------------------------------
	Typedefs
	-------- */
typedef MEM_HDR * (*FIND_ROUTINE)(MEM_HDR *Head, U32 Size);

/* ---------------------------------------------------------------------------
	Module Variables
	---------------- */
static MEM_INIT_INFO *	MemInitBlocks;					/* Head of list of descriptions of different types of mem					*/
static MEM_HDR			MemHdrBlocks[MAX_MEM_BLOCKS];	/* Memory Header Blocks 													*/
static MEM_HDR *		FreeBlocks;						/* List of unused header blocks 											*/
static GAL_ERROR_CODE	LastError;						/* Last error that happened 												*/
static int				TimeStamp;						/* Blocks alloced are stamped with this time 								*/
static BOOL				FullErrorChecking;				/* Perform full error and consistancy cheaking on mem areas after defrags	*/
static U32				LastAttemptedAlloc;				/* Last alloc we tried														*/
static U32				LastDeallocedBlock;				/* Last alloc we tried														*/
static GAL_VERB_LEV		VerbLev;						/* How noisy gal is 														*/
static int				NumOfFreeHdrs;					/* How many headers are free 												*/
static u32				LastTypeAlloced;
static GAL_FILTER		AllocFilter;
static BOOL				HaltOnError;					/* Set to true if we want GAL to halt if there is an error */


/* ---------------------------------------------------------------------------
	Function Prototypes for internal functions
	------------------------------------------ */
static void				GraftMemHdrList(MEM_HDR ** ToList,MEM_HDR ** FromList);
static MEM_HDR *		FindNextBlock(void *Addr,MEM_HDR * Blocks);
static BOOL				CollideRegions(MEM_REG *Reg1,MEM_REG *Reg2);
static U32				ShuffleBlocks(MEM_HDR *Blocks,MEM_REG *Reg,MEM_INIT_INFO *M);
static void				PutBlocksInRegionIntoList(MEM_REG *Reg,MEM_HDR **ToList,MEM_HDR **FromList);
static BOOL				GazDefragMem(U32 MemType);
static void				DeleteEmptyBlocks(MEM_INIT_INFO *M);
static BOOL				GetRegion(MEM_REG *Reg,MEM_HDR * LockedBlocks,MEM_INIT_INFO *M);
static void				SortMemHdrListByAddr(MEM_HDR **Head);
static void				PutAllLockedBlocksOntoList(MEM_HDR **ToHead,MEM_HDR **FromHead);

static void 			AttachHdrToList(MEM_HDR **Head,MEM_HDR *Block);
static void 			DetachHdrFromList(MEM_HDR **Head,MEM_HDR *Block);
static BOOL 			IsActiveValidHandle(MHANDLE Handle);
static void * 			AlignPtr(void *P,U32 Align);
static U32				AlignSize(U32 Size,U32 Align);
static void 			MergeToEmptyList(MEM_INIT_INFO *MI,MEM_HDR *M);
static MHANDLE 			LoAlloc(MEM_INIT_INFO *M,MEM_HDR *Block,void *Addr,U32 Size,const char *Name);
static MEM_HDR *		GetFreeMemHdrBlock(void);
static void				ReleaseMemHdrBlock(MEM_HDR * Hdr);
static MEM_HDR *		FindBlockInTheseBounds(MEM_HDR *Head,void *Addr,U32 Size);

static BOOL				CheckCollisions(MEM_INIT_INFO * M,MEM_HDR *MemHdr);
static BOOL				AreBlocksColliding(MEM_HDR *Hdr1,MEM_HDR *Hdr2);

static BOOL 			GSetError(GAL_ERROR_CODE Err);

static MEM_INIT_INFO *	GetMemInitInfoBlockFromType(U32 Type);

static	MEM_HDR *		FindHighestMemBlock(MEM_HDR *Head, U32 Size);
		MEM_HDR *		FindClosestSizedBlock(MEM_HDR *Head, U32 Size);
static	MEM_HDR *		FindLowestMemBlock(MEM_HDR *Head, U32 Size);

static	int				CountFreeBlocks(void);
static	void			SetBlockName(MEM_HDR * MemHdr,const char * NewName);
static	char const *	GetBlockName(MEM_HDR * MemHdr);

static BOOL				SortAddr(MEM_HDR *B1,MEM_HDR * B2);
static BOOL				SortSize(MEM_HDR *B1,MEM_HDR * B2);
static void				SortMemHdrList(MEM_HDR **Head,BOOL (*CompFunc)(MEM_HDR *B1,MEM_HDR * B2));


/* ---------------------------------------------------------------------------
	Tables
	------ */
char *GalErrors[]=
{
	NULL,
	"Run out of mem handles",
	"Mem type already existed",
	"Mem type added overlaps previously defined one",
	"Invalid mem type",
	"Invalid handle",
	"Handle already unlocked",
	"Memory blocks in type overlap",
	"Not all memory of type covered",
	"Defrag attempted but no mem functions available",
	"Not enough memory to allocate block",
};


/*	Phantom mem you want a MHANDLE but is actually static (Not in yet)
	------------------------------------------------------------------ */
MEM_INIT_INFO PhantomMem=
{
	NULL,
	0,
	GAL_PHANTOM_MEM,
};


/*	---------------------------------------------------------------------------
	Function:	void GAL_SetErrorChecking(BOOL OnOff)

	Purpose:	Set error checking of mem regions after defrag

	Params:		OnOff = what to do

	--------------------------------------------------------------------------- */
void GAL_SetErrorChecking(BOOL OnOff)
{
	FullErrorChecking=OnOff;
}	

/*	---------------------------------------------------------------------------
	Function:	MHANDLE GAL_SplitBlock(MHANDLE CurBlock,U32 Split)

	Purpose:	Split a Block into two. If Split point outside
				block to be split then this will return an error.
				Split is rounded rounded up to be on mem type alignment
				boundary

	Params:		CurBlock = block to split
				Size = New Size

	Returns:	Handle of second block else
				NULL_HANDLE if unsucessful

	--------------------------------------------------------------------------- */
MHANDLE GAL_SplitBlock(MHANDLE CurBlock,U32 Size)
{
	MEM_INIT_INFO *		M;
	MEM_HDR *			MemHdr;
	MEM_HDR *			SplitBlock;

	if (!IsActiveValidHandle(CurBlock))
		{
		GSetError(ERR_GAL_INVALID_MEM_HANDLE);
		return NULL_HANDLE;
		}

	MemHdr=&MemHdrBlocks[CurBlock];

	if (!(M=GetMemInitInfoBlockFromType(MemHdr->Type)))
		{
		GSetError(ERR_GAL_INVALID_MEM_TYPE);
		return NULL_HANDLE;
		}


	/* Round up split to size lie on mem types alignment & Check to see if it's not too big */
	Size=AlignSize(Size,M->Alignment);

	if (Size >= MemHdr->Size)
		return NULL_HANDLE;

	/* Get a free block to put split into */

	if (!(SplitBlock=GetFreeMemHdrBlock()))
		{
		GSetError(ERR_RUN_OUT_OF_MEM_HDRS);
		return NULL_HANDLE;
		}

	/* Fill it up with the correct details */
	SplitBlock->Type=MemHdr->Type;
	SplitBlock->Owners=1;
	SplitBlock->Mem=(void *)((U32)(MemHdr->Mem)+Size);
	SplitBlock->Size=MemHdr->Size-Size;
	SplitBlock->TimeStamp=MemHdr->TimeStamp;
	SetBlockName(SplitBlock,GetBlockName(MemHdr));

	/* And add to the list of used blocks */

	AttachHdrToList(&M->Used,SplitBlock);

	/* Now Adjust Block that's been split */
	MemHdr->Size=Size;

	/* Return handle of the split block */
	return SplitBlock->Handle;

}	

/*	---------------------------------------------------------------------------
	Function:		void GAL_InitModule(void);

	Purpose:		Initialise memory manager
	--------------------------------------------------------------------------- */
void GAL_InitModule(void)
{
	int f;
	MemInitBlocks=NULL;
	FreeBlocks=NULL;
	NumOfFreeHdrs=0;

	GAL_SetVerbosity(GAL_SILENT);

	GAL_SetTimeStamp(0);

	LastError=ERR_GAL_NO_ERROR;

	GAL_SetErrorChecking(FALSE);
	GAL_ReturnOnError();

	for (f=0;f<MAX_MEM_BLOCKS;f++)
		{
		MemHdrBlocks[f].Prev=NULL;
		MemHdrBlocks[f].Next=NULL;
		MemHdrBlocks[f].Handle=f;
		ReleaseMemHdrBlock(&MemHdrBlocks[f]);
		}
}

/*	---------------------------------------------------------------------------
	Function:	int GAL_AddMemType(MEM_INIT_INFO *M)

	Purpose:	Add a type of memory to the list of mem types

	Params:		M -> to structure describing new mem type

	Returns:	FALSE if there was an error

	--------------------------------------------------------------------------- */
BOOL GAL_AddMemType(MEM_INIT_INFO *M)
{
	MEM_INIT_INFO *	P;
	MEM_HDR *		FreeMemHdr;
	U32				Addr1;
	U32				Addr2;

	/* Scan through any memory regions GAL has in it's charge to
		see if there's any conflict with the block we want to add */

	P=MemInitBlocks;

	while (P)
		{
		/* See if this block overlaps any others */

		Addr1=(U32) M->Mem;
		Addr2=(U32) P->Mem;

		if (Addr1 < (Addr2+P->Size))
			{
			if (Addr1+M->Size >= Addr2)
				{
				GSetError(ERR_GAL_MEM_TYPE_OVERLAP);
				return FALSE;
				}
			}

		/* Check to see if there's a memory region already designated of
			this type */
		if (P->Type == M->Type)
			return ERR_GAL_MEM_TYPE_EXISTS;

		/*	Next Block	*/
		P=P->NextInitBlock;
		}

	/* Add to the list of memory regions */

	M->NextInitBlock=MemInitBlocks;
	MemInitBlocks=M;

	/* Initialise Info Block */

	M->Flags=0;
	M->Used=NULL;
	M->Empty=NULL;

	/* Get A Mem HDR for the free region if we can */

	FreeMemHdr=GetFreeMemHdrBlock();

	if (!FreeMemHdr)
		{
		GSetError(ERR_RUN_OUT_OF_MEM_HDRS);
		return FALSE;
		}

	/* Set empty region to be this mem header */

	AttachHdrToList(&M->Empty,FreeMemHdr);

	/* Init this mem hdr as the empty region */

	FreeMemHdr->Mem=AlignPtr(M->Mem,M->Alignment);
	Addr1=(U32)FreeMemHdr->Mem-(U32)M->Mem;
	FreeMemHdr->Size=M->Size-Addr1;
	
	/* Return without an error */

	return TRUE;
}

/*	---------------------------------------------------------------------------
	Function:	MHANDLE GAL_Alloc(U32 MemNeeded,U32 Type,const char *Name)

	Purpose:	Alloc some memory

	Parms:		MemNeeded	= Amount Wanted
				Type		= Type of mem wanted
				Name		= Name of mem block (NULL if no name wanted)

	Returns:	A handle to memory if succesful
				NULL_HANDLE if failed
	--------------------------------------------------------------------------- */
MHANDLE GAL_Alloc(U32 Size,U32 Type,const char *Name)
{
	MEM_HDR *			Block;
	MEM_INIT_INFO *		M;
	U32					FullSize;
	FIND_ROUTINE		FRoute;
	BOOL				High;


	High = (Type&GAL_HIGH) == GAL_HIGH;

	Type&=~GAL_FLAGS;

	if (High)
		FRoute=FindHighestMemBlock;
	else
		FRoute=FindLowestMemBlock;

	LastAttemptedAlloc=Size;
	LastTypeAlloced=Type;

	/* Find out the full mem aligned size of this chunk */

	/* If this is a valid memory type get info about this mem type */

	if (!(M=GetMemInitInfoBlockFromType(Type)))
		{
		GSetError(ERR_GAL_INVALID_MEM_TYPE);
		return NULL_HANDLE;
		}


	FullSize=AlignSize(Size,M->Alignment);

	if (AllocFilter)
		AllocFilter(Type,FullSize,Name);

	/* Find a smallest block in memory pool that is big enough in size  and error if not*/

	if (!(Block=FRoute(M->Empty,FullSize)))
		{
		/* Oh dear couldn't alloc, is it worth trying a defrag? */

		if (FullSize<=GAL_GetFreeMem(Type))
			{
			/* Yes, it could be worth it so lets try */

			if (!GAL_DefragMem(Type))
				return NULL_HANDLE;
			else
				{
				/* Sucessfully defraged, now try again */

				if (!(Block=FRoute(M->Empty,FullSize)))
					{
					GSetError(ERR_GAL_NOT_ENOUGH_MEM);
					return NULL_HANDLE;
					}
				}
			}
		else
			{
			GSetError(ERR_GAL_NOT_ENOUGH_MEM);
			return NULL_HANDLE;
			}
		}

	/* Take it out of the empty list */
	DetachHdrFromList(&M->Empty,Block);

	/* No Do the grunt work */

	if (High)
		{
		u8 * BaseAddr;

		BaseAddr=(void*)((u32) Block->Mem + Block->Size-FullSize);

		return LoAlloc(M,Block,BaseAddr,Size,Name);
		}
	else	
		return LoAlloc(M,Block,Block->Mem,Size,Name);
}


/*	---------------------------------------------------------------------------
	Function:	UINT GAL_GetMemSize(MHANDLE Handle)

	Purpose:	Find out how much memory is attached to this handle

	Parms:		MHANDLE Handle = handle of allocated memory

	Returns:	Size of block (not neccesarily the allocated size, but the rounded
				up according to the memory region it was allocated in size )
	--------------------------------------------------------------------------- */
UINT GAL_GetMemSize(MHANDLE Handle)
{
	MEM_HDR	*MemHdr;

	if (!IsActiveValidHandle(Handle))
		{
		GSetError(ERR_GAL_INVALID_MEM_HANDLE);
		return 0;
		}

	MemHdr=&MemHdrBlocks[Handle];

	return(MemHdr->Size);
}

/*	---------------------------------------------------------------------------
	Function:	void *GAL_Lock(MHANDLE Handle)

	Purpose:	Lock some alloced memory

	Parms:		MHANDLE Handle = handle to alloced memory

	Returns:	A handle to memory if succesful
				NULL_HANDLE if failed
	--------------------------------------------------------------------------- */
void *GAL_Lock(MHANDLE Handle)
{
	MEM_HDR	*MemHdr;

	/* check to see if this is a valid handle, error if not */

	if (!IsActiveValidHandle(Handle))
		{
		GSetError(ERR_GAL_INVALID_MEM_HANDLE);
		return NULL;
		}

	/* get ptr to memory block */

	MemHdr=&MemHdrBlocks[Handle];

	/* Everything ok so bump up the amount of owners */

	MemHdr->Owners++;

	/* return pointer to blocks memory */

	return MemHdr->Mem;
}

/*	---------------------------------------------------------------------------
	Function:	void *GAL_Unlock(MHANDLE Handle)

	Purpose:	Unlock previously locked memory

	Parms:		MHANDLE Handle to mem block

	Returns:	TRUE 	if succesfully unlocked
				FALSE if not
	--------------------------------------------------------------------------- */
BOOL GAL_Unlock(MHANDLE Handle)
{
	MEM_HDR	*MemHdr;

	/* check to see if this is a valid handle, error if not */

	if (!IsActiveValidHandle(Handle))
		{
		GSetError(ERR_GAL_INVALID_MEM_HANDLE);
		return FALSE;
		}

	/* get ptr to memory block */
	MemHdr=&MemHdrBlocks[Handle];

	/* If this mem block has no owners error */

	if (!MemHdr->Owners)
		{
		GSetError(ERR_GAL_MEM_ALREADY_UNLOCKED);
		return FALSE;
		}


	/* Decrease amount of owners */

	MemHdr->Owners--;

	/* Complete Succesfully */

	return TRUE;

}

/*	---------------------------------------------------------------------------
	Function:	BOOL GAL_Free(MHANDLE Handle);

	Purpose:	Free up previously allocated memory

 	Parms:		MEMINFO *Mem -> Linked list of memory information blocks

	Returns:	How succesful it all was
	--------------------------------------------------------------------------- */
BOOL GAL_Free(MHANDLE Handle)
{
	MEM_INIT_INFO *	M;
	MEM_HDR *			MemHdr;

	/* check to see if this is a valid handle, error if not */

	if (!IsActiveValidHandle(Handle))
		{
		GSetError(ERR_GAL_INVALID_MEM_HANDLE);
		return FALSE;
		}


	/* get ptr to memory block */

	MemHdr=&MemHdrBlocks[Handle];

	/* Get -> to memory region structure this block belongs to and error if there wasn't one */

	if (!(M=GetMemInitInfoBlockFromType(MemHdr->Type)))
		{
		GSetError(ERR_GAL_INVALID_MEM_TYPE);
		return FALSE;
		}


	/* Report what amount of mem this was being freed */

	LastDeallocedBlock=MemHdr->Size;

	/* Take mem block from used list */

	DetachHdrFromList(&M->Used,MemHdr);

	/* Merge into the empty blocks list */

	MergeToEmptyList(M,MemHdr);

	return TRUE;
}

/*	---------------------------------------------------------------------------
	Function:	U32 GAL_GetFreeMem(U32 Type)

	Purpose:	Return how much free mem this mem type has

	Parms:		U32 Type = memory type to do

	Returns:	Total size of free mem
				
	--------------------------------------------------------------------------- */
U32 GAL_GetFreeMem(U32 Type)
{
	U32					FreeMem;
	MEM_INIT_INFO *	M;

	Type&=~GAL_FLAGS;

	FreeMem=0;

	if ((M=GetMemInitInfoBlockFromType(Type)))
		{
		MEM_HDR *			Block;

		Block=M->Empty;

		while (Block)
			{
			FreeMem+=Block->Size;
			Block=Block->Next;
			}
		}
	else
		GSetError(ERR_GAL_INVALID_MEM_TYPE);

	return(FreeMem);
}	


/*	---------------------------------------------------------------------------
	Function:	U32 GAL_GetFreeMem(U32 Type)

	Purpose:	Return how much free mem this mem type has

	Parms:		U32 Type = memory type to do

	Returns:	Total size of free mem
				
	--------------------------------------------------------------------------- */
U32 GAL_GetUsedMem(U32 Type)
{
	U32					FreeMem;
	MEM_INIT_INFO *	M;

	Type&=~GAL_FLAGS;

	FreeMem=0;

	if ((M=GetMemInitInfoBlockFromType(Type)))
		{
		MEM_HDR *			Block;

		Block=M->Used;

		while (Block)
			{
			FreeMem+=Block->Size;
			Block=Block->Next;
			}
		}
	else
		GSetError(ERR_GAL_INVALID_MEM_TYPE);

	return(FreeMem);
}	



/*	---------------------------------------------------------------------------
	Function:	U32 GAL_LargestFreeBlock(U32 Type)

	Purpose:	Return the largest block of mem available

	Parms:		U32 Type = memory type to do

	Returns:	Size of largest mem
				
	--------------------------------------------------------------------------- */
U32 GAL_LargestFreeBlock(U32 Type)
{
	U32					Largest;
	MEM_HDR *			Index;
	MEM_INIT_INFO *	MI;

	Type&=~GAL_FLAGS;

	if (!(MI=GetMemInitInfoBlockFromType(Type)))
		{
		GSetError(ERR_GAL_INVALID_MEM_TYPE);
		return 0;
		}

	Largest = 0;

	Index=MI->Empty;

	while (Index)
		{
		if (Index->Size > Largest)
			Largest=Index->Size;

		Index=Index->Next;
		}

	return Largest;
}

/*	---------------------------------------------------------------------------
	Function:	static void AttachHdrToList(MEM_HDR **Head,MEM_HDR *Block)

	Purpose:	Attach this block to a linked list

	Parms:		MEM_HDR **Head -> Head of list holder
				MEM_HDR *Block -> Block to add
				
	--------------------------------------------------------------------------- */
static void AttachHdrToList(MEM_HDR **Head,MEM_HDR *Block)
{
	Block->Prev=NULL;

	if ((Block->Next=*Head))
		Block->Next->Prev=Block;

	*Head=Block;
}


/*	---------------------------------------------------------------------------
	Function:	static void DetachHdrFromList(MEM_HDR **Head,MEM_HDR *Block)

	Purpose:	Detach this block from a linked list

	Parms:		MEM_HDR **Head -> Head of list holder
				MEM_HDR *Block -> Block to detach
				
	--------------------------------------------------------------------------- */
static void DetachHdrFromList(MEM_HDR **Head,MEM_HDR *Block)
{
	if (Block->Prev)
		Block->Prev->Next=Block->Next;
	else
		*Head=Block->Next;

	if (Block->Next)
		Block->Next->Prev=Block->Prev;
}


/*	---------------------------------------------------------------------------
	Function:	BOOL IsActiveValidHandle(MHANDLE Handle)

	Purpose:	Is this an active valid handle

	Parms:		MHANDLE Handle to mem block

	Returns:	TRUE 	if is
				FALSE if not
	--------------------------------------------------------------------------- */
static BOOL IsActiveValidHandle(MHANDLE Handle)
{
	MEM_HDR	*MemHdr;

	if (Handle >= MAX_MEM_BLOCKS || Handle < 0)
		return FALSE;

	MemHdr=&MemHdrBlocks[Handle];

	if (!(MemHdr->Mem))
		return FALSE;


	return TRUE;
}


/*	---------------------------------------------------------------------------
	Function:	static void *AlignPtr(void *P,U32 Align)

	Purpose:	Align this PTR to next aligned memory

	Parms:		P		= Ptr to align
				Align	= aligment

	Returns:	Aligned ptr
	--------------------------------------------------------------------------- */
static void *AlignPtr(void *P,U32 Align)
{
	U32 Addr,Temp;

	Addr= (U32) P;

	Temp=Align-(Addr%Align);

	Addr+=(Temp == Align ? 0 : Temp);

	return (void *) Addr;
}

/*	---------------------------------------------------------------------------
	Function:	static U32 AlignSize(U32 Size,U32 Align)

	Purpose:	Pad out a size to this aligment

	Parms:		Size = Size to pad out
				Align = aligmnet to pad if out to

	Returns:	Aligned Size
	--------------------------------------------------------------------------- */
static U32 AlignSize(U32 Size,U32 Align)
{
	Size+= Size%Align ? (Align-(Size%Align)) : 0;
	return Size;
}

/*	---------------------------------------------------------------------------
	Function:	MEM_HDR *FindClosestSizedBlock(MEM_HDR *Head, U32 Size)

	Purpose:	Get the mem info structure for this type of memory

	Parms:		U32 Type = Type of mem MEM_INIT_INFO needed for

	Returns:	-> MEM_INIT_INFO if succesful else NULL
	--------------------------------------------------------------------------- */
MEM_HDR *FindClosestSizedBlock(MEM_HDR *Head, U32 Size)
{
	MEM_HDR *	Closest;
	MEM_HDR *	Index;

	Closest=NULL;

	Index=Head;

	while (Index)
		{
		if (Index->Size >= Size)
			{
			if (Closest)
				{
				if ((Index->Size - Size) < (Closest->Size - Size))
					Closest=Index;
				}
			else
				Closest=Index;
			}

		Index=Index->Next;
		}

	return Closest;
}

/*	---------------------------------------------------------------------------
	Function:	MEM_HDR *FindClosestSizedBlock(MEM_HDR *Head, U32 Size)

	Purpose:	Get the mem info structure for this type of memory

	Parms:		U32 Type = Type of mem MEM_INIT_INFO needed for

	Returns:	-> MEM_INIT_INFO if succesful else NULL
	--------------------------------------------------------------------------- */
MEM_HDR *FindHighestMemBlock(MEM_HDR *Head, U32 Size)
{
	MEM_HDR *	Closest;
	MEM_HDR *	Index;
	void *		Highest;

	Closest=NULL;
	Highest=NULL;

	Index=Head;

	while (Index)
		{
		if (Index->Size >= Size)
			{
			if (Closest)
				{
				if ((u32) Index->Mem > (u32) Highest)
					{
					Closest=Index;
					Highest=Index->Mem;
					}
				}
			else
				{
				Closest=Index;
				Highest=Index->Mem;
				}
			}

		Index=Index->Next;
		}

	return Closest;
}
/*	---------------------------------------------------------------------------
	Function:	static MEM_HDR *FindLowestMemBlock(MEM_HDR *Head, U32 Size)

	Purpose:	Get the mem info structure for this type of memory

	Parms:		U32 Type = Type of mem MEM_INIT_INFO needed for

	Returns:	-> MEM_INIT_INFO if succesful else NULL
	--------------------------------------------------------------------------- */
MEM_HDR *FindLowestMemBlock(MEM_HDR *Head, U32 Size)
{
	MEM_HDR *	Closest;
	MEM_HDR *	Index;
	u32			Lowest;

	Closest=NULL;

	Index=Head;

	Lowest=0xffffffff;
	Closest=NULL;

	while (Index)
		{
		if (Index->Size >= Size)
			{
			if (Closest)
				{
				if ((u32) Index->Mem < Lowest)
					{
					Closest=Index;
					Lowest=(u32)Index->Mem;
					}
				}
			else
				{
				Closest=Index;
				Lowest=(u32)Index->Mem;
				}
			}

		Index=Index->Next;
		}

	return Closest;
}

/*	---------------------------------------------------------------------------
	Function:	static MEM_INIT_INFO *GetMemInitInfoBlockFromType(U32 Type)

	Purpose:	Get the mem info structure for this type of memory

	Parms:		U32 Type = Type of mem MEM_INIT_INFO needed for

	Returns:	-> MEM_INIT_INFO if succesful else NULL
	--------------------------------------------------------------------------- */
static MEM_INIT_INFO *GetMemInitInfoBlockFromType(U32 Type)
{
	MEM_INIT_INFO *	P;
	MEM_INIT_INFO *	RetBlock;

	RetBlock=NULL;
	
	P=MemInitBlocks;

	while (P)
		{
		/* Is the type we are looking for? */
		if (P->Type==Type)
			return P;

		/* No, so go onto next block */

		P=P->NextInitBlock;

		}

	return RetBlock;
}

/*	---------------------------------------------------------------------------
	Function:	void MergeFromUsedtoFreeList(MEM_HDR *M,MEM_INIT_INFO *MI)

	Purpose:	Merge this block into this mem info's empty list. Merge
				it with any adjacent blocks

	Parms:		U32 Type = Type of mem MEM_INIT_INFO needed for

	Returns:	-> MEM_INIT_INFO if succesful else NULL
	--------------------------------------------------------------------------- */
static void MergeToEmptyList(MEM_INIT_INFO *MI,MEM_HDR *M)
{
	MEM_HDR *	Index;
	MEM_HDR *	NextIndex;

	void *		Start;
	void *		End;


	/* Work out end address of this block */

	Start=M->Mem;
	End=(void *)(((U32) M->Mem) + M->Size);

	/* Init Loop Vars */
	Index=MI->Empty;

	/* Tumble through empty list grabbing empty blocks adjacent to our block we're freeing */

	while (Index)
		{
		void *		ThisStart;
		void *		ThisEnd;

		/* Work out end of address of current header */
		ThisStart=Index->Mem;
		ThisEnd=(void *)(((U32) Index->Mem)+Index->Size);

		/* Work out next block in list */
		NextIndex=Index->Next;

		/* If we're adjacent then merge and remove from empty list */

		if (Start==ThisEnd  || End==ThisStart)
			{
			/* Merge found block to me */

			M->Size+=Index->Size;

			if (Start==ThisEnd)
				M->Mem=Index->Mem;

			/* Detach from empty and chuck into free */

			DetachHdrFromList(&MI->Empty,Index);

			ReleaseMemHdrBlock(Index);

			}

		Index=NextIndex;
		}

	AttachHdrToList(&MI->Empty,M);
}



/*	---------------------------------------------------------------------------
	Function:	MHANDLE GAL_AllocAt(U32 MemNeeded,void *Addr,U32 Type,const char *Name)

	Purpose:	Alloc some memory at a specefic address
				WARNING ***
				if Addr is not aligned to this memory types alignment then
				block allocated will be at next aligned address, size will
				still be correct.

	Parms:		MemNeeded	= Amount Wanted
				Addr		= Address needed at
				Type		= Type of mem wanted
				Name		= Name of mem block (NULL if no name wanted)

	Returns:	A handle to memory if succesful
				NULL_HANDLE if failed
	--------------------------------------------------------------------------- */
MHANDLE GAL_AllocAt(U32 Size,void *Addr,U32 Type,const char *Name)
{
	MEM_HDR *			Block;
	MEM_INIT_INFO *	M;
	U32					PhysSize;

	/* If this is a valid memory type get info about this mem type */

	Type&=~GAL_FLAGS;

	if (!(M=GetMemInitInfoBlockFromType(Type)))
		{
		GSetError(ERR_GAL_INVALID_MEM_TYPE);
		return NULL_HANDLE;
		}

	/* Align address asked for and align size */

	Addr=AlignPtr(Addr,M->Alignment);
	PhysSize=AlignSize(Size,M->Alignment);

	/* Find block we can fit into else error out if one not found */

	if (!(Block=FindBlockInTheseBounds(M->Empty,Addr,PhysSize)))
		return NULL_HANDLE;

	/* Take it out of the empty list */
	DetachHdrFromList(&M->Empty,Block);

	/* Now do the business */
	return LoAlloc(M,Block,Addr,Size,Name);
}


/*	---------------------------------------------------------------------------
	Function:	static MHANDLE LoAlloc(MEM_INIT_INFO *M,MEM_HDR *Block,void *Addr,U32 Size,const char *Name)

	Purpose:	Lo level alloc used by GAL_AllocAt && GAL_Alloc
		
				Alloc some memory at a specefic address
				WARNING ***
				if Addr is not aligned to this memory types alignment then
				block allocated will be at next aligned address, size will
				still be correct.

	Parms:		M				= Mem region to alloc from
				Block			= Free block in mem region to alloc into
				Size			= Amount Wanted
				Addr			= Address needed at
				Name			= Name of block

	Returns:	A handle to memory if succesful
				NULL_HANDLE if failed
	--------------------------------------------------------------------------- */
static MHANDLE LoAlloc(MEM_INIT_INFO *M,MEM_HDR *Block,void *Addr,U32 Size,const char *Name)
{
	MEM_HDR *			SplitBlock;
	U32					PhysSize;

	
	PhysSize=AlignSize(Size,M->Alignment);

	/* if block base isn't the as address we asked for ... */

	if (Block->Mem != Addr)
		{
		/* Pare of begining go block and chuck back onto free list*/

		if (!(SplitBlock=GetFreeMemHdrBlock()))
			{
			GSetError(ERR_RUN_OUT_OF_MEM_HDRS);
			return NULL_HANDLE;
			}

		SplitBlock->Mem=Block->Mem;
		SplitBlock->Size=(U32) Addr-(U32)(Block->Mem);
		SplitBlock->Type=(u16) M->Type;

		/* And put it into empty list in MEM_INIT_INFO */

		MergeToEmptyList(M,SplitBlock);

		/* And adjust this block */
		Block->Mem=Addr;
		Block->Size-=SplitBlock->Size;
		}


	/* if block found is bigger than memory wanted ... */
	if ((Block->Size > PhysSize))
		{
		/* if can't find an unsused MEM_HDR in global pool then error */

		if (!(SplitBlock=GetFreeMemHdrBlock()))
			{
			GSetError(ERR_RUN_OUT_OF_MEM_HDRS);
			return NULL_HANDLE;
			}

		/* Make new block point @ spare mem of alloced block */
		
		SplitBlock->Mem=(void *) ((U32)(Block->Mem)+PhysSize);
		SplitBlock->Size=Block->Size-PhysSize;
		SplitBlock->Type=(u16) M->Type;

		/* And put it into empty list in MEM_INIT_INFO */

		MergeToEmptyList(M,SplitBlock);

		/* Adjust alloced Block so that it is same size as memory wanted */

		Block->Size=PhysSize;
		}

	/* Detach block from memory pool from Empty list  & add to Used List*/

	AttachHdrToList(&M->Used,Block);
	
	/* Initialise the block */ 
	Block->Owners = 0;		/* Say no one owns this block */


	Block->Type= (u16)M->Type;
	Block->TimeStamp=TimeStamp;

	/* Do Some reporting */

	if (VerbLev >= GAL_NOISY)
		{
		DBG_SendMessage("Succesfully alloced block of %d",(int)Size);
		GAL_MemDump(M->Type);
		}


	/* Set the blocks name */

	SetBlockName(Block,Name);


	/* return Block's handle */

	return Block->Handle;
}


/*	---------------------------------------------------------------------------
	Function:	static MEM_HDR *FindBlockInTheseBounds(MEM_INIT_INFO *MI,void *Addr,U32 Size)

	Purpose:	Find a free block that fully covers from Addr to Addr[Size-1]

	Parms:		Head	= First block in link list to search
				Addr	= Address needed at
				Size	= Size of block

	Returns:	A -> to mem block if found else
				NULL
	--------------------------------------------------------------------------- */
static MEM_HDR *FindBlockInTheseBounds(MEM_HDR *Head,void *Addr,U32 Size)
{
	MEM_HDR *	Index;
	BOOL			Done;
	U32			ThisStart,ThisEnd;
	U32			Start,End;

	Start=(U32) Addr;
	End=Start+Size;;

	Done=FALSE;
	Index=Head;

	while (Index && !Done)
		{
		ThisStart=(U32) (Index->Mem);
		ThisEnd=ThisStart+Index->Size;

		if ((Start >= ThisStart) && (End <= ThisEnd))
			Done=TRUE;
		else
			Index=Index->Next;
		}

	if (Done)
		return Index;
	else
		return NULL;
}


/*	---------------------------------------------------------------------------
	Function:	static MEM_HDR *GetFreeMemHdrBlock(void)

	Purpose:	Get a mem hdr block from the list of free ones

	Returns:	A -> to mem block if found else
				NULL
	--------------------------------------------------------------------------- */
static MEM_HDR *GetFreeMemHdrBlock(void)
{
	MEM_HDR *RetBlock;

	RetBlock=NULL;

	if (FreeBlocks)
		{
		NumOfFreeHdrs--;

		if (VerbLev >= GAL_AVERAGE && NumOfFreeHdrs == 9)
			DBG_SendMessage("GAL: Warning Number of free headers < 10");

		RetBlock=FreeBlocks;
		DetachHdrFromList(&FreeBlocks,FreeBlocks);
		}

	return RetBlock;
}


/*	---------------------------------------------------------------------------
	Function:	static void ReleaseMemHdrBlock(MEM_HDR * Index)

	Purpose:	Puts a block back onto the list and does some
				auditing

	Params:		Index -> block to be put back on unused blocks list

	--------------------------------------------------------------------------- */
static void	ReleaseMemHdrBlock(MEM_HDR * Index)
{
	NumOfFreeHdrs++;
	AttachHdrToList(&FreeBlocks,Index);
}

/*	---------------------------------------------------------------------------
	Function:	void GAL_IterateEmptyMem(U32 MemType,void (*Func)(void *Addr,U32 Size,const char *Name))

	Purpose:	Iterate Through empry mem blocks of a mem type with a callback

	Params:		MemType = 	Type of mem to iterate through

	--------------------------------------------------------------------------- */
void GAL_IterateEmptyMem(U32 MemType,void (*Func)(void *Addr,U32 Size,const char *Name))
{
	MEM_INIT_INFO *	M;

	MemType&=~GAL_FLAGS;

	if ((M=GetMemInitInfoBlockFromType(MemType)))
		{
		MEM_HDR *			Block;

		Block=M->Empty;

		while (Block)
			{
			Func(Block->Mem,Block->Size,GetBlockName(Block));
			Block=Block->Next;
			}
		}
	else
		GSetError(ERR_GAL_INVALID_MEM_TYPE);

}	
/*	---------------------------------------------------------------------------
	Function:	void GAL_IterateUsedMem(U32 MemType,void (*Func)(MHANDLE hnd,void *Addr,U32 Size,const char *Name,int Users,int TimeStamp))

	Purpose:	Iterate Through empry mem blocks of a mem type with a callback

	Params:		MemType = 	Type of mem to iterate through

	--------------------------------------------------------------------------- */
void GAL_IterateUsedMem(U32 MemType,void (*Func)(MHANDLE hnd,void *Addr,U32 Size,const char *Name,int Users,int TimeStamp))
{
	MEM_INIT_INFO *	M;

	MemType&=~GAL_FLAGS;

	if ((M=GetMemInitInfoBlockFromType(MemType)))
		{
		MEM_HDR * Block;

		Block = M->Used;

		while (Block)
			{
			Func(Block->Handle,Block->Mem, Block->Size, GetBlockName(Block), Block->Owners,Block->TimeStamp);
			Block=Block->Next;
			}
		}
	else
		GSetError(ERR_GAL_INVALID_MEM_TYPE);

}	
/*	---------------------------------------------------------------------------
	Function:	BOOL GAL_SetMemName(MHANDLE Hnd,const char *Text);

	Purpose:	Set this mem's name

	Params:		Hnd = handle of block to set name of
				Text -> text to set it to

	--------------------------------------------------------------------------- */
BOOL GAL_SetMemName(MHANDLE Hnd,const char *Text)
{
	if (!IsActiveValidHandle(Hnd))
		{
		GSetError(ERR_GAL_INVALID_MEM_HANDLE);
		return(FALSE);
		}

	SetBlockName(&MemHdrBlocks[Hnd],Text);

	return(TRUE);
}	


/*	---------------------------------------------------------------------------
	Function:	U32 GAL_TotalMem(U32 Type)

	Purpose:	Tells how much mem in total this mem block has

	Params:		Type = 	Type of mem you want to know about

	Returns:	Amount of mem, 0 if an erroneous type is passed

	--------------------------------------------------------------------------- */
U32 GAL_TotalMem(U32 Type)
{
	U32					TotalMem;
	MEM_INIT_INFO *	M;

	Type&=~GAL_FLAGS;

	TotalMem=0;

	if ((M=GetMemInitInfoBlockFromType(Type)))
		TotalMem=M->Size;
	else
		GSetError(ERR_GAL_INVALID_MEM_TYPE);

	return(TotalMem);
}


/*	---------------------------------------------------------------------------
	Function:	void *GAL_MemBase(U32 Type)

	Purpose:	Says where the base of this mem type is

	Params:		Type = 	Type of mem you want to know about

	Returns:	Amount of mem, 0 if an erroneous type is passed

	--------------------------------------------------------------------------- */
void *GAL_MemBase(U32 Type)
{
	void *				Ret;
	MEM_INIT_INFO *	M;

	Ret=NULL;

	Type&=~GAL_FLAGS;


	if ((M=GetMemInitInfoBlockFromType(Type)))
		Ret=M->Mem;
	else
		GSetError(ERR_GAL_INVALID_MEM_TYPE);


	return(Ret);
}


/*	---------------------------------------------------------------------------
	Function:	BOOL GAL_DefragMem(U32 type)

	Purpose:	Defrag a mem type

	Params:		Type = 	Type of mem you want to defrag

	Returns:	TRUE if AOK

	--------------------------------------------------------------------------- */
BOOL GAL_DefragMem(U32 type)
{
	BOOL	GalRet;

	if (VerbLev >= GAL_AVERAGE)
		DBG_SendMessage("GAL: Attempting defrag");

	GalRet=GazDefragMem(type);

	if (FullErrorChecking)
		{
		if (!GAL_CheckMem(type))
			return FALSE;
		}

	NumOfFreeHdrs=CountFreeBlocks();

	return(TRUE);
}	


/*	---------------------------------------------------------------------------
	Function:	static BOOL GSetError(GAL_ERROR_CODE Err)

	Purpose:	Set's the internal error code to a vale

	Params:		Error code to set internal var to 

	Returns:	FALSE
	--------------------------------------------------------------------------- */
static BOOL GSetError(GAL_ERROR_CODE Err)
{
	if	(VerbLev >= GAL_AVERAGE)
		DBG_SendMessage("GAL Error: %s ",GalErrors[Err]);

	if (HaltOnError)
		DBG_Halt();

	LastError=Err;
	return(FALSE);
}	


/*	---------------------------------------------------------------------------
	Function:	BOOL GAL_CheckMem(U32 Type)

	Purpose:	Checks a memory types internal records for consistency

	Params:		Type = type of mem to check

	Returns:	False if there's some internal inconsistency

	--------------------------------------------------------------------------- */
BOOL GAL_CheckMem(U32 Type)
{
	MEM_INIT_INFO *	M;
	MEM_HDR *			MemHdr;
	u32					TotalMem;

	Type&=~GAL_FLAGS;

	TotalMem=0;

	if (!(M=GetMemInitInfoBlockFromType(Type)))
		{
		GSetError(ERR_GAL_INVALID_MEM_TYPE);
		return FALSE;
		}


	/* Check all of the empty blocks for collisions with any others */

	MemHdr=M->Empty;

	while (MemHdr)
		{
		if (CheckCollisions(M,MemHdr))
			return(GSetError(ERR_GAL_MEM_BLOCK_COLLISION));
			
		TotalMem+=MemHdr->Size;

		MemHdr=MemHdr->Next;
		}

	/* Check all of the used blocks for collisions with any others */

	MemHdr=M->Used;

	while (MemHdr)
		{
		if (CheckCollisions(M,MemHdr))
			return(GSetError(ERR_GAL_MEM_BLOCK_COLLISION));

		TotalMem+=MemHdr->Size;

		MemHdr=MemHdr->Next;
		}


	/* Check that the total mem is exactly the same as the mem info mem */

	if (M->Size != TotalMem)
		return(GSetError(ERR_GAL_MEM_AREA_NOT_COVERED));


	return(TRUE);
}	


/*	---------------------------------------------------------------------------
	Function:	static void CheckCollisions(MEM_INIT_INFO * M,MEM_HDR *MemHdr)

	Purpose:	Checks if this mem block overlaps with any other in this mem
				types empty or used list

	Params:		M -> Header for type of mem
				MemHdr -> Block to check

	Returns:	TRUE if it does
	--------------------------------------------------------------------------- */
static BOOL CheckCollisions(MEM_INIT_INFO * M,MEM_HDR *MemHdr)
{
	MEM_HDR *	CheckHdr;

	CheckHdr=M->Used;

	while (CheckHdr)
		{
		if ((CheckHdr != MemHdr) && AreBlocksColliding(MemHdr,CheckHdr))
			return(TRUE);

		CheckHdr=CheckHdr->Next;
		}


	CheckHdr=M->Empty;

	while (CheckHdr)
		{
		if ((CheckHdr != MemHdr) && AreBlocksColliding(MemHdr,CheckHdr))
			return(TRUE);

		CheckHdr=CheckHdr->Next;
		}

	return(FALSE);
}	

/*	---------------------------------------------------------------------------
	Function:	static BOOL AreBlocksColliding(MEM_HDR *Hdr1,MEM_HDR *Hdr1)

	Purpose:	Do this two blocks overlap

	Params:		Hdr1 -> First block
				Hdr2 -> Second block

	Returns:	TRUE if it does
	--------------------------------------------------------------------------- */
static BOOL AreBlocksColliding(MEM_HDR *Hdr1,MEM_HDR *Hdr2)
{
	U32	Addr1;
	U32	Addr2;

	Addr1=(U32) Hdr1->Mem;
	Addr2=(U32) Hdr2->Mem;

	if ((Addr1< Addr2+Hdr2->Size) && (Addr1 >= Addr2))
		return(TRUE);
		
	if ((Addr2< Addr1+Hdr1->Size) && (Addr2 >= Addr1))
		return(TRUE);
		
	return(FALSE);
	
}	

/*	---------------------------------------------------------------------------
	Function:	const char *GAL_GetErrorText(GAL_ERROR_CODE Err)

	Purpose:	Converts a GAL error to 

	Params:		Err = Error to get code for

	Returns:	NULL if no error || -> error text
	--------------------------------------------------------------------------- */
char *GAL_GetErrorText(GAL_ERROR_CODE Err)
{
	if (Err>= NUM_OF_ERROR_MESSAGES)
		return("Invalid error code");
	else
		return(GalErrors[Err]);
}	

/*	---------------------------------------------------------------------------
	Function:	GAL_ERROR_CODE GAL_GetLastErrorCode(void)

	Purpose:	Get's last error gal had

	Returns:	Last error code
	--------------------------------------------------------------------------- */
GAL_ERROR_CODE GAL_GetLastErrorCode(void)
{
	return(LastError);
}	

/*	---------------------------------------------------------------------------
	Function:	char *GAL_GetLastErrorText(void)

	Purpose:	Get's text for last error gal had

	Returns:	NULL if no error || -> error text
	--------------------------------------------------------------------------- */
char *GAL_GetLastErrorText(void)
{
	return(GAL_GetErrorText(LastError));
}	


/*	---------------------------------------------------------------------------
	Function:	int GAL_HowManyFreeBlocksUsed(U32 Type)

	Purpose:	Find's out how many free blocks are used by this mem type

	Returns:	Blocks used
	--------------------------------------------------------------------------- */
int GAL_HowManyEmptyRegions(U32 Type)
{
	MEM_INIT_INFO * 	m;
	int					Count;

	Type&=~GAL_FLAGS;

	if ((m = GetMemInitInfoBlockFromType(Type)))
		{
		MEM_HDR *	mh;

		Count=0;
		
		mh=m->Empty;

		while (mh)
			{
			Count++;
			mh=mh->Next;
			}
		}
	else
		{
		GSetError(ERR_GAL_INVALID_MEM_TYPE);
		Count=-1;
		}

	return Count;
}

/*	---------------------------------------------------------------------------
	Function:	int GAL_HowManyFreeBlocksUsed(U32 Type)

	Purpose:	Find's out how many free blocks are used by this mem type

	Returns:	Blocks used
	--------------------------------------------------------------------------- */
int GAL_HowManyUsedRegions(U32 Type)
{
	MEM_INIT_INFO * 	m;
	int					Count;

	Type&=~GAL_FLAGS;

	if ((m = GetMemInitInfoBlockFromType(Type)))
		{
		MEM_HDR *	mh;
		mh=m->Used;
		Count=0;

		while (mh)
			{
			Count++;
			mh=mh->Next;
			}
		}
	else
		{
		GSetError(ERR_GAL_INVALID_MEM_TYPE);
		Count=-1;
		}

	return Count;
}	


/*	---------------------------------------------------------------------------
	Function:	void GAL_SetTimeStamp(int Time)

	Purpose:	All blocks alloced will be of this time stamp
	--------------------------------------------------------------------------- */
void GAL_SetTimeStamp(int Time)
{
	TimeStamp=Time;
}	


/*	---------------------------------------------------------------------------
	Function:	void GAL_SetTimeStamp(int Time)

	Purpose:	All blocks alloced will be of this time stamp
	--------------------------------------------------------------------------- */
void GAL_IncTimeStamp(void)
{
	TimeStamp++;
}	

/*	---------------------------------------------------------------------------
	Function:	void GAL_SetTimeStamp(int Time)

	Purpose:	All blocks alloced will be of this time stamp
	--------------------------------------------------------------------------- */
int GAL_GetTimeStamp(void)
{
	return TimeStamp;
}	


/*	---------------------------------------------------------------------------
	Function:	S32 GAL_AlignSizeToType(U32 Size,U32 MemType)

	Purpose:	Align a size to the alignment specified by a mem type

	Params:		Size	=	Size to align
				MemType =	Mem type

	Returns:	Aligned size or -1 if an error

	--------------------------------------------------------------------------- */
S32 GAL_AlignSizeToType(U32 Size,U32 MemType)
{
	MEM_INIT_INFO *	Mi;

	MemType&=~GAL_FLAGS;

	Mi=GetMemInitInfoBlockFromType(MemType);

	if (Mi)
		return(AlignSize(Size,Mi->Alignment));
	else
		return(-1);
}	


/*	---------------------------------------------------------------------------
	Function:	MHANDLE GAL_AllocMultiStruct(GAL_STRUCT * G,U32 Type,const char *Name)

	Purpose:	Alloc a load of structures back to back all aligned to correct
				boundaries for the mem type. Then store the offset into the mem
				mem alloced for each struct back into the GAL_STRUC_ALLOC array.
				The array should be terminated by an element with an original size
				entry of -1. See TASKER.C TSK_AddTask for an example

	Params:		G ->		Array of GAL_STRUC_ALLOC
				Type =		Mem to put it into
				Name =		Name of this block

	Returns:	HND to mem the mem if ok || NULL_HANDLE if not

	--------------------------------------------------------------------------- */
MHANDLE GAL_AllocMultiStruct(GAL_STRUCT * G,U32 Type,const char *Name)
{
	int	TotalMem;
	TotalMem=GAL_ProcessMultiStruct(G,Type&~GAL_FLAGS);

	return(GAL_Alloc(TotalMem,Type,Name));
}

/*	---------------------------------------------------------------------------
	Function:	UINT GAL_ProcessMultiStruct(GAL_STRUCT * G,U32 Type)

	Purpose:	Works out the offsets for this multi struc in a particular mem
				type and stores em back into the Offset stuff

	Params:		G ->		Array of GAL_STRUC_ALLOC
				Type =		Mem to put it into

	Returns:	How big the alloced block would be

	--------------------------------------------------------------------------- */
UINT GAL_ProcessMultiStruct(GAL_STRUCT * G,U32 Type)
{
	UINT TotalMem;
	int	f;

	Type&=~GAL_FLAGS;

	TotalMem=0;

	for (f=0;G[f].OriginalSize != -1;f++)
		{
		G[f].Offset=TotalMem;
		TotalMem+=GAL_AlignSizeToType((UINT) G[f].OriginalSize,Type);
		}

	G[f].Offset=TotalMem;

	return(TotalMem);
}	

/*	---------------------------------------------------------------------------
	Function:	static BOOL GazDefragMem(U32 MemType)

	Purpose:	Defrag the memory of this mem type

	Returns:	If there was a problem or not

	--------------------------------------------------------------------------- */
s32 GAL_GetSize(MHANDLE hnd)
{
	MEM_HDR *			MemHdr;

	if (!IsActiveValidHandle(hnd))
		{
		GSetError(ERR_GAL_INVALID_MEM_HANDLE);
		return -1;
		}

	MemHdr=&MemHdrBlocks[hnd];
	return(MemHdr->Size);
}



/*	---------------------------------------------------------------------------
	Function:	static BOOL GazDefragMem(U32 MemType)

	Purpose:	Defrag the memory of this mem type

	Returns:	If there was a problem or not

	--------------------------------------------------------------------------- */

static BOOL GazDefragMem(U32 MemType)
{
	MEM_HDR *			LockedBlocks;
	MEM_INIT_INFO *	M;
	MEM_REG				Reg;

	MemType&=~GAL_FLAGS;

	if (!(M=GetMemInitInfoBlockFromType(MemType)))
		{
		GSetError(ERR_GAL_INVALID_MEM_TYPE);
		return (FALSE);
		}

	/* Only defrag if a memove defined in memory profile */

	if (!M->MemMove)
		{
		GSetError(ERR_GAL_NO_MEM_MOVE);
		return (FALSE);
		}

	
	/* Take all the locked blocks off the used list and chuck then onto our local list */

	LockedBlocks=NULL;
	PutAllLockedBlocksOntoList(&LockedBlocks,&M->Used);

	/* Sort our brand new list by size */

	/* Sort remaining used but unlocked blocks into addr order */

	SortMemHdrListByAddr(&LockedBlocks);


	/* Get Rid of all Empty blocks in MemType */

	DeleteEmptyBlocks(M);

	/* Now loop through sorted empty blocks */

	Reg.Mem=NULL;

	while (GetRegion(&Reg,LockedBlocks,M))
		{

		/* If there was a region and it has a size the do the business */

		if (Reg.Size)
			{
			MEM_HDR *	NewEmptyBlock;
			MEM_HDR *	ListOfBlocksInRegion;
			U32			ShuffledSize;
			int			GapSize;

			ListOfBlocksInRegion=NULL;

			PutBlocksInRegionIntoList(&Reg,&ListOfBlocksInRegion,&M->Used);

			SortMemHdrListByAddr(&ListOfBlocksInRegion);
			ShuffledSize=ShuffleBlocks(ListOfBlocksInRegion,&Reg,M);

			/* Create the empty block for the rest of the region */

			GapSize=Reg.Size-ShuffledSize;

			if (GapSize)
				{
				if (!(NewEmptyBlock=GetFreeMemHdrBlock()))
					{
					GSetError(ERR_RUN_OUT_OF_MEM_HDRS);
					return FALSE;
					}

				NewEmptyBlock->Mem=(void *)((U32)Reg.Mem+ShuffledSize);
				NewEmptyBlock->Size= GapSize;
				NewEmptyBlock->Type=(u16)MemType;

				/* And put it into the list */

				MergeToEmptyList(M,NewEmptyBlock);
				}

			GraftMemHdrList(&M->Used,&ListOfBlocksInRegion);
			}
		}

	/* Now add locked blocks back onto list */

	PutAllLockedBlocksOntoList(&M->Used,&LockedBlocks);


	return(TRUE);
}

/*	---------------------------------------------------------------------------
	Function:  void PutBlocksInRegionIntoList(MEM_REG *Reg,MEM_HDR **ToList,MEM_HDR **FromList)

	Purpose:   Go through From list and take out all the blocks in the
			   region and put it into ToList

	Params:	   Reg -> Struct describing region
			   ToList -> Head of list to put them into
			   FromList -> Head of list to take them from
	--------------------------------------------------------------------------- */
static void PutBlocksInRegionIntoList(MEM_REG *Reg,MEM_HDR **ToList,MEM_HDR **FromList)
{
	MEM_HDR *	ThisBlock;


	ThisBlock=*FromList;

	while (ThisBlock)
		{
		MEM_HDR *	NextBlock;
		MEM_REG		MemReg;

		NextBlock=ThisBlock->Next;

		MemReg.Mem=ThisBlock->Mem;
		MemReg.Size=ThisBlock->Size;

		if (CollideRegions(Reg,&MemReg))
			{
			DetachHdrFromList(FromList,ThisBlock);
			AttachHdrToList(ToList,ThisBlock);
			}

		ThisBlock=NextBlock;
		}
}	

/*	---------------------------------------------------------------------------
	Function:	BOOL CollideRegions(MEM_REG *Reg1,MEM_REG *Reg2)

	Purpose:	Do these regions collide

	Params:		Reg1 -> Region descriptor 1
				Reg2 -> Region descriptor 2

	Return:		TRUE if so
	--------------------------------------------------------------------------- */
static BOOL CollideRegions(MEM_REG *Reg1,MEM_REG *Reg2)
{

	if (((U32) Reg1->Mem + Reg1->Size) <= (U32) Reg2->Mem)
		return(FALSE);

	if ((U32) Reg1->Mem >= ((U32) Reg2->Mem + Reg2->Size))
		return(FALSE);

	return(TRUE);
}	

/*	---------------------------------------------------------------------------
	Function:	static void DeleteEmptyBlocks(MEM_INIT_INFO *M)

	Purpose:	Go through this mem type and delete all empty blocks

	Params:		M -> Information about this mem type
	--------------------------------------------------------------------------- */
static void DeleteEmptyBlocks(MEM_INIT_INFO *M)
{
	while (M->Empty)
		{
		MEM_HDR *	ThisBlock;
		
		ThisBlock=M->Empty;

		DetachHdrFromList(&M->Empty,ThisBlock);
		AttachHdrToList(&FreeBlocks,ThisBlock);
		}
}

/*	---------------------------------------------------------------------------
	Function:	void BOOL GetRegion(MEM_REG *Reg,MEM_HDR * LockedBlocks,MEM_INIT_INFO *M)

	Purpose:	Find the next region of memory after Region that has no locked blocks
				a new list

	Params:		Reg ->				Description of previous region (If base is NULL will
									from beggining
				LockedBlocks -> 	List of locked blocks sorted by addr
				M ->				Information about this mem type

	Returns:	TRUE if there was another region and info about it in Reg
				FALSE if not another region

	--------------------------------------------------------------------------- */
static BOOL GetRegion(MEM_REG *Reg,MEM_HDR * LockedBlocks,MEM_INIT_INFO *M)
{
	MEM_HDR *	FirstBlock;
	MEM_HDR *	SecondBlock;
	MEM_REG		NewReg;
	U32			FirstSize;

	FirstBlock=FindNextBlock(Reg->Mem,LockedBlocks);

	FirstSize=0;

	if (FirstBlock)
		NewReg.Mem=(void *)((U32)FirstBlock->Mem+FirstBlock->Size);
	else
		{
		if (Reg->Mem)
			return(FALSE);
		else
			NewReg.Mem=M->Mem;
		}

	SecondBlock=FindNextBlock(NewReg.Mem,LockedBlocks);

	if (SecondBlock)
		NewReg.Size=(U32) SecondBlock->Mem - (U32) NewReg.Mem;
	else
		NewReg.Size=(U32) M->Mem +M->Size - (U32) NewReg.Mem;

	if (CollideRegions(Reg,&NewReg) && Reg->Mem)
		return(FALSE);
	else
		{
		Reg->Mem=NewReg.Mem;
		Reg->Size=NewReg.Size;
		return(TRUE);
		}
}	

/*	---------------------------------------------------------------------------
	Function:	static MEM_HDR *FindStartBlock(void *Addr,MEM_HDR * Blocks)

	Purpose:	Find the first block in this list that Sits addres this addr

	Params:		Addr = addr to look from
				Blocks -> List of blocks to look in

	Returns:	-> Block || Null if none
	--------------------------------------------------------------------------- */

/*
static MEM_HDR *FindStartBlock(void *Addr,MEM_HDR * Blocks)
{
	while (Blocks)
		{
		if (Blocks->Mem==Addr)
			return(Blocks);
		else
			Blocks=Blocks->Next;
		}

	return(NULL);
}
*/

/*	---------------------------------------------------------------------------
	Function:	static MEM_HDR *FindNextBlock(void *Addr,MEM_HDR * Blocks)

	Purpose:	Find the first block that is at >= Addr

	Params:		Addr = addr to look from
				Blocks -> List of blocks to look in

	Returns:	-> Block || Null if none
	--------------------------------------------------------------------------- */
static MEM_HDR *FindNextBlock(void *Addr,MEM_HDR * Blocks)
{
	if (Addr)
		{
		while (Blocks)
			{
			U32	BlockAddr;
			U32	AddrU32;

			AddrU32=(U32) Addr;
			BlockAddr=(U32) Blocks->Mem;

			if (BlockAddr >= AddrU32)
				return(Blocks);
			else
				Blocks=Blocks->Next;
			}
		}

	return(NULL);
}

/*	---------------------------------------------------------------------------
	Function:	static U32 ShuffleBlocks(MEM_HDR *Blocks,MEM_REG *Reg,MEM_INIT_INFO *M)

	Purpose:	Compact a linked list of sorted by addr blocks in mem

	Params:		Blocks -> First in list of blocks
				Reg -> Region the blocks should compact themselves in
				M -> Description of mem region

	Returns:	Size all blocks take up in memory
	--------------------------------------------------------------------------- */
static U32 ShuffleBlocks(MEM_HDR *Blocks,MEM_REG *Reg,MEM_INIT_INFO *M)
{
	U32			NewSize;
	void *		MemBase;
	MEM_HDR *	ThisBlock;

	NewSize=0;

	MemBase=Reg->Mem;

	ThisBlock=Blocks;

	while (ThisBlock)
		{
		NewSize+=ThisBlock->Size;

		if (MemBase != ThisBlock->Mem)
			{
			M->MemMove(MemBase,ThisBlock->Mem,ThisBlock->Size);
			ThisBlock->Mem=MemBase;
			}

		MemBase=(void *)((U32) ThisBlock->Mem+ThisBlock->Size);

		ThisBlock=ThisBlock->Next;
		}

	return(NewSize);
	

}	

/*	---------------------------------------------------------------------------
	Function:	void PutAllLockedBlocksOntoList(MEM_HDR **ToHead,MEM_HDR **FromHead)

	Purpose:	Take all the locked blocks from a list and chuck them onto
				a new list

	Params:		ToHead -> Head of list to put stuff onto
				FromHead -> Head of list to take blocks off
	--------------------------------------------------------------------------- */
static void PutAllLockedBlocksOntoList(MEM_HDR **ToHead,MEM_HDR **FromHead)
{
	MEM_HDR *	CurHdr;

	CurHdr = *FromHead;

	while (CurHdr)
		{
		MEM_HDR *	NextCurHdr;

		NextCurHdr=CurHdr->Next;

		if (CurHdr->Owners)
			{
			DetachHdrFromList(FromHead,CurHdr);
			AttachHdrToList(ToHead,CurHdr);
			}

		CurHdr=NextCurHdr;
		}
}	


/*	---------------------------------------------------------------------------
	Function:	void SortMemHdrListByAddr(MEM_HDR **Head)

	Purpose:	Sort all the memhdr blocks in a list in descending
				order by base Address and yes, it's a bubble sort

	Params:		Head -> Head of list to sort
	--------------------------------------------------------------------------- */

void SortMemHdrListByAddr(MEM_HDR **Head)
{
	BOOL		DidASwap;
	MEM_HDR *	CurHdr;
	MEM_HDR *	NextHdr;
	
	if (*Head && (*Head)->Next)
		{
		do
			{

			DidASwap=FALSE;

			CurHdr=*Head;

			do
				{

				NextHdr=CurHdr->Next;

				if ((U32) CurHdr->Mem > (U32) NextHdr->Mem)
					{
					MEM_HDR * OldPrev;

					OldPrev=CurHdr->Prev;

					/*		Swap them in the link list */

					CurHdr->Next = NextHdr->Next;
					CurHdr->Prev = NextHdr;
					NextHdr->Next= CurHdr;
					if (CurHdr->Next)
						CurHdr->Next->Prev=CurHdr;

					NextHdr->Next=CurHdr;
					NextHdr->Prev=OldPrev;
					if (NextHdr->Prev)
						NextHdr->Prev->Next=NextHdr;
					else
						*Head=NextHdr;

					DidASwap=TRUE;
					}
				else
					CurHdr=CurHdr->Next;
				}
			while(CurHdr->Next);

			}
		while(DidASwap);
		}
}	

/*	---------------------------------------------------------------------------
	Function:	void GraftMemHdrList(MEM_HDR ** ToList,MEM_HDR ** FromList)

	Purpose:	Graft one list to another

	Params:		ToList -> Head of list to graft to
				FromList -> Head of list to graft
	--------------------------------------------------------------------------- */
static void GraftMemHdrList(MEM_HDR ** ToList,MEM_HDR ** FromList)
{
	MEM_HDR *	OldFirst;

	OldFirst=*ToList;

	if (*FromList)
		{
		MEM_HDR *	LastHdr;

		*ToList=*FromList;

		LastHdr=*FromList;

		while (LastHdr->Next)
			LastHdr=LastHdr->Next;

		LastHdr->Next=OldFirst;

		if (OldFirst)
			OldFirst->Prev=LastHdr;

		*FromList=NULL;
		}
}	

/*	---------------------------------------------------------------------------
	Function:	void GAL_MemDump(U32 Type)

	Purpose:	Do a dump of this type to the debug messager

	Params:		Type = type to dump

	--------------------------------------------------------------------------- */
void GAL_MemDump(U32 Type)
{
	Type&=~GAL_FLAGS;

	DBG_SendMessage("%d : mem left",(int)GAL_GetFreeMem(Type));
	DBG_SendMessage("%d : largest block",(int)GAL_GetFreeMem(Type));
	DBG_SendMessage("%d : Last attempted alloc",(int)LastAttemptedAlloc);
}	


/*	---------------------------------------------------------------------------

	Function:	void GAL_SetVerbosity(GAL_VERB_LEV G)

	Purpose:	Say how noisy gal should be

	Params:		Type = type to dump

	--------------------------------------------------------------------------- */
void GAL_SetVerbosity(GAL_VERB_LEV G)
{
	VerbLev=G;
}	


/*	---------------------------------------------------------------------------
	Function:	void GAL_SetVerbosity(GAL_VERB_LEV G)

	Purpose:	Say how noisy gal should be

	Params:		Type = type to dump

	--------------------------------------------------------------------------- */
int CountFreeBlocks(void)
{
	MEM_HDR *	RetBlock;
	int			Count;

	RetBlock=FreeBlocks;
	Count=0;

	while (RetBlock)
		{
		Count++;
		RetBlock=RetBlock->Next;
		}

	return(Count);
}	

/*	---------------------------------------------------------------------------
	Function:	void SetBlockName(MEM_HDR *	MemHdr,const char * NewName)

	Purpose:	Set the name of this block

	Params:		MemHdr - >			Block to set name of
				NewName ->			New name of block

	--------------------------------------------------------------------------- */
void SetBlockName(MEM_HDR *	MemHdr,const char * NewName)
{
#ifdef __GL_DEBUG__

	int		IndexSoFar;

	IndexSoFar=0;

	if (NewName)
		{
		while ((IndexSoFar != MAX_NAME_SIZE-1) && *NewName)
			MemHdr->Name[IndexSoFar++]=*NewName++;
		}

	MemHdr->Name[IndexSoFar]=0;
#endif
}

/*	---------------------------------------------------------------------------
	Function:	void SetBlockName(MEM_HDR *	MemHdr,const char * NewName)

	Purpose:	Set the name of this block

	Params:		MemHdr - >			Block to set name of
				NewName ->			New name of block

	--------------------------------------------------------------------------- */
char const * GetBlockName(MEM_HDR * MemHdr)
{
#ifdef __GL_DEBUG__
	return(MemHdr->Name);
#else
	return("NO NAME");
#endif
}


/*	---------------------------------------------------------------------------
	Function:	int GAL_GetNumFreeHeaders(void)
	Purpose:	Find out how many more free hdrs there are
	Returns:	Info we want
	--------------------------------------------------------------------------- */
int GAL_GetNumFreeHeaders(void)
{
	return(NumOfFreeHdrs);
}

u32 GAL_GetLastTypeAlloced(void)
{
	return(LastTypeAlloced);
}

/*	---------------------------------------------------------------------------
	Function:	int GAL_GetAlignment(u32 MemType)
	Purpose:	Get the allocation boundary alignment for this memory type
	Returns:	Info we want or -1 if an error
	--------------------------------------------------------------------------- */
u32 GAL_GetAlignment(u32 MemType)
{
	MEM_INIT_INFO *	M;

	if (!(M=GetMemInitInfoBlockFromType(MemType)))
		{
		GSetError(ERR_GAL_INVALID_MEM_TYPE);
		return -1;
		}
	else
		return(M->Alignment);
}


/*	---------------------------------------------------------------------------
	Function:	void GAL_HaltOnError(void)
	Purpose:	Say that gal will halt on an error
	--------------------------------------------------------------------------- */
void GAL_HaltOnError(void)
{
	HaltOnError=TRUE;
}				

/*	---------------------------------------------------------------------------
	Function:	int GAL_GetAlignment(u32 MemType)
	Purpose:	Say that gal WONT will halt on an error (Default behaviour)
	--------------------------------------------------------------------------- */
void GAL_ReturnOnError(void)
{
	HaltOnError=FALSE;
}

/*	---------------------------------------------------------------------------
	Function:	GAL_FILTER	GAL_SetAllocFilter(GAL_FILTER NewFilter)
	Purpose:	Set a callback to be executed b4 every alloc
	Params:		NewFilter -> Filter func (NULL means no filtering)
	Returns:	Old filter
	--------------------------------------------------------------------------- */
GAL_FILTER	GAL_SetAllocFilter(GAL_FILTER NewFilter)
{
	GAL_FILTER	OldFilter;

	OldFilter=AllocFilter;
	AllocFilter=NewFilter;

	return(OldFilter);
}


/*	---------------------------------------------------------------------------
	Function:	void GAL_SortUsedRegionsBySize(U32 Type)
	Purpose:	Sort the order of the used regions by size (descending)
	Params:		MemType=type of mem to sort
	--------------------------------------------------------------------------- */
GLIB_API BOOL GAL_SortUsedRegionsBySize(U32 MemType)
{
	MEM_INIT_INFO *	M;

	MemType&=~GAL_FLAGS;

	if (!(M=GetMemInitInfoBlockFromType(MemType)))
		{
		GSetError(ERR_GAL_INVALID_MEM_TYPE);
		return (FALSE);
		}
	SortMemHdrList(&M->Used,SortSize);
	return(TRUE);
}

BOOL SortSize(MEM_HDR *B1,MEM_HDR * B2)
{
	if (B1->Size<B2->Size)
		return(TRUE);
	else
		return(FALSE);
}

/*	---------------------------------------------------------------------------
	Function:	void GAL_SortUsedRegionsBySize(U32 Type)
	Purpose:	Sort the order of the used regions by size (descending)
	Params:		MemType=type of mem to sort
	--------------------------------------------------------------------------- */
GLIB_API BOOL GAL_SortUsedRegionsByAddress(U32 MemType)
{
	MEM_INIT_INFO *	M;

	MemType&=~GAL_FLAGS;

	if (!(M=GetMemInitInfoBlockFromType(MemType)))
		{
		GSetError(ERR_GAL_INVALID_MEM_TYPE);
		return (FALSE);
		}
	SortMemHdrList(&M->Used,SortAddr);
	return(TRUE);
}

BOOL SortAddr(MEM_HDR *B1,MEM_HDR * B2)
{
	if ((u32)B1->Mem<(u32)B2->Mem)
		return(TRUE);
	else
		return(FALSE);
}


/*	---------------------------------------------------------------------------
	Function:	void SortMemHdrList(MEM_HDR **Head,bool (*CompFunc)(MEM_HDR *B1,MEM_HDR * B2))
	Purpose:	Sort a list of memhdr blocks
	Params:		Head->->		Head of list to be sorted
				CompFunc		Should I swap routine
	--------------------------------------------------------------------------- */
void SortMemHdrList(MEM_HDR **Head,BOOL (*CompFunc)(MEM_HDR *B1,MEM_HDR * B2))
{
	BOOL		DidASwap;
	MEM_HDR *	CurHdr;
	MEM_HDR *	NextHdr;
	
	if (*Head && (*Head)->Next)
		{
		do
			{

			DidASwap=FALSE;

			CurHdr=*Head;

			do
				{
				NextHdr=CurHdr->Next;

				if (CompFunc(CurHdr,NextHdr))
					{
					MEM_HDR * OldPrev;

					OldPrev=CurHdr->Prev;

					/*		Swap them in the link list */

					CurHdr->Next = NextHdr->Next;
					CurHdr->Prev = NextHdr;
					NextHdr->Next= CurHdr;
					if (CurHdr->Next)
						CurHdr->Next->Prev=CurHdr;

					NextHdr->Next=CurHdr;
					NextHdr->Prev=OldPrev;
					if (NextHdr->Prev)
						NextHdr->Prev->Next=NextHdr;
					else
						*Head=NextHdr;

					DidASwap=TRUE;
					}
				else
					CurHdr=CurHdr->Next;
				}
			while(CurHdr->Next);

			}
		while(DidASwap);
		}
}	

/* ---------------------------------------------------------------------------
	ends
	---- */
