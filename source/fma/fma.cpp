/*=========================================================================

	gameover.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose:	
				
	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "fma\fma.h"

#ifndef __LEVEL_LEVEL_H__
#include "level\level.h"
#endif

#ifndef __THING_THING_H__
#include "thing\thing.h"
#endif

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef __MAP_MAP_H__
#include "map\map.h"
#endif

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef __ACTOR_HEADER__
#include "gfx\actor.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

#ifndef	__ANIM_SPONGEBOB_HEADER__
#include "actor_spongebob_anim.h"
#endif

#ifndef	__ANIM_MERMAIDMAN_HEADER__
#include "actor_mermaidman_anim.h"
#endif

#ifndef	__ANIM_BARNACLEBOY_HEADER__
#include "actor_barnacleboy_anim.h"
#endif

#ifndef	__ANIM_GARY_HEADER__
#include "actor_gary_anim.h"
#endif


/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

// Available actors
enum
{
	ACTOR_SPONGEBOB,
	ACTOR_MM,
	ACTOR_BB,
	ACTOR_GARY,

	NUM_ACTORS
};

// Actor animation types
enum
{
	ANIM_IDLE,
	ANIM_WALK,

	NUM_ANIMS,
};

// Available script commands
typedef enum
{
	SC_SNAP_CAMERA_TO,			// x,y
	SC_MOVE_CAMERA_TO,			// x,y,frames

	SC_REGISTER_CONVERSATION,	// scriptId

	SC_WAIT_ON_TIMER,			// frames						(nonblocking)
	SC_WAIT_ON_ACTOR_STOP,		// actor						(nonblocking)
	SC_WAIT_ON_CAMERA_STOP,		//								(nonblocking)
	SC_WAIT_ON_CONVERSATION,	// scriptId						(nonblocking)
	
	SC_SET_ACTOR_VISIBILITY,	// actor,on/off
	SC_SET_ACTOR_POSITION,		// actor,x,y
	SC_SET_ACTOR_FACING,		// actor,facing
	SC_SET_ACTOR_ANIM_STATE,	// actor,state
	SC_WALK_ACTOR_TO_POSITION,	// actor,x,y,frames

	SC_STOP,					//
} SCRIPT_COMMAND;


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

// Data for an actors graphics
typedef struct
{
	FileEquate		m_file;
	int				m_anims[NUM_ANIMS];

} ACTOR_GRAPHICS_DATA;

// Actor data
typedef struct
{
	CActorGfx	*m_gfx;
	u8			m_active;

	u8			m_animState,m_animFrame;
	
	DVECTOR		m_pos;
	
	u16			m_startMoveFrame;
	DVECTOR		m_startMovePos;
	u16			m_endMoveFrame;
	DVECTOR		m_endMovePos;
	u8			m_moving;

	u8			m_facing;
} ACTOR_DATA;


/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

CFmaScene	FmaScene;


// A test script..
int s_testScript[]=
{
	SC_REGISTER_CONVERSATION,	SCRIPTS_CH1L1_01_DAT,
	SC_SNAP_CAMERA_TO,			0,0,
	SC_SET_ACTOR_VISIBILITY,	ACTOR_SPONGEBOB,true,
	SC_SET_ACTOR_POSITION,		ACTOR_SPONGEBOB,100,100,

	SC_MOVE_CAMERA_TO,			100,100,150+(60*2),
	SC_WALK_ACTOR_TO_POSITION,	ACTOR_SPONGEBOB,200,100,10,
	SC_WAIT_ON_CAMERA_STOP,

	SC_WAIT_ON_CONVERSATION,	SCRIPTS_CH1L1_01_DAT,

	SC_WAIT_ON_TIMER,			60,

	SC_SET_ACTOR_ANIM_STATE,	ACTOR_SPONGEBOB,ANIM_WALK,
	SC_SET_ACTOR_FACING,		ACTOR_SPONGEBOB,1,
	SC_WALK_ACTOR_TO_POSITION,	ACTOR_SPONGEBOB,250,200,5,
	SC_WAIT_ON_ACTOR_STOP,		ACTOR_SPONGEBOB,

	SC_WAIT_ON_TIMER,			60*5,

	SC_STOP
};


// Actor graphics data
const ACTOR_GRAPHICS_DATA	s_actorGraphicsData[NUM_ACTORS]=
{
	{	ACTORS_SPONGEBOB_SBK,			{	ANIM_SPONGEBOB_IDLEBREATH,	ANIM_SPONGEBOB_RUN			}	},	// ACTOR_SPONGEBOB
	{	ACTORS_MERMAIDMAN_SBK,			{	ANIM_MERMAIDMAN_IDLEBREATHE,ANIM_MERMAIDMAN_IDLEBREATHE	}	},	// ACTOR_MM
	{	ACTORS_BARNACLEBOY_SBK,			{	ANIM_BARNACLEBOY_IDLE,		ANIM_BARNACLEBOY_IDLE		}	},	// ACTOR_BB
	{	ACTORS_GARY_SBK,				{	ANIM_GARY_IDLE,				ANIM_GARY_SLITHER			}	},	// ACTOR_GARY
};


ACTOR_DATA		m_actorData[NUM_ACTORS];


#include "pad\pads.h"
#include "gfx\font.h"


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::init()
{
	int			i;
	ACTOR_DATA	*actor;


	CThingManager::init();
	CConversation::init();

	m_level=new ("FMALevel") CLevel();
	m_level->init(25);

	m_cameraPos.vx=30;
	m_cameraPos.vy=280;
	m_cameraMoving=false;
	m_readyToShutdown=false;

	CActorPool::Reset();
	actor=m_actorData;
	for(i=0;i<NUM_ACTORS;i++)
	{
		actor->m_gfx=CActorPool::GetActor(s_actorGraphicsData[i].m_file);
		actor->m_active=false;
		actor->m_animState=ANIM_IDLE;
		actor->m_animFrame=0;
		actor->m_pos.vx=0;
		actor->m_pos.vy=0;
		actor->m_moving=false;
		actor->m_facing=0;
		actor++;
	}
	CActorPool::SetUpCache();
	CActorPool::CleanUpCache();

	m_frameCount=0;

	m_scriptRunning=true;
	m_pc=s_testScript;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::shutdown()
{
	for(int i=0;i<NUM_ACTORS;i++)
	{
		delete m_actorData[i].m_gfx;
	}
	CActorPool::Reset();

	m_level->shutdown();	delete m_level;
	CSoundMediator::dumpSong();

	CConversation::shutdown();
	CThingManager::shutdown();
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::render()
{
	int			i;
	ACTOR_DATA	*actor;

	CThingManager::renderAllThings();
	CConversation::render();
	m_level->render();

	actor=m_actorData;
	for(i=0;i<NUM_ACTORS;i++)
	{
		if(actor->m_active)
		{
			DVECTOR	pos;
			pos.vx=actor->m_pos.vx-m_cameraPos.vx;
			pos.vy=actor->m_pos.vy-m_cameraPos.vy;
			actor->m_gfx->Render(pos,s_actorGraphicsData[i].m_anims[actor->m_animState],actor->m_animFrame,actor->m_facing);
		}
		actor++;
	}

	CActorPool::CleanUpCache();

#if defined (__USER_paul__) || defined (__USER_daveo__)
FontBank	*font;
char		buf[20];
font=new ("Font") FontBank();
font->initialise(&standardFont);
sprintf(buf,"%d,%d",m_cameraPos.vx,m_cameraPos.vy);
font->print(400,30,buf);
font->dump();
delete font;
#endif
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::think(int _frames)
{
#if defined (__USER_paul__) || defined (__USER_daveo__)
	if(PadGetHeld(0)&PAD_UP)
	{
		m_cameraPos.vy-=10*_frames;
	}
	else if(PadGetHeld(0)&PAD_DOWN)
	{
		m_cameraPos.vy+=10*_frames;
	}
	else if(PadGetHeld(0)&PAD_LEFT)
	{
		m_cameraPos.vx-=10*_frames;
	}
	else if(PadGetHeld(0)&PAD_RIGHT)
	{
		m_cameraPos.vx+=10*_frames;
	}
#endif
	if(m_scriptRunning==false&&PadGetDown(0)&(PAD_CROSS|PAD_START))
	{
		startShutdown();
	}

	for(int i=0;i<_frames;i++)
	{
		if(m_scriptRunning)
		{
			int			i;
			ACTOR_DATA	*actor;

			actor=m_actorData;
			for(i=0;i<NUM_ACTORS;i++)
			{
				if(actor->m_active)
				{
					// Move actor?
					if(actor->m_moving)
					{
						int	totalFrames,currentFrame;
						totalFrames=actor->m_endMoveFrame-actor->m_startMoveFrame;
						currentFrame=totalFrames-(actor->m_endMoveFrame-m_frameCount);
						if(currentFrame==0)
						{
							actor->m_pos=actor->m_startMovePos;
						}
						else if(currentFrame==totalFrames)
						{
							actor->m_pos=actor->m_endMovePos;
							actor->m_moving=false;
						}
						else
						{
							actor->m_pos.vx=actor->m_startMovePos.vx+(((actor->m_endMovePos.vx-actor->m_startMovePos.vx)*currentFrame)/totalFrames);
							actor->m_pos.vy=actor->m_startMovePos.vy+(((actor->m_endMovePos.vy-actor->m_startMovePos.vy)*currentFrame)/totalFrames);
						}
					}
				}

				// Anim
				actor->m_animFrame++;
				if(actor->m_animFrame>=actor->m_gfx->getFrameCount(s_actorGraphicsData[i].m_anims[actor->m_animState]))
				{
					actor->m_animFrame=0;
				}

				actor++;
			}

			// Move Camera
			if(m_cameraMoving)
			{
				int	totalFrames,currentFrame;
				totalFrames=m_endCameraFrame-m_startCameraFrame;
				currentFrame=totalFrames-(m_endCameraFrame-m_frameCount);
				if(currentFrame==0)
				{
					m_cameraPos=m_startCameraPos;
				}
				else if(currentFrame==totalFrames)
				{
					m_cameraPos=m_endCameraPos;
					m_cameraMoving=false;
				}
				else
				{
					m_cameraPos.vx=m_startCameraPos.vx+(((m_endCameraPos.vx-m_startCameraPos.vx)*currentFrame)/totalFrames);
					m_cameraPos.vy=m_startCameraPos.vy+(((m_endCameraPos.vy-m_startCameraPos.vy)*currentFrame)/totalFrames);
				}
			}

			// Process script
			do
			{
				if(!m_stillProcessingCommand)
				{
					startNextScriptCommand();
				}
				if(m_stillProcessingCommand)
				{
					processCurrentScriptCommand();
				}
			}
			while(!m_doOtherProcessing);
		}

		m_frameCount++;
	}

	CThingManager::thinkAllThings(_frames);
	CConversation::think(_frames);
	m_level->setCameraCentre(m_cameraPos);
	m_level->think(_frames);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int		CFmaScene::readyToShutdown()
{
	return m_readyToShutdown;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::startShutdown()
{
	GameState::setNextScene(&MapScene);
	m_readyToShutdown=true;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::startNextScriptCommand()
{
	m_stillProcessingCommand=false;
	m_doOtherProcessing=false;

	switch(*m_pc)
	{
		case SC_SNAP_CAMERA_TO:			// x,y
			m_pc++;
			m_cameraPos.vx=*m_pc++;
			m_cameraPos.vy=*m_pc++;
			break;

		case SC_MOVE_CAMERA_TO:			// x,y,frames
			m_pc++;
			m_cameraMoving=true;
			m_startCameraPos=m_cameraPos;
			m_startCameraFrame=m_frameCount;
			m_endCameraPos.vx=*m_pc++;
			m_endCameraPos.vy=*m_pc++;
			m_endCameraFrame=m_frameCount+*m_pc++;
			break;
	
		case SC_REGISTER_CONVERSATION:	// scriptId
			m_pc++;
			CConversation::registerConversationScript((FileEquate)*(m_pc++));
			break;

		case SC_WAIT_ON_TIMER:			// frames
			m_timer=*(m_pc+1);
			m_stillProcessingCommand=true;
			break;

		case SC_WAIT_ON_ACTOR_STOP:		// actor
			m_stillProcessingCommand=true;
			break;

		case SC_WAIT_ON_CAMERA_STOP:	//
			m_stillProcessingCommand=true;
			break;

		case SC_WAIT_ON_CONVERSATION:	// scriptId
			CConversation::trigger((FileEquate)*(m_pc+1));
			m_stillProcessingCommand=true;
			break;

		case SC_SET_ACTOR_VISIBILITY:	// actor,on/off
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_active=*m_pc++;
			}
			break;

		case SC_SET_ACTOR_POSITION:		// actor,x,y
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_pos.vx=*m_pc++;
				actor->m_pos.vy=*m_pc++;
			}
			break;

		case SC_SET_ACTOR_FACING:		// actor,facing
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_facing=*m_pc++;
			}
			break;

		case SC_SET_ACTOR_ANIM_STATE:	// actor,state
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_animState=*m_pc++;
			}
			break;

		case SC_WALK_ACTOR_TO_POSITION:	// actor,x,y,frames
			{
				ACTOR_DATA	*actor;
				m_pc++;
				actor=&m_actorData[*m_pc++];
				actor->m_startMoveFrame=m_frameCount;
				actor->m_startMovePos=actor->m_pos;
				actor->m_endMovePos.vx=*m_pc++;
				actor->m_endMovePos.vy=*m_pc++;
				actor->m_endMoveFrame=m_frameCount+*m_pc++;
				actor->m_moving=true;
			}
			break;

		case SC_STOP:					//
			m_scriptRunning=false;
			m_doOtherProcessing=true;
			break;

		default:
			ASSERT(!"Bad script command");
			break;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CFmaScene::processCurrentScriptCommand()
{
	switch(*m_pc)
	{
		case SC_SNAP_CAMERA_TO:			// x,y
		case SC_MOVE_CAMERA_TO:			// x,y,frames
		case SC_REGISTER_CONVERSATION:	// scriptId
		case SC_SET_ACTOR_VISIBILITY:	// actor,on/off
		case SC_SET_ACTOR_POSITION:		// actor,x,y
		case SC_SET_ACTOR_FACING:		// actor,facing
		case SC_SET_ACTOR_ANIM_STATE:	// actor,state
		case SC_WALK_ACTOR_TO_POSITION:	// actor,x,y,frames
		case SC_STOP:					//
			ASSERT(!"Shouldn't be here..");
			break;

		case SC_WAIT_ON_TIMER:			// frames
			if(m_timer--==0)
			{
				m_pc+=2;
				m_stillProcessingCommand=false;
			}
			else
			{
				m_doOtherProcessing=true;
			}
			break;
		
		case SC_WAIT_ON_ACTOR_STOP:		// actor
			if(!m_actorData[*(m_pc+1)].m_moving)
			{
				m_pc+=2;
				m_stillProcessingCommand=false;
			}
			else
			{
				m_doOtherProcessing=true;
			}
			break;

		case SC_WAIT_ON_CAMERA_STOP:	//
			if(!m_cameraMoving)
			{
				m_pc++;
				m_stillProcessingCommand=false;
			}
			else
			{
				m_doOtherProcessing=true;
			}
			break;


		case SC_WAIT_ON_CONVERSATION:	// scriptId
			if(!CConversation::isActive())
			{
				m_pc+=2;
				m_stillProcessingCommand=false;
			}
			else
			{
				m_doOtherProcessing=true;
			}
			break;

		default:
			ASSERT(!"Bad script command");
			break;
	}
}


/*===========================================================================
 end */