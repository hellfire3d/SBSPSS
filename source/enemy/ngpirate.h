/*=========================================================================

	ngpirate.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__ENEMY_NGPIRATE_H__
#define	__ENEMY_NGPIRATE_H__

class CNpcGhostPirateEnemy : public CNpcEnemy
{
public:
	void				postInit();
protected:
	bool				processSensor();
	void				processClose( int _frames );
};

#endif