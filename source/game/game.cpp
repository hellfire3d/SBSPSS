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


#include "gfx\actor.h"

int			RenderZ=378;//256; Increased to make depth less, and SB more visible
// Horrible evil bodge 4 Petro, via Gary :o(
#define	USE_GLOBAL_RGB
#ifdef	USE_GLOBAL_RGB
u8		GlobalRGB[4]={127,127,127,127};
int		GlobalRGBSel=0;
char	*GlobalRGBName[4]={"Sprites","Action","Mid","Other"};
int		GlobalRGBX=200;
int		GlobalRGBY=28;
#endif

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

/*****************************************************************************/
typedef struct
{
	u16							m_titleTextId;
	u16							m_instructionsTextId;
	CSoundMediator::SONGID		m_songId;
} BOSS_DATA;

static const BOSS_DATA	s_bossData[]=
{
	{	STR__CHAPTER_1__BOSS_TITLE,	STR__CHAPTER_1__BOSS_INSTRUCTIONS,	CSoundMediator::SONG_CHAPTER1_BOSS			},
	{	STR__CHAPTER_2__BOSS_TITLE,	STR__CHAPTER_2__BOSS_INSTRUCTIONS,	CSoundMediator::SONG_CHAPTER2_BOSS			},
	{	STR__CHAPTER_3__BOSS_TITLE,	STR__CHAPTER_3__BOSS_INSTRUCTIONS,	CSoundMediator::SONG_CHAPTER3_BOSS_ALSEEP	},
	{	STR__CHAPTER_4__BOSS_TITLE,	STR__CHAPTER_4__BOSS_INSTRUCTIONS,	CSoundMediator::SONG_CHAPTER4_BOSS			},
	{	STR__CHAPTER_5__BOSS_TITLE,	STR__CHAPTER_5__BOSS_INSTRUCTIONS,	CSoundMediator::SONG_CHAPTER5_BOSS			},
};


/*****************************************************************************/


CGameScene	GameScene;

/*****************************************************************************/
void 	CGameScene::init()
{
// Setup Constant Camera Matrix
//		SetIdentTrans(&CamMtx,0,0,RenderZ);
//		SetGeomScreen(RenderZ);
//		SetTransMatrix(&CamMtx);		

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

		CFader::setFadingIn();
		CLevel::setIsBossRespawn(false);
		initLevel();

		m_gamestate=GAMESTATE_SHOWING_LIVES;
		m_showingLivesTimer=0;
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
			render_showing_lives();
			break;
		case GAMESTATE_PLAYING:
		case GAMESTATE_FADING_INTO_BOSS_INTRO:
			render_playing();
			break;
		case GAMESTATE_BOSS_INTRO:
		case GAMESTATE_FADING_OUT_OF_BOSS_INTRO:
			render_boss_intro();
			break;
	}
}

/*****************************************************************************/
void CGameScene::render_showing_lives()
{
	int			colour;
	char		buf[20];
	POLY_F4		*f4;
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
	f4=GetPrimF4();
	setXYWH(f4,0,0,512,256);
	setRGB0(f4,colour,colour,colour);
	setShadeTex(f4,0);
	setSemiTrans(f4,1);
	AddPrimToList(f4,0);
	ft3=GetPrimFT3();
	setPolyFT3(ft3);
	setShadeTex(ft3,1);
	setSemiTrans(ft3,1);
	ft3->tpage=2<<5;
	setXY3(ft3,512,512,512,512,512,512);
	AddPrimToList(ft3,0);

	render_playing();
}

/*****************************************************************************/
void CGameScene::render_playing()
{
//		CamMtx.t[2]=ZPos;	// Temp
#ifdef	USE_GLOBAL_RGB
		if (PadIsConnected(1))
		{	
			s_genericFont->setTrans(0);
			s_genericFont->setSMode(0);

			for (int i=0; i<3; i++)
			{
				char	Buf[32];
				if (i==GlobalRGBSel)
					s_genericFont->setColour(255,255,255);
				else
					s_genericFont->setColour(64,64,64);
				
				sprintf(Buf,"%s: %i",GlobalRGBName[i],GlobalRGB[i]);
				s_genericFont->print(GlobalRGBX,GlobalRGBY+(i*16),Buf);
			}
		}
	
#endif
		if(m_levelHasTimer)
		{
			int		timerValue;
			char	buf[10];

			timerValue=m_timer/55;
			if(timerValue<0)timerValue=0;
			sprintf(buf,"%d",timerValue);
			m_scalableFont->print(VidGetScrW()/2,30,buf);
		}

		m_pauseMenu->render();
		CConversation::render();
		CThingManager::renderAllThings();

		setCameraMtx();
//		SetIdentTrans(&CamMtx,0,0,RenderZ);
//		SetGeomScreen(RenderZ);
//		SetRotMatrix(&CamMtx);
//		SetTransMatrix(&CamMtx);

		Level.render();
		m_HealthManager->render();

		CActorPool::CleanUpCache();
}

