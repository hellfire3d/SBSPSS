/******************************/
/*** Collision  Layer Class ***/
/******************************/

#include 	"system\global.h"
#include	<DStructs.h>
#include 	"utils\utils.h"

#include	"LayerCollision.h"

#ifndef _FILEIO_HEADER_
#include 	"fileio\fileio.h"
#endif


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

		m_collisionTable=CFileIO::loadFile(COLLISION_COLLTAB_DAT);
}

/*****************************************************************************/
CLayerCollision::~CLayerCollision()
{
}

/*****************************************************************************/
void		CLayerCollision::shutdown()
{
	MemFree(m_collisionTable);
}

/*****************************************************************************/
int			CLayerCollision::getHeightFromGround(int _x,int _y,int _maxHeight)
{
	int	mapX,mapY,xFraction,yFraction;
	int	distanceFromGround;
	int	colHeight;
	int	maxHeightToCheck;

	mapX=_x>>4;
	mapY=(_y>>4)*MapWidth;
	xFraction=_x&0x0f;
	yFraction=16-(_y&0x0f);
	distanceFromGround=0;

	colHeight=m_collisionTable[((Map[mapX+mapY]&COLLISION_MASK)*16)+xFraction];
	if(colHeight)
	{
		// Inside a collision block.. find the nearest ground above this point
		maxHeightToCheck=-_maxHeight-16;	// Need to check one block more incase we cross onto a new block
		while(colHeight==16)
		{
			_y-=16;
			mapY-=MapWidth;
			distanceFromGround-=16;
			if(distanceFromGround<=maxHeightToCheck)
			{
				return -_maxHeight;
			}
			colHeight=m_collisionTable[((Map[mapX+mapY]&COLLISION_MASK)*16)+xFraction];
		}
		distanceFromGround+=yFraction-colHeight;
		if(distanceFromGround<-_maxHeight)distanceFromGround=-_maxHeight;

	}
	else
	{
		// Not inside a collision block.. find the nearest ground below this point
		maxHeightToCheck=_maxHeight+16;		// Need to check one block more incase we cross onto a new block
		while(colHeight==0)
		{
			_y+=16;
			mapY+=MapWidth;
			distanceFromGround+=16;
			if(distanceFromGround>=maxHeightToCheck)
			{
				return _maxHeight;
			}
			colHeight=m_collisionTable[((Map[mapX+mapY]&COLLISION_MASK)*16)+xFraction];
		}
		distanceFromGround+=yFraction-colHeight;
		if(distanceFromGround>_maxHeight)distanceFromGround=_maxHeight;
	}

	return distanceFromGround;
}


/*****************************************************************************/
int			CLayerCollision::getCollisionType(int _x,int _y)
{
	int	mapX,mapY,xFraction,yFraction;
	int	distanceFromGround;
	int	block,colHeight;
	int	ret;

	mapX=_x>>4;
	mapY=(_y>>4)*MapWidth;
	xFraction=_x&0x0f;
	yFraction=16-(_y&0x0f);

	block=Map[mapX+mapY];
	colHeight=m_collisionTable[((block&COLLISION_MASK)*16)+xFraction];
	if(colHeight==yFraction)
	{
		ret=block;
	}
	else
	{
		PAUL_DBGMSG("not on ground in getCollisionType()");
		ret=0;
	}

	return ret;
}


/*****************************************************************************/
#ifdef __SHOW_COLLISION__
#include "gfx\prim.h"
#include "pad\pads.h"
#if defined (__USER_paul__) || (__USER_art__)
int	showCollision=true;
#else
int	showCollision=false;
#endif
void CLayerCollision::render(DVECTOR &MapPos)
{
	if(PadGetDown(0)&PAD_TRIANGLE)
	{
		showCollision=!showCollision;
	}
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
				switch((*coll)&COLLISION_MASK)
				{
					case 0:
						break;

					case 1:
					case 2:
					case 3:
					case 4:
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

					case 15:
						f3=GetPrimF3();
						setXY3(f3,x+16,y,
								  x,y,
								  x+16,y+16);
						setRGB0(f3,0,0,90);
						setSemiTrans(f3,true);
						AddPrimToList(f3,0);
						break;

					case 16:
						f3=GetPrimF3();
						setXY3(f3,x,y,
								  x+16,y,
								  x,y+16);
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
