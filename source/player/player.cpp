/**************/
/*** Player ***/
/**************/

#include "system\global.h"
#include "Game\Thing.h"
#include "Gfx\Skel.h"
#include "gfx\anim.h"
#include "Player\Player.h"

// to be removed
#include "fileio\fileio.h"
#include "utils\utils.h"
#include "gfx\tpage.h"
#include "gfx\prim.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CPlayer::CPlayer()
{
		SetRenderObj(&Skel);
};

/*****************************************************************************/
CPlayer::~CPlayer()
{
}

/*****************************************************************************/
void	CPlayer::init()
{
sActor3dHdr	*Hdr=Skel.Load(ACTORS_SPONGEBOB_A3D);		
		Skel.Init(Hdr);
		TPLoadTex(ACTORS_SPONGEBOB_TEX);
		Skel.setAnimDatabase(CAnimDB::GetPlayerAnimBank());

}

/*****************************************************************************/
void	CPlayer::shutdown()
{
}

/*****************************************************************************/
void	CPlayer::Animate()
{
		Skel.Animate(this);
}
