/*=========================================================================

	neyeball.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NEYEBALL_H__
#define	__ENEMY_NEYEBALL_H__

class CNpcEyeballEnemy : public CNpcEnemy
{
public:
	void				render();
	void				postInit();
	void				shutdown();
	int					getFrameCount()									{return( 1 );}
	void				processEvent( GAME_EVENT evt, CThing *sourceThing );
protected:
	void				processEnemyCollision( CThing *thisThing );
	bool				processSensor();
	void				processClose( int _frames );
	void				processShotDeathStart( int _frames );
	void				processShotDeathEnd( int _frames );

	s32					m_fireHeading;

	enum
	{
		EYEBALL_DIST = 30,
	};
};

#endif