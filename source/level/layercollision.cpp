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


// Manual collision height table (pkg)
u8 CLayerCollision::s_collisionTable[]=
{
	 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

	16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,

	 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8,
	 8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1,

	 9, 9,10,10,11,11,12,12,13,13,14,14,15,15,16,16,
	16,16,15,15,14,14,13,13,12,12,11,11,10,10, 9, 9,

	 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,
	16,15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1,
};


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerCollision::CLayerCollision(sLayerHdr *Hdr)
{
		LayerHdr=Hdr;
		Map=(u8*)MakePtr(Hdr,sizeof(sLayerHdr));
		MapWidth=LayerHdr->Width;
		MapHeight=LayerHdr->Height;

int		TabH=MapHeight+(COL_Y_OFS*2);

		ColYTable=(int*)MemAlloc(TabH*sizeof(int),"ColYTable");
		ASSERT(ColYTable);

		for (int i=0; i<TabH; i++)
		{
			int	Y=i-COL_Y_OFS;
			if (Y<0)								// extend off top
			{
				ColYTable[i]=0;
			}
			else
			{
				if (Y>=MapHeight) Y=MapHeight-1;	// extend off bottom
				ColYTable[i]=Y*MapWidth;
			}
			
		}
}

/*****************************************************************************/
CLayerCollision::~CLayerCollision()
{
}

/*****************************************************************************/
void		CLayerCollision::shutdown()
{
	MemFree(ColYTable);
}

/*****************************************************************************/
int			CLayerCollision::getHeightFromGround(int _x,int _y,int _maxHeight)
{
int	xFraction,yFraction;
int	distanceFromGround;
int	colHeight;
int	maxHeightToCheck;
u8	*MapPtr,T;

	xFraction=_x&0x0f;
	yFraction=16-(_y&0x0f);
	MapPtr=getMapPtr(_x,_y);
	distanceFromGround=0;

	T=*MapPtr;
	colHeight=s_collisionTable[((T&COLLISION_TILE_MASK)*16)+xFraction];
	if(colHeight)
	{
		// Inside a collision block.. find the nearest ground above this point
		maxHeightToCheck=-_maxHeight-16;	// Need to check one block more incase we cross onto a new block
		while(colHeight==16)
		{
			MapPtr-=MapWidth;
			distanceFromGround-=16;
			if(distanceFromGround<=maxHeightToCheck)
			{
				return -_maxHeight;
			}
			T=*MapPtr;
			colHeight=s_collisionTable[((T&COLLISION_TILE_MASK)*16)+xFraction];
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
			MapPtr+=MapWidth;
			distanceFromGround+=16;
			if(distanceFromGround>=maxHeightToCheck)
			{
				return _maxHeight;
			}
			T=*MapPtr;
			colHeight=s_collisionTable[((T&COLLISION_TILE_MASK)*16)+xFraction];
		}
		distanceFromGround+=yFraction-colHeight;
		if(distanceFromGround>_maxHeight)distanceFromGround=_maxHeight;
	}

	return distanceFromGround;
}

/*****************************************************************************/
int			CLayerCollision::getHeightFromGroundExcluding(int _x,int _y,int _exclusion,int _maxHeight=32)
{
int	xFraction,yFraction;
int	distanceFromGround;
int	colHeight;
int	maxHeightToCheck;
u8	*MapPtr,T;

	xFraction=_x&0x0f;
	yFraction=16-(_y&0x0f);
	MapPtr=getMapPtr(_x,_y);
	distanceFromGround=0;

	T=*MapPtr;
	colHeight=s_collisionTable[((T&COLLISION_TILE_MASK)*16)+xFraction];
	if ( (T & COLLISION_TYPE_MASK) == _exclusion )
	{
		colHeight = 0;
	}
	if(colHeight)
	{
		// Inside a collision block.. find the nearest ground above this point
		maxHeightToCheck=-_maxHeight-16;	// Need to check one block more incase we cross onto a new block
		while(colHeight==16)
		{
			MapPtr-=MapWidth;
			distanceFromGround-=16;
			if(distanceFromGround<=maxHeightToCheck)
			{
				return -_maxHeight;
			}
			T=*MapPtr;
			colHeight=s_collisionTable[((T&COLLISION_TILE_MASK)*16)+xFraction];
			if ( (T & COLLISION_TYPE_MASK) == _exclusion )
			{
				colHeight = 0;
			}
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
			MapPtr+=MapWidth;
			distanceFromGround+=16;
			if(distanceFromGround>=maxHeightToCheck)
			{
				return _maxHeight;
			}
			T=*MapPtr;
			colHeight=s_collisionTable[((T&COLLISION_TILE_MASK)*16)+xFraction];
			if ( (T & COLLISION_TYPE_MASK) == _exclusion )
			{
				colHeight = 0;
			}
		}
		distanceFromGround+=yFraction-colHeight;
		if(distanceFromGround>_maxHeight)distanceFromGround=_maxHeight;
	}

	return distanceFromGround;
}

