/**********************/
/*** Main Game File ***/
/**********************/

#include 	"system\global.h"
#include 	"fileio\fileio.h"
#include 	"pad\pads.h"
#include 	"system\vid.h"
#include 	"gfx\prim.h"
#include	"utils\utils.h"
#include	"level\level.h"
#include	"game\game.h"
#include	"system\gstate.h"
#include	"gfx\font.h"
#include	"gfx\fdata.h"

#ifndef	__GAME_CONVO_H__
#include "game\convo.h"
#endif

#ifndef __PLAYER_PLAYER_H__
#include "player\player.h"
#endif
#include "player\demoplay.h"

#ifndef	__ENEMY_NPC_H__
#include "enemy\npc.h"
#endif

#ifndef __ENEMY_NSJFISH_H__
#include "enemy\nsjfish.h"
#endif

#ifndef __ENEMY_NHCRAB_H__
#include "enemy\nhcrab.h"
#endif

#ifndef __ENEMY_NSCRAB_H__
#include "enemy\nscrab.h"
#endif

#ifndef __ENEMY_NGEN_H__
#include "enemy\ngen.h"
#endif

#ifndef	__ENEMY_NANEMONE_H__
#include "enemy\nanemone.h"
#endif

#ifndef	__ENEMY_NCLAM_H__
#include "enemy\nclam.h"
#endif

#ifndef	__ENEMY_NOCTO_H__
#include "enemy\nocto.h"
#endif

#ifndef __ENEMY_NFFOLK_H__
#include "enemy\nffolk.h"
#endif

#ifndef __ENEMY_NBBLOB_H__
#include "enemy\nbblob.h"
#endif

#ifndef	__ENEMY_NGPIRATE_H__
#include "enemy\ngpirate.h"
#endif

#ifndef	__ENEMY_NPUFFA_H__
#include "enemy\npuffa.h"
#endif

#ifndef	__ENEMY_NSHRKMAN_H__
#include "enemy\nshrkman.h"
#endif

#ifndef	__ENEMY_NSKLFISH_H__
#include "enemy\nsklfish.h"
#endif

#ifndef	__ENEMY_NEYEBALL_H__
#include "enemy\neyeball.h"
#endif

#ifndef	__ENEMY_NFSKULL_H__
#include "enemy\nfskull.h"
#endif

#ifndef __ENEMY_NPLATFRM_H__
#include "enemy\nplatfrm.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __GFX_FADER_H__
#include "gfx\fader.h"
#endif

#ifndef __GFX_BUBICLES__
#include "gfx\bubicles.h"
#endif

#ifndef __SPR_SPRITES_H__
#include <sprites.h>
#endif

#ifndef _ANIMTEX_HEADER_
#include "gfx\animtex.h"
#endif

#ifndef	__GAME_PAUSE_H__
#include "game\pause.h"
#endif

#ifndef	__FRONTEND_FRONTEND_H__
#include "frontend\frontend.h"
#endif

#ifndef __PICKUPS_PICKUP_H__
#include "pickups\pickup.h"
#endif


#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#include "gfx\actor.h"

int			RenderZ=256;

/*****************************************************************************/

FontBank	*CGameScene::s_genericFont;
MATRIX		CGameScene::CamMtx;

/*****************************************************************************/

#ifndef __USER_paul__
int s_globalLevelSelectThing=0;
#else
int s_globalLevelSelectThing=120;
#endif
int CGameScene::s_readyToExit;
int CGameScene::s_levelFinished;

/*****************************************************************************/

CGameScene	GameScene;

/*****************************************************************************/
void	CGameScene::AspectCorrectCamera()
{
const s32 Scale = (512<<12)/(256);

		CamMtx.m[0][0] = ((s32)CamMtx.m[0][0] * Scale)>>12;
		CamMtx.m[0][1] = ((s32)CamMtx.m[0][1] * Scale)>>12;
		CamMtx.m[0][2] = ((s32)CamMtx.m[0][2] * Scale)>>12;

		CamMtx.t[0]  = (CamMtx.t[0] * Scale)>>12;
}

