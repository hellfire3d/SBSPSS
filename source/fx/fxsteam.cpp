/*************/
/*** Steam ***/
/*************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"gfx\sprbank.h"
#include	<sprites.h>
#include	"level\level.h"

#include	"FX\FXBaseTrail.h"
#include	"FX\FXSteam.h"

static const	int		DefSize=4*ONE;
static const	int		DefAngleInc=999;
static const	int		DefShadeBase=255;
static const	int		DefShadeDec=8;
static const	int		DefShadeDieDec=24;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXSteam::init(DVECTOR const &_Pos)
{
		CFXBaseTrail::init(_Pos);

		Trans=3;
		ShadeDec=DefShadeDec;
		DieOut=false;
		SetSize(DefSize);
		IsHorizontal=false;
}

/*****************************************************************************/
void	CFXSteam::SetSize(int Size)
{
		Size>>=12;
		ScaleInc=(4096/LIST_SIZE)*Size;
		BaseVel.vx=0;
		BaseVel.vy=-Size;
}

/*****************************************************************************/
void	CFXSteam::SetVel(DVECTOR const &Velocity)
{
	BaseVel.vx=Velocity.vx;
	BaseVel.vy=Velocity.vy;
}

/*****************************************************************************/
void	CFXSteam::killFX()
{
		DieOut=true;
		for (int i=0; i<LIST_SIZE; i++)
		{ // Set drift off
			sList	&ThisElem=List[i];
			ThisElem.Vel.vx+=getRndRange(9)-4;	// give it x motion
		}
		ShadeDec=DefShadeDieDec;
		ScaleInc=0;

		if( m_soundId != NOT_PLAYING )
		{
			CSoundMediator::stopAndUnlockSfx(m_soundId);
			m_soundId=NOT_PLAYING;
		}
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXSteam::think(int _frames)
{
		CFX::think(_frames);

		if (!DieOut)
		{ // Replace Head
			DVECTOR	Vel=BaseVel;
			if ( IsHorizontal )
			{
				Vel.vy+=getRndRange(3)-1;
			}
			else
			{
				Vel.vx+=getRndRange(3)-1;
			}

			sList	&Head=moveHead();
			Head.Ofs=Vel;
			Head.Vel=Vel;
			Head.Frame=FRM__SMOKE;
			Head.Shade=DefShadeBase;
			Head.Scale=ScaleInc;
			Head.Angle=getRndRange(ONE);
		}

// Move em all
int		Head=HeadIdx;
int		TotalLife=0;

		for (int i=0; i<ListCount; i++)
		{
			sList	&ThisElem=List[Head];
			Head++;
			Head&=LIST_SIZE-1;

			ThisElem.Ofs.vx+=ThisElem.Vel.vx;
			ThisElem.Ofs.vy+=ThisElem.Vel.vy;

			ThisElem.Angle+=DefAngleInc;
			ThisElem.Angle&=4095;
			ThisElem.Scale+=ScaleInc;
			ThisElem.Shade-=ShadeDec;
			if (ThisElem.Shade<0) ThisElem.Shade=0;
			TotalLife+=ThisElem.Shade;
		}

		if(m_soundId==NOT_PLAYING)
		{
			m_soundId=CSoundMediator::playSfx( CSoundMediator::SFX_HAZARD__STEAM,true,true);
		}
		else
		{
			if(!CSoundMediator::isSfxStillPlaying(m_soundId))
			{
				CSoundMediator::stopAndUnlockSfx(m_soundId);
				m_soundId=NOT_PLAYING;
			}
		}

		if (DieOut && TotalLife==0)
		{
			setToShutdown();
		}
}

