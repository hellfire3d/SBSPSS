/*******************************/
/*** Action Tile Layer Class ***/
/*******************************/

#ifndef __LAYER_ACTION_H__
#define __LAYER_ACTION_H__


/*****************************************************************************/
class CLayerAction : public CLayerTile
{
public:
		CLayerAction(sLayerHdr *Hdr,sTile *TileList,sTri *TriList,sQuad *QuadList,sVtx *VtxList);
		~CLayerAction();

		void	init(VECTOR &MapPos,int Shift,int Width,int Height);
		void	shutdown();
		void	render();

protected:

};



/*****************************************************************************/

#endif