int counter=0;
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

#ifndef __FRIEND_FRIEND_H__
#include "friend\friend.h"
#endif

#ifndef __PLATFORM_PLATFORM_H__
#include "platform\platform.h"
#endif

#ifndef __HAZARD_HAZARD_H__
#include "hazard\hazard.h"
#endif

#ifndef __PROJECTL_PROJECTL_H__
#include "projectl\projectl.h"
#endif

#ifndef __JELLFISH_JELLFISH_H__
#include "jellfish\jellfish.h"
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

#ifndef	__SOUND_SOUND_H__
#include "sound\sound.h"
#endif

#ifndef __MAP_MAP_H__
#include "map\map.h"
#endif

#ifndef __SHOP_SHOP_H__
#include "shop\shop.h"
#endif

#ifndef __FMA_FMA_H__
#include "fma\fma.h"
#endif

#ifndef	__BACKEND_GAMEOVER_H__
#include "backend\gameover.h"
#endif

#ifndef	__GAME_GAMESLOT_H__
#include "game\gameslot.h"
#endif

#ifndef	__GAME_HEALTH_MANAGER_H__
#include "game\healthman.h"
#endif

#ifndef __LOCALE_TEXTDBASE_H__
#include "locale\textdbase.h"
#endif

#ifndef	__SAVE_SAVE_H__
#include "save\save.h"
#endif

#ifndef __GAME_BOSSTEXT_H__
#include "game\bosstext.h"
#endif

#include "gfx\actor.h"

/*****************************************************************************/

FontBank		*CGameScene::s_genericFont;
SpriteBank		*CGameScene::s_GlobalSpritebank;
CLayerCollision	*CGameScene::s_GlobalCollision;
CHealthManager	*CGameScene::m_HealthManager;

/*****************************************************************************/
CGameScene::ACTOR_TYPE CGameScene::actorType[40] =
{
	ACTOR_PLAYER,				//SpongeBob=0
	ACTOR_FRIEND_NPC,			//BarnacleBoy=1
	ACTOR_FRIEND_NPC,			//Gary=2
	ACTOR_FRIEND_NPC,			//Krusty=3
	ACTOR_FRIEND_NPC,			//MermaidMan=4
	ACTOR_FRIEND_NPC,			//Patrick=5
	ACTOR_FRIEND_NPC,			//Sandy=6
	ACTOR_FRIEND_NPC,			//Squidward=7
	ACTOR_FRIEND_NPC,			//Plankton=8
	ACTOR_UNKNOWN,
	ACTOR_ENEMY_NPC,			//SmallJellyfish-Level1=10
	ACTOR_ENEMY_NPC,			//SmallJellyfish-Level2=11
	ACTOR_ENEMY_NPC,			//Motherjellyfish=12
	ACTOR_ENEMY_NPC,			//Anenome-Level1=13
	ACTOR_ENEMY_NPC,			//SpikeyAnenome=14
	ACTOR_ENEMY_NPC,			//Anenome-Level3=15
	ACTOR_ENEMY_NPC,			//BabyOctopus=16
	ACTOR_ENEMY_NPC,			//Ballblob=17
	ACTOR_ENEMY_NPC,			//Boogermonster=18
	ACTOR_ENEMY_NPC,			//Caterpillar=19
	ACTOR_ENEMY_NPC,			//Clam-Level1=20
	ACTOR_ENEMY_NPC,			//Clam-Level2=21
	ACTOR_ENEMY_NPC,			//Eyeball=22
	ACTOR_ENEMY_NPC,			//Flamingskull=23
	ACTOR_ENEMY_NPC,			//FlyingDutchman=24
	ACTOR_ENEMY_NPC,			//Ghost=25
	ACTOR_ENEMY_NPC,			//GiantWorm=26
	ACTOR_ENEMY_NPC,			//HermitCrab=27
	ACTOR_ENEMY_NPC,			//IronDogFish=28
	ACTOR_ENEMY_NPC,			//PuffaFish=29
	ACTOR_ENEMY_NPC,			//SeaSnake=30
	ACTOR_ENEMY_NPC,			//Sharkman=31
	ACTOR_ENEMY_NPC,			//SharkSub=32
	ACTOR_ENEMY_NPC,			//Skeletalfish=33
	ACTOR_ENEMY_NPC,			//SpiderCrab=34
	ACTOR_ENEMY_NPC,			//Squiddart=35
	ACTOR_ENEMY_NPC,			//Stomper=36
	ACTOR_ENEMY_NPC,			//DustDevil=37
	ACTOR_ENEMY_NPC,			//SiderCrabSpawner=38
	ACTOR_ENEMY_NPC,			//Shell=39
};

