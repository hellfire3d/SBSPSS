/******************/
/*** Actor Bank ***/
/******************/

#include "system\global.h"
#include "mem\memory.h"
#include "fileio\fileio.h"
#include "utils\quat.h"
#include "gfx\actor.h"
#include "utils\utils.h"

#include <dstructs.h>

/*****************************************************************************/
CActorGfx	*CActorPool::ActorList[CActorPool::MAX_ACTORS];

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CActorGfx::CActorGfx(FileEquate _Filename)
{
		Filename=_Filename;
/*		ActorData=(sActorHdr*)CFileIO::loadFile(Filename);

		for (int i=0; i<ActorData->AnimCount; i++)
		{
			ActorData->AnimList[i].Move=(s32*)		MakePtr(ActorData,(int)ActorData->AnimList[i].Move);
			ActorData->AnimList[i].Anim=(AnimIdx*)	MakePtr(ActorData,(int)ActorData->AnimList[i].Anim);
		}

		DAVE_DBGMSG("Nodes %i\n",ActorData->BoneCount);
		DAVE_DBGMSG("Tris  %i\n",ActorData->TriCount);
		DAVE_DBGMSG("Vtx %i\n",ActorData->VtxCount);
		DAVE_DBGMSG("Anims %i\n",ActorData->AnimCount);
*/
}

/*****************************************************************************/
CActorGfx::~CActorGfx()
{
		MemFree(ActorData);
}

/*****************************************************************************/
void	CActorGfx::Render(DVECTOR &Pos,int Anim,int Frame,bool FlipX)
{

}

/*****************************************************************************/
void	CActorGfx::Dump()
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CActorPool::Init()
{
		for (int i=0; i<MAX_ACTORS; i++)
		{
			ActorList[i]=0;
		}
}

/*****************************************************************************/
int		CActorPool::FindIdx(FileEquate Filename)
{
		for (int i=0; i<MAX_ACTORS; i++)
		{
			if (ActorList[i] && ActorList[i]->GetFilename()==Filename) return(i);
		}
		return(-1);
}

/*****************************************************************************/
int		CActorPool::FindFreeIdx()
{
		for (int i=0; i<MAX_ACTORS; i++)
		{
			if (!ActorList[i]) return(i);
		}
		return(-1);
}

/*****************************************************************************/
/*** Load ********************************************************************/
/*****************************************************************************/
CActorGfx	*CActorPool::GetActor(FileEquate Filename)
{
CActorGfx	*NewActor;
int			Idx;
// Already Loaded?
		Idx=FindIdx(Filename);
		if (Idx!=-1) return(ActorList[Idx]);

// Create and Load
		Idx=FindFreeIdx();
		ASSERT(Idx!=-1);

		NewActor=new ("ActorPool") CActorPool(Filename);
		ActorList[Idx]=NewActor;

		return(NewActor);
}

/*****************************************************************************/
/*** Dump ********************************************************************/
/*****************************************************************************/
void	CActorPool::DumpActor(FileEquate Filename)
{
CActorPool	*ThisActor;
int			Idx;

// Is Loaded?
		Idx=FindIdx(Filename);
		ASSERT(Idx!=-1); 
		ThisActor=ActorList[Idx];
		ActorList[Idx]=0;
		delete ThisActor;
}

