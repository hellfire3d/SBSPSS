/*=========================================================================

	thing.h

	Author:		PKG
	Created:	
	Project:	Spongebob
	Purpose:	

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__THING_THING_H__
#define __THING_THING_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif

#ifndef __GAME_EVENT_H__
#include "game\event.h"
#endif

#include <dstructs.h>

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

// Collision rectangle definition
struct	CRECT
{
	int	x1,y1,x2,y2;
};

/*----------------------------------------------------------------------*/
// Thing manager class
class	CThing;
class	CThingManager
{
public:
static	void		init();
static	void		shutdown();
static	void		killAllThingsForRespawn();

static	void		initAllThings();
static	void		thinkAllThings(int _frames);
static	void		renderAllThings();
static	void		processEventAllThings(GAME_EVENT _event,CThing *_sourceThing);

static	CThing		*checkCollisionAreaAgainstThings(CRECT *_area,int _type,int _continue);
static	void		initCollision();
static	void		matchWheelsAndWeights();
static	void		matchPressureSwitches();
static	void		matchGaryTriggers();
static	void		shakePlatformLoose();

static	sBBox		&getRenderBBox()			{return(m_RenderBBox);}
static	sBBox		&getThinkBBox()				{return(m_ThinkBBox);}
static	void		setMapWH(DVECTOR const &WH);

protected:
static	void		initList(CThing **List);
static	void		addToThingList(CThing *_this);
static	void		removeFromThingList(CThing *_this);

static void			addToCollisionList(CThing *_this);
friend	class CThing;

private:
static	CThing		*s_thingLists[];
static	int			s_initialised;

static	CThing		*s_CollisionLists[];
static	sBBox		m_RenderBBox;
static	sBBox		m_ThinkBBox;
public:
// FreeList Stuff
static	void		initFreeList();
static	void		resetFreeList();
static	void		shutdownFreeList();
static	CThing		*GetThing(int Type,int SubType);
static	void		DeleteThing(CThing *Thing);

private:
static	CThing		**s_FreeList[];
static	DVECTOR		MapWH,MapWH16;
};


/*----------------------------------------------------------------------*/
// Base thing class
class CThing
{
public:
	enum TYPE
	{
		TYPE_PICKUP,
		TYPE_PLATFORM,
		TYPE_PLAYER,
		TYPE_PLAYERPROJECTILE,
		TYPE_NPC,
		TYPE_ENEMY,
		TYPE_ENEMYPROJECTILE,
		TYPE_TRIGGER,
		TYPE_HAZARD,
		TYPE_FX,

		MAX_TYPE,
	};
//	TYPE;
				CThing()	
				{
					initDef();
					m_SubType=255;
				}
virtual			~CThing()	{;}

		void	initDef()
				{
					Pos.vx=0; Pos.vy=0;
					PosDelta=Pos;
					PosLast=Pos;
					m_isShuttingDown=false;
					NextFreeThing=0;
				}

virtual	TYPE			getThingType()=0;

		void			setThingSubType(int	T)	{m_SubType=T;}
		int				getThingSubType()		{return(m_SubType);}

virtual	void			init();
virtual	void			shutdown();


virtual	void			think(int _frames);
virtual	void			render();
		void			setToShutdown(bool f=true)								{m_isShuttingDown = f;}
		u8				isSetToShutdown()										{return( m_isShuttingDown);}
virtual int				dontKillDuringLevelRespawn()							{return false;}
		void			calcRenderPos(DVECTOR const &Pos,DVECTOR &renderPos);
		bool			isOnScreen(DVECTOR Pos);
		
// Linkage
		void			addChild(CThing *Child);
		void			removeChild(CThing *Child);
		void			removeAllChild();
		void			deleteAllChild();
		int				getNumChildren()				{return( m_numChildren );}

		DVECTOR const	&getPos()						{return Pos;}
		void			setPos(DVECTOR const &newPos)	{Pos=newPos;}
		DVECTOR	const	&getPosDelta()					{return PosDelta;}
		CThing			*getParent()					{return ParentThing;}
		CThing			*getNext()						{return NextThing;}

virtual void			processEvent(GAME_EVENT _event,CThing *_sourceThing);

protected:
// Pos
		DVECTOR			Pos, PosLast, PosDelta;

// Parent Child Linkage
		CThing			*ParentThing,*NextThing;
		u8				m_numChildren;

public:
// Collision/List Link List
		CThing			*m_nextCollisionThing;
		CThing			*m_nextListThing;

public:
		CThing			*m_NextFreeThing;

// -- Collision --
public:
virtual	CRECT const		*getRenderBBox()							{return &m_collisionArea;}
virtual	CRECT const		*getThinkBBox()								{return &m_collisionArea;}
virtual	bool			alwaysThink()								{return(false);}
virtual	void			leftThinkZone(int _frames)					{}

