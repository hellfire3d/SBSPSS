//************************************************************************** 
//* Asciiexp.h	- Katana File Exporter
//* 
//* By Tim Swann
//* Climax Development
//*
//* July 7, 1998	TS Initial coding
//*
//* Class definition 
//*
//* Copyright (c) 1998, All Rights Reserved. 
//***************************************************************************

#ifndef __ASCIIEXP__H
#define __ASCIIEXP__H

#define		MAXCOUNT	(256*16)
#include	<fcntl.h>
#include	<io.h>
#include	<Max.h>
#include	"Resource.h"
#include	"Istdplug.h"
#include	"Stdmat.h"
#include	"Decomp.h"
#include	"Shape.h"
#include	"Interpik.h"

//#include	"Asciitok.h"

#include	"PhyExp.h"
#include    "triobjed.h"
#include "ffd.h"
#include "Bmmlib.h"
#include "bipexp.h"
//#include	<Kamui.h>
//#include	<NinjaWin.h>
//#include	<Nindows.h>
//#include	<NjDef.h>

typedef unsigned int Uint32;
typedef unsigned short Uint16;
typedef unsigned char Uint8;
typedef bool Bool;


extern ClassDesc *	GetAsciiExpDesc();
extern TCHAR *		GetString(int id);
extern HINSTANCE	hInstance;
extern DWORD WINAPI	fn(LPVOID arg);


//#define	MAKE_LONG( a,b,c,d )	(a | (b<<8) | (c<<16) | (d<<24))
#define MAKE_LONG(a,b,c,d) { a, b, c, d, 0}
#define	MAX_FILE_CHUNKS			(4096*2)

#define OMNI_LIGHT		0	// Omnidirectional
#define TSPOT_LIGHT		1	// Targeted
#define DIR_LIGHT		2	// Directional
#define FSPOT_LIGHT		3	// Free
#define TDIR_LIGHT		4   // Targeted directional

#define GIN_FLAG_USE_TEXTURE  1
#define GIN_FLAG_DOUBLE_SIDE  2
#define GIN_FLAG_IGNORE_LIGHT 4
#define GIN_FLAG_USE_ENVTEXTURE 32

const   Uint32  MAX_NODES					= 1000; // MAX NODES IN SCENE
const	Uint32	MAX_CHUNKFILE_CHUNK_DEPTH	= 20;	// MAXIMUM NESTED DEPTH OF CHUNKS IN A CHUNKFILE:
const	Uint32	MAX_NAME_LENGTH				= 32;	// MAXIMUM LENGTH OF A NAME STRING
const	Uint32	ID_SIZE						= 4;	// SIZE OF CHUNK ID

// ANIMATION CHANNEL DEFINITIONS
const	Uint32	CHANNEL_TYPE_NONE		=	0;		// NO CHANNEL APPLIED
const	Uint32	CHANNEL_TYPE_VERTEX		=	1;		// VERTEX ANIMATION
const	Uint32	CHANNEL_TYPE_BONE		=	2;		// BONE ANIMATION
const	Uint32	CHANNEL_TYPE_WEIGHTS	=	3;		// BONE VERTEX WEIGHT ANIMATION
const	Uint32	CHANNEL_TYPE_NORMALS	=	4;		// NORMAL ANIMATION
const   Uint32  CHANNEL_TYPE_FFD		=	5;		// FFD ANIMATION
const   Uint32  CHANNEL_TYPE_NODE		=	6;		// NODE ANIMATION
const   Uint32  CHANNEL_TYPE_TREE		=	7;		// NODE ANIMATION


const	Uint32	CHANNEL_INFO_POSITION   =   0;		// NO CHANNEL APPLIED
const	Uint32	CHANNEL_INFO_ROTATION   =	1;		// VERTEX ANIMATION
const	Uint32	CHANNEL_INFO_SCALE		=	2;		// BONE ANIMATION
const	Uint32	CHANNEL_INFO_ALL		=	3;		// BONE ANIMATION

// COUNT OF NUMBER OF FLOATS FOR EACH ANIMATION CHANNEL TYPE
const	Uint32	ChannelTypeSize[] = { 0, 3, 7, 1, 3 };