/*****************************************************************************/
void 	CGameScene::init()
{
// Setup Constant Camera Matrix
//		SetIdentNoTrans(&CamMtx);
//		CamMtx.t[2]=ZPos;
//		AspectCorrectCamera();
//		SetRotMatrix(&CamMtx);
//		SetTransMatrix(&CamMtx);

		SetGeomScreen(RenderZ);
		CamMtx.t[2]=RenderZ;
		SetTransMatrix(&CamMtx);		

		s_genericFont=new ("CGameScene::Init") FontBank();
		s_genericFont->initialise( &standardFont );
		s_genericFont->setColour( 255, 255 , 0 );
		VidSetClearScreen(0);



		m_pauseMenu=new ("Pause Menu") CPauseMenu();
		m_pauseMenu->init();

		s_readyToExit=false;

		CFader::setFadingIn();
		initLevel();
}

/*****************************************************************************/
// This is a seperate funtion ( and virtual ) so that we can overload it for
// the demo mode (pkg)
// NB: This function should *only* include the new function - Don't add anything else here!
void	CGameScene::createPlayer()
{
		m_player=new ("player") CPlayer();
}

// This is also to be overloaded for demomode.. to stop the pause menu appearing
int		CGameScene::canPause()
{
		return true;
}


/*****************************************************************************/

void	CGameScene::shutdown()
{
		shutdownLevel();
		CSoundMediator::dumpSong();

		m_pauseMenu->shutdown();	delete m_pauseMenu;
		s_genericFont->dump();		delete s_genericFont;
}

/*****************************************************************************/
void 	CGameScene::render()
{
//		CamMtx.t[2]=ZPos;	// Temp

		m_pauseMenu->render();
		CConversation::render();
		CThingManager::renderAllThings();
		Level.render();
}

/*****************************************************************************/
void	CGameScene::think(int _frames)
{
//#ifdef __USER_paul__
//	if(!CConversation::isActive()&&PadGetDown(0)&PAD_START)
//	{
//		CConversation::trigger(SCRIPTS_SPEECHTEST_DAT);
//	}
//#endif

	if(s_readyToExit)
	{
		// Temporarily.. exiting game scene always goes back to the front end (pkg)
		GameState::setNextScene(&FrontEndScene);
	}
	else if(s_levelFinished)
	{
		s_globalLevelSelectThing++;
		shutdownLevel();
		initLevel();
	s_levelFinished=false;
	}


	if(!m_pauseMenu->isActive()&&PadGetDown(0)&PAD_START&&canPause())
	{
		m_pauseMenu->select();
	}

/*	if (!s_levelFinished) */CConversation::think(_frames);
/*	if (!s_levelFinished) */m_pauseMenu->think(_frames);
	if(!CConversation::isActive()&& !m_pauseMenu->isActive())
	{
		DVECTOR	camPos;
/*		if (!s_levelFinished) */CThingManager::thinkAllThings(_frames);
/*		if (!s_levelFinished) */camPos=m_player->getCameraPos();
//PKG
//		if(camPos.vx<0){camPos.vx=0;PAUL_DBGMSG("cx<0");}
//		if(camPos.vy<0){camPos.vy=0;PAUL_DBGMSG("cy<0");}
//PKG
/*		if (!s_levelFinished) */CBubicleFactory::setMapOffset(&camPos);
/*		if (!s_levelFinished) */Level.setCameraCentre(camPos);
/*		if (!s_levelFinished) */Level.think(_frames);

		if(PadGetDown(0)&PAD_R2)
		{
			levelFinished();
		}
	}
//	s_levelFinished=false;
}


/*****************************************************************************/
int		CGameScene::readyToShutdown()
{
	return s_readyToExit;
}

/*****************************************************************************/
CPlayer	* CGameScene::getPlayer()
{
	return( m_player );
}

/*****************************************************************************/
void CGameScene::sendEvent( GAME_EVENT evt, CThing *sourceThing )
{
	CThingManager::processEventAllThings(evt, sourceThing);
}