/*****************************************************************************/

int		s_globalLevelSelectThing=0;
int		CGameScene::s_readyToExit;
int		CGameScene::s_levelFinished;
#ifdef __VERSION_DEBUG__
int		CGameScene::s_skipToNextLevel;
#endif
int		CGameScene::s_restartLevel;
int		CGameScene::s_bossHasBeenKilled;
int		CGameScene::s_justHitBossArenaTrigger;
DVECTOR	CGameScene::s_CamShake={0,0};
int		CGameScene::s_showBossTextOnRespawn;


// Evil global pointer to the boss character
CThing			*g_bossThing;


/*****************************************************************************/


CGameScene	GameScene;

/*****************************************************************************/
void 	CGameScene::init()
{
		setCameraMtx();

		s_genericFont=new ("CGameScene::Init") FontBank();
		s_genericFont->initialise( &standardFont );
		s_genericFont->setColour( 255, 255 , 0 );

		m_scalableFont=new ("CountdownTimerFont") ScalableFontBank();
		m_scalableFont->initialise(&standardFont);
		m_scalableFont->setColour(255,255,255);
		m_scalableFont->setScale(511);

		VidSetClearScreen(0);

		m_pauseMenu=new ("Pause Menu") CPauseMenu();
		m_pauseMenu->init();

		m_HealthManager= new ("Health Manager") CHealthManager();
		m_HealthManager->init();

		s_readyToExit=false;
		s_restartLevel=false;

		m_bossText=new ("BossText") CBossText();
		m_bossText->init();

		CFader::setFadingIn();
		CLevel::setIsBossRespawn(false);
		initLevel();

		m_gamestate=GAMESTATE_SHOWING_LIVES;
		m_showingLivesTimer=0;

		s_showBossTextOnRespawn=false;
		m_playingBossMusic=false;

		m_isPaused=false;
}
/*****************************************************************************/
// This is a seperate funtion ( and virtual ) so that we can overload it for
// the demo mode (pkg)
// NB: This function should *only* include the new function - Don't add anything else here!
void	CGameScene::createPlayer()
{
		m_player=new ("player") CPlayer();
		m_player->setThingSubType(0);
}

// This is also to be overloaded for demomode.. to stop the pause menu appearing
int		CGameScene::canPause()
{
		return m_gamestate==GAMESTATE_PLAYING;
}


/*****************************************************************************/

void	CGameScene::shutdown()
{
		m_bossText->shutdown();		delete m_bossText;

		shutdownLevel();

		m_pauseMenu->shutdown();	delete m_pauseMenu;
		m_scalableFont->dump();		delete m_scalableFont;
		s_genericFont->dump();		delete s_genericFont;
		m_HealthManager->shutdown();delete m_HealthManager;

}

/*****************************************************************************/
void 	CGameScene::render()
{
	switch(m_gamestate)
	{
		case GAMESTATE_SHOWING_LIVES:
		case GAMESTATE_SHOWING_LIVES_BUT_GOING_TO_BOSS_TEXT:
			render_showing_lives();
			break;
		case GAMESTATE_PLAYING:
		case GAMESTATE_FADING_INTO_BOSS_INTRO:
		case GAMESTATE_TELEPORT_START:
		case GAMESTATE_TELEPORT_END:
			render_playing();
			break;
		case GAMESTATE_BOSS_INTRO:
			m_bossText->render();
			break;
	}
}

