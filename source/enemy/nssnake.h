/*=========================================================================

	nssnake.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NSSNAKE_H__
#define	__ENEMY_NSSNAKE_H__

#ifndef __ENEMY_NBOSS_H__
#include "enemy\nboss.h"
#endif


class CNpcSeaSnakeSegment
{
public:
	void				init();
	void				shutdown();
	void				render();
	void				processEnemyCollision( CThing *thisThing );
	void				setScale( u16 scale )							{m_scale = scale;}
	CNpcSeaSnakeSegment	*m_nextSegment;
	void				setPos( DVECTOR const &newPos )					{Pos = newPos;}
	DVECTOR	const 		&getPos()										{return( Pos );}
	void				setHeading( s16 newHeading )					{m_heading = newHeading;}
	void				updateCollisionArea();
	int					checkCollisionAgainst(CThing *_thisThing, int _frames);
	void				setCollisionSize(int _w,int _h);
	void				setCollisionCentreOffset(int _x,int _y)		{m_collisionCentreOffset.vx=_x;m_collisionCentreOffset.vy=_y;}
	CRECT const			&getCollisionArea()							{return m_collisionArea;}
	DVECTOR	const		&getCollisionCentre()						{return m_collisionCentre;}
	int					getCollisionRadius()						{return m_collisionRadius;}
	void				setAnim( int newAnim )						{m_anim = newAnim;}

protected:
	u16								m_scale;
	CActorGfx						*m_actorGfx;
	int								m_anim;
	DVECTOR							Pos;
	s16								m_heading;
	CRECT							m_collisionArea;
	DVECTOR							m_collisionCentre;
	DVECTOR							m_collisionCentreOffset;
	DVECTOR							m_collisionSize;
	int								m_collisionRadius;
};

class CNpcSeaSnakeEnemy : public CNpcBossEnemy
{
public:
	void				postInit();
	void				shutdown();
	void				render();
	virtual void		renderOnMapScreen( DVECTOR drawPos );
	int					checkCollisionAgainst(CThing *_thisThing, int _frames);
protected:
	s32					getFrameShift( int _frames );
	bool				processSensor();
	void				processClose( int _frames );
	void				processFrameClose();
	void				processMovement( int _frames );
	void				processFrameMovement();
	void				processShot( int _frames );
	void				processEnemyCollision( CThing *thisThing );
	void				processUserCollision( CThing *thisThing );
	u8					processPathMove( int _frames, s32 *moveX, s32 *moveY, s32 *moveVel, s32 *moveDist );
	u8					isSnakeStopped();
	void				moveEntireSnake( DVECTOR const &newPos );
	void				addHealthMeter();
	void				updateTail( DVECTOR &oldPos, int _frames );
	void				collidedWith(CThing *_thisThing);

	enum
	{
		NPC_SEA_SNAKE_SPACING = 7,
		NPC_SEA_SNAKE_LENGTH = 9,
		NPC_SEA_SNAKE_CIRCLE_CLOCKWISE = 1,
		NPC_SEA_SNAKE_CIRCLE_ANTICLOCKWISE = 2,
	};

	enum SEA_SNAKE_STATES
	{
		SEA_SNAKE_VERTICAL_LINEUP = 0,
		SEA_SNAKE_CHARGE = 1,
	};

	// position history stuff

	class CNpcPositionHistory
	{
	public:
		DVECTOR						pos;
		CNpcPositionHistory			*next;
		CNpcPositionHistory			*prev;
	};

	u8								m_segmentCount;
	CNpcSeaSnakeSegment				m_segmentArray[NPC_SEA_SNAKE_LENGTH];

	CNpcPositionHistory				*m_positionHistory;
	CNpcPositionHistory				m_positionHistoryArray[NPC_SEA_SNAKE_SPACING * NPC_SEA_SNAKE_LENGTH * 2];

	s32								m_collTimer;
	s32								m_snapTimer;
	//s32								m_openTimer;
	u8								m_turnDir;
	s16								m_circleHeading;
	s16								m_origHeading;
	s32								m_waitTimer;
	u8								m_collCount;
	s8								m_sign;
};

#endif