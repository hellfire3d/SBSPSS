/***********************/
/*** Data Structures ***/
/***********************/

#ifndef		__DATA_STRUCTS_HEADER__
#define		__DATA_STRUCTS_HEADER__

//***************************************************************************
// Taken from Map editor  layerdef.h
enum	PSX_COLLSION_ENUM
{
	COLLISION_TYPE_NORMAL			=0,
	COLLISION_TYPE_DAMAGE			=1,
	COLLISION_TYPE_SLIPPERY			=2,
	COLLISION_TYPE_ELECTRIC			=3,
	COLLISION_TYPE_STICKY			=4,
	COLLISION_TYPE_WATER			=5,

	COLLISION_TYPE_FLAG_SHIFT		=5,
	COLLISION_TYPE_FLAG_NORMAL		=COLLISION_TYPE_NORMAL << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_DAMAGE		=COLLISION_TYPE_DAMAGE << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_SLIPPERY	=COLLISION_TYPE_SLIPPERY << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_ELECTRIC	=COLLISION_TYPE_ELECTRIC << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_STICKY		=COLLISION_TYPE_STICKY << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_WATER		=COLLISION_TYPE_WATER << COLLISION_TYPE_FLAG_SHIFT,

	COLLISION_TYPE_MASK				= (0xff << COLLISION_TYPE_FLAG_SHIFT)&0xff,
	COLLISION_MASK					= (0xff-COLLISION_TYPE_MASK)

};
enum	PSX_DATA_ENUM
{
	LAYER_SHADE_RGB_MAX=4,
};


//***************************************************************************
//*** Base Types ************************************************************
//***************************************************************************
struct sShortXYZ
{
		s16		vx,vy,vz;
};

//***************************************************************************
struct	sVtx
{
		s16	vx, vy, vz, pad;

#ifdef	WIN32
bool	operator==(sVtx const &v1)	{return((vx==v1.vx) && (vy==v1.vy) && (vz==v1.vz));}
#endif
};

#define	NormalScale	4096
struct	sNormal
{
		s16		nX,nY,nZ,pD;
};

//***************************************************************************
struct  sBBox
{
        s16			XMin,YMin,ZMin;
        s16			XMax,YMax,ZMax;
};

//***************************************************************************

struct	sMat
{
		u16			TPage;
		u16			Clut;
//		s32			DblFlag;
};

//***************************************************************************
//*** Poly Types ************************************************************
//***************************************************************************
struct	sTri
{
		u16			P0;						//  2
		u16			P1;						//  2
		u8			uv0[2];					//  2
		u16			Clut;					//  2
		u8			uv1[2];					//  2
		u16			TPage;					//  2	(Or material)
		u8			uv2[2];					//  2
		u16			P2;						//  2
};											// 16

//---------------------------------------------------------------------------
struct	sQuad
{
		u16			P0;						//  2
		u16			P1;						//  2
		u16			P2;						//  2
		u16			P3;						//  2
		u8			uv0[2];					//  2
		u16			Clut;					//  2
		u8			uv1[2];					//  2
		u16			TPage;					//  2	(Or material)
		u8			uv2[2];					//  2
		u8			uv3[2];					//  2
};											// 20

//***************************************************************************
//*** Game Types and Headers ************************************************
//***************************************************************************
// Maps
typedef	u16	sTileMapElem;	// Tile or Tri Start

struct	sTile
{
// 2d Tile
		u8		u0,v0;								// 2
		u16		Clut;								// 2
		u16		TPage;								// 2
		u16		Pad;	// :o( need this?			// 2

};													// 8

//---------------------------------------------------------------------------
// Layers
struct	sLayerHdr
{
	int		Type;
	int		SubType;
	int		Width;
	int		Height;

	/*int	TileData[W][H];....*/
};

//---------------------------------------------------------------------------
struct	sLayer3d
{
	u16		TriCount;
	u16		QuadCount;
	u16		VtxCount;
	u16		Pad;

	u32		TriList;
	u32		QuadList;
	u32		VtxList;
};

//---------------------------------------------------------------------------
// Shade Layer
struct	sLayerShade
{
	u32		Ofs;
	u8		RGB[4];
};
struct	sLayerShadeHdr
{
	u16				Count;
	sLayerShade		Data[4];
};

//---------------------------------------------------------------------------
// Header
struct	sLvlHdr
{
	u32		BackLayer;
	u32		MidLayer;
	u32		ActionLayer;
	u32		ForeLayer;
	u32		CollisionLayer;
	u32		ActorList;
	u32		ItemList;
	u32		PlatformList;
	u32		Pad4;
	u32		Pad5;
	u32		Pad6;
	u32		Pad7;

};

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Actors
struct	sSpriteFrame
{
	u8			*PAKSpr;	// 4
	s8			XOfs,YOfs;	// 2
	u8			W,H;		// 2
};

struct	sSpriteAnim
{
	u16			FrameCount;	// 2
	u16			Pad;
	u16			*Anim;		// 4
};

struct	sSpriteAnimBank
{
	u16				ColorCount;		// 2
	u16				AnimCount;		// 2
	u16				FrameCount;		// 2
	u16				Pad;			// 2

	u8				*Palette;		// 4
	sSpriteAnim		*AnimList;		// 4
	sSpriteFrame	*FrameList;		// 4
};


//***************************************************************************
//***************************************************************************
//***************************************************************************
// Things - Must be 4 byte aligned for pos data
struct	sThingHdr
{
	u16		Count;
	u16		Pad;
};

struct	sThingPoint
{
	u16		X,Y;
};

struct	sThingActor
{
	u16		Type;
	u16		Health;
	u16		AttackStrength;
	u16		Speed;
	u16		TurnRate;
	u8		Flags;
	u8		PointCount;
	// Point List...
}; // 12

struct	sThingItem
{
	u16			Type;
	u16			Pad; // Poo!
	sThingPoint	Pos;
};

struct	sThingPlatform
{
	u16		Type;
	u16		Speed;
	u16		TurnRate;
	u8		Flags;
	u8		PointCount;
	// Point List...
}; // 10

//***************************************************************************
#endif