/*****************************************************************************/
void CGameScene::render_showing_lives()
{
	int			colour;
	char		buf[20];
	TPOLY_F4		*f4;
	POLY_FT3	*ft3;


	colour=m_showingLivesTimer-TIME_TO_DISPLAY_LIVES_COUNT;
	if(colour<0)
	{
		colour=0;
	}
	else
	{
		colour*=SPEED_OF_FADE;
		if(colour>255)
		{
			colour=255;
		}
	}
	colour=255-colour;

	// Text
	m_scalableFont->setPrintArea(0,0,512,256);
	s_genericFont->setJustification(FontBank::JUST_CENTRE);
	s_genericFont->setColour(colour,colour,colour);
	s_genericFont->setTrans(1);
	s_genericFont->setSMode(1);
	s_genericFont->print(256,50,"Now entering:");
	s_genericFont->print(256,80,Level.getChapterLoadingText());
	s_genericFont->print(256,100,Level.getLevelLoadingText());
	sprintf(buf,"Lives x %d",CGameSlotManager::getSlotData()->m_lives);
	s_genericFont->print(256,140,buf);

	// Black background
	f4=GetPrimTF4();
	setXYWH(f4,0,0,512,256);
	setRGB0(f4,colour,colour,colour);
	setTShadeTex(f4,0);
	setTSemiTrans(f4,1);
	setTABRMode(f4,2);
	AddPrimToList(f4,0);
/*	ft3=GetPrimFT3();
	setPolyFT3(ft3);
	setShadeTex(ft3,1);
	setSemiTrans(ft3,1);
	ft3->tpage=2<<5;
	setXY3(ft3,512,512,512,512,512,512);
	AddPrimToList(ft3,0);
*/
	if(m_gamestate==GAMESTATE_SHOWING_LIVES_BUT_GOING_TO_BOSS_TEXT)
	{
		m_bossText->render();
	}
	else
	{
		render_playing();
	}
}

/*****************************************************************************/
void CGameScene::render_playing()
{
//		CamMtx.t[2]=ZPos;	// Temp

		if(m_levelHasTimer)
		{
			int		timerValue;
			char	buf[10];

			timerValue=m_timer/55;
			if(timerValue<0)timerValue=0;
			sprintf(buf,"%d",timerValue);
			if(timerValue<=5)
			{
				m_scalableFont->setColour(255,0,0);
			}
			else
			{
				m_scalableFont->setColour(255,255,255);
			}
			m_scalableFont->setScale(511);
			m_scalableFont->print(VidGetScrW()/2,30,buf);
		}

		m_pauseMenu->render();
		CConversation::render();
		CThingManager::renderAllThings();

		setCameraMtx();

		Level.render();
		m_HealthManager->render();

		CActorPool::CleanUpCache();
}

/*****************************************************************************/
void	CGameScene::think(int _frames)
{
//#if defined (__USER_paul__)
//if(PadGetDown(0)&PAD_UP)
//{
//}
//#endif
	if(!m_musicStarted&&!CFader::isFading())
	{
		// Song is loaded/dumped by the level, and played from here. This just gives some
		// better timing over when it starts (pkg)
		CSoundMediator::playSong();
		CSoundMediator::setCanPlaySfx(true);
		m_musicStarted=true;
	}

	switch(m_gamestate)
	{
		case GAMESTATE_SHOWING_LIVES:
			think_showing_lives(_frames);
			break;
		case GAMESTATE_SHOWING_LIVES_BUT_GOING_TO_BOSS_TEXT:
			think_showing_lives(_frames);
			break;
		case GAMESTATE_PLAYING:
			think_playing(_frames);
			break;
		case GAMESTATE_FADING_INTO_BOSS_INTRO:
			if(!CFader::isFading())
			{
				m_bossText->select();
				m_gamestate=GAMESTATE_BOSS_INTRO;
				CFader::setFadingIn();
			}
			break;
		case GAMESTATE_BOSS_INTRO:
			m_bossText->think(_frames);
			if(m_bossText->isReadyToExit())
			{
				m_gamestate=GAMESTATE_PLAYING;
				if(!m_playingBossMusic)
				{
					CSoundMediator::playSong();
					m_playingBossMusic=true;
				}
				sendEvent( BOSS_FOUND_EVENT, NULL );
				CFader::setFadingIn();
			}
			break;
		case GAMESTATE_TELEPORT_START:
			if(!CFader::isFading())
			{
				think_playing(1);	// Let the teleport happen..
				think_playing(1);
				think_playing(1);
				m_gamestate=GAMESTATE_TELEPORT_END;
				CFader::setFadingIn();
			}
			break;
		case GAMESTATE_TELEPORT_END:
			if(!CFader::isFading())
			{
				m_gamestate=GAMESTATE_PLAYING;
			}
			break;
	}


#ifdef __VERSION_DEBUG__
#if defined (__USER_paul__) || defined (__USER_daveo__) || defined (__USER_charles__)
	char	buf[20];
	sprintf(buf,"Frames: %d",_frames);
	m_scalableFont->setTrans(0);
	m_scalableFont->setSMode(0);
	m_scalableFont->setColour(128,128,128);
	m_scalableFont->setScale(300);
	m_scalableFont->print(280,160,buf);
#endif
#endif


counter+=_frames;
}

