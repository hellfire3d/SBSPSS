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
