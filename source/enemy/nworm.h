/*=========================================================================

	nworm.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NWORM_H__
#define	__ENEMY_NWORM_H__

class CNpcParasiticWormSegment
{
public:
	virtual void		init();
	virtual void		render();
	virtual void		processEnemyCollision( CThing *thisThing );
	virtual void		setScale( u16 scale )							{m_scale = scale;}
	CNpcParasiticWormSegment		*m_nextSegment;
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

protected:
	u16								m_scale;
	CActorGfx						*m_actorGfx;
	DVECTOR							Pos;
	s16								m_heading;
	CRECT							m_collisionArea;
	DVECTOR							m_collisionCentre;
	DVECTOR							m_collisionCentreOffset;
	DVECTOR							m_collisionSize;
	int								m_collisionRadius;
};

class CNpcParasiticWormEnemy : public CNpcEnemy
{
public:
	virtual void		postInit();
	virtual void		shutdown();
	virtual void		render();
	virtual int			checkCollisionAgainst(CThing *_thisThing, int _frames);
protected:
	virtual bool		processSensor();
	virtual void		processClose( int _frames );
	virtual void		processMovement( int _frames );
	//void				resetParasiticWormHeadToTail();
	virtual void		processEnemyCollision( CThing *thisThing );

	enum
	{
		NPC_PARASITIC_WORM_SPACING = 4,
		NPC_PARASITIC_WORM_LENGTH = 10,
	};

	CNpcParasiticWormSegment		*m_segment;
};

#endif