/*****************************************************************************/
void CGameScene::think_showing_lives(int _frames)
{
	if(m_gamestate!=GAMESTATE_SHOWING_LIVES_BUT_GOING_TO_BOSS_TEXT)
	{
		if(m_showingLivesTimer==0)
		{
			think_playing(0);
		}
		else if((m_showingLivesTimer-TIME_TO_DISPLAY_LIVES_COUNT)*SPEED_OF_FADE>128)
		{
			think_playing(_frames);
		}
	}

	if(PadGetDown(0)&PAD_CROSS&&m_showingLivesTimer<TIME_TO_DISPLAY_LIVES_COUNT)
	{
		m_showingLivesTimer=TIME_TO_DISPLAY_LIVES_COUNT;
		m_player->ignoreNewlyPressedButtonsOnPadThisThink();
	}

	if(_frames>2)
	{
		_frames=2;
	}
	m_showingLivesTimer+=_frames;
	
	if((m_showingLivesTimer-TIME_TO_DISPLAY_LIVES_COUNT)*SPEED_OF_FADE>255)
	{
		if(m_gamestate!=GAMESTATE_SHOWING_LIVES_BUT_GOING_TO_BOSS_TEXT)
		{
			m_gamestate=GAMESTATE_PLAYING;
		}
		else
		{
			m_gamestate=GAMESTATE_BOSS_INTRO;
		}
	}
}

