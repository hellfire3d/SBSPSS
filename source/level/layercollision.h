/******************************/
/*** Collision  Layer Class ***/
/******************************/

#ifndef __LAYER_COLLISION_H__
#define __LAYER_COLLISION_H__

#include	<dstructs.h>

/*****************************************************************************/
class CLayerCollision
{
public:
		CLayerCollision(sLayerHdr *Hdr);
virtual	~CLayerCollision();

virtual	void			shutdown();

protected:
		sLayerHdr		*LayerHdr;
		int				MapWidth,MapHeight;

		u8				*Map;
};



/*****************************************************************************/

#endif