const char	FILE_ID[]			=	MAKE_LONG( 'G', 'I', 'N', '4' );
const char	OBJ_ID[]			=	MAKE_LONG( 'O', 'B', 'J', '4' );
const char	POLY_ID[]			=	MAKE_LONG( 'P', 'O', 'L', 'Y' );
const char	UV_ID[]				=	MAKE_LONG( 'M', 'A', 'P', '4' );
const char	MESH_ID[]			=	MAKE_LONG( 'M', 'E', 'S', 'H' );
const char	POINTS_ID[]			=	MAKE_LONG( 'P', 'T', 'S', '4' );
const char	NORMAL_ID[]			=	MAKE_LONG( 'N', 'O', 'R', 'M' );
const char	MODEL_ID[]			=	MAKE_LONG( 'M', 'O', 'D', '4' );
const char	TEXTURE_ID[]		=	MAKE_LONG( 'T', 'E', 'X', '4' );
const char	MATERIAL_ID[]		=	MAKE_LONG( 'M', 'A', 'T', '4' );
const char	CAMERA_ID[]			=	MAKE_LONG( 'C', 'A', 'M', '4' );
const char	LIGHT_ID[]			=	MAKE_LONG( 'L', 'G', 'H', 'T' );
const char	BONE_ID[]			=	MAKE_LONG( 'B', 'O', 'N', 'E' );
const char	ANIM_ID[]			=	MAKE_LONG( 'A', 'N', 'I', 'M' );
const char	FFD_ID[]			=	MAKE_LONG( 'F', 'F', 'D', '4' );
const char	VCOL_ID[]			=	MAKE_LONG( 'V', 'C', 'O', 'L' );
const char	PART_ID[]			=	MAKE_LONG( 'P', 'A', 'R', 'T' );
const char	VIS_ID[]			=	MAKE_LONG( 'V', 'I', 'S', '4' );
const char	KEY_ID[]			=	MAKE_LONG( 'K', 'E', 'Y', '4' );
const char	FACENORMAL_ID[]		=	MAKE_LONG( 'F', 'N', 'R', 'M' );
const char  OBJNODE_ID[]        =   MAKE_LONG( 'N', 'O', 'D', 'E' );
const char  ORIG_ID[]			=   MAKE_LONG( 'O', 'R', 'I', 'G' );
const char  VERSION_ID[]		=   MAKE_LONG( 'V', 'E', 'R', 'S' );
const char  KID_ID[]			=	MAKE_LONG( 'K', 'I', 'D', '4' );
const char  TREE_ID[]			=	MAKE_LONG( 'T', 'R', 'E', 'E' );
const char  VECT_ID[]			=	MAKE_LONG( 'V', 'E', 'C', 'T' );
//const char  MOTION_ID[]			=	MAKE_LONG( 'M', 'O', 'V', 'E' );
const char	TERMINATOR_ID[]		=	{0};



#define VERSION				131									// Version number * 100
#define KATEXP_CLASS_ID		Class_ID(0x37bf4fd7, 0x29976333)	// CLASS_ID


const float MPI				= 3.1415926535897932384626433932795028841792f;
const float HALF_PI			= (MPI / 2);
const float ONE_DEGREE		= 0.0174532925199432957692369076848861271344f;
const float INV_ONE_DEGREE	= 1/ONE_DEGREE;	

#define DELTA 1e-6			// error tolerance


enum	{VX, VY, VZ, VW};		    // axes

inline float DEG2RAD (float dval)	{	return(dval*ONE_DEGREE);	}
inline float RAD2DEG (float dval)	{	return(dval*(INV_ONE_DEGREE));	}


#define NJD_PI  3.14159265358979323846
#define NJM_DEG_RAD(n)  ((n) * NJD_PI / 180.0)									// deg -> rad
#define NJM_DEG_ANG(n)  (((Angle)(((n) * 65536.0) / 360.0)) & 0x0000ffff)		// deg -> ang
#define NJM_RAD_ANG(n)  (((Angle)(((n) * 65536.0) / (2 * NJD_PI))) & 0xffff)	// rad -> ang
#define NJM_RAD_DEG(n)  ((n) * 180.0 / NJD_PI)									// rad -> deg
#define NJM_ANG_DEG(n)  ((n) * 360.0 / 65536.0)									// ang -> deg
#define NJM_ANG_RAD(n)  ((n) * (2 * NJD_PI) / 65536.0)							// ang -> rad


const	float	UV_X		= 255.f;
const	float	UV_Y		= -255.f;

typedef struct 
{
	INode *N;
	int   Id;
} NODE_ID;

typedef struct 
{
	MtlID	MatId;
	Uint16	nbFaces;
} MESH_SET;

typedef struct 
{
	Color colour;
	float  alpha;
} COLOURDEF;

typedef struct 
{
	COLOURDEF diffuse;
	COLOURDEF specular;
	float     exponent;
	Uint32    tex_flags;
	Uint32	  attr_flags;
} MATERIAL;

typedef struct 
{
	char			name[256];
	MATERIAL		material;
	Uint16			TexID;
	Uint16			SubTex[256];
} MATLIST;



#define	MAX_BONE_NODES	64