/*****************************************************************************/
void	CGameScene::initLevel()
{
	printf("InitLevel\n");
	CThingManager::init();

	CConversation::init();
	CConversation::registerConversationScript(SCRIPTS_SPEECHTEST_DAT);	// Register one script for testing..

#ifdef __USER_paul__
	DVECTOR pos={16*10,16*10};
				createPickup(PICKUP__BIG_HEALTH,&pos);
	pos.vx+=64;	createPickup(PICKUP__MEDIUM_HEALTH,&pos);
	pos.vx+=64;	createPickup(PICKUP__SMALL_HEALTH,&pos);
	pos.vx+=64;	createPickup(PICKUP__LIFE,&pos);
	pos.vx+=64;	createPickup(PICKUP__SPATULA,&pos);
	pos.vx+=64;	createPickup(PICKUP__JELLY_LAUNCHER_AMMO,&pos);
	pos.vx+=64;	createPickup(PICKUP__BUBBLE_MIXTURE,&pos);
	pos.vx+=64;	createPickup(PICKUP__BUBBLE_WAND,&pos);
	pos.vx+=64;	createPickup(PICKUP__NET,&pos);
	pos.vx+=64;	createPickup(PICKUP__GLASSES,&pos);
	pos.vx+=64;	createPickup(PICKUP__SQUEAKY_SHOES,&pos);
	pos.vx+=64;	createPickup(PICKUP__BALLOON,&pos);
	pos.vx+=64;	createPickup(PICKUP__HELMET,&pos);
	pos.vx+=64;	createPickup(PICKUP__CORAL_BLOWER,&pos);
	pos.vx+=64;	createPickup(PICKUP__QUEST_ITEM__TEST,&pos);

//	CNpcPlatform	*platform;
//	platform=new ("test platform") CNpcPlatform;
//	platform->init();
//	platform->setLayerCollision( Level.getCollisionLayer() );
#endif

	s_levelFinished=false;
	Level.init(s_globalLevelSelectThing);

	createPlayer();
	m_player->setRespawnPos(Level.getPlayerSpawnPos());
	m_player->init();
	m_player->setLayerCollision(Level.getCollisionLayer());
	m_player->setMapSize(Level.getMapSize());

// Init actors (needs moving and tidying
	int pointNum;
	int actorNum;
	int platformNum;

	sThingActor **actorList = Level.getActorList();
	if (actorList)
	{
		for ( actorNum = 0 ; actorNum < Level.getActorCount() ; actorNum++ )
		{
			CActorPool::ACTOR_TYPE actorType = CActorPool::getActorType( actorList[actorNum]->Type );

			switch ( actorType )
			{
				case CActorPool::ACTOR_ENEMY_NPC:
				{
					CNpcEnemy *enemy;

					CNpcEnemy::NPC_UNIT_TYPE enemyType = CNpcEnemy::getTypeFromMapEdit( actorList[actorNum]->Type );

					switch( enemyType )
					{
						case CNpcEnemy::NPC_SMALL_JELLYFISH_1:
						{
							enemy = new ("small jellyfish") CNpcSmallJellyfishEnemy;
							break;
						}

						case CNpcEnemy::NPC_HERMIT_CRAB:
						{
							enemy = new ("hermit crab") CNpcHermitCrabEnemy;
							break;
						}

						case CNpcEnemy::NPC_SPIDER_CRAB:
						{
							enemy = new ("spider crab") CNpcSpiderCrabEnemy;
							break;
						}

						case CNpcEnemy::NPC_SPIDER_CRAB_SPAWNER:
						{
							enemy = new ("spider crab spawner") CNpcEnemyGenerator;
							break;
						}

						case CNpcEnemy::NPC_ANEMONE_1:
						{
							enemy = new ("anemone 1") CNpcAnemone1Enemy;
							break;
						}

						case CNpcEnemy::NPC_ANEMONE_2:
						{
							enemy = new ("anemone 2") CNpcAnemone2Enemy;
							break;
						}

						case CNpcEnemy::NPC_ANEMONE_3:
						{
							enemy = new ("anemone 3") CNpcAnemone3Enemy;
							break;
						}

						case CNpcEnemy::NPC_CLAM_JUMP:
						{
							enemy = new ("jumping clam") CNpcJumpingClamEnemy;
							break;
						}

						case CNpcEnemy::NPC_CLAM_STATIC:
						{
							enemy = new ("static clam") CNpcStaticClamEnemy;
							break;
						}

						case CNpcEnemy::NPC_BABY_OCTOPUS:
						{
							enemy = new ("baby octopus") CNpcBabyOctopusEnemy;
							break;
						}

						case CNpcEnemy::NPC_FISH_FOLK:
						case CNpcEnemy::NPC_ZOMBIE_FISH_FOLK:
						{
							enemy = new ("fish folk") CNpcFishFolk;
							break;
						}

						case CNpcEnemy::NPC_BALL_BLOB:
						{
							enemy = new ("ball blob") CNpcBallBlobEnemy;
							break;
						}

						case CNpcEnemy::NPC_GHOST_PIRATE:
						{
							enemy = new ("ghost pirate") CNpcGhostPirateEnemy;
							break;
						}

						case CNpcEnemy::NPC_PUFFA_FISH:
						{
							enemy = new ("puffa fish") CNpcPuffaFishEnemy;
							break;
						}

						case CNpcEnemy::NPC_SHARK_MAN:
						{
							enemy = new ("shark man") CNpcSharkManEnemy;
							break;
						}

						case CNpcEnemy::NPC_SKELETAL_FISH:
						{
							enemy = new ("skeletal fish") CNpcSkeletalFishEnemy;
							break;
						}

						case CNpcEnemy::NPC_EYEBALL:
						{
							enemy = new ("eyeball") CNpcEyeballEnemy;
							break;
						}

						case CNpcEnemy::NPC_FLAMING_SKULL:
						{
							enemy = new ("flaming skull") CNpcFlamingSkullEnemy;
							break;
						}

						default:
						{
							enemy = new ("npc enemy") CNpcEnemy;
							break;
						}
					}

					ASSERT(enemy);
					enemy->setTypeFromMapEdit( actorList[actorNum]->Type );
					enemy->init();
					enemy->setLayerCollision( Level.getCollisionLayer() );

					u16	*PntList=(u16*)MakePtr(actorList[actorNum],sizeof(sThingActor));

					u16 newXPos, newYPos;

					newXPos = (u16) *PntList;
					PntList++;
					newYPos = (u16) *PntList;
					PntList++;

					enemy->setStartPos( newXPos, newYPos );
					enemy->addWaypoint( newXPos, newYPos );

					if ( actorList[actorNum]->PointCount > 1 )
					{
						for ( pointNum = 1 ; pointNum < actorList[actorNum]->PointCount ; pointNum++ )
						{
							newXPos = (u16) *PntList;
							PntList++;
							newYPos = (u16) *PntList;
							PntList++;

							enemy->addWaypoint( newXPos, newYPos );

							if ( pointNum == 1 )
							{
								enemy->setHeading( newXPos, newYPos );
							}
						}
					}

					enemy->postInit();

					break;
				}

				default:
					break;
			}
		}
	}

	sThingPlatform **platformList = Level.getPlatformList();
	if (platformList)
	{
		for ( platformNum = 0 ; platformNum < Level.getPlatformCount() ; platformNum++ )
		{
			CNpcPlatform *platform;
			platform = new ("platform") CNpcPlatform;
			ASSERT(platform);
			platform->setTypeFromMapEdit( platformList[platformNum]->Type );

			u16	*PntList=(u16*)MakePtr(platformList[platformNum],sizeof(sThingPlatform));

			u16 newXPos, newYPos;

			newXPos = (u16) *PntList;
			PntList++;
			newYPos = (u16) *PntList;
			PntList++;

			DVECTOR startPos;
			startPos.vx = newXPos << 4;
			startPos.vy = newYPos << 4;

			platform->init( startPos );
			platform->setLayerCollision( Level.getCollisionLayer() );
			platform->setTiltable( false );

			platform->addWaypoint( newXPos, newYPos );

			if ( platformList[platformNum]->PointCount > 1 )
			{
				for ( pointNum = 1 ; pointNum < platformList[platformNum]->PointCount ; pointNum++ )
				{
					newXPos = (u16) *PntList;
					PntList++;
					newYPos = (u16) *PntList;
					PntList++;

					platform->addWaypoint( newXPos, newYPos );
				}
			}

		}
	}

	// Song is loaded/dumped by the level, and played from here. This just gives some
	// better timing over when it starts (pkg)
	CSoundMediator::playSong();
	printf("InitLevelDone\n");

}


/*****************************************************************************/
void	CGameScene::shutdownLevel()
{
	CConversation::shutdown();
	CThingManager::shutdown();
	Level.shutdown();
}

/*****************************************************************************/