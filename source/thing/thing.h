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


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

// Thing manager class
class CThingManager
{
public:
	static void		init();
	static void		shutdown();

	static void		thinkAllThings(int _frames);
	static void		renderAllThings();
	static void		processEventAllThings(GAME_EVENT _event,class CThing *_sourceThing);

protected:
	static void					addToThingList(class CThing *_this);
	static void					removeFromThingList(CThing *_this);
	friend class CThing;

private:
	static class CThing			*s_thingLists[];

};


// Base thing class
class CThing
{
public:
	typedef enum
	{
		TYPE_THING,		// temp.. (pkg)
		TYPE_PICKUP,
		TYPE_PLAYER,
		TYPE_ENEMY,

		MAX_TYPE,
	}
	TYPE;

					CThing()	{;}
	virtual			~CThing()	{;}

	virtual TYPE	getThingType()					{return TYPE_THING;}	//=0; (pkg)


	virtual	void	init();
	virtual	void	shutdown();
	virtual	void	think(int _frames);
	virtual	void	render();

// Linkage
	void			addChild(CThing *Child);
	void			removeChild(CThing *Child);
	void			removeAllChild();


	DVECTOR			getPos()						{return Pos;}


	virtual void	processEvent(GAME_EVENT _event,CThing *_sourceThing);


protected:

	
// Linkage
		CThing		*Parent,*Next;

// Pos
		DVECTOR		Pos, PosDelta;

	// Big linked list that contains all CThings ( possibly a temp measure PKG )
//	static CThing	*s_thingList;
public:
		class CThing			*m_nextThing;


		
		

	// -- Collision --
public:
	DVECTOR			getCollisionSize()							{return m_collisionSize;}
	virtual int		checkCollisionAgainst(CThing *_thisThing);
	void			updateCollisionArea();
	virtual void	collidedWith(CThing *_thisThing)	{;}	//=0; (pkg)
protected:
	typedef struct
	{
		int	x1,y1,x2,y2;
	}
	CRECT;

	void			setCollisionSize(int _w,int _h);
	void			setCollisionCentreOffset(int _x,int _y)		{m_collisionCentreOffset.vx=_x;m_collisionCentreOffset.vy=_y;}
	void			setCollisionCentreOffset(DVECTOR xy)		{m_collisionCentreOffset=xy;}
	int				getCollisionRadius()						{return m_collisionRadius;}
	CRECT			getCollisionArea()							{return m_collisionArea;}
	DVECTOR			getCollisionCentre()						{return m_collisionCentre;}
private:
	DVECTOR			m_collisionSize;
	DVECTOR			m_collisionCentreOffset;
	int				m_collisionRadius;
	CRECT			m_collisionArea;
	DVECTOR			m_collisionCentre;

};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __THING_THING_H__ */

/*===========================================================================
 end */
