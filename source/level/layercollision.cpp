/******************************/
/*** Collision  Layer Class ***/
/******************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"

#include	"LayerCollision.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerCollision::CLayerCollision(sLayerHdr *Hdr)
{
		LayerHdr=Hdr;
		Map=(u8*)MakePtr(Hdr,sizeof(sLayerHdr));
		MapWidth=LayerHdr->Width;
		MapHeight=LayerHdr->Height;
		printf("COLLISION LAYER = %i %i\n",MapWidth,MapHeight);
}

/*****************************************************************************/
CLayerCollision::~CLayerCollision()
{
}

/*****************************************************************************/
void		CLayerCollision::shutdown()
{
}

/*****************************************************************************/
#ifdef __SHOW_COLLISION__
#include "gfx\prim.h"
int	showCollision=true;
void CLayerCollision::render(DVECTOR &MapPos)
{
	if(showCollision)
	{
		int		x,y;
		int		mapx,mapy;
		int		xoff,yoff;
		u8		*coll;
		POLY_F4	*f4;

		xoff=MapPos.vx&15;
		yoff=MapPos.vy&15;
		mapx=MapPos.vx/16;
		mapy=MapPos.vy/16;
		for(y=-yoff;y<(20*16)-yoff;y+=16)
		{
			coll=&Map[mapx+(mapy*MapWidth)];
			for(x=-xoff;x<(33*16)-xoff;x+=16)
			{
				if(*coll)
				{
					f4=GetPrimF4();
					setXYWH(f4,x,y,16,16);
					setRGB0(f4,0,0,90);
					setSemiTrans(f4,true);
					AddPrimToList(f4,0);
				}
				coll++;
			}
			mapy++;
		}
	}
}
#endif
