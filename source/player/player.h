/**************/
/*** Player ***/
/**************/

#ifndef	__PLAYER_H__
#define	__PLAYER_H__

#include <dstructs.h>
#include "Game/Thing.h"
#include "Gfx/Skel.h"

/*****************************************************************************/

class	CPlayer : public CThing
{
public:
		CPlayer();
		virtual	~CPlayer();

		void		init();
		void		shutdown();

		void		Animate();
		
protected:

		CSkel		Skel;
};

/*****************************************************************************/
#endif
