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
// biped bone IDs
/*
enum BONE_NAME
{
	BIP0,
	BIP01_PELVI,
	BIP01_SPIN,
	BIP01_SPINE,
	BIP01_NEC,
	BIP01_HEA,
	BIP01_L_CLAVICL,
	BIP01_L_UPPERAR,
	BIP01_L_FOREAR,
	BIP01_L_HAN,
	BIP01_R_CLAVICL,
	BIP01_R_UPPERAR,
	BIP01_R_FOREAR,
	BIP01_R_HAN,
	BIP01_L_THIG,
	BIP01_L_CAL,
	BIP01_L_FOO,
	BIP01_L_TOE,
	BIP01_R_THIG,
	BIP01_R_CAL,
	BIP01_R_FOO,
	BIP01_R_TOE0,
	MAX_BONE
};
*/

//***************************************************************************
//*** Base Types ************************************************************
//***************************************************************************
#ifndef	sQuat
struct 	sQuat
{
	s16		vx,vy,vz,vw;
#ifdef	WIN32
bool	operator==(sQuat const &v1)	{return((vx==v1.vx) && (vy==v1.vy) && (vz==v1.vz) && (vw==v1.vw));}
#endif
};
#endif

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
struct	sTexInfo	// Basically same as PSX RECT
{
		s16		x, y, w, h;
};

//***************************************************************************
struct sWeight
{
		s16		vx,vy,vz,VtxNo;				//  8
};

//***************************************************************************
struct sBone
{
		sVtx		BoneSize;				//  8
		s16			Parent;					//  2
		s16			TriStart;				//  2
		s16			TriCount;				//  2
		s16			VtxCount;				//  2
};											// 16
/*
struct sBone
{
		sVtx		BoneSize;				//  8
		s16			Parent,Idx;				//  4
		s32			WeightCount;			//  4
		sWeight		*WeightList;			//  4
};											// 20
*/

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

enum	TILE3D_FLAGS
{
	TILE3D_FLAGS_F=0,
	TILE3D_FLAGS_U,
	TILE3D_FLAGS_D,
	TILE3D_FLAGS_L,
	TILE3D_FLAGS_R,
//	TILE3D_FLAGS_B,	// Should never have back facing polys
	TILE3D_FLAGS_MAX
};

struct	sTileMapElem
{
		u16		Tile;
};

struct	sTileMapElem3d : public sTileMapElem
{
		u16		Flags;
};

/*
struct	sTileTable
{
		u16		TriList;
		u16		TriCount;
};
*/
struct	sTile
{
// 3d Tile
//		sTileTable	TileTable[TILE3D_FLAGS_MAX];	// 20 (4*5)
		u16		TriStart;							// 2
		u16		TileTable[TILE3D_FLAGS_MAX];		// 10
// 2d Tile
		u8		u0,v0;								// 2
		u16		Clut;								// 2
		u16		TPage;								// 2
		u16		Pad;	// :o( need this?			// 2

#ifdef	WIN32
//bool	operator==(sTile const &v1)	{return(false);}
#endif
};													// 20

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
	u32		Pad1;
	u32		Pad2;
	u32		Pad3;
	u32		Pad4;
	u32		Pad5;
	u32		Pad6;
	u32		Pad7;

};

//---------------------------------------------------------------------------
// TileBank
struct	sTileBankHdr
{
	u32		TriList;
	u32		QuadList;
	u32		VtxList;
	u32		TileList;
};


//***************************************************************************
//***************************************************************************
//***************************************************************************
// Actors

struct	sActor3dHdr
{
	u16		BoneCount;
	u16		TriCount;
	u16		QuadCount;
//	u16		WeightCount;
	u16		VtxCount;

	sBone		*BoneList;
	sTri		*TriList;
	sQuad		*QuadList;
//	sWeight		*WeightList;
	sVtx		*VtxList;
	sTexInfo	*TexInfo;
};

//***************************************************************************
// Anim
typedef	u16	AnimIdx;

struct	sAnim3dFileHdr
{
	u16		BoneCount;
	u16		AnimCount;
	sQuat	*QuatTable;
//	Anim Hdrs....
};

struct	sAnim3dHdr
{
	u16		FrameCount;
	u16		Pad;
	AnimIdx	*Anim;
	s32		*Move;
};


#endif