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
	void				init();
	void				shutdown();
	void				render();
	void				processEnemyCollision( CThing *thisThing );
	void				setScale( u16 scale )							{m_scale = scale;}
	CNpcParasiticWormSegment		*m_nextSegment;
	void				setPos( DVECTOR const &newPos )					{Pos = newPos;}
	DVECTOR	const		&getPos()										{return( Pos );}
	void				setHeading( s16 newHeading )					{m_heading = newHeading;}
	void				updateCollisionArea();
	int					checkCollisionAgainst(CThing *_thisThing, int _frames);
	void				setCollisionSize(int _w,int _h);
	void				setCollisionCentreOffset(int _x,int _y)		{m_collisionCentreOffset.vx=_x;m_collisionCentreOffset.vy=_y;}
	CRECT const			&getCollisionArea()							{return m_collisionArea;}
	DVECTOR	const		&getCollisionCentre()						{return m_collisionCentre;}
	int					getCollisionRadius()						{return m_collisionRadius;}
	void				setAnim( int newAnim )						{m_animNo = newAnim;}

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
	int								m_animNo;
};

class CNpcParasiticWormEnemy : public CNpcEnemy
{
public:
	void				postInit();
	void				shutdown();
	void				render();
	int					checkCollisionAgainst(CThing *_thisThing, int _frames);
	bool				allowOffMap()								{return (true);}

protected:
	bool				processSensor();
	void				processClose( int _frames );
	void				processMovement( int _frames );
	void				processShotDeathEnd( int _frames );
	//void				resetParasiticWormHeadToTail();
	void				processEnemyCollision( CThing *thisThing );

	enum
	{
		NPC_PARASITIC_WORM_SPACING = 1,
		NPC_PARASITIC_WORM_LENGTH = 30,
	};

	// position history stuff

	class CNpcPositionHistory
	{
	public:
		DVECTOR						pos;
		CNpcPositionHistory			*next;
		CNpcPositionHistory			*prev;
	};

	CNpcParasiticWormSegment		m_segmentArray[NPC_PARASITIC_WORM_LENGTH];

	CNpcPositionHistory				*m_positionHistory;
	CNpcPositionHistory				m_positionHistoryArray[NPC_PARASITIC_WORM_SPACING * NPC_PARASITIC_WORM_LENGTH];

	s32								m_collTimer;
};

#endif