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
#ifdef __USER_paul__
int	showCollision=true;
#else
int	showCollision=false;
#endif
void CLayerCollision::render(DVECTOR &MapPos)
{
	if(showCollision)
	{
		int		x,y;
		int		mapx,mapy;
		int		xoff,yoff;
		u8		*coll;
		POLY_F4	*f4;
		POLY_F3	*f3;

		xoff=MapPos.vx&15;
		yoff=MapPos.vy&15;
		mapx=MapPos.vx/16;
		mapy=MapPos.vy/16;
		for(y=-yoff;y<(20*16)-yoff;y+=16)
		{
			coll=&Map[mapx+(mapy*MapWidth)];
			for(x=-xoff;x<(33*16)-xoff;x+=16)
			{
				switch(*coll)
				{
					case 0:
						break;

					case 1:
					case 2:
						f4=GetPrimF4();
						setXYWH(f4,x,y,16,16);
						setRGB0(f4,0,0,90);
						setSemiTrans(f4,true);
						AddPrimToList(f4,0);
						break;

					case 5:
						f3=GetPrimF3();
						setXY3(f3,x+16,y+16,
								  x,y+16,
								  x+16,y+8);
						setRGB0(f3,0,0,90);
						setSemiTrans(f3,true);
						AddPrimToList(f3,0);
						break;

					case 6:
						f3=GetPrimF3();
						setXY3(f3,x,y+16,
								  x+16,y+16,
								  x,y+8);
						setRGB0(f3,0,0,90);
						setSemiTrans(f3,true);
						AddPrimToList(f3,0);
						break;

					case 9:
						f4=GetPrimF4();
						setXY4(f4,x+16,y,
								  x,y+8,
								  x+16,y+16,
								  x,y+16);
						setRGB0(f4,0,0,90);
						setSemiTrans(f4,true);
						AddPrimToList(f4,0);
						break;

					case 10:
						f4=GetPrimF4();
						setXY4(f4,x,y,
								  x+16,y+8,
								  x,y+16,
								  x+16,y+16);
						setRGB0(f4,0,0,90);
						setSemiTrans(f4,true);
						AddPrimToList(f4,0);
						break;

					case 13:
						f3=GetPrimF3();
						setXY3(f3,x+16,y+16,
								  x,y+16,
								  x+16,y);
						setRGB0(f3,0,0,90);
						setSemiTrans(f3,true);
						AddPrimToList(f3,0);
						break;

					case 14:
						f3=GetPrimF3();
						setXY3(f3,x,y+16,
								  x+16,y+16,
								  x,y);
						setRGB0(f3,0,0,90);
						setSemiTrans(f3,true);
						AddPrimToList(f3,0);
						break;

					default:
						f4=GetPrimF4();
						setXYWH(f4,x,y,16,16);
						setRGB0(f4,200,0,0);
						setSemiTrans(f4,true);
						AddPrimToList(f4,0);
						break;
				}
				coll++;
			}
			mapy++;
		}
	}
}
#endif
