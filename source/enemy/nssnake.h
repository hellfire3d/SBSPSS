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

class CNpcSeaSnakeSegment
{
public:
	virtual void		init();
	virtual void		shutdown();
	virtual void		render();
	virtual void		processEnemyCollision( CThing *thisThing );
	virtual void		setScale( u16 scale )							{m_scale = scale;}
	CNpcSeaSnakeSegment	*m_nextSegment;
	virtual void		setPos( DVECTOR newPos )						{Pos = newPos;}
	virtual DVECTOR		getPos()										{return( Pos );}
	virtual void		setHeading( s16 newHeading )					{m_heading = newHeading;}
	void				updateCollisionArea();
	virtual int			checkCollisionAgainst(CThing *_thisThing, int _frames);
	virtual void		setCollisionSize(int _w,int _h);
	virtual void		setCollisionCentreOffset(int _x,int _y)		{m_collisionCentreOffset.vx=_x;m_collisionCentreOffset.vy=_y;}
	virtual	CRECT const	&getCollisionArea()							{return m_collisionArea;}
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

class CNpcSeaSnakeEnemy : public CNpcEnemy
{
public:
	virtual void		postInit();
	virtual void		shutdown();
	virtual void		render();
	virtual int			checkCollisionAgainst(CThing *_thisThing, int _frames);
	//virtual	CRECT const		*getThinkBBox();
protected:
	virtual s32			getFrameShift( int _frames );
	virtual bool		processSensor();
	virtual void		processClose( int _frames );
	virtual void		processMovement( int _frames );
	virtual void		processShot( int _frames );
	virtual void		processEnemyCollision( CThing *thisThing );
	virtual void		processUserCollision( CThing *thisThing );
	u8					isSnakeStopped();
	void				moveEntireSnake( DVECTOR newPos );

	enum
	{
		NPC_SEA_SNAKE_SPACING = 6,
		NPC_SEA_SNAKE_LENGTH = 10,
		NPC_SEA_SNAKE_CIRCLE_CLOCKWISE = 1,
		NPC_SEA_SNAKE_CIRCLE_ANTICLOCKWISE = 2,
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
	CNpcPositionHistory				m_positionHistoryArray[NPC_SEA_SNAKE_SPACING * NPC_SEA_SNAKE_LENGTH];
	s32								m_collTimer;
	s32								m_snapTimer;
	//s32								m_openTimer;
	bool							m_meterOn;
	u8								m_turnDir;
	s16								m_circleHeading;
	s16								m_origHeading;
};

#endif