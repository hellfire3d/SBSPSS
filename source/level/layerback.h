/************************/
/*** Back Layer Class ***/
/************************/

#ifndef __LAYER_BACK_H__
#define __LAYER_BACK_H__


/*****************************************************************************/
struct	sBackSprite
{
		POLY_GT4	Poly;
		DVECTOR		Pos,PosInc;
		DVECTOR		Scale,ScaleInc;
		int			W,H;
		s16			Angle,AngleInc;
};

/*****************************************************************************/
class SpriteBank;

class CLayerBack : public CLayerTile
{
public:
		enum
		{
			MOVE_SHIFT=4,
			SPRITE_MAX=16,
		};

		CLayerBack(sLayerHdr *Hdr,sTile *TileBank);
		~CLayerBack();

		void	init(DVECTOR &MapPos,int Shift);
		void	SetFrames(int Frm0,int Frm1);
		void	shutdown();
		void	think(DVECTOR &MapPos);
		void	render();

protected:
		void	InitSprite(sBackSprite *SpritePtr);

		sLayerShadeHdr	*Data;
		int				XOfs,YOfs;
		int				PosDx,PosDy;
		int				BandCount,BandHeight;
		POLY_G4			Band[LAYER_SHADE_RGB_MAX-1];

		SpriteBank		*Sprites;
		sBackSprite		SpriteList[SPRITE_MAX];
		int				SprFrame[2];
};



/*****************************************************************************/

#endif