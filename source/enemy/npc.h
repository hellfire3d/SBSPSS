
#ifndef	__NPC_H__
#define	__NPC_H__

//#include <dstructs.h>
#include "Game/Thing.h"
#include "Gfx/Skel.h"


/*****************************************************************************/

class	CNpc : public CThing
{
public:
	void				init();
	void				shutdown();
	void				think(int _frames);
	virtual void		render();
	void				processEvent( GAME_EVENT evt, CThing *sourceThing );


protected:
	// NPC data structure definitions //

	enum NPC_UNIT_TYPE
	{
		NPC_TEST_TYPE = 0,
		NPC_SANDY_CHEEKS = 1,
		NPC_UNIT_TYPE_MAX,
	};

	enum NPC_INIT_FUNC
	{
		NPC_INIT_DEFAULT = 0,
		NPC_INIT_SNAKE = 1,
		NPC_INIT_ACID,
	};

	enum NPC_CONTROL_FUNC
	{
		NPC_CONTROL_MOVEMENT,
		NPC_CONTROL_SHOT,
		NPC_CONTROL_CLOSE,
		NPC_CONTROL_COLLISION,
	};

	enum NPC_SENSOR_FUNC
	{
		NPC_SENSOR_NONE = 0,
		NPC_SENSOR_USER_CLOSE = 1,
	};

	enum NPC_MOVEMENT_FUNC
	{
		NPC_MOVEMENT_STATIC = 0,
		NPC_MOVEMENT_FIXED_PATH = 1,
		NPC_MOVEMENT_USER_SEEK,
		NPC_MOVEMENT_VERTICAL,
	};

	enum NPC_MOVEMENT_MODIFIER_FUNC
	{
		NPC_MOVEMENT_MODIFIER_NONE = 0,
		NPC_MOVEMENT_MODIFIER_BOB = 1,
	};


	typedef struct NPC_DATA_TYPE
	{
		NPC_INIT_FUNC					initFunc;
		NPC_SENSOR_FUNC					sensorFunc;
		NPC_MOVEMENT_FUNC				movementFunc;
		NPC_MOVEMENT_MODIFIER_FUNC		movementModifierFunc;
		bool							canTalk;
	}
	NPC_DATA;

	NPC_UNIT_TYPE		m_type;
	NPC_CONTROL_FUNC	m_controlFunc;

	bool				processSensor();
	void				processMovement();
	void				processShot();
	void				processClose();
	void				processCollision();
	void				processTimer();

	static NPC_DATA		m_data[NPC_UNIT_TYPE_MAX];
};


/*****************************************************************************/
#endif
