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

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

class CNpcSquidDartEnemy : public CNpcEnemy
{
public:
	void				render();
	virtual int			getFrameCount()							{return( FRM_SQUIDDART_SWIM0004 - FRM_SQUIDDART_SWIM0001 + 1 );}
	virtual u8			canCollideWithEnemy()					{return( false );}
};

#endif