/*****************************************************************************/
void CGameScene::think_playing(int _frames)
{
	if(s_readyToExit)
	{
		return;
	}

	if(s_justHitBossArenaTrigger)
	{
		if(m_gamestate==GAMESTATE_SHOWING_LIVES)
		{
			m_bossText->select();
			m_gamestate=GAMESTATE_SHOWING_LIVES_BUT_GOING_TO_BOSS_TEXT;
			s_justHitBossArenaTrigger=false;
		}
		else if(m_gamestate==GAMESTATE_PLAYING)
		{
			m_gamestate=GAMESTATE_FADING_INTO_BOSS_INTRO;
			CFader::setFadingOut();
			s_justHitBossArenaTrigger=false;
		}
	}


	// Auto-timer stuff
	if(m_levelHasTimer&&
	   !CConversation::isActive()&&!m_pauseMenu->isActive())
	{
		int	oldTimer;
		
		oldTimer=m_timer/55;
		m_timer-=_frames;

		if(oldTimer>m_timer/55)
		{
			CSoundMediator::playSfx(oldTimer>6?CSoundMediator::SFX_BEEP3:CSoundMediator::SFX_BEEP7);
		}

		if(m_timer<0)
		{
			s_levelFinished=true;
		}
	}

	if(s_levelFinished)
	{
		// Do the gameslot stuff..
		CGameSlotManager::GameSlot	*gameSlot;
		int							level,chapter;
		int							openNextLevel,levelToOpen,chapterToOpen;
		gameSlot=CGameSlotManager::getSlotData();
		level=getLevelNumber();
		chapter=getChapterNumber();

		// Open next level?
		gameSlot->levelHasBeenCompleted(chapter-1,level-1);
		if(level!=5&&					// Don't open any levels after finishing a bonus level
		   !(level==4&&chapter==5))		// Don't open any levels after finishing final level
		{
			if(level!=4)
			{
				// Open next level in this chapter..
				levelToOpen=level+1;
				chapterToOpen=chapter;
			}
			else
			{
				// Open first level in next chapter
				levelToOpen=1;
				chapterToOpen=chapter+1;
			}
			gameSlot->levelIsNowOpen(chapterToOpen-1,levelToOpen-1);
			CMapScene::setLevelToStartOn(chapterToOpen-1,levelToOpen-1);
		}

		// New hi-spatula-count?
		if(m_player->getSpatulasHeld()>gameSlot->getSpatulaCollectedCount(chapter-1,level-1))
		{
			gameSlot->setSpatulaCollectedCount(chapter-1,level-1,m_player->getSpatulasHeld(),getTotalSpatCountForThisLevel());
		}

		// Level finished - go to map or fma
		CFmaScene::FMA_SCRIPT_NUMBER	fma;
		fma=Level.getFMAToFollow();
		if(fma!=CFmaScene::FMA_SCRIPT__NONE)
		{
			FmaScene.selectFma(fma);
			GameState::setNextScene(&FmaScene);

			// After the last FMA it goes to save. We then need to make it go to PARTY
			SaveScene.setNextScene(CSaveScene::NEXTSCENE_PARTY);
		}
		else if(level==5)
		{
			GameState::setNextScene(&ShopScene);
		}
		else
		{
			GameState::setNextScene(&MapScene);
		}
		s_readyToExit=true;
		s_levelFinished=false;
		CFader::setFadingOut();
	}
#ifdef __VERSION_DEBUG__
	else if(s_skipToNextLevel)
	{
		// Skip to next level
		bool	Finished=Level.GetNextLevel(s_globalLevelSelectThing);
		shutdownLevel();
		initLevel();
		s_skipToNextLevel=false;
	}
#endif
	else if (s_restartLevel)
	{
		if(getLevelNumber()==5)
		{
			// Bonus level.. go back to map after death
			GameState::setNextScene(&MapScene);
			s_readyToExit=true;
			s_levelFinished=false;
			CFader::setFadingOut();
		}
		else if(m_player->getLivesLeft()>=0)
		{
			respawnLevel();
		}
		else
		{
			s_readyToExit=true;
			GameState::setNextScene(&GameOverScene);
			CFader::setFadingOut();
		}
		s_restartLevel=false;
	}


	if(canPause()&&!m_pauseMenu->isActive())
	{
		if(PadGetDown(0)&PAD_START||!PadIsConnected(0))
		{
			m_pauseMenu->select();
		}
	}

	// paused for pause menu or conversation?
	m_isPaused = m_pauseMenu->isActive() || CConversation::isActive();

	// Conversation think ( with pad debounce stuff.. )
	if(CConversation::isActive())m_player->ignoreNewlyPressedButtonsOnPadThisThink();
	if(!m_pauseMenu->isActive())
	{
		CConversation::think(_frames);
	}
	else
	{
		// Must be a better way to do this? :/
		CConversation::ignoreNewlyPressedButtonsOnPadThisThink();
	}
	
	// Pause menu think ( with pad debounce stuff.. )
	if(m_pauseMenu->isActive())m_player->ignoreNewlyPressedButtonsOnPadThisThink();
	m_pauseMenu->think(_frames);

	if(!CConversation::isActive()&&!m_pauseMenu->isActive())
	{
		DVECTOR camPos;
		CJellyfishGenerator::think( _frames, &Level );
		CThingManager::thinkAllThings(_frames);
		camPos=m_player->getCameraPos();
		CBubicleFactory::setMapOffset(&camPos);
		Level.setCameraCentre(camPos);
		Level.think(_frames);
		thinkCameraShake(_frames);
		m_HealthManager->think(_frames);
		m_HealthManager->checkPlayerCol(getPlayer());

#ifdef __VERSION_DEBUG__
		if(PadGetDown(0)&PAD_R2)
		{
			s_skipToNextLevel=true;
		}
#endif
	}
}


/*****************************************************************************/
int		CGameScene::readyToShutdown()
{
	return s_readyToExit&&!CFader::isFading();
}

/*****************************************************************************/
CPlayer	* CGameScene::getPlayer()
{
	return( m_player );
}

/*****************************************************************************/
int		CGameScene::getChapterNumber()
{
	return Level.getCurrentChapter();
}
int		CGameScene::getLevelNumber()
{
	return Level.getCurrentChapterLevel();
}

int		CGameScene::getTotalSpatCountForThisLevel()
{
	return Level.getTotalSpatCount();
}


/*****************************************************************************/
int		CGameScene::triggerTeleportEffect()
{
	if(m_gamestate==GAMESTATE_PLAYING)
	{
		m_gamestate=GAMESTATE_TELEPORT_START;
		CFader::setFadingOut();
		return true;
	}
	return false;
}


/*****************************************************************************/
void	CGameScene::respawnLevel()
{
	CSoundMediator::setCanPlaySfx(false);
	m_player->respawn();
	Level.respawnLevel();
	m_HealthManager->init();
	if(!s_showBossTextOnRespawn)
	{
		m_gamestate=GAMESTATE_SHOWING_LIVES;
	}
	else
	{
		m_gamestate=GAMESTATE_SHOWING_LIVES_BUT_GOING_TO_BOSS_TEXT;
		m_bossText->select(m_playingBossMusic);
	}
	m_showingLivesTimer=0;
	CSoundMediator::setCanPlaySfx(true);
}


