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

#include	"utils\utils.h"

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

CThing			*CThingManager::s_thingLists[CThing::MAX_TYPE]={NULL,NULL};
int				CThingManager::s_initialised=false;


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void		CThingManager::init()
{
	int		i;

	ASSERT(!s_initialised);
	for(i=0;i<CThing::MAX_TYPE;i++)
	{
		s_thingLists[i]=NULL;
	}
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
void		CThingManager::thinkAllThings(int _frames)
{
	int		i;
	CThing	*thing;
	CThing	*thing1,*thing2;

	for(i=0;i<CThing::MAX_TYPE;i++)
	{
		thing=s_thingLists[i];
		while(thing)
		{
			thing->think(_frames);
			thing->updateCollisionArea();
			thing=thing->m_nextThing;
		}
	}

	CPlayer *player = GameScene.getPlayer();

	if ( player )
	{
		player->clearPlatform();
	}

	// Player -> Platform collision
	thing1=s_thingLists[CThing::TYPE_PLATFORM];
	thing2=s_thingLists[CThing::TYPE_PLAYER];

	thing2->setHasPlatformCollided( false );
	thing2->setNewCollidedPos( thing2->getPos() );

	while(thing1&&thing2)
	{
		//if ( !thing1->hasChild( thing2 ) )
		{
			thing1->removeAllChild();

			if(thing1->canCollide()&&
			   thing1->checkCollisionAgainst(thing2, _frames))
			{
				thing1->addChild( thing2 );

				thing1->collidedWith(thing2);
			}
		}
		/*else
		{
			DVECTOR thatPos = thing2->getPos();

			thatPos.vy = thing1->getNewYPos( thing2 );
			thing2->setNewCollidedPos( thatPos );
			thing1->collidedWith(thing2);
		}*/

		thing1=thing1->m_nextThing;
	}

	// Player -> Pickup collision
	thing1=s_thingLists[CThing::TYPE_PICKUP];
	thing2=s_thingLists[CThing::TYPE_PLAYER];
	while(thing1&&thing2)
	{
		if(thing1->canCollide()&&
		   thing1->checkCollisionAgainst(thing2, _frames))
		{
			thing1->collidedWith(thing2);
		}
		thing1=thing1->m_nextThing;
	}

	// Player -> Enemy collision
	thing1=s_thingLists[CThing::TYPE_ENEMY];
	thing2=s_thingLists[CThing::TYPE_PLAYER];
	while(thing1&&thing2)
	{
		if(thing1->canCollide()&&
		   thing1->checkCollisionAgainst(thing2, _frames))
		{
			thing1->collidedWith(thing2);
		}
		thing1=thing1->m_nextThing;
	}

	// Player -> Enemy projectile collision
	thing1=s_thingLists[CThing::TYPE_ENEMYPROJECTILE];
	thing2=s_thingLists[CThing::TYPE_PLAYER];
	while(thing1&&thing2)
	{
		if(thing1->canCollide()&&
		   thing1->checkCollisionAgainst(thing2, _frames))
		{
			thing1->collidedWith(thing2);
		}
		thing1=thing1->m_nextThing;
	}

	// Player -> Trigger collision
	thing1=s_thingLists[CThing::TYPE_TRIGGER];
	thing2=s_thingLists[CThing::TYPE_PLAYER];
	while(thing1&&thing2)
	{
		if(thing1->canCollide()&&
		   thing1->checkCollisionAgainst(thing2, _frames))
		{
			thing1->collidedWith(thing2);
		}
		thing1=thing1->m_nextThing;
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
	int		i;
	CThing	*thing;

	for(i=0;i<CThing::MAX_TYPE;i++)
	{
		thing=s_thingLists[i];
		while(thing)
		{
			thing->render();
			thing=thing->m_nextThing;
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
			thing=thing->m_nextThing;
		}
	}
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void			CThingManager::addToThingList(CThing *_this)
{
	_this->m_nextThing=s_thingLists[_this->getThingType()];
	s_thingLists[_this->getThingType()]=_this;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void			CThingManager::removeFromThingList(CThing *_this)
{
	CThing	*prevThing,*thing;

	prevThing=NULL;
	thing=s_thingLists[_this->getThingType()];
	while(thing!=_this)
	{
		prevThing=thing;
		thing=thing->m_nextThing;
		ASSERT(thing);	// Not in the list!?!?
	}
	if(prevThing)
	{
		prevThing->m_nextThing=_this->m_nextThing;
	}
	else
	{
		s_thingLists[_this->getThingType()]=_this->m_nextThing;
	}
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

	Pos.vx=Pos.vy=10;

	// Add to thing list
	CThingManager::addToThingList(this);

	setCollisionSize(20,20);	// Some temporary defaults.. (pkg)
	setCollisionCentreOffset(0,0);
	m_collisionAngle = 0;
	m_centreCollision = false;
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
int showthings=true;
#include "gfx\prim.h"
#include "level\level.h"
void	CThing::render()
{
	if(showthings)
	{
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
	}
}
#else
void	CThing::render()
{
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
	}
	else
	{
// List does not exist, create
		Next = Child;
		Child->Parent=this;
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
	DVECTOR thatPos = _thisThing->getPos();

	thisRect = getCollisionArea();

	// 'render' collision box at correct angle

	SVECTOR testPointsNonRel[4];
	VECTOR testPoints[4];

	testPointsNonRel[0].vx = thisRect.x1 - Pos.vx;
	testPointsNonRel[0].vy = thisRect.y1 - Pos.vy;

	testPointsNonRel[1].vx = thisRect.x2 - Pos.vx;
	testPointsNonRel[1].vy = thisRect.y1 - Pos.vy;

	testPointsNonRel[2].vx = thisRect.x2 - Pos.vx;
	testPointsNonRel[2].vy = thisRect.y2 - Pos.vy;

	testPointsNonRel[3].vx = thisRect.x1 - Pos.vx;
	testPointsNonRel[3].vy = thisRect.y2 - Pos.vy;

	MATRIX mtx;
	SetIdentNoTrans(&mtx );
	RotMatrixZ( m_collisionAngle, &mtx );

	int i;

	for ( i = 0 ; i < 4 ; i++ )
	{
		ApplyMatrix( &mtx, &testPointsNonRel[i], &testPoints[i] );

		testPoints[i].vx += Pos.vx;
		testPoints[i].vy += Pos.vy;
	}

	// now find the highest y pos

	// first set highestY to lowest of the four points
	
	s16 highestY = testPoints[0].vy;

	for ( i = 1 ; i < 4 ; i++ )
	{
		if ( testPoints[i].vy > highestY ) // remember y is inverted
		{
			highestY = testPoints[i].vy;
		}
	}

	for ( i = 0 ; i < 4 ; i++ )
	{
		int j = i + 1;
		j %= 4;

		VECTOR highestX, lowestX;

		if ( testPoints[i].vx < testPoints[j].vx )
		{
			lowestX = testPoints[i];
			highestX = testPoints[j];
		}
		else
		{
			lowestX = testPoints[j];
			highestX = testPoints[i];
		}

		if ( highestX.vx == lowestX.vx )
		{
			// have to compare heights of both points to get highest

			if ( lowestX.vy < highestY )
			{
				highestY = lowestX.vy;
			}

			if ( highestX.vy < highestY )
			{
				highestY = highestX.vy;
			}
		}
		else
		{
			if ( thatPos.vx >= lowestX.vx && thatPos.vx <= highestX.vx )
			{
				// current position is above or below this line

				s16 testY;

				testY = lowestX.vy + ( ( thatPos.vx - lowestX.vx ) * ( highestX.vy - lowestX.vy ) ) /
							( highestX.vx - lowestX.vx );

				if ( testY < highestY )
				{
					highestY = testY;
				}
			}
		}
	}

	return( highestY );
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

	MATRIX mtx;

	pos=getCollisionCentre();
	thisThingPos=_thisThing->getCollisionCentre();

	radius=getCollisionRadius()+_thisThing->getCollisionRadius();
	collided=false;
	if(abs(pos.vx-thisThingPos.vx)<radius&&
	   abs(pos.vy-thisThingPos.vy)<radius)
	{
		CRECT	thisRect,thatRect;

		thisRect=getCollisionArea();

		// ensure user 'sticks' to platform whilst it is moving along

		thatRect=_thisThing->getCollisionArea();

		// rotate thatPos opposite way to this CThing's collision angle, so that we can regard them both as being at 0 rotation

		// get target thing's position

		DVECTOR thatPos = _thisThing->getPos();

		// get target thing's position relative to this thing's position

		SVECTOR relativePos;
		relativePos.vx = thatPos.vx - Pos.vx;
		relativePos.vy = thatPos.vy - Pos.vy;

		VECTOR newPos;

		// get target thing's collision area relative to 0

		thatRect.x1 -= thatPos.vx;
		thatRect.y1 -= thatPos.vy;
		thatRect.x2 -= thatPos.vx;
		thatRect.y2 -= thatPos.vy;

		SetIdentNoTrans(&mtx );
		RotMatrixZ( -m_collisionAngle, &mtx );

		// rotation target relative position back to 0 by this thing's collision angle

		ApplyMatrix( &mtx, &relativePos, &newPos );

		// add on this thing's position to get new target thing's position after rotation around this thing

		newPos.vx += Pos.vx;
		newPos.vy += Pos.vy;

		// reposition target thing's collision area

		thatRect.x1 += newPos.vx;
		thatRect.y1 += newPos.vy;
		thatRect.x2 += newPos.vx;
		thatRect.y2 += newPos.vy;

		// check to see if bounding boxes collide

		if(((thisRect.x1>=thatRect.x1&&thisRect.x1<=thatRect.x2)||(thisRect.x2>=thatRect.x1&&thisRect.x2<=thatRect.x2)||(thisRect.x1<=thatRect.x1&&thisRect.x2>=thatRect.x2))&&
		   ((thisRect.y1>=thatRect.y1&&thisRect.y1<=thatRect.y2)||(thisRect.y2>=thatRect.y1&&thisRect.y2<=thatRect.y2)||(thisRect.y1<=thatRect.y1&&thisRect.y2>=thatRect.y2)))
		{
			collided=true;

			// check to see if centre point (i.e. where the object is standing) collides too

			if ( ( newPos.vx >= thisRect.x1 && newPos.vx <= thisRect.x2 ) &&
					( newPos.vy >= thisRect.y1 && newPos.vy <= thisRect.y2 ) )
			{
				thatPos.vy = getNewYPos( _thisThing );

				// vertical height change is the sum of the maximums of BOTH objects
				// potentially, one object could be falling down through another object that is moving up

				s32 verticalDelta = abs( _thisThing->getPosDelta().vy ) + abs( this->getPosDelta().vy );

				if ( thatPos.vy - _thisThing->getPos().vy >= -verticalDelta )
				{
					if ( _thisThing->getHasPlatformCollided() )
					{
						// if this has already collided with a platform, check the current platform is
						// (a) within 10 units,
						// (b) higher

						DVECTOR oldCollidedPos = _thisThing->getNewCollidedPos();

						s32 oldY = abs( oldCollidedPos.vy - ( _thisThing->getPos().vy - verticalDelta ) );
						s32 currentY = abs( thatPos.vy - ( _thisThing->getPos().vy - verticalDelta ) );

						//if ( thatPos.vy < oldCollidedPos.vy )
						if ( currentY < oldY )
						{
							_thisThing->setNewCollidedPos( thatPos );
						}
					}
					else
					{
						_thisThing->setHasPlatformCollided( true );

						_thisThing->setCentreCollision( true );

						_thisThing->setNewCollidedPos( thatPos );
					}
				}
				else
				{
					_thisThing->setCentreCollision( false );
				}
			}
			else
			{
				_thisThing->setCentreCollision( false );
			}
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
void	CThing::shove( DVECTOR move )
{
	Pos.vx += move.vx;
	Pos.vy += move.vy;
}

/*===========================================================================
end */
