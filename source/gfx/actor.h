/******************/
/*** Actor Bank ***/
/******************/

#ifndef __ACTOR_HEADER__
#define __ACTOR_HEADER__

#include "system\global.h"
#include <dstructs.h>

#ifndef	__BigLump_H__
#include <biglump.h>
#endif

/*****************************************************************************/
class	CActorGfx
{
public:
	CActorGfx(){};
	CActorGfx(FileEquate Filename);
	~CActorGfx();

	void		Render(DVECTOR &Pos,int Anim,int Frame,bool FlipX);
	void		Dump();

	int			getFrameCount()		{return(0);}

	FileEquate		GetFilename()	{return(Filename);}
private:
		FileEquate		Filename;
		sSpriteAnimBank	*ActorData;
};

/*****************************************************************************/
class CActorBank
{
public:
		enum
		{
			MAX_ACTORS=8,
		};
//		virtual ~ActorBank(){};

static	void		Init();

static	void			AddActor(FileEquate Filename)		{GetActor(Filename);}
static	CActorGfx		*GetActor(FileEquate Filename);
static	void			DumpActor(FileEquate Filename);
		
private:
static	int			FindIdx(FileEquate Filename);
static	int			FindFreeIdx();

static	CActorGfx	*ActorList[MAX_ACTORS];

};

/*****************************************************************************/

#endif