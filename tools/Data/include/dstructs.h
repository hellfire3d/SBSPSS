/***********************/
/*** Data Structures ***/
/***********************/

#ifndef		__DATA_STRUCTS_HEADER__
#define		__DATA_STRUCTS_HEADER__


//***************************************************************************
// Taken from Map editor  layerdef.h
enum	PSX_COLLSION_ENUM
{
	COLLISION_TYPE_BLANK					=0,
	COLLISION_TYPE_NORMAL					=0,
	COLLISION_TYPE_DAMAGE					=1,
	COLLISION_TYPE_SLIPPERY					=2,
	COLLISION_TYPE_ELECTRIC					=3,
	COLLISION_TYPE_DESTRUCTABLE_WALL		=4,
	COLLISION_TYPE_SOAKUP					=5,
	COLLISION_TYPE_SOLID					=6,
	COLLISION_TYPE_DEATH_FALL				=7,
	COLLISION_TYPE_DESTRUCTABLE_FLOOR		=8,
	COLLISION_TYPE_DEATH_INSTANT			=9,
	COLLISION_TYPE_MOVE_LEFT				=10,
	COLLISION_TYPE_MOVE_RIGHT				=11,
	COLLISION_TYPE_DEATH_LIQUID				=12,
	COLLISION_TYPE_SB_NOMOVE				=13,

	COLLISION_TYPE_FLAG_SHIFT				=3,
	COLLISION_TYPE_FLAG_NORMAL				=COLLISION_TYPE_NORMAL << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_DAMAGE				=COLLISION_TYPE_DAMAGE << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_SLIPPERY			=COLLISION_TYPE_SLIPPERY << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_ELECTRIC			=COLLISION_TYPE_ELECTRIC << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_DESTRUCTABLE_WALL	=COLLISION_TYPE_DESTRUCTABLE_WALL << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_SOAKUP				=COLLISION_TYPE_SOAKUP << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_SOLID				=COLLISION_TYPE_SOLID << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_DEATH_FALL			=COLLISION_TYPE_DEATH_FALL << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_DESTRUCTABLE_FLOOR	=COLLISION_TYPE_DESTRUCTABLE_FLOOR << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_DEATH_INSTANT		=COLLISION_TYPE_DEATH_INSTANT << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_MOVE_LEFT			=COLLISION_TYPE_MOVE_LEFT << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_MOVE_RIGHT			=COLLISION_TYPE_MOVE_RIGHT << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_DEATH_LIQUID		=COLLISION_TYPE_DEATH_LIQUID << COLLISION_TYPE_FLAG_SHIFT,
	COLLISION_TYPE_FLAG_SB_NOMOVE			=COLLISION_TYPE_SB_NOMOVE << COLLISION_TYPE_FLAG_SHIFT,

	COLLISION_TYPE_MASK						= ((0xff<<COLLISION_TYPE_FLAG_SHIFT)&0xff),
	COLLISION_TILE_MASK						= (0xff - COLLISION_TYPE_MASK)

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
		s16	vx, vy, vz, rgb;

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
        s16			XMin,YMin;//,ZMin;
        s16			XMax,YMax;//,ZMax;
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
		u16			TPage;					//  2
		u8			uv2[2];					//  2
		u16			P2;						//  2
		s16			OTOfs;					//  2
		u8			PolyCode;				//  1
		u8			Pad;					//  1
		u8			C0,C1,C2;
		u8			Pad2;				
};											// 24

//---------------------------------------------------------------------------
struct	sQuad
{
		u16			P0;						//  2
		u16			P1;						//  2
		u8			uv0[2];					//  2
		u16			Clut;					//  2
		u8			uv1[2];					//  2
		u16			TPage;					//  2
		u8			uv2[2];					//  2
		u16			P2;						//  2
		u8			uv3[2];					//  2
		u16			P3;						//  2
		s16			OTOfs;					//  2
		u8			PolyCode;				//  1
		u8			Pad;					//  1
		u8			C0,C1,C2,C3;
};											// 28


struct	sPoly
{
		u16			P0;						//  2
		u16			P1;						//  2
		u16			P2;						//  2
		u16			P3;						//  2

		u8			uv0[2];					//  2
		u16			Clut;					//  2
		u8			uv1[2];					//  2
		u16			TPage;					//  2
		u8			uv2[2];					//  2
		u8			uv3[2];					//  2
		s16			OTOfs;					//  2
		u8			PolyCode;				//  1
		u8			PolyTag;				//  1

};

//***************************************************************************
//*** Game Types and Headers ************************************************
//***************************************************************************
// Tiles 
struct	sTileMapElem
{
		u16		Tile;
//		u16		LightIdx;
};

struct	sElem2d
{
// 2d Tile
		u8		u0,v0;						// 2
		u16		Clut;						// 2
		u32		TPage;						// 4	// due to pre-gen TSPRT tpage
};											// 8