typedef struct
{
	int		nodecount;
	int		vertno;
	char	names[MAX_BONE_NODES][256];
	float	weights[MAX_BONE_NODES];
	int		NodeId[MAX_BONE_NODES];
	Point3  Offsets[MAX_BONE_NODES];
} WEIGHTS;


class MtlKeeper
{
public:
	int     MtlCount;
	int		GetSubMtlCount(Mtl* mtl, bool recurse);
	BOOL	AddMtl(Mtl* mtl);
	int		GetMtlID(Mtl* mtl);
	int		Count();
	Mtl*	GetMtl(int id);

	Tab<Mtl*> mtlTab;
};


class TimeList {
	
	TimeValue Value[1000];
	int count;

public:

	TimeList() {
		count = 0;
	}

	void Add(TimeValue t) {
		int i,j;
		for (i = 0 ;i<count; i++) {
			if (Value[i] == t) {
				return;
			}
			if (Value[i]>t) {
				for (j = count; j>i; j--) {
					Value[j] = Value[j-1];
				}
				Value[i] = t;
				count++;
				return;
			}
		}
		Value[count] = t;
		count++;
	}

	void Clear() {
		count = 0;
	}

	int Count() {
		return count;
	}

	TimeValue Get(int p) {
		return Value[p];
	}
};


class AsciiExp : public SceneExport
{
public:
	AsciiExp();
	~AsciiExp();

	// SceneExport methods
	int				ExtCount();					// Number of extensions supported 
	const TCHAR *	Ext(int n);					// Extension #n (i.e. "ASC")
	const TCHAR *	LongDesc();					// Long ASCII description (i.e. "Ascii Export") 
	const TCHAR *	ShortDesc();				// Short ASCII description (i.e. "Ascii")
	const TCHAR *	AuthorName();				// ASCII Author name
	const TCHAR *	CopyrightMessage();			// ASCII Copyright message 
	const TCHAR *	OtherMessage1();			// Other message #1
	const TCHAR *	OtherMessage2();			// Other message #2
	unsigned int	Version();					// Version number * 100 (i.e. v3.01 = 301) 
	void			ShowAbout(HWND hWnd);		// Show DLL's "About..." box
#if MAX_RELEASE == 2500
	int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE); // Export	file
#else
	int				DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE,DWORD options=0); // Export	file
#endif

	// Node enumeration
	BOOL		nodeEnum(INode* node);
	void		BoneProcess(INode* node, int& nodeCount);
	void		FfdProcess(INode* node, int& nodeCount);
	void		PreProcessBone( INode* node );
	void		NodePreProcess(INode *node);

	// High level export
	void		ExportNode(INode * node);
	void		ExportMaterialList();
	void		ExportModel(INode* node);
	void		ExportAnimMesh(INode* node);
	void		ExportCamera(INode* node);
	void		ExportLight(INode* node);

	void		ExportTree();
	// Mid level export
	void		ExportMesh(INode* node);
	void		ExportBones( INode* node );
	void		ExportMaterial(INode* node);
	void		ExportAnimKeys(INode* node);
	void		ExportFFD( INode* node , FFDMod *FfdMod, ModContext *mc, Mesh *);

	// Low level export
	void		ExportBoneKeyAnim( INode* node );
	void		ExportBoneKeyAnimPos( INode* node );
	void		ExportBoneKeyAnimRot( INode* node );
	void		ExportBoneKeyAnimScl( INode* node );
	void		ExportBoneKeyAnimPos( INode* node , TimeList tl);
	void		ExportBoneKeyAnimRot( INode* node , TimeList tl);
	void		ExportBoneKeyAnimScl( INode* node , TimeList tl);
	void		ExportNodeKeyAnim( INode *node);
	void		ExportNodeKeyPos( INode *node);
	void		ExportNodeKeyRot( INode *node);
	void		ExportNodeKeyScl( INode *node);
	void		ExportMeshList(INode * node, Mesh * mesh);
	void		ExportPoints(INode * node, Mesh * mesh);
	void		ExportVectors(INode * node, Mesh * mesh);
	void		ExportNormals(INode * node, Mesh * mesh);
	void		ExportVertCols(INode * node, Mesh * mesh);
	void		ExportVisTrack( INode* node );
	void		ExportBoneAnim( INode* node );
	void		ExportLimb( INode* nodeptr );
	void		ExportWeights( INode* nodeptr );
	void		ExportNodeAnimMesh( INode* node );
	void		ExportMeshVertAnim(INode *node, EditTriObject *e);
	void		DumpMeshAnim(INode* node, TimeValue t);
	void		CalcMeshAnimInfo(INode* node, TimeValue t);
	void		DumpPoly(PolyLine* line, Matrix3 tm);
	void		DumpMatrix3(Matrix3* m);
	void		DumpMaterial(Mtl* mtl, int matno, int subno , bool recurse = false);
	void		DumpJointParams(JointParams* joint);
	void		DumpUVGen(StdUVGen* uvGen); 
	void		DumpCamPosSample(INode* node);
	void		DumpPosSample(INode* node);
	void		DumpPosQuatSample(INode* node);
	void		DumpRotSample(INode* node);
	void		DumpScaleSample(INode* node);
	void		DumpPoint3Keys(Control* cont);
	void		DumpFloatKeys(Control* cont);
	void		DumpPosKeys(Control* cont);
	void		DumpRotKeys(Control* cont);
	void		DumpScaleKeys(Control* cont);
	void		DumpTexture( Mtl* mtl, int matno, int subno );
	void		ExportTreeNodes( INode * node );
	void		ExportTreeAnim( INode* node );
	void		ExportTreeKeyAnim( INode* node);
	void		ExportMotionAnim( INode* node);

	// Erm, dunno export
	void		ExportProp( INode* node );

	// Misc methods
	int			getTextureId( Mtl *mtl );
	void		SubAnims(Animatable *a);
	bool		IsBoned(INode *node);
	bool		WriteChunkHdr( char* ChunkID, long Size );
	void		CalcChunkSizes( void );
	void		GetNoFrames( void );
	int			ValidateBoneKids( INode * nodeptr );
	Modifier *  FindPhysiqueModifier (INode* node, ModContext**mc);
	Modifier *	FindPhysiqueModifier (INode* nodePtr);
	bool     	FindFFDModifier (INode* node);
	void		FindEditableMesh(INode *node);
	TCHAR *		GetMapID(Class_ID cid, int subNo);
	Point3		GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv);
	BOOL		CheckForAndExportFaceMap(Mtl* mtl, Mesh* mesh, int matno);
	void		make_face_uv(Face *f, Point3 *tv);
	BOOL		TMNegParity(Matrix3 &m);
	TCHAR *		FixupName(TCHAR* name);
	BOOL		CheckForAnimation(INode* node, BOOL& pos, BOOL& rot, BOOL& scale);
	TriObject *	GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt);
	TriObject * GetTriObjectFromObject(Object *obj, TimeValue t, int &deleteIt);
	BOOL		IsKnownController(Control* cont);
	Point3		GetAverageVertNormal(INode *node, Mesh* mesh, int VertNo);
	void		OrigProcess(INode *node);
	TSTR		FindTextureFName(TSTR MapName);
	void		ExportParticles();
	void		ExportParticleNodes(INode *node);
	// Interface to member variables
	inline Interface *	GetInterface()			{ return ip; }
