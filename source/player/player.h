/**************/
/*** Player ***/
/**************/

#ifndef	__PLAYER_H__
#define	__PLAYER_H__

#include "Game/Thing.h"

/*****************************************************************************/
class	CPlayer : public CThing
{
public:
		CPlayer();
virtual	~CPlayer();

		void		init();
		void		shutdown();
		void		think();
		void		render();

protected:
};

/*****************************************************************************/
#endif
