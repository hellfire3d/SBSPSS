/************************/
/*** Back Layer Class ***/
/************************/

#ifndef __LAYER_BACK_H__
#define __LAYER_BACK_H__


/*****************************************************************************/
/*
struct	sBackSpriteInfo
{
		int			Frame;
		int			Flags;
		u16			Trans;
};

enum	BackSpriteFlag
{
};

struct	sBackSprite
{
		int			Type;
		POLY_GT4	Poly;
		DVECTOR		Pos,PosInc;
		DVECTOR		Scale,ScaleInc;
		int			W,H;
		s16			Angle,AngleInc;
};

struct sBackRGBTable
{
		u8	R,G,B,P;
};
*/
/*****************************************************************************/
class SpriteBank;

class CLayerBack : public CLayerTile
{
public:
		enum
		{
			MOVE_SHIFT=4,
		};

		CLayerBack(sLevelHdr *LevelHdr,sLayerHdr *Hdr);
		~CLayerBack();

		void	init(DVECTOR &MapPos,int Shift);
		void	shutdown();
		void	think(DVECTOR &MapPos);
		void	render();

protected:
		sLayerShadeHdr	*Data;
		int				XOfs,YOfs;
		int				PosDx,PosDy;
		int				BandCount,BandHeight;
		POLY_G4			Band[LAYER_SHADE_RGB_MAX-1];
};



/*****************************************************************************/

#endif