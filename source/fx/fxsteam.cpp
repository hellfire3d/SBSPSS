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

#include	"FX\FXSteam.h"

static const int		Size=2;
static const int		AngleInc=999;
static const int		ShadeBase=255;
static const int		ShadeDec=8;
static const int		ShadeDieDec=24;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXSteam::init(DVECTOR const &_Pos)
{
		CFXTrail::init(_Pos);

		Angle=0;
		Trans=3;
		ShadeDec=ShadeDec;
		DieOut=false;
		SetSize(Size);
}

/*****************************************************************************/
void	CFXSteam::shutdown()
{
		CFX::shutdown();
}

/*****************************************************************************/
void	CFXSteam::SetSize(int Size)
{
		ScaleInc=(4096/LIST_SIZE)*Size;
		BaseVel.vx=0;
		BaseVel.vy=-Size;
}

/*****************************************************************************/
void	CFXSteam::setDie()
{
	CFXTrail::setDie();
	for (int i=0; i<LIST_SIZE; i++)
	{ // Set drift off
		sList	&ThisElem=List[i];
		ThisElem.Vel.vx+=getRndRange(9)-4;	// give it x motion
	}
	ShadeDec=ShadeDieDec;
	ScaleInc=0;
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXSteam::think(int _frames)
{
int		TotalLife=0;

		CFX::think(_frames);

		Angle+=AngleInc;
		Angle&=4095;
		if (!DieOut)
		{ // Replace Head
			DVECTOR	Vel=BaseVel;
			Vel.vx+=getRndRange(3)-1;

			sList	&Head=moveHead();
			Head.Ofs=Vel;
			Head.Vel=Vel;
			Head.Frame=FRM__SMOKE;
			Head.Shade=ShadeBase;
			Head.Scale=ScaleInc;
			Head.Angle=getRndRange(ONE);
		}

// Move em all
int		Head=HeadIdx;
		for (int i=0; i<ListCount; i++)
		{
			sList	&ThisElem=List[Head];
			Head++;
			Head&=LIST_SIZE-1;

			ThisElem.Ofs.vx+=ThisElem.Vel.vx;
			ThisElem.Ofs.vy+=ThisElem.Vel.vy;

			ThisElem.Angle+=AngleInc;
			ThisElem.Angle&=4095;
			ThisElem.Scale+=ScaleInc;
			if (ThisElem.Scale>ONE*2) ThisElem.Scale=ONE*2;
					
			ThisElem.Shade-=ShadeDec;
			if (ThisElem.Shade<0) ThisElem.Shade=0;
			TotalLife+=ThisElem.Shade;
		}

		if (DieOut && TotalLife==0)
		{
			setToShutdown();
		}
}