/*****************************************************************************/
void CGameScene::render_boss_intro()
{
	SpriteBank		*sb;
	sFrameHdr		*fhCorner,*fhSideBorder,*fhTopBorder;
	int				x,y;
	POLY_F4			*f4;
	POLY_G4			*g4;
	const BOSS_DATA	*bd;

	// Scroll effect type thingy stuff
	sb=getSpriteBank();
	fhCorner=sb->getFrameHeader(FRM__HELPBOX1);
	fhSideBorder=sb->getFrameHeader(FRM__HELPBOX2);
	fhTopBorder=sb->getFrameHeader(FRM__HELPBOX3);

	// Corners
	sb->printFT4(fhCorner,  0,  0,false,false,4);
	sb->printFT4(fhCorner,512,  0,true ,false,4);
	sb->printFT4(fhCorner,  0,256,false,true ,4);
	sb->printFT4(fhCorner,512,256,true ,true ,4);

	// Top/bottom
	for(x=fhCorner->W;x<512-fhCorner->W;x+=fhTopBorder->W)
	{
		sb->printFT4(fhTopBorder,x,  0,false,false,4);
		sb->printFT4(fhTopBorder,x,256,false,true ,4);
	}

	// Left/right
	for(y=fhCorner->H;y<256-fhCorner->H;y+=fhSideBorder->H)
	{
		sb->printFT4(fhSideBorder,  0,y,false,false,4);
		sb->printFT4(fhSideBorder,512,y,true ,false,4);
	}

	// Middle
	f4=GetPrimF4();
	setXYWH(f4,fhCorner->W,fhCorner->H,512-(fhCorner->W*2),256-(fhCorner->H*2));
	setRGB0(f4,224,184,107);
	AddPrimToList(f4,5);

	// Background
	g4=GetPrimG4();
	setXYWH(g4,0,0,512,256);
	setRGB0(g4,70,50,60);
	setRGB1(g4,70,50,60);
	setRGB2(g4,50,60,70);
	setRGB3(g4,50,60,70);
	AddPrimToList(g4,5);

	// Instructions..
	bd=&s_bossData[Level.getCurrentChapter()-1];
	m_scalableFont->setColour(255,255,255);
	m_scalableFont->setTrans(0);
	m_scalableFont->setSMode(0);
	m_scalableFont->setScale(300);
	m_scalableFont->setJustification(FontBank::JUST_CENTRE);
	m_scalableFont->setPrintArea(30,0,512-60,256);
	m_scalableFont->print(256-30,25,bd->m_titleTextId);
	m_scalableFont->setPrintArea(0,0,256,512);

	s_genericFont->setColour(255,255,255);
	s_genericFont->setTrans(0);
	s_genericFont->setSMode(0);
	s_genericFont->setJustification(FontBank::JUST_CENTRE);
	s_genericFont->setPrintArea(30,0,512-60,256);
	s_genericFont->print(256-30,60,bd->m_instructionsTextId);
	s_genericFont->setPrintArea(0,0,256,512);
}

/*****************************************************************************/
void	CGameScene::think(int _frames)
{
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
		case GAMESTATE_PLAYING:
			think_playing(_frames);
			break;
		case GAMESTATE_FADING_INTO_BOSS_INTRO:
			if(!CFader::isFading())
			{
				// Swap to the boss tune whilst it's all quiet! :)
				CSoundMediator::stopSong();
				CSoundMediator::setSong(s_bossData[Level.getCurrentChapter()-1].m_songId);
				m_gamestate=GAMESTATE_BOSS_INTRO;
				CFader::setFadingIn();
			}
			break;
		case GAMESTATE_BOSS_INTRO:
			think_boss_intro(_frames);
			break;
		case GAMESTATE_FADING_OUT_OF_BOSS_INTRO:
			if(!CFader::isFading())
			{
				m_gamestate=GAMESTATE_PLAYING;
				CSoundMediator::playSong();
				CFader::setFadingIn();
			}
			break;
	}
}

