/**************************/
/*** Memory Alloc Stuff ***/
/**************************/

#ifndef	__MEMORY_HEADER__
#define	__MEMORY_HEADER__


#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif


/*****************************************************************************/
// Define if you want to debug memory
#ifdef __USER_paul__
	#ifdef	__VERSION_DEBUG__
	#define	__DEBUG_MEM__
	#endif
#endif

/*****************************************************************************/
#define LListLen		(256)

/*****************************************************************************/
typedef struct
	{
	char		*Addr;
	u32 		Len;
	u16			Prev;
	u16			Next;
	}sLLNode;

typedef	struct
	{
	u32			TotalRam;
	u32			RamUsed;

	u16			Head;
	u16			Tail;
	u16			SP;

	u16			Stack[LListLen];

	sLLNode		Nodes[LListLen];
	} sLList;

/*****************************************************************************/

char *	MemAllocate( u32 Size, char const *Name, char const * File, int LineNumber);

void	MemInit();
void  	MemFree(void *Addr);

void *	operator new(size_t Size, const char * name = NULL);
void *	operator new[](size_t Size, const char * name = NULL);
void	operator delete(void *Ptr);
void	operator delete[](void *Ptr);


#ifdef __DEBUG_MEM__
	void	dumpDebugMem();
	void	DebugMemFontInit();
	#define MemAlloc( Size, Name )	MemAllocate( (Size), (Name), __FILE__, __LINE__ )
#else
	#define MemAlloc(Size,Name)	MemAllocate( (Size), NULL, NULL, 0 )
	#define	dumpDebugMem	;
	#define	DebugMemFontInit	;

#endif


/*****************************************************************************/
extern sLList		MainRam;		// Ah well!

/*****************************************************************************/

#endif