/*****************************************************************************/
int			CLayerCollision::getHeightFromGroundCart(int _x,int _y,int _maxHeight)
{
	int	mapX,mapY,xFraction,yFraction;
	int	distanceFromGround;
	int	colHeight;
	int	maxHeightToCheck;

	mapX=_x>>4;
//	mapY=(_y>>4)*MapWidth;
	mapY=GetYPos(_y>>4);
	xFraction=_x&0x0f;
	yFraction=16-(_y&0x0f);
	distanceFromGround=0;

	colHeight=s_collisionTable[((Map[mapX+mapY]&COLLISION_TILE_MASK)*16)+xFraction];
	if ( (Map[mapX+mapY] & COLLISION_TYPE_MASK) == COLLISION_TYPE_FLAG_DAMAGE ||
			(Map[mapX+mapY] & COLLISION_TYPE_MASK) == COLLISION_TYPE_FLAG_DEATH_FALL )
	{
		colHeight = 0;
	}
	if(colHeight)
	{
		// Inside a collision block.. find the nearest ground above this point
		maxHeightToCheck=-_maxHeight-16;	// Need to check one block more incase we cross onto a new block
		while(colHeight==16)
		{
			mapY-=MapWidth;
			distanceFromGround-=16;
			if(distanceFromGround<=maxHeightToCheck)
			{
				return -_maxHeight;
			}
			colHeight=s_collisionTable[((Map[mapX+mapY]&COLLISION_TILE_MASK)*16)+xFraction];
			if ( (Map[mapX+mapY] & COLLISION_TYPE_MASK) == COLLISION_TYPE_FLAG_DAMAGE ||
					(Map[mapX+mapY] & COLLISION_TYPE_MASK) == COLLISION_TYPE_FLAG_DEATH_FALL )
			{
				colHeight = 0;
			}
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
			mapY+=MapWidth;
			distanceFromGround+=16;
			if(distanceFromGround>=maxHeightToCheck)
			{
				return _maxHeight;
			}
			colHeight=s_collisionTable[((Map[mapX+mapY]&COLLISION_TILE_MASK)*16)+xFraction];
			if ( (Map[mapX+mapY] & COLLISION_TYPE_MASK) == COLLISION_TYPE_FLAG_DAMAGE ||
					(Map[mapX+mapY] & COLLISION_TYPE_MASK) == COLLISION_TYPE_FLAG_DEATH_FALL )
			{
				colHeight = 0;
			}
		}
		distanceFromGround+=yFraction-colHeight;
		if(distanceFromGround>_maxHeight)distanceFromGround=_maxHeight;
	}

	return distanceFromGround;
}