/*****************************************************************************/
void CGameScene::think_showing_lives(int _frames)
{
	if(m_showingLivesTimer==0)
	{
		think_playing(0);
	}
	else if((m_showingLivesTimer-TIME_TO_DISPLAY_LIVES_COUNT)*SPEED_OF_FADE>128)
	{
		think_playing(_frames);
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
		m_gamestate=GAMESTATE_PLAYING;
	}
}

/*****************************************************************************/
void CGameScene::think_playing(int _frames)
{
#ifdef	USE_GLOBAL_RGB
		if (PadIsConnected(1))
		{	
			if (PadGetDown(1) & PAD_UP)		GlobalRGBSel--;
			if (PadGetDown(1) & PAD_DOWN)	GlobalRGBSel++;
			GlobalRGBSel%=3;

			if(PadGetHeld(1)&PAD_LEFT ) GlobalRGB[GlobalRGBSel]--;
			if(PadGetHeld(1)&PAD_RIGHT ) GlobalRGB[GlobalRGBSel]++;
			GlobalRGB[GlobalRGBSel]&=255;
		}
#endif

	if(s_readyToExit)
	{
		return;
	}

	if(s_justHitBossArenaTrigger)
	{
		m_gamestate=GAMESTATE_FADING_INTO_BOSS_INTRO;
		CFader::setFadingOut();
		s_justHitBossArenaTrigger=false;
	}


	// Auto-timer stuff
	if(m_levelHasTimer)
	{
		m_timer-=_frames;
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
		}
		else if(level==5)
		{
			if(chapter==5)
			{
				ShopScene.setGotoPartyScreen();
			}
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
		if(m_player->getLivesLeft()>=0)
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

	// Conversation think ( with pad debounce stuff.. )
	if(CConversation::isActive())m_player->ignoreNewlyPressedButtonsOnPadThisThink();
	CConversation::think(_frames);
	
	// Pause menu think ( with pad debounce stuff.. )
	if(m_pauseMenu->isActive())m_player->ignoreNewlyPressedButtonsOnPadThisThink();
	m_pauseMenu->think(_frames);

	if(!CConversation::isActive()&&!m_pauseMenu->isActive())
	{
		DVECTOR	camPos;
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
void	CGameScene::think_boss_intro(int _frames)
{
	if(!CFader::isFading())
	{
		if(PadGetDown(0)&PAD_CROSS)
		{
			m_gamestate=GAMESTATE_FADING_OUT_OF_BOSS_INTRO;
			CFader::setFadingOut();
			sendEvent( BOSS_FOUND_EVENT, NULL );
		}
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
void	CGameScene::respawnLevel()
{
	CSoundMediator::setCanPlaySfx(false);
	m_player->respawn();
	Level.respawnLevel();
	m_gamestate=GAMESTATE_SHOWING_LIVES;
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
	CFader::setFadingOut();
}

/*****************************************************************************/
void CGameScene::hitBossArenaTrigger()
{
	s_justHitBossArenaTrigger=true;
}

/*****************************************************************************/
void	CGameScene::initLevel()
{
	CSoundMediator::setCanPlaySfx(false);
	
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
	if(getLevelNumber()==5)
	{
		m_player->setCanExitLevelNow();
	}
	DVECTOR				mapSize=Level.getMapSize();
	CPlayer::CameraBox	camBox={0,0,mapSize.vx<<4,mapSize.vy<<4};
	m_player->setCameraBox(camBox);

	s_bossHasBeenKilled=false;
	s_justHitBossArenaTrigger=false;

	if(getLevelNumber()==5&&getChapterNumber()==3)
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

	SYSTEM_DBGMSG("InitLevelDone\n");
}


/*****************************************************************************/
void	CGameScene::dropHealth(DVECTOR &Pos,int Amount,int Vel)
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
		SetIdentTrans(&CamMtx,0,0,RenderZ);
		SetGeomScreen(RenderZ);
		SetRotMatrix(&CamMtx);
		SetTransMatrix(&CamMtx);

}
