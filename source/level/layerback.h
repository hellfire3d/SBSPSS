/************************/
/*** Back Layer Class ***/
/************************/

#ifndef __LAYER_BACK_H__
#define __LAYER_BACK_H__


/*****************************************************************************/
class CLayerBack : public CLayerTile
{
public:
		CLayerBack(sLayerHdr *Hdr,sTile *TileBank);
		~CLayerBack();

		void	init(DVECTOR &MapPos,int Shift);
		void	shutdown();
		void	think(DVECTOR &MapPos);
		void	render();

protected:

		sLayerShadeHdr	*Data;
		int				YOfs;
		int				BandCount,BandHeight;
		POLY_G4			Band[LAYER_SHADE_RGB_MAX-1];

};



/*****************************************************************************/

#endif