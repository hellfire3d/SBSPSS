/*=========================================================================

	trigger source file

	Author:		Davey Boy
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2001 Climax Development Ltd

===========================================================================*/

#ifndef __TRIGGER_TRIGGER_HEADER__
#include "triggers\trigger.h"
#endif

#ifndef	__TRIGGERS_TLEVEXIT_H__
#include "triggers\tlevexit.h"
#endif

#ifndef	__TRIGGERS_TRESTART_H__
#include "triggers\trestart.h"
#endif

#ifndef	__TRIGGERS_TTELEPRT_H__
#include "triggers\tteleprt.h"
#endif

#ifndef	__TRIGGERS_TCAMLOCK_H__
#include "triggers\tcamlock.h"
#endif

#ifndef	__TRIGGERS_TWATER_H__
#include "triggers\twater.h"
#endif

#ifndef	__TRIGGERS_TPLATFRM_H__
#include "triggers\tplatfrm.h"
#endif

#ifndef __TRIGGERS_TGARYGO_H__
#include "triggers\tgarygo.h"
#endif

#ifndef __TRIGGERS_TFEMIT_H__
#include "triggers\tfemit.h"
#endif

#ifndef __TRIGGERS_TIFEMIT_H__
#include "triggers\tifemit.h"
#endif

#ifndef __TRIGGERS_TBEMIT_H__
#include "triggers\tbemit.h"
#endif

#ifndef __TRIGGERS_TGGLEFT_H__
#include "triggers\tggleft.h"
#endif

#ifndef __TRIGGERS_TGGRIGHT_H__
#include "triggers\tggright.h"
#endif

#ifndef __TRIGGERS_TGSTOP_H__
#include "triggers\tgstop.h"
#endif

#ifndef __TRIGGERS_THAZARD_H__
#include "triggers\thazard.h"
#endif

#ifndef __TRIGGERS_THAZWALK_H__
#include "triggers\thazwalk.h"
#endif

#ifndef __TRIGGERS_TBGEYSER_H__
#include "triggers\tbgeyser.h"
#endif

#ifndef __TRIGGERS_TSECRET_H__
#include "triggers\tsecret.h"
#endif

#ifndef __TRIGGERS_TSEMIT_H__
#include "triggers\tsemit.h"
#endif

#ifndef __TRIGGERS_TSSWITCH_H__
#include "triggers\tsswitch.h"
#endif

#ifndef __TRIGGERS_TGBOWL_H__
#include "triggers\tgbowl.h"
#endif

#ifndef __TRIGGERS_TWDRIPEMIT_H__
#include "triggers\twdripemit.h"
#endif

#ifndef __TRIGGERS_TADRIPEMIT_H__
#include "triggers\tadripemit.h"
#endif

#ifndef __TRIGGERS_TODRIPEMIT_H__
#include "triggers\todripemit.h"
#endif

#ifndef __TRIGGERS_TLDRIPEMIT_H__
#include "triggers\tldripemit.h"
#endif

#ifndef __TRIGGERS_TABEMIT_H__
#include "triggers\tabemit.h"
#endif

#ifndef __TRIGGERS_TOBEMIT_H__
#include "triggers\tobemit.h"
#endif

#ifndef __TRIGGERS_TLBEMIT_H__
#include "triggers\tlbemit.h"
#endif

#ifndef __TRIGGERS_TSDOWNEMIT_H__
#include "triggers\tsdownemit.h"
#endif

#ifndef __TRIGGERS_TSLEFTEMIT_H__
#include "triggers\tsleftemit.h"
#endif

#ifndef __TRIGGERS_TSRIGHTEMIT_H__
#include "triggers\tsrightemit.h"
#endif

#ifndef __TRIGGERS_TWINDUP_H__
#include "triggers\twindup.h"
#endif

#ifndef __TRIGGERS_TWINDDOWN_H__
#include "triggers\twinddown.h"
#endif

#ifndef __TRIGGERS_TWINDLEFT_H__
#include "triggers\twindleft.h"
#endif

#ifndef __TRIGGERS_TWINDRIGHT_H__
#include "triggers\twindright.h"
#endif

#ifndef	__TRIGGERS_TOILGEYSER_H__
#include "triggers\toilgeyser.h"
#endif

#ifndef	__TRIGGERS_TBOSS_H__
#include "triggers\tboss.h"
#endif

#ifndef __TRIGGERS_TSPEECH_H__
#include "triggers\tspeech.h"
#endif

#ifndef	__TRIGGERS_TLOOK_H__
#include "triggers\tlook.h"
#endif

