/*=========================================================================

	nsdart.h

	Author:		CRB
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __ENEMY_NSDART_H__
#define __ENEMY_NSDART_H__

class CNpcSquidDartEnemy : public CNpcEnemy
{
public:
	void				render();
	virtual int			getFrameCount()							{return( 4 );}
	virtual u8			canCollideWithEnemy()					{return( false );}
};

#endif
