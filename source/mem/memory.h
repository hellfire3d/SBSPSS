/**************************/
/*** Memory Alloc Stuff ***/
/**************************/

#ifndef	__MEMORY_HEADER__
#define	__MEMORY_HEADER__


#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif


/*****************************************************************************/

#ifdef	__VERSION_DEBUG__
#define	__DEBUG_MEM__		// Define if you want to debug memory - TS
#endif

/*****************************************************************************/


/*****************************************************************************/
#define LListLen		50

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
void	DebugMemFontInit();
void  	MemFree(void *Addr);

void *	operator new(size_t Size, const char * name = NULL);
void *	operator new[](size_t Size, const char * name = NULL);
void	operator delete(void *Ptr);
void	operator delete[](void *Ptr);


#ifdef __VERSION_DEBUG__
	void	dumpDebugMem();
	#define MemAlloc( Size, Name )	MemAllocate( (Size), (Name), __FILE__, __LINE__ )
#else
	#define MemAlloc(Size,Name)	MemAllocate( (Size), NULL, NULL, 0 )
#endif


/*****************************************************************************/
extern sLList		MainRam;		// Ah well!

/*****************************************************************************/

#endif