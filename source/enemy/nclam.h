/*=========================================================================

	nclam.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NCLAM_H__
#define	__ENEMY_NCLAM_H__

class CNpcClamEnemy : public CNpcEnemy
{
public:
	u8					canCollideWithEnemy()					{return( false );}
protected:
	void				processUserCollision( CThing *thisThing );
	void				processEnemyCollision( CThing *thisThing );
	bool				processSensor();
};

class CNpcJumpingClamEnemy : public CNpcClamEnemy
{
public:
	void				postInit();
protected:
	void				processClose( int _frames );
	void				setupWaypoints( sThingActor *ThisActor );

	s32					m_maxExtension;
};

class CNpcStaticClamEnemy : public CNpcClamEnemy
{
public:
	void				postInit();
	u8					hasBeenAttacked()						{return( false );}
	CRECT const			*getThinkBBox()							{return( CThing::getThinkBBox() );}
protected:
	s32					getFrameShift( int _frames );
	void				collidedWith(CThing *_thisThing);
	void				processClose( int _frames );
	void				processCollision();
	void				processAnimFrames( int _frames );

	s32					m_isStunned;
};

#endif