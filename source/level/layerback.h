/************************/
/*** Back Layer Class ***/
/************************/

#ifndef __LAYER_BACK_H__
#define __LAYER_BACK_H__


/*****************************************************************************/
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

/*****************************************************************************/
class SpriteBank;

class CLayerBack : public CLayerTile
{
public:
		enum
		{
			MOVE_SHIFT=4,
			SPRITE_MAX=16,

			NO_SPIN=1<<0,
			NO_SCALE=1<<1,
			NO_MOVE=1<<2,
			NO_COLOR=1<<3,
		};

		enum	BackSpriteType
		{
			FLOWER=0,
			GHOST,
			PUMPKIN,
			BUBBLE,
			BUBBLESMALL,
			FISHBONE,
		};

		CLayerBack(sLayerHdr *Hdr,sTile *TileBank);
		~CLayerBack();

		void	init(DVECTOR &MapPos,int Shift);
		void	SetFrames(int Spr0,int Spr1);
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

static	sBackRGBTable	BackRGBTable[];
static	sBackSpriteInfo	InfoTab[];

};



/*****************************************************************************/

#endif