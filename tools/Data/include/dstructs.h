/***********************/
/*** Data Structures ***/
/***********************/

#ifndef		__DATA_STRUCTS_HEADER__
#define		__DATA_STRUCTS_HEADER__


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
#ifndef	sQuat
struct 	sQuat
{
	s16		vx,vy,vz,vw;
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

//---------------------------------------------------------------------------
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
struct	sTri
{
		u16			P0,P1,P2;				//  6
		u16			TPage;					//  2
		u8			uv0[2];					//  2
		u8			uv1[2];					//  2
		u8			uv2[2];					//  2
		u16			Clut;					//  2
};											// 16

//		u8			r0, g0, b0, code;		//  4
//		u8			r1, g1, b1, Mat;		//  4
//		u8			r2, g2, b2;				//	3
//		s8			OtOfs;					//  1

//---------------------------------------------------------------------------
struct	sQuad
{
		u16			P0,P1,P2,P3;			//  8
		u8			uv0[2];					//  2
		u8			uv1[2];					//  2
		u8			uv2[2];					//  2
		u8			uv3[2];					//  2
		u16			TPage;					//  2
		u16			Clut;					//  2
};											// 20

//		u8			r0, g0, b0, code;		//  4
//		u8			r1, g1, b1, Mat;		//  4
//		u8			r2, g2, b2;				//	3
//		s8			OtOfs;					//  1
//		u8			r3, g3, b3, Pad3;		//  4

//***************************************************************************
/*
struct sWeight
{
		s16		X,Y,Z,VtxNo;
};
*/
//***************************************************************************
/*
struct sBone
{
		SVECTOR		BoneSize;					//  8
		s16			Parent,Idx;
		s32			WeightCount;				//  4
		sWeight		*WeightList;
};											// 16
*/
//***************************************************************************
//***************************************************************************
//***************************************************************************
// Tiles

struct	sTileMapElem
{
	u16		Tile;
};

struct	sTile
{
// 3d Tile
		u16		TriList;
		u16		TriCount;
		u16		QuadList;
		u16		QuadCount;
// 2d Tile
//		s16		Mat2d;
//		s8		XOfs,YOfs;
		u16		TPage;
		u16		Clut;
		u8		uv0[2];
		u8		uv1[2];
		u8		uv2[2];
		u8		uv3[2];

#ifdef	WIN32
bool	operator==(sTile const &v1)	
{
	if (TriCount!=v1.TriCount) return(false);
	if (TriList!=v1.TriList) return(false);
//	if (QuadCount!=v1.QuadCount) return(false);
//	if (QuadList!=v1.QuadList) return(false);

	if (TPage!=v1.TPage) return(false);
//	if (Mat!=v1.Mat) return(false);
	if (uv0[0]!=v1.uv0[0]) return(false);
	if (uv0[1]!=v1.uv0[1]) return(false);
	if (uv1[0]!=v1.uv1[0]) return(false);
	if (uv1[1]!=v1.uv1[1]) return(false);
	if (uv2[0]!=v1.uv2[0]) return(false);
	if (uv2[1]!=v1.uv2[1]) return(false);
	if (uv3[0]!=v1.uv3[0]) return(false);
	if (uv3[1]!=v1.uv3[1]) return(false);
	return(TRUE);
}
#endif
};

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
// Header
struct	sLvlHdr
{
//	sTri	*TriList;
//	sQuad	*QuadList;
//	sVtx	*VtxList;
//	sMat	*MatList;
//	sTile	*TileList;
//	int		LayerCount;
	/*int	LayerOfs[LayerCount]...*/
	u32		TriList;
	u32		QuadList;
	u32		VtxList;
	u32		TileList;
// Layers
	u32		BackLayer;
	u32		MidLayer;
	u32		ActionLayer;
	u32		ForeLayer;
	u32		Pad0;
	u32		Pad1;
	u32		Pad2;
	u32		Pad3;
	u32		Pad4;
	u32		Pad5;
	u32		Pad6;
	u32		Pad7;

};




#endif