#ifndef __TRIGGERS_TBUTTFLY_H__
#include "triggers\tbuttfly.h"
#endif

#ifndef __GAME_GAME_H__
#include "game\game.h"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CTrigger	*CTrigger::Create(int Type)
{
CTrigger	*trigger;

	trigger = (CTrigger*)CThingManager::GetThing(CThing::TYPE_TRIGGER,Type);
	if (!trigger)
	switch( Type )
	{
		// Exit trigger
		case TRIGGER_EXIT:
			trigger=(CTrigger*)new ("LevelExitTrigger") CLevelExitTrigger();
			break;

		// Level respawn trigger
		case TRIGGER_RESPAWN:
			trigger=(CRestartPointTrigger*)new ("RestartTrigger") CRestartPointTrigger();
			break;

		// Teleport trigger
		case TRIGGER_TELEPORT:
			trigger=(CTeleportTrigger*)new ("TeleportTrigger") CTeleportTrigger();
			break;

		// Camera lock trigger
		case TRIGGER_CAMLOCK:
			trigger=(CCameraLockTrigger*)new ("CameraLockTrigger") CCameraLockTrigger();
			break;

		// In/Out of water triggers
		case TRIGGER_INWATER:
			trigger=(CInWaterTrigger*)new ("InWaterTrigger") CInWaterTrigger();
			break;
		case TRIGGER_OUTWATER:
			trigger=(COutOfWaterTrigger*)new ("OutOfWaterTrigger") COutOfWaterTrigger();
			break;

		// Gary start trigger
		case TRIGGER_GARY_START:
			trigger=(CGaryStartTrigger*)new("GaryStartTrigger") CGaryStartTrigger();
			break;

		// Gary stop trigger
		case TRIGGER_GARY_STOP:
			trigger=(CGaryStopTrigger*)new("GaryStopTrigger") CGaryStopTrigger();
			break;

		// Gary go left trigger
		case TRIGGER_GARY_GO_LEFT:
			trigger=(CGaryGoLeftTrigger*)new("GaryGoLeftTrigger") CGaryGoLeftTrigger();
			break;

		// Gary go right trigger
		case TRIGGER_GARY_GO_RIGHT:
			trigger=(CGaryGoRightTrigger*)new("GaryGoRightTrigger") CGaryGoRightTrigger();
			break;

		// Gary bowl trigger
		case TRIGGER_GARY_BOWL:
			trigger=(CGaryBowlTrigger*)new("GaryBowlTrigger") CGaryBowlTrigger();
			break;

		// Flame emitter
		case TRIGGER_FLAME_EMITTER:
			trigger=(CFlameEmitterTrigger*)new( "FlameEmitterTrigger") CFlameEmitterTrigger();
			break;

		case TRIGGER_INTERMITTENT_FLAME_EMITTER:
		case TRIGGER_INTERMITTENT_LEFT_FLAME_EMITTER:
		case TRIGGER_INTERMITTENT_DOWN_FLAME_EMITTER:
			trigger=(CIntermittentFlameEmitterTrigger*)new( "IntermittentFlameEmitterTrigger") CIntermittentFlameEmitterTrigger();
			break;

		// Bubble emitter
		case TRIGGER_BUBBLE_EMITTER:
			trigger=(CBubbleEmitterTrigger*)new( "BubbleEmitterTrigger") CBubbleEmitterTrigger();
			break;

		// Acid bubble emitter
		case TRIGGER_ACID_BUBBLE_EMITTER:
			trigger=(CAcidBubbleEmitterTrigger*)new( "AcidBubbleEmitterTrigger") CAcidBubbleEmitterTrigger();
			break;

		// Oil bubble emitter
		case TRIGGER_OIL_BUBBLE_EMITTER:
			trigger=(COilBubbleEmitterTrigger*)new( "OilBubbleEmitterTrigger") COilBubbleEmitterTrigger();
			break;

		// Lava bubble emitter
		case TRIGGER_LAVA_BUBBLE_EMITTER:
			trigger=(CLavaBubbleEmitterTrigger*)new( "LavaBubbleEmitterTrigger") CLavaBubbleEmitterTrigger();
			break;

		// Bubble geyser emitter
		case TRIGGER_BUBBLE_GEYSER_EMITTER:
			trigger=(CBubbleGeyserEmitterTrigger*)new( "BubbleGeyserEmitterTrigger") CBubbleGeyserEmitterTrigger();
			break;

		// Steam emitter
		case TRIGGER_STEAM_EMITTER:
			trigger=(CSteamEmitterTrigger*)new("SteamEmitterTrigger") CSteamEmitterTrigger();
			break;

		// Down steam emitter
		case TRIGGER_STEAM_DOWN_EMITTER:
			trigger=(CSteamDownEmitterTrigger*)new("SteamDownEmitterTrigger") CSteamDownEmitterTrigger();
			break;

		// Left steam emitter
		case TRIGGER_STEAM_LEFT_EMITTER:
			trigger=(CSteamLeftEmitterTrigger*)new("SteamLeftEmitterTrigger") CSteamLeftEmitterTrigger();
			break;

		// Right steam emitter
		case TRIGGER_STEAM_RIGHT_EMITTER:
			trigger=(CSteamRightEmitterTrigger*)new("SteamRightEmitterTrigger") CSteamRightEmitterTrigger();
			break;

		// Water drip emitter
		case TRIGGER_WATER_DRIP_EMITTER:
			trigger=(CWaterDripEmitterTrigger*)new("WaterDripEmitterTrigger") CWaterDripEmitterTrigger();
			break;

		// Acid drip emitter
		case TRIGGER_ACID_DRIP_EMITTER:
			trigger=(CAcidDripEmitterTrigger*)new("AcidDripEmitterTrigger") CAcidDripEmitterTrigger();
			break;

		// Oil drip emitter
		case TRIGGER_OIL_DRIP_EMITTER:
			trigger=(COilDripEmitterTrigger*)new("OilDripEmitterTrigger") COilDripEmitterTrigger();
			break;

		// Lava drip emitter
		case TRIGGER_LAVA_DRIP_EMITTER:
			trigger=(CLavaDripEmitterTrigger*)new("LavaDripEmitterTrigger") CLavaDripEmitterTrigger();
			break;

		case TRIGGER_PLATFORM:
			trigger = (CPlatformTrigger*)new ("PlatformTrigger") CPlatformTrigger();
			break;

		case TRIGGER_HAZARD:
			trigger = (CHazardTrigger*)new("HazardTrigger") CHazardTrigger();
			break;

		case TRIGGER_HAZARD_WALK:
			trigger = (CHazardWalkTrigger*)new("HazardWalkTrigger") CHazardWalkTrigger();
			break;

		// Steam switch emitter
		case TRIGGER_STEAM_SWITCH_EMITTER:
			trigger=(CSteamSwitchEmitterTrigger*)new("SteamSwitchEmitterTrigger") CSteamSwitchEmitterTrigger();
			break;

		// Wind up
		case TRIGGER_WIND_UP:
			trigger = (CWindUpTrigger*)new("WindUpTrigger") CWindUpTrigger();
			break;

		// Wind down
		case TRIGGER_WIND_DOWN:
			trigger = (CWindDownTrigger*)new("WindDownTrigger") CWindDownTrigger();
			break;

		// Wind up
		case TRIGGER_WIND_LEFT:
			trigger = (CWindLeftTrigger*)new("WindLeftTrigger") CWindLeftTrigger();
			break;

		// Wind up
		case TRIGGER_WIND_RIGHT:
			trigger = (CWindRightTrigger*)new("WindRightTrigger") CWindRightTrigger();
			break;
		// Oil Geyser
		case TRIGGER_OIL_GEYSER:
			trigger = (COilGeyserTrigger*)new("OilGeyserTrigger") COilGeyserTrigger();
			break;
		// Boss Arena
		case TRIGGER_BOSS_ARENA:
			trigger = (CBossArenaTrigger*)new("BossArenaTrigger") CBossArenaTrigger();
			break;
		// Secret Area
		case TRIGGER_SECRET_AREA:
			trigger = (CSecretAreaTrigger*)new("SecretAreaTrigger") CSecretAreaTrigger();
			break;

		case TRIGGER_SPEECH_FIRST_NET:
		{
			trigger = (CSpeechTrigger*)new("SpeechTrigger") CSpeechTrigger();
			CSpeechTrigger *speechTrigger = (CSpeechTrigger*) trigger;
			speechTrigger->setData(0);
			break;
		}

		case TRIGGER_SPEECH_FIRST_CORAL:
		{
			trigger = (CSpeechTrigger*)new("SpeechTrigger") CSpeechTrigger();
			CSpeechTrigger *speechTrigger = (CSpeechTrigger*) trigger;
			speechTrigger->setData(1);
			break;
		}

		case TRIGGER_SPEECH_GARY_CH2L1:
		{
			trigger = (CSpeechTrigger*)new("SpeechTrigger") CSpeechTrigger();
			CSpeechTrigger *speechTrigger = (CSpeechTrigger*) trigger;
			speechTrigger->setData(2);
			break;
		}

		case TRIGGER_SPEECH_BREAK_FLOOR:
		{
			trigger = (CSpeechTrigger*)new("SpeechTrigger") CSpeechTrigger();
			CSpeechTrigger *speechTrigger = (CSpeechTrigger*) trigger;
			speechTrigger->setData(3);
			break;
		}

		case TRIGGER_SPEECH_BUBBLE_GEYSER:
		{
			trigger = (CSpeechTrigger*)new("SpeechTrigger") CSpeechTrigger();
			CSpeechTrigger *speechTrigger = (CSpeechTrigger*) trigger;
			speechTrigger->setData(4);
			break;
		}

		case TRIGGER_SPEECH_RISING_PLATFORM:
		{
			trigger = (CSpeechTrigger*)new("SpeechTrigger") CSpeechTrigger();
			CSpeechTrigger *speechTrigger = (CSpeechTrigger*) trigger;
			speechTrigger->setData(5);
			break;
		}

		case TRIGGER_SPEECH_FIRST_BUBBLE:
		{
			trigger = (CSpeechTrigger*)new("SpeechTrigger") CSpeechTrigger();
			CSpeechTrigger *speechTrigger = (CSpeechTrigger*) trigger;
			speechTrigger->setData(6);
			break;
		}

		case TRIGGER_SPEECH_USE_BUBBLE:
		{
			trigger = (CSpeechTrigger*)new("SpeechTrigger") CSpeechTrigger();
			CSpeechTrigger *speechTrigger = (CSpeechTrigger*) trigger;
			speechTrigger->setData(7);
			break;
		}

		case TRIGGER_SPEECH_WEIGHT:
		{
			trigger = (CSpeechTrigger*)new("SpeechTrigger") CSpeechTrigger();
			CSpeechTrigger *speechTrigger = (CSpeechTrigger*) trigger;
			speechTrigger->setData(8);
			break;
		}

		case TRIGGER_CAMERAYPOSITIONTRIGGER:
			trigger = (CLookTrigger*)new("LookTrigger") CLookTrigger();
			break;

		case TRIGGER_BUTTERFLY_ON:
			trigger = (CButterflyTrigger*)new("ButterflyTrigger") CButterflyTrigger();
			break;

		default:
			trigger=NULL;
	}
	ASSERT( trigger);

	trigger->setThingSubType(Type);
	trigger->init();

	return( trigger);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTrigger	*CTrigger::Create(sThingTrigger *ThisTrigger)
{
CTrigger	*trigger=Create(ThisTrigger->Type);

			trigger->setPositionAndSize(ThisTrigger->Pos.X<<4,ThisTrigger->Pos.Y<<4,ThisTrigger->Width<<4,ThisTrigger->Height<<4);
			trigger->setTargetBox(ThisTrigger->TargetPos.X<<4,ThisTrigger->TargetPos.Y<<4,ThisTrigger->TargetSize.X<<4,ThisTrigger->TargetSize.Y<<4);
			trigger->setVal(ThisTrigger->Val0);

			switch( ThisTrigger->Type )
			{
				case TRIGGER_INTERMITTENT_LEFT_FLAME_EMITTER:
				{
					CIntermittentFlameEmitterTrigger *m_flameEmitter = (CIntermittentFlameEmitterTrigger *) trigger;

					m_flameEmitter->setHeading( 3072 );
					m_flameEmitter->setSize( ThisTrigger->Height<<4, ThisTrigger->Width<<4 );

					break;
				}

				case TRIGGER_INTERMITTENT_DOWN_FLAME_EMITTER:
				{
					CIntermittentFlameEmitterTrigger *m_flameEmitter = (CIntermittentFlameEmitterTrigger *) trigger;

					m_flameEmitter->setHeading( 2048 );

					break;
				}

				default:
					break;
			}

		return( trigger);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	CTrigger::init()
{
	CTriggerThing::init();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	CTrigger::shutdown()
{
	CTriggerThing::shutdown();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	CTrigger::think(int _frames)
{
		CTriggerThing::think(_frames);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	CTrigger::render()
{
		CTriggerThing::render();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void	CTrigger::setPositionAndSize(int _x,int _y,int _w,int _h)
{
	Pos.vx=_x+(_w/2);
	Pos.vy=_y+(_h/2);
	setCollisionSize(_w,_h);
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CTrigger::setTargetBox(int _x,int _y,int _w,int _h)
{
	m_boxX1=_x;
	m_boxY1=_y;
	m_boxX2=_x+_w;
	m_boxY2=_y+_h;
}

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void	CTrigger::setVal(int _val)
{
	m_val0=_val;
}

