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

static	const s16	SteamSize=4;
static	const s16	SteamAngleInc=999;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXSteam::init(DVECTOR const &_Pos)
{
		CFXTrail::init(_Pos);

		ColBase=255;
		ColInc=-8;
		Trans=3;
		DieOut=false;
		SetSize(SteamSize);
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
		AngleInc=SteamAngleInc;
		BaseVel.vx=0;
		BaseVel.vy=-Size;
}

/*****************************************************************************/
void	CFXSteam::setDie()
{
	CFXTrail::setDie();
	for (int i=0; i<LIST_SIZE; i++)
	{
		sList	&ThisElem=List[i];
		ThisElem.Vel.vx+=getRndRange(9)-4;	// give is x motion
		ThisElem.Vel.vy+=getRndRange(ThisElem.Vel.vy/2);	// Slow down y Inc
	}
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
void	CFXSteam::think(int _frames)
{
		CFX::think(_frames);

// Move em all
int		Head=HeadIdx;
		for (int i=0; i<LIST_SIZE; i++)
		{
			sList	&ThisElem=List[Head];
			Head++;
			Head&=LIST_SIZE-1;

			ThisElem.Ofs.vx+=ThisElem.Vel.vx;
			ThisElem.Ofs.vy+=ThisElem.Vel.vy;
		}

		if (DieOut)
		{
			int	Col;
			Col=ColBase-16;
			if (Col<0) Col=0;
			ColBase=Col;
		}
		else
		{
			DVECTOR	Vel=BaseVel;
			Vel.vx+=getRndRange(3)-1;

			setHead(Vel,Vel,FRM__SMOKE,1);
			if (ListCount<LIST_SIZE) ListCount++;
		}
}