/*****************************************************************************/
void CGameScene::sendEvent( GAME_EVENT evt, CThing *sourceThing )
{
	CThingManager::processEventAllThings(evt, sourceThing);
}


/*****************************************************************************/
void CGameScene::setReadyToExit()
{
	s_readyToExit=true;
	GameState::setNextScene(&FrontEndScene);
	CFrontEndScene::setStartMode(CFrontEndScene::MODE__CHOOSE_SLOT);
	CFader::setFadingOut();
}

/*****************************************************************************/
void CGameScene::setBossHasBeenKilled()
{
	s_bossHasBeenKilled=true;
	s_showBossTextOnRespawn=false;
}
/*****************************************************************************/
void CGameScene::hitBossArenaTrigger()
{
	if(!s_showBossTextOnRespawn)
	{
		s_justHitBossArenaTrigger=true;
		s_showBossTextOnRespawn=true;
	}
}

/*****************************************************************************/
void	CGameScene::initLevel()
{
	g_bossThing=NULL;

	CSoundMediator::setCanPlaySfx(false);
	Level.DisplayLoadingScreen(s_globalLevelSelectThing);
	
	SYSTEM_DBGMSG("InitLevel\n");
	CThingManager::init();

	CConversation::init();
	CConversation::registerConversationLevelScripts( s_globalLevelSelectThing );

	s_levelFinished=false;
#ifdef __VERSION_DEBUG__
	s_skipToNextLevel=false;
#endif
	CActorPool::Reset();
	Level.init(s_globalLevelSelectThing);

	createPlayer();
	m_player->setRespawnPos(Level.getPlayerSpawnPos());
	m_player->init();
	m_player->setFacing(Level.getPlayerStartFacing());
	if(getLevelNumber()==5)
	{
		m_player->setCanExitLevelNow();
	}
	DVECTOR	const &mapSize=Level.getMapSize();
	CPlayer::CameraBox	camBox={0,0,mapSize.vx<<4,mapSize.vy<<4};
	m_player->setCameraBox(camBox);

	s_bossHasBeenKilled=false;
	s_justHitBossArenaTrigger=false;

	if(getLevelNumber()==5&&getChapterNumber()==1)
	{
		m_levelHasTimer=true;
		m_timer=30*55;
	}
	else
	{
		m_levelHasTimer=false;
	}

	CActorPool::SetUpCache();

	m_musicStarted=false;
	SetPadRescan();

	StopLoad();
	SYSTEM_DBGMSG("InitLevelDone\n");
}


/*****************************************************************************/
void	CGameScene::dropHealth(DVECTOR const &Pos,int Amount,int Vel)
{
	m_HealthManager->drop(Pos,Amount,Vel);
}

/*****************************************************************************/
void	CGameScene::shutdownLevel()
{
	CSoundMediator::dumpSong();
	CConversation::shutdown();
	CThingManager::shutdown();
	Level.shutdown();
}

/*****************************************************************************/
void	CGameScene::setCameraShake(s16 X,s16 Y)
{
	s_CamShake.vx=X;
	s_CamShake.vy=Y;
}

/*****************************************************************************/
void	CGameScene::shakeCamera(DVECTOR &CamPos)
{
	CamPos.vx+=s_CamShake.vx;
	CamPos.vy+=s_CamShake.vy;
}

/*****************************************************************************/
void	CGameScene::thinkCameraShake(int _frames)
{
	if (s_CamShake.vx)
	{
		if (s_CamShake.vx<0)
		{
			s_CamShake.vx=-(s_CamShake.vx+1);
		}
		else
		{
			s_CamShake.vx=-(s_CamShake.vx-1);
		}
	}
	if (s_CamShake.vy)
	{
		if (s_CamShake.vy<0)
		{
			s_CamShake.vy=-(s_CamShake.vy+1);
		}
		else
		{
			s_CamShake.vy=-(s_CamShake.vy-1);
		}
	}

}

/*****************************************************************************/
void	CGameScene::setCameraMtx()
{
MATRIX			CamMtx;
		SetIdentTrans(&CamMtx,0,0,RENDER_Z);
		SetGeomScreen(RENDER_Z);
		SetRotMatrix(&CamMtx);
		SetTransMatrix(&CamMtx);

}
