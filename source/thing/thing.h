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
	static void		init();
	static void		shutdown();
	static void		killAllThingsForRespawn();

	static void		initAllThings();
	static void		thinkAllThings(int _frames);
	static void		renderAllThings();
	static void		processEventAllThings(GAME_EVENT _event,CThing *_sourceThing);

	static CThing*	checkCollisionAreaAgainstThings(CRECT *_area,int _type,int _continue);
	static void		initCollision();
	static void		matchWheelsAndWeights();
	static void		matchPressureSwitches();

	static sBBox	&getRenderBBox()			{return(m_RenderBBox);}
	static sBBox	&getThinkBBox()				{return(m_ThinkBBox);}

protected:
	static void		initList(CThing **List);
	static void		addToThingList(CThing *_this);
	static void		removeFromThingList(CThing *_this);

	static void		addToCollisionList(CThing *_this);

friend class CThing;

private:
	static CThing			*s_thingLists[];
	static int				s_initialised;

	static CThing			*s_CollisionLists[];
	static sBBox			m_RenderBBox;
	static sBBox			m_ThinkBBox;
public:
// FreeList Stuff
	static void		initFreeList();
	static void		resetFreeList();
	static void		shutdownFreeList();
	static	CThing	*GetThing(int Type,int SubType);
	static	void	DeleteThing(CThing *Thing);

private:
	static CThing			**s_FreeList[];
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
					m_SubType=1234;
				}
virtual			~CThing()	{;}

virtual	void	initDef()
				{
					m_isShuttingDown=false;
					NextFreeThing=0;
				}

virtual	TYPE	getThingType()=0;

virtual	void	setThingSubType(int	T)	{m_SubType=T;}
virtual	int		getThingSubType()		{return(m_SubType);}

virtual	void	init();
virtual	void	shutdown();
virtual	void	think(int _frames);
virtual	void	render();
		void	setToShutdown(bool f=true)								{m_isShuttingDown = f;}
		u8		isSetToShutdown()										{return( m_isShuttingDown);}
virtual int		dontKillDuringLevelRespawn()							{return false;}

// Linkage
		void			addChild(CThing *Child);
		void			removeChild(CThing *Child);
		void			removeAllChild();
		void			deleteAllChild();
		bool			hasChild(CThing *Child);
		int				getNumChildren();


		DVECTOR const	&getPos()						{return Pos;}
		void			setPos(DVECTOR newPos)			{Pos=newPos;}
		DVECTOR			getPosDelta()					{return PosDelta;}
		CThing			*getParent()					{return ParentThing;}
		CThing			*getNext()						{return NextThing;}

virtual void			processEvent(GAME_EVENT _event,CThing *_sourceThing);

protected:
// Pos
		DVECTOR			Pos, PosLast, PosDelta;

// Parent Child Linkage
		CThing			*ParentThing,*NextThing;
		int				m_numChildren;

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
virtual	void			leftThingZone(int _frames)					{}
virtual	void			enterThingZone(int _frames)					{}

		void			ShowBBox();
		DVECTOR	const	&getCollisionCentre()						{return m_collisionCentre;}
		DVECTOR const	&getCollisionCentreOffset()					{return m_collisionCentreOffset;}
		int				getCollisionRadius()						{return m_collisionRadius;}
virtual	CRECT const		&getCollisionArea()							{return m_collisionArea;}
		DVECTOR const	&getCollisionSize()							{return m_collisionSize;}

virtual int				canCollide()								{return true;}
virtual int				checkCollisionAgainst(CThing *_thisThing, int _frames);
		int				checkCollisionAgainstArea(CRECT *_rect);
		void			updateCollisionArea();
virtual void			collidedWith(CThing *_thisThing)			{;}
virtual void			setHasPlatformCollided( bool newVal )		{;}
virtual bool			getHasPlatformCollided()					{return false;}
virtual s32				getNewYPos( CThing *_thisThing );

public:
// Thing states
		void			setRenderFlag(bool f)						{m_renderFlag=f;}
		void			setThinkFlag(bool f)						{m_thinkFlag=f;}
		bool			getThinkFlag()								{return(m_thinkFlag);}
		bool			canRender()									{return (m_renderFlag);}
		DVECTOR			&getRenderPos()								{return(m_RenderPos);}
		bool			canThink()									{return (m_thinkFlag);}
protected:
		bool			m_renderFlag,m_thinkFlag;
		DVECTOR			m_RenderPos;
		bool			m_isShuttingDown;
		int				m_SubType;

protected:
virtual void			setCollisionSize(int _w,int _h);
virtual void			setCollisionCentreOffset(int _x,int _y)		{m_collisionCentreOffset.vx=_x;m_collisionCentreOffset.vy=_y;}
virtual void			setCollisionCentreOffset(DVECTOR xy)		{m_collisionCentreOffset=xy;}

private:
		DVECTOR			m_collisionSize;
		DVECTOR			m_collisionCentreOffset;
		int				m_collisionRadius;
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
	enum
	{ // For Dynamic ThingCache
		MAX_SUBTYPE	=1,
	};

virtual TYPE		getThingType()					{return TYPE_TRIGGER;}
virtual void		setPositionAndSize(int _x,int _y,int _w,int _h);	// Wonder if this might be better in CThing? (pkg)
virtual void		setTargetBox(int _x,int _y,int _w,int _h);			// Wonder if this might be better in CThing? (pkg)

protected:
		int			m_boxX1,m_boxY1,m_boxX2,m_boxY2;
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

#endif	/* __THING_THING_H__ */

/*===========================================================================
 end */