/*****************************************************************************/
#ifdef __SHOW_COLLISION__
#include "gfx\prim.h"
#include "pad\pads.h"
typedef struct{int r,g,b;} colrgb;
const colrgb s_typeColours[]={
	{90,90,90},
	{90,0,0},	// damage
	{0,90,0},	// slippery
	{90,0,90},	// electric
	{0,90,90},	// sticky
	{0,0,90},	// water
	{45,45,90},	// solid
	{90,45,45},	// death
};
typedef enum {SHOW_NONE,SHOW_OPAQUE,SHOW_SEMITRANS}ColShowType;
#if defined (__USER_paul__)
ColShowType	showCollision=SHOW_OPAQUE;
#elif defined (__USER_art__) || defined (__USER_sbart__)
ColShowType	showCollision=SHOW_SEMITRANS;
#else
ColShowType showCollision=SHOW_NONE;
#endif
void CLayerCollision::render(DVECTOR &MapPos)
{
	if(PadGetDown(0)&PAD_L2)
	{
		showCollision=(ColShowType)(showCollision+1);
		if(showCollision>SHOW_SEMITRANS)showCollision=SHOW_NONE;
	}
	if(showCollision!=SHOW_NONE)
	{
		int				semiTrans;
		int				x,y;
		int				mapx,mapy;
		int				xoff,yoff;
		u8				*coll;
		POLY_F4			*f4;
		POLY_F3			*f3;
		const colrgb	*colour;

		semiTrans=showCollision==SHOW_SEMITRANS;
		xoff=MapPos.vx&15;
		yoff=MapPos.vy&15;
		mapx=MapPos.vx/16;
		mapy=MapPos.vy/16;
		for(y=-yoff;y<(20*16)-yoff;y+=16)
		{
//			coll=&Map[mapx+(mapy*MapWidth)];
			coll=&Map[mapx+(GetYPos(mapy))];
			for(x=-xoff;x<(33*16)-xoff;x+=16)
			{
				colour=&s_typeColours[((*coll)&COLLISION_TYPE_MASK)>>COLLISION_TYPE_FLAG_SHIFT];
				switch((*coll)&COLLISION_TILE_MASK)
				{
					case 0:
						break;

					case 1:
						f4=GetPrimF4();
						setXYWH(f4,x,y,16,16);
						setRGB0(f4,colour->r,colour->g,colour->b);
						setSemiTrans(f4,semiTrans);
						AddPrimToList(f4,0);
						break;

					case 2:
						f3=GetPrimF3();
						setXY3(f3,x+16,y+16,
								  x,y+16,
								  x+16,y+8);
						setRGB0(f3,colour->r,colour->g,colour->b);
						setSemiTrans(f3,semiTrans);
						AddPrimToList(f3,0);
						break;

					case 3:
						f3=GetPrimF3();
						setXY3(f3,x,y+16,
								  x+16,y+16,
								  x,y+8);
						setRGB0(f3,colour->r,colour->g,colour->b);
						setSemiTrans(f3,semiTrans);
						AddPrimToList(f3,0);
						break;

					case 4:
						f4=GetPrimF4();
						setXY4(f4,x+16,y,
								  x,y+8,
								  x+16,y+16,
								  x,y+16);
						setRGB0(f4,colour->r,colour->g,colour->b);
						setSemiTrans(f4,semiTrans);
						AddPrimToList(f4,0);
						break;

					case 5:
						f4=GetPrimF4();
						setXY4(f4,x,y,
								  x+16,y+8,
								  x,y+16,
								  x+16,y+16);
						setRGB0(f4,colour->r,colour->g,colour->b);
						setSemiTrans(f4,semiTrans);
						AddPrimToList(f4,0);
						break;

					case 6:
						f3=GetPrimF3();
						setXY3(f3,x+16,y+16,
								  x,y+16,
								  x+16,y);
						setRGB0(f3,colour->r,colour->g,colour->b);
						setSemiTrans(f3,semiTrans);
						AddPrimToList(f3,0);
						break;

					case 7:
						f3=GetPrimF3();
						setXY3(f3,x,y+16,
								  x+16,y+16,
								  x,y);
						setRGB0(f3,colour->r,colour->g,colour->b);
						setSemiTrans(f3,semiTrans);
						AddPrimToList(f3,0);
						break;
/*
					case 15:
						f3=GetPrimF3();
						setXY3(f3,x+16,y,
								  x,y,
								  x+16,y+16);
						setRGB0(f3,colour->r,colour->g,colour->b);
						setSemiTrans(f3,semiTrans);
						AddPrimToList(f3,0);
						break;

					case 16:
						f3=GetPrimF3();
						setXY3(f3,x,y,
								  x+16,y,
								  x,y+16);
						setRGB0(f3,colour->r,colour->g,colour->b);
						setSemiTrans(f3,semiTrans);
						AddPrimToList(f3,0);
						break;
*/
					default:
						f4=GetPrimF4();
						setXYWH(f4,x,y,16,16);
						setRGB0(f4,255,0,0);
						setSemiTrans(f4,semiTrans);
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
