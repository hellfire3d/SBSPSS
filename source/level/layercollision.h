/******************************/
/*** Collision  Layer Class ***/
/******************************/

#ifndef __LAYER_COLLISION_H__
#define __LAYER_COLLISION_H__

#include	<dstructs.h>


#ifdef __VERSION_DEBUG__
#define __SHOW_COLLISION__
#endif


/*****************************************************************************/
class CLayerCollision
{
public:
		CLayerCollision(sLayerHdr *Hdr);
virtual	~CLayerCollision();

virtual	void			shutdown();

		u8				Get(int X,int Y)	{return(Map[X+(Y*MapWidth)]&COLLISION_MASK);}
		int				getHeightFromGround(int _x,int _y,int _maxHeight=32);
		int				getHeightFromCeiling(int _x,int _y,int _maxHeight=32);
		int				getCollisionType(int _x,int _y);

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