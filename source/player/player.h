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
	virtual			~CPlayer();

	void			init();
	void			shutdown();
	void			think(int _frames);
	void			render();


private:		
	typedef enum
	{
		STATE_IDLE,
		STATE_RUNSTART,
		STATE_RUN,
		STATE_RUNSTOP,
			
		NUM_STATES,
	}PLAYER_STATE;

	void			setState(PLAYER_STATE _state);
	void			finishedAnim();

	int				m_frame;
	int				m_animNo;
	PLAYER_STATE	m_state;
	CSkel			m_skel;


	enum
	{
		MAX_RUN_VELOCITY=8,
		RUN_SPEEDUP=4,
		RUN_REVERSESLOWDOWN=2,
		RUN_SLOWDOWN=1,
	};
	int				m_runVel;
	
	enum
	{
		FACING_LEFT=+1,
		FACING_RIGHT=-1,
	};
	int				m_facing;

	static int		s_stateAnims[NUM_STATES];

};


/*****************************************************************************/
#endif