//	void		SetExportAnimFlag(int State)	{ExportAnimFlag=State;}
//	int			GetExportAnimFlag()				{return(ExportAnimFlag);}
private:
//	int			ExportAnimFlag;
	char		drive[_MAX_DRIVE];
	char		dir[_MAX_DIR];
	char		fname[_MAX_FNAME];
	char		ext[_MAX_EXT];
	char		filename[_MAX_DRIVE+_MAX_DIR+_MAX_FNAME+_MAX_EXT];

	int         Export[MAX_NODES];

	char		SceneName[256];
	char		MeshFname[256];

	Interface *	ip;
	ModContext  *PhyMc;

	FILE *		tempStream;						// Temp debug file
	FILE *		expStream;						// EXPORT file

	int			nbChunks;
	long		SizeTab[MAX_FILE_CHUNKS];

	int			nTotalNodeCount;
	int			nCurNode;
	int			nCurObj;
	int			nbFrames;
	Uint32		totFrames;
	int			CurBone;
	int			NbEmmitters;

	float		radius;

	int			nbBones;
	int			nbWeights;
	WEIGHTS *	Weights;

	int			FfdNum;

	int			nbTexs;
	char		TexList[MAXCOUNT][64];

	int			nbMats;
	MATLIST		MatList[MAXCOUNT];

	int			nbMeshs;
	MESH_SET	MeshSet[MAXCOUNT];


	HWND		MAX_hWnd;
//	char		BoxString[MAXCOUNT];
	MtlKeeper	mtlList;

//	NODE_ID		NodeList[MAXCOUNT];
//	int			NodeCount;
};


class AsciiExpClassDesc : public ClassDesc
{
public:
	int				IsPublic()						{ return 1; }
	void *			Create(BOOL loading = FALSE)	{ return new AsciiExp; } 
	const TCHAR*	ClassName()						{ return GetString(IDS_ASCIIEXP); }
	SClass_ID		SuperClassID()					{ return SCENE_EXPORT_CLASS_ID; } 
	Class_ID		ClassID()						{ return KATEXP_CLASS_ID; }
	const TCHAR*	Category()						{ return GetString(IDS_CATEGORY); }
};

#endif // __ASCIIEXP__H

