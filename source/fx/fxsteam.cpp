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

static	s16			SteamSize=1;
const	s16			SteamAngleInc=1111;

int	SLife=64;
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFXSteam::init(DVECTOR const &_Pos)
{
		CFXCloud::init(_Pos);
		Life=SLife;

		RBase=255;
		GBase=255;
		BBase=255;
		RInc=-8;
		GInc=-8;
		BInc=-8;

		Trans=3;

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
		Vel.vx=0;
		Vel.vy=-Size;
}

/*****************************************************************************/
/*** Think *******************************************************************/
/*****************************************************************************/
int	LifeStart=32;

void	CFXSteam::think(int _frames)
{
		SetSize(SteamSize);
		CFXCloud::think(_frames);

		if (!DieOut)
		{
			setHead(Vel,Vel,LifeStart);
		}

		Life--;
		if (!Life) DieOut=true;

}

