/*=========================================================================

	thing.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "thing\thing.h"

#ifndef	__PLAYER_PLAYER_H__
#include "player\player.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

#ifndef	__UTILS_HEADER__
#include	"utils\utils.h"
#endif

#include "level\level.h"


/*	Std Lib
	------- */

/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

static int	s_RenderBBoxX0=0;
static int	s_RenderBBoxX1=512;
static int	s_RenderBBoxY0=0;
static int	s_RenderBBoxY1=256;
static int	s_ThinkBBoxX0=0-256;
static int	s_ThinkBBoxX1=512+526;
static int	s_ThinkBBoxY0=0-128;
static int	s_ThinkBBoxY1=256+128;


CThing			*CThingManager::s_thingLists[CThing::MAX_TYPE];//={NULL,NULL};
CThing			*CThingManager::s_CollisionLists[CThing::MAX_TYPE];
int				CThingManager::s_initialised=false;

sBBox			CThingManager::m_RenderBBox;
sBBox			CThingManager::m_ThinkBBox;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void		CThingManager::init()
{
	ASSERT(!s_initialised);
	initList(s_thingLists);
	initList(s_CollisionLists);
	s_initialised=true;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void		CThingManager::shutdown()
{
	int		i;
	CThing	*thing;

	ASSERT(s_initialised);
	for(i=0;i<CThing::MAX_TYPE;i++)
	{
		while(s_thingLists[i])
		{
			thing=s_thingLists[i];
			thing->shutdown();
			delete thing;
		}
	}
	s_initialised=false;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void		CThingManager::initList(CThing **List)
{
int		i;

		for(i=0 ;i<CThing::MAX_TYPE; i++)
		{
			List[i]=NULL;
		}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:	Kills every CThing except the player
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void		CThingManager::killAllThingsForRespawn()
{
	int		i;
	CThing	*thing;

	ASSERT(s_initialised);
	for(i=0;i<CThing::MAX_TYPE;i++)
	{
		if(i!=CThing::TYPE_PLAYER)
		{
			while(s_thingLists[i])
			{
				thing=s_thingLists[i];
				thing->shutdown();
				delete thing;
			}
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void		CThingManager::thinkAllThings(int _frames)
{
// Setup Screen BBox's
DVECTOR	const	&CamPos=CLevel::getCameraPos();

	m_ThinkBBox.XMin=s_ThinkBBoxX0+CamPos.vx;
	m_ThinkBBox.XMax=s_ThinkBBoxX1+CamPos.vx;
	m_ThinkBBox.YMin=s_ThinkBBoxY0+CamPos.vy;
	m_ThinkBBox.YMax=s_ThinkBBoxY1+CamPos.vy;

	int		i;
	CThing	*thing;
	CThing	*thing1,*thing2,*playerThing;
	initList(s_CollisionLists);

	for(i=0;i<CThing::MAX_TYPE;i++)
	{
		thing=s_thingLists[i];
		while(thing)
		{
			// Check If in Thinkable range
			CRECT	const	&ThingRect= thing->getCollisionArea();
			bool	Flag=true;
			// Will speed this up
			
			if (ThingRect.x2<m_ThinkBBox.XMin || ThingRect.x1>m_ThinkBBox.XMax) Flag=false;
			if (ThingRect.y2<m_ThinkBBox.YMin || ThingRect.y1>m_ThinkBBox.YMax) Flag=false;
			thing->setThinkFlag(Flag);

			if (Flag)
			{
				thing->think(_frames);
				thing->updateCollisionArea();
				if (thing->canCollide())
				{
				CThingManager::addToCollisionList(thing);
				}
			}
			thing=thing->m_nextListThing;
		}
	}

	CPlayer *player = GameScene.getPlayer();

	if ( player )
	{
		player->clearPlatform();
	}

	playerThing=s_CollisionLists[CThing::TYPE_PLAYER];

	if (playerThing)
	{
		playerThing->setHasPlatformCollided( false );
		playerThing->setNewCollidedPos( playerThing->getPos() );

		// Player -> Platform collision
		thing1=s_CollisionLists[CThing::TYPE_PLATFORM];
		while(thing1)
		{
			if(thing1->checkCollisionAgainst(playerThing, _frames))
			{
				thing1->collidedWith(playerThing);
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Player -> Pickup collision
		thing1=s_CollisionLists[CThing::TYPE_PICKUP];
		while(thing1)
		{
			if(thing1->checkCollisionAgainst(playerThing, _frames))
			{
				thing1->collidedWith(playerThing);
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Player -> Enemy collision
		thing1=s_CollisionLists[CThing::TYPE_ENEMY];
		while(thing1)
		{
			if(thing1->checkCollisionAgainst(playerThing, _frames))
			{
				thing1->collidedWith(playerThing);
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Player -> Friend collision
		thing1=s_CollisionLists[CThing::TYPE_NPC];
		while(thing1)
		{
			if(thing1->checkCollisionAgainst(playerThing, _frames))
			{
				thing1->collidedWith(playerThing);
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Player -> Hazard collision
		thing1=s_CollisionLists[CThing::TYPE_HAZARD];
		while(thing1)
		{
		if(thing1->checkCollisionAgainst(playerThing, _frames))
			{
				thing1->collidedWith(playerThing);
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Player -> Enemy projectile collision
		thing1=s_CollisionLists[CThing::TYPE_ENEMYPROJECTILE];
		while(thing1)
		{
			if(thing1->checkCollisionAgainst(playerThing, _frames))
			{
				thing1->collidedWith(playerThing);
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Player -> Trigger collision
		thing1=s_CollisionLists[CThing::TYPE_TRIGGER];
		while(thing1)
		{
			if(thing1->checkCollisionAgainst(playerThing, _frames))
			{
				thing1->collidedWith(playerThing);
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Enemy -> Player projectile collision
		thing1=s_CollisionLists[CThing::TYPE_PLAYERPROJECTILE];
		thing2=s_CollisionLists[CThing::TYPE_ENEMY];
		while(thing1)
		{
			while(thing2)
			{
				if(thing1->checkCollisionAgainst(thing2, _frames))
				{
					thing1->collidedWith(thing2);
				}
				thing2=thing2->m_nextCollisionThing;
			}
			thing1=thing1->m_nextCollisionThing;
		}

		// Enemy -> Enemy collision
		thing1=s_CollisionLists[CThing::TYPE_ENEMY];
		while(thing1)
		{
			thing2=thing1->m_nextCollisionThing;//s_CollisionLists[CThing::TYPE_ENEMY];

			while(thing2)
			{
				ASSERT(thing1 != thing2 );
//				if ( thing1 != thing2 )
				{
					if (thing1->checkCollisionAgainst( thing2, _frames ) )
					{
						thing1->collidedWith( thing2 );
						//thing2->collidedWith( thing1 );
					}
				}
				thing2 = thing2->m_nextCollisionThing;
			}
			thing1 = thing1->m_nextCollisionThing;
		}

		// Hazard -> Platform collision
		thing1=s_CollisionLists[CThing::TYPE_PLATFORM];
		while(thing1)
		{
			thing2=s_CollisionLists[CThing::TYPE_HAZARD];

			while(thing2)
			{
				if ( thing1 != thing2 )
				{
					if (thing1->checkCollisionAgainst( thing2, _frames ) )
					{
						thing1->collidedWith( thing2 );
						//thing2->collidedWith( thing1 );
					}
				}

				thing2 = thing2->m_nextCollisionThing;
			}

			thing1 = thing1->m_nextCollisionThing;
		}
	}
// Shut emm down, sh sh shut em down, we shutem down
	for(i=0;i<CThing::MAX_TYPE;i++)
	{
		thing=s_CollisionLists[i];
		CThing	*nextThing = thing;
		while(thing)
		{
			nextThing=thing->m_nextListThing;

			if ( thing->isSetToShutdown() )
			{
				thing->shutdown();
				delete thing;
			}

			thing = nextThing;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void		CThingManager::renderAllThings()
{
// Setup Screen BBox's
DVECTOR	const	&CamPos=CLevel::getCameraPos();

	m_RenderBBox.XMin=s_RenderBBoxX0+CamPos.vx;
	m_RenderBBox.XMax=s_RenderBBoxX1+CamPos.vx;
	m_RenderBBox.YMin=s_RenderBBoxY0+CamPos.vy;
	m_RenderBBox.YMax=s_RenderBBoxY1+CamPos.vy;


	for(int i=0;i<CThing::MAX_TYPE;i++)
	{
		CThing	*thing=s_thingLists[i];
		while(thing)
		{
			thing->render();
			thing=thing->m_nextListThing;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void		CThingManager::processEventAllThings(GAME_EVENT _event,CThing *_sourceThing)
{
	int		i;
	CThing	*thing;

	for(i=0;i<CThing::MAX_TYPE;i++)
	{
		thing=s_thingLists[i];
		while(thing)
		{
			thing->processEvent(_event,_sourceThing);
			thing=thing->m_nextListThing;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:	Searches through a list of things to check for collision against an area.
				The first time this is called, _continue should be false. If no colliding things are found then
				NULL will be returned. If a colliding thing is found then it's address gets returned. To continue
				searching through the list for the next colliding thing, call the function again with _continue set
				to true.
				NB: This function could probly cause weird bugs if not used properly! BE AWARE!
	Params:		*_area		Area to check against
				_type		Type of thing to search for
				_continue	If false then the list is searched from the start, if true then the search continues
							from the last thing that was found ( um.. see above )
	Returns:
  ---------------------------------------------------------------------- */
CThing		*CThingManager::checkCollisionAreaAgainstThings(CRECT *_area,int _type,int _continue)
{
	static CThing	*thing=NULL;

	ASSERT(_type<CThing::MAX_TYPE);

	if(_continue)
	{
		ASSERT(thing);
		thing=thing->m_nextListThing;
	}
	else
	{
		thing=s_thingLists[_type];
	}
	while(thing)
	{
		if(thing->canCollide() && thing->checkCollisionAgainstArea(_area))
		{
			return thing;
		}
		thing=thing->m_nextListThing;
	}

	return NULL;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThingManager::addToThingList(CThing *_this)
{
int	Type=_this->getThingType();
	_this->m_nextListThing=s_thingLists[Type];
	s_thingLists[Type]=_this;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThingManager::removeFromThingList(CThing *_this)
{
	CThing	*prevThing,*thing;

	prevThing=NULL;
	thing=s_thingLists[_this->getThingType()];
	while(thing!=_this)
	{
		prevThing=thing;
		thing=thing->m_nextListThing;
		ASSERT(thing);	// Not in the list!?!?
	}
	if(prevThing)
	{
		prevThing->m_nextListThing=_this->m_nextListThing;
	}
	else
	{
		s_thingLists[_this->getThingType()]=_this->m_nextListThing;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThingManager::addToCollisionList(CThing *_this)
{
int	Type=_this->getThingType();

		_this->m_nextCollisionThing=s_CollisionLists[Type];
		s_CollisionLists[Type]=_this;

}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::init()
{
	Parent=NULL;
	Next=NULL;
	m_numChildren = 0;

	Pos.vx=Pos.vy=10;

	// Add to thing list
	CThingManager::addToThingList(this);

	setCollisionSize(20,20);	// Some temporary defaults.. (pkg)
	setCollisionCentreOffset(0,0);
	setCollisionAngle(0);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::shutdown()
{
	if (Parent) 
	{ // Is child
		Parent->removeChild(this);
	}
	else
	{ // Is Parent
		removeAllChild();
	}

	// Remove from thing list
	CThingManager::removeFromThingList(this);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::think(int _frames)
{
	PosDelta.vx=Pos.vx-PosLast.vx; 
	PosDelta.vy=Pos.vy-PosLast.vy;
	PosLast=Pos;


}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
#if defined (__USER_paul__) || defined (__USER_charles__)
#define	SHOW_BBOX	1
int showthings=true;
#endif

#if defined (__USER_daveo__)
#define	SHOW_BBOX	1
int showthings=false;
#endif

void	CThing::render()
{
// Check Is Onscreen
CRECT	const	&ThingRect= getCollisionArea();
sBBox			&ScrBBox=CThingManager::getRenderBBox();
DVECTOR	const	&CamPos=CLevel::getCameraPos();

		m_RenderPos.vx = Pos.vx - CamPos.vx;
		m_RenderPos.vy = Pos.vy - CamPos.vy;

// Will speed this up
		m_renderFlag=true;
		if (ThingRect.x2<ScrBBox.XMin || ThingRect.x1>ScrBBox.XMax) m_renderFlag=false;
		if (ThingRect.y2<ScrBBox.YMin || ThingRect.y1>ScrBBox.YMax) m_renderFlag=false;

/***/
#ifdef	SHOW_BBOX
	if(showthings) ShowBBox();
#endif

}
/****************************************************************************************/
#ifdef	SHOW_BBOX
#include "gfx\prim.h"

void	CThing::ShowBBox()
{
	if(showthings)
	{
		DVECTOR	const	&ofs=CLevel::getCameraPos();
		CRECT	area;

		area=getCollisionArea();
		area.x1-=ofs.vx;
		area.y1-=ofs.vy;
		area.x2-=ofs.vx;
		area.y2-=ofs.vy;

		if(area.x1<=511&&area.x2>=0 && area.y1<=255&&area.y2>=0)
		{
			DrawLine(area.x1,area.y1,area.x2,area.y1,255,255,255,0);
			DrawLine(area.x2,area.y1,area.x2,area.y2,255,255,255,0);
			DrawLine(area.x2,area.y2,area.x1,area.y2,255,255,255,0);
			DrawLine(area.x1,area.y2,area.x1,area.y1,255,255,255,0);

			area.x1=Pos.vx-10-ofs.vx;
			area.y1=Pos.vy-10-ofs.vy;
			area.x2=Pos.vx+10-ofs.vx;
			area.y2=Pos.vy+10-ofs.vy;
			DrawLine(area.x1,area.y1,area.x2,area.y2,255,0,0,0);
			DrawLine(area.x2,area.y1,area.x1,area.y2,255,0,0,0);
		}
/*
		DVECTOR	ofs;
		CRECT	area;

		ofs=CLevel::getCameraPos();

		area=getCollisionArea();
		area.x1-=ofs.vx;
		area.y1-=ofs.vy;
		area.x2-=ofs.vx;
		area.y2-=ofs.vy;

		if(area.x1<=511&&area.x2>=0&&
		   area.y1<=255&&area.y2>=0)
		{
			area=getCollisionArea();

			SVECTOR points[4];
			VECTOR vecPoints[4];

			points[0].vx = area.x1 - Pos.vx;
			points[0].vy = area.y1 - Pos.vy;

			points[1].vx = area.x1 - Pos.vx;
			points[1].vy = area.y2 - Pos.vy;

			points[2].vx = area.x2 - Pos.vx;
			points[2].vy = area.y2 - Pos.vy;

			points[3].vx = area.x2 - Pos.vx;
			points[3].vy = area.y1 - Pos.vy;

			MATRIX mtx;

			SetIdentNoTrans(&mtx );
			RotMatrixZ( m_collisionAngle, &mtx );

			ApplyMatrix( &mtx, &points[0], &vecPoints[0] );
			ApplyMatrix( &mtx, &points[1], &vecPoints[1] );
			ApplyMatrix( &mtx, &points[2], &vecPoints[2] );
			ApplyMatrix( &mtx, &points[3], &vecPoints[3] );

			vecPoints[0].vx += Pos.vx - ofs.vx;
			vecPoints[0].vy += Pos.vy - ofs.vy;

			vecPoints[1].vx += Pos.vx - ofs.vx;
			vecPoints[1].vy += Pos.vy - ofs.vy;

			vecPoints[2].vx += Pos.vx - ofs.vx;
			vecPoints[2].vy += Pos.vy - ofs.vy;

			vecPoints[3].vx += Pos.vx - ofs.vx;
			vecPoints[3].vy += Pos.vy - ofs.vy;

			//DrawLine(area.x1,area.y1,area.x1,area.y2,255,255,255,0);
			//DrawLine(area.x1,area.y2,area.x2,area.y2,255,255,255,0);
			//DrawLine(area.x2,area.y2,area.x2,area.y1,255,255,255,0);
			//DrawLine(area.x2,area.y1,area.x1,area.y1,255,255,255,0);

			DrawLine( vecPoints[0].vx, vecPoints[0].vy, vecPoints[1].vx, vecPoints[1].vy,255,255,255,0);
			DrawLine( vecPoints[1].vx, vecPoints[1].vy, vecPoints[2].vx, vecPoints[2].vy,255,255,255,0);
			DrawLine( vecPoints[2].vx, vecPoints[2].vy, vecPoints[3].vx, vecPoints[3].vy,255,255,255,0);
			DrawLine( vecPoints[3].vx, vecPoints[3].vy, vecPoints[0].vx, vecPoints[0].vy,255,255,255,0);

			area.x1=Pos.vx-10-ofs.vx;
			area.y1=Pos.vy-10-ofs.vy;
			area.x2=Pos.vx+10-ofs.vx;
			area.y2=Pos.vy+10-ofs.vy;
			DrawLine(area.x1,area.y1,area.x2,area.y2,255,0,0,0);
			DrawLine(area.x2,area.y1,area.x1,area.y2,255,0,0,0);
		}
*/
	}
}
#endif

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::addChild(CThing *Child)
{
CThing	*List=Next;

	if ( List )
	{
// Find end of list
		while (List->Next) 
		{
			List=List->Next;
		}
		List->Next=Child;
		Child->Parent=this;
		m_numChildren++;
	}
	else
	{
// List does not exist, create
		Next = Child;
		Child->Parent=this;
		m_numChildren++;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::removeChild(CThing *Child)
{
CThing	*List=Next;
CThing	*Last=NULL;

// Find Child
		while ( List != Child && List->Next )
		{
			Last = List;
			List = List->Next;
		}

// if there are no more links to continue down, the current 'List' must either be the correct item, or the item does not
// exist in the list at all

		ASSERT( List == Child );

		if ( Last )
		{
			Last->Next = List->Next;
		}
		else
		{
			this->Next = List->Next;
		}

		m_numChildren--;

		Child->Parent=NULL;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::removeAllChild()
{
CThing	*List=Next;

		while (List)
		{
			CThing	*Next=List->Next;
			List->Parent=NULL;
			List->Next=NULL;
			List=Next;
		}
		Next=NULL;

		m_numChildren = 0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::deleteAllChild()
{
CThing	*List=Next;

		while (List)
		{
			CThing	*Next=List->Next;
			List->Parent=NULL;
			List->Next=NULL;
			List->shutdown();
			delete List;
			List=Next;
		}
		Next=NULL;

		m_numChildren = 0;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CThing::getNumChildren()
{
	return( m_numChildren );
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
bool	CThing::hasChild(CThing *Child)
{
	CThing *nextChild = Next;

	while( nextChild )
	{
		if ( nextChild == Child )
		{
			return( true );
		}

		nextChild = nextChild->Next;
	}

	return( false );
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::setCollisionSize(int _w,int _h)
{
	m_collisionSize.vx=_w;
	m_collisionSize.vy=_h;
	if(m_collisionSize.vx>m_collisionSize.vy)
	{
		m_collisionRadius=m_collisionSize.vx;
	}
	else
	{
		m_collisionRadius=m_collisionSize.vy;
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::updateCollisionArea()
{
	m_collisionCentre.vx=Pos.vx+m_collisionCentreOffset.vx;
	m_collisionCentre.vy=Pos.vy+m_collisionCentreOffset.vy;
	m_collisionArea.x1=m_collisionCentre.vx-(m_collisionSize.vx/2);
	m_collisionArea.x2=m_collisionArea.x1+m_collisionSize.vx;
	m_collisionArea.y1=m_collisionCentre.vy-(m_collisionSize.vy/2);
	m_collisionArea.y2=m_collisionArea.y1+m_collisionSize.vy;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

s32		CThing::getNewYPos(CThing *_thisThing)
{
	CRECT	thisRect;
	thisRect = getCollisionArea();

	if ( thisRect.y1 < thisRect.y2 )
	{
		return( thisRect.y1 );
	}
	else
	{
		return( thisRect.y2 );
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CThing::checkCollisionAgainst(CThing *_thisThing, int _frames)
{
	DVECTOR	pos,thisThingPos;
	int		radius;
	int		collided;

	pos=getCollisionCentre();
	thisThingPos=_thisThing->getCollisionCentre();

	radius=getCollisionRadius()+_thisThing->getCollisionRadius();
	collided=false;
	if(abs(pos.vx-thisThingPos.vx)<radius&&
	   abs(pos.vy-thisThingPos.vy)<radius)
	{
		CRECT	thisRect,thatRect;

		thisRect=getCollisionArea();

		thatRect=_thisThing->getCollisionArea();

		if(((thisRect.x1>=thatRect.x1&&thisRect.x1<=thatRect.x2)||(thisRect.x2>=thatRect.x1&&thisRect.x2<=thatRect.x2)||(thisRect.x1<=thatRect.x1&&thisRect.x2>=thatRect.x2))&&
		   ((thisRect.y1>=thatRect.y1&&thisRect.y1<=thatRect.y2)||(thisRect.y2>=thatRect.y1&&thisRect.y2<=thatRect.y2)||(thisRect.y1<=thatRect.y1&&thisRect.y2>=thatRect.y2)))
		{
			collided=true;
		}
	}

	return collided;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CThing::checkCollisionAgainstArea(CRECT *_rect)
{
	CRECT	thisRect;
	int		ret;

	thisRect=getCollisionArea();
	ret=false;

	if(((thisRect.x1>=_rect->x1&&thisRect.x1<=_rect->x2)||(thisRect.x2>=_rect->x1&&thisRect.x2<=_rect->x2)||(thisRect.x1<=_rect->x1&&thisRect.x2>=_rect->x2))&&
	   ((thisRect.y1>=_rect->y1&&thisRect.y1<=_rect->y2)||(thisRect.y2>=_rect->y1&&thisRect.y2<=_rect->y2)||(thisRect.y1<=_rect->y1&&thisRect.y2>=_rect->y2)))
	{
		ret=true;
	}

	return ret;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CThing::processEvent(GAME_EVENT _event,CThing *_sourceThing)
{
	// do nothing by default - ignore event
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CTriggerThing::setPositionAndSize(int _x,int _y,int _w,int _h)
{
	Pos.vx=_x+(_w/2);
	Pos.vy=_y+(_h/2);
	setCollisionSize(_w,_h);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CTriggerThing::setTargetBox(int _x,int _y,int _w,int _h)
{
	m_boxX1=_x;
	m_boxY1=_y;
	m_boxX2=_x+_w;
	m_boxY2=_y+_h;
}

/*===========================================================================
end */

