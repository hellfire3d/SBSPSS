/******************************/
/*** Collision  Layer Class ***/
/******************************/

#ifndef __LAYER_COLLISION_H__
#define __LAYER_COLLISION_H__

#include	<dstructs.h>


//#if	!defined(__USER_CDBUILD__)
#define __SHOW_COLLISION__
//#endif


/*****************************************************************************/
class CLayerCollision
{
public:
		CLayerCollision(sLayerHdr *Hdr);
virtual	~CLayerCollision();

virtual	void			shutdown();

		u8				Get(int X,int Y)					{return(Map[X+(Y*MapWidth)]&COLLISION_TILE_MASK);}
		int				getHeightFromGround(int _x,int _y,int _maxHeight=32);
		int				getCollisionBlock(int _x,int _y)	{return Map[(_x>>4)+((_y>>4)*MapWidth)];}

#ifdef __SHOW_COLLISION__
		void			render(DVECTOR &MapPos);
#endif


protected:
		sLayerHdr		*LayerHdr;
		int				MapWidth,MapHeight;


		u8				*Map;

		static u8		s_collisionTable[];
};



/*****************************************************************************/

#endif