struct	sElem3d
{
// 3d Elem (tile/model)
		u16		TriStart;
		u16		TriCount;
		u16		QuadStart;
		u16		QuadCount;
		u16		VtxIdxStart;
		u16		VtxTriCount;
		u32		Pad;						// for quicker mult
};											// 16

//***************************************************************************
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
// Shade Layer
struct	sLayerShadeBackGfxType
{
		u16			TPage;
		u16			Clut;
		u8			U,V;
		u8			W,H;
};

struct	sLayerShadeBackGfx
{
		u16			Type;
		u16			Trans;
		u16			PosX,PosY;
		s8			Ofs[4][2];
		u8			RGB[4][3];
};

struct	sLayerShadeHdr
{
	u16						BandCount;
	u16						GfxCount;
	u8						RGB[4][3];
	sLayerShadeBackGfxType	*TypeList;
	sLayerShadeBackGfx		*GfxList;
};

//---------------------------------------------------------------------------
struct	sLayerRGBHdr
{
	u32		RGBMap;
	u32		RGBTable;
};
//---------------------------------------------------------------------------
struct	sModel
{
		u16		ElemID;
		u16		Pad;
		sBBox	BBox;
};

//---------------------------------------------------------------------------
// Header
struct	sLevelHdr
{
	u32			BackLayer;
	u32			MidLayer;
	u32			ActionLayer;
	u32			ForeLayer;
	u32			CollisionLayer;
	u32			ActorList;
	u32			ItemList;
	u32			PlatformList;
	u32			TriggerList;
	u32			FXList;
	u32			HazardList;
	u32			RGBLayer;

	u16			PlayerStartX,PlayerStartY;

	sElem2d		*ElemBank2d;
	sElem3d		*ElemBank3d;
	sTri		*TriList;
	sQuad		*QuadList;
	sVtx		*VtxList;
	u16			*VtxIdxList;
	sModel		*ModelList;
};

//***************************************************************************
//***************************************************************************
//***************************************************************************
// Actors
struct	sSpriteFrameGfx
{
		u8			*PAKSpr;			// 4	Needs to be u32, so might as well make it a ptr
		s8			AspectX0,AspectX1;	// 2	Aspect correction value 
		u8			W,H;				// 2
		u8			W0,W1;				// 2	Need these for rotating :o(
		u8			OW,OH;				// 2	So might as well pad with these :o(
};

struct	sSpriteFrame
{
		u16		FrameIdx;
		s8		XOfs,YOfs;

};

struct	sSpriteAnim
{
	u16				FrameCount;	// 2
	u16				Pad;
	sSpriteFrame	*Anim;		// 4
};

struct	sSpriteAnimBank
{
	u16					Clut;			// 2
	u16					AnimCount;		// 2
	u16					FrameCount;		// 2
	u8					MaxW,MaxH;		// 2

	u8					*Palette;		// 4
	sSpriteAnim			*AnimList;		// 4
	sSpriteFrameGfx		*FrameList;		// 4
};


//***************************************************************************
//***************************************************************************
//***************************************************************************
// Things - Must be 4 byte aligned for pos data
enum	THING_FLAG
{
		THING_FLAG_COLLISION=1<<0,
		THING_FLAG_MIRRORX=1<<1,
		THING_FLAG_MIRRORY=1<<2,
};

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
	u16			Type;
	u16			Health;
	u16			AttackStrength;
	u16			Speed;
	u16			TurnRate;
	u8			Flags;
	u8			Angle;
	u8			PointCount;
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
	u16			Gfx;
	u16			Type;
	u16			Speed;
	u16			TurnRate;
	u16			Param0;
	u8			Flags;
	u8			PointCount;
	// Point List...
}; // 10

struct	sThingFX
{
	u16			Type;
	u16			Speed;
	u8			Flags;
	u8			Angle;
	sThingPoint	Pos,Size;
}; // 8

struct	sThingTrigger
{
	u16			Type;
	sThingPoint	Pos;
	u8			Width,Height;
	sThingPoint	TargetPos;
	sThingPoint	TargetSize;
	s16			Val0,Val1;
}; // 12

struct	sThingHazard
{
	u16		Gfx;
	u16		Type;
	u16		Health;
	u16		AttackStrength;
	u16		Speed;
	u16		TurnRate;
	u16		Respawn;
	u8		Flags;
	u8		Angle;
	u8		PointCount;
	// Point List...
}; // 12

//***************************************************************************
//*** FX Stuff **************************************************************
//***************************************************************************
#if	0
enum FX_FLAG
{
	FX_FLAG_LOOP			=1<<0,
	FX_FLAG_COLLIDE_KILL	=1<<1,
	FX_FLAG_HAS_GRAVITY		=1<<2,
	FX_FLAG_INJURE_PLAYER	=1<<3,
	FX_FLAG_TRANS			=1<<4,
};

//***************************************************************************
struct sFXData
{
	s16				Scale;
//	s16				StartFrame,EndFrame;
//	s8				FrameShift;
//	u8				R,G,B;
	u16				Flags;
//	DVECTOR			Velocity;
	u16				Velocity,Heading;
//	u16				EndFX;
};
#endif
//***************************************************************************

#endif