		void			ShowBBox();
		DVECTOR	const	&getCollisionCentre()						{return m_collisionCentre;}
		DVECTOR const	&getCollisionCentreOffset()					{return m_collisionCentreOffset;}
		int				getCollisionRadius()						{return m_collisionRadius;}
		CRECT const		&getCollisionArea()							{return m_collisionArea;}
		DVECTOR const	&getCollisionSize()							{return m_collisionSize;}

virtual int				canCollide()								{return (true);}
virtual bool			allowOffMap()								{return (false);}
virtual int				checkCollisionAgainst(CThing *_thisThing, int _frames);
		int				checkCollisionAgainstArea(CRECT *_rect);
		void			updateCollisionArea();
virtual void			collidedWith(CThing *_thisThing)			{;}

public:
// Thing states
		void			setRenderFlag(bool f)						{m_renderFlag=f;}
		void			setThinkFlag(bool f)						{m_thinkFlag=f;}
		bool			getThinkFlag()								{return(m_thinkFlag);}
		bool			canRender()									{return (m_renderFlag);}
		DVECTOR			&getRenderPos()								{return(m_RenderPos);}
		bool			canThink()									{return (m_thinkFlag);}

protected:
		DVECTOR			m_RenderPos;
		u8				m_renderFlag,m_thinkFlag;
		u8				m_isShuttingDown;
		u8				m_SubType;

protected:
		void			setCollisionSize(int _w,int _h);
		void			setCollisionCentreOffset(int _x,int _y)			{m_collisionCentreOffset.vx=_x;m_collisionCentreOffset.vy=_y;}
		void			setCollisionCentreOffset(DVECTOR const &xy)		{m_collisionCentreOffset=xy;}

private:
		DVECTOR			m_collisionSize;
		DVECTOR			m_collisionCentreOffset;
		s16				m_collisionRadius;
		CRECT			m_collisionArea;
		DVECTOR			m_collisionCentre;

// Free List Stuff
public:
		CThing			*NextFreeThing;

};

/*---------------------------------------------------------------------- */
/* These are the individual base classes for each of the seperate thing types */
class CPickupThing : public CThing
{
public:
virtual TYPE		getThingType()					{return TYPE_PICKUP;}
};

class CPlayerThing : public CThing
{
public:
	enum
	{ // For Dynamic ThingCache
		MAX_SUBTYPE	=1,
	};
virtual	TYPE		getThingType()					{return TYPE_PLAYER;}
virtual	bool		alwaysThink()					{return(true);}
bool				allowOffMap()					{return(true);}

};

class CPlayerProjectileThing : public CThing
{
public:
virtual TYPE		getThingType()					{return TYPE_PLAYERPROJECTILE;}
};

class CNpcThing : public CThing
{
public:
virtual TYPE		getThingType()					{return TYPE_NPC;}
};

class CEnemyThing : public CThing
{
public:
virtual TYPE		getThingType()					{return TYPE_ENEMY;}
};

class CEnemyProjectileThing : public CThing
{
public:
virtual TYPE		getThingType()					{return TYPE_ENEMYPROJECTILE;}
};

class CPlatformThing : public CThing
{
public:
virtual TYPE		getThingType()					{return TYPE_PLATFORM;}
};

class CTriggerThing : public CThing
{
public:
virtual TYPE		getThingType()					{return TYPE_TRIGGER;}
};

class CHazardThing : public CThing
{
public:
virtual TYPE		getThingType()					{return TYPE_HAZARD;}
};

class CFXThing : public CThing
{
public:
virtual TYPE		getThingType()					{return TYPE_FX;}
};

inline	bool	CheckRect2Rect(CRECT const &Rect0,CRECT const &Rect1)
{
		if (Rect0.x2<Rect1.x1 || Rect0.x1>Rect1.x2) return(false);
		if (Rect0.y2<Rect1.y1 || Rect0.y1>Rect1.y2) return(false);
		return(true);
}

#endif	/* __THING_THING_H__ */


/*===========================================================================

 end */
