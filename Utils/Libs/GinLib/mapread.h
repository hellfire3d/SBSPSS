/*=========================================================================

	EXPORT.H

	Author:  
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 Climax Development Ltd

===========================================================================*/

#pragma warning( disable : 4786 )

#ifndef __EXPORT_H__
#define __EXPORT_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <vector>
#include <algorithm>


/*	Glib
	---- */
#include <gtypes.h>
#include <misc.hpp>
#include <gstring.hpp>

/*	Local
	----- */
#include "repread.h"
#include "animheader.h"


/*----------------------------------------------------------------------
Tyepdefs && Defines
	------------------- */

#define	MAX_MASKX	8
#define	MAX_MASKY	8

#define	MAX_LINES	512

enum CELL_MODES
{
	CELL_Void					=(0),

	CELL_Hidden					=(1),

	CELL_Path					=(2),
	CELL_Water					=(3),
	CELL_Queue					=(4),
	CELL_Thing					=(5),
//	CELL_TrackRide				=(6),
	CELL_NoGo					=(7),
	CELL_Entrance				=(8),
	CELL_Exit					=(9),
	CELL_QueueEdit				=(10), 
	CELL_CoasterPylon			=(11), 

// actions

	CELL_Delete					=(50),
	CELL_DeleteLine				=(51),

	CELL_PatrolArea				=(52),
	CELL_BacktrackVoid			=(53),

// special modes

//	CM_SpecialMode				=(128),		// yes, these 2 are supposed to be the same
	CM_CalcNeighbours			=(128),
	CM_CalcTile					=(129),
	CM_CalcDirections			=(130),
	CM_SetParent				=(131),
	CM_CreateBridges			=(132),
	CM_CreateBridgeSpans		=(133),

	CM_ShowTransparent			=(256),

	CM_NoModify					=(512),
};


enum	MAP_TILE_FLAGS
{
	TF_NONE			= (0<<0),
	TF_HIDDEN		= (1<<0),
	TF_NO_BUILD		= (1<<1),
	TF_SPAWN		= (1<<2),
	TF_ENTRANCE		= (1<<3),
	TF_TRACKRIDE	= (1<<4),
	TF_NO_WALK		= (1<<5),
};

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

enum
{
	MAP_MAX_FILE_NAME_LENGTH = 512,
};

enum	TILE_TYPE
{
	TT_VOID = 0,
	TT_PATH = 1,
};

struct CFileName
{
	bool	Stripped;
	char	Name[MAP_MAX_FILE_NAME_LENGTH];
	char	FullName[MAP_MAX_FILE_NAME_LENGTH];
};

struct TILE_PACKET
{
	u8		r, g, b, pad;
	int		height;
	int		flags;
	int		frame;
	int		angle;
	int		xflip;
	int		yflip;
	int		type;
};

#pragma pack(1)
struct OUT_TILE_PACKET				// *** TS - Must be reflected in Tile.h - CTile ***
{
	u8		mType;				// 1

	u8		ypos;				// 1
	u8		mNeighbours;		// 1
	u8		mDirection;			// 1	-	4

	u16		mTileIndex;			// 2
	u8		ColIdx;				// 1
	u8		mFlags;				// 1	-	4
};
#pragma pack()

/*	CELL_MODES	mType;			// 4 make a u8							saves 3

	s16		Height;				//	Make a u8							saves 1
	u8		mNeighbours;		//  Take out							saves 1
	u8		mDirection;			// 2 bits, put in flags					saves 1

	u16		mTileIndex;			// 2	- Also contains frame angle

	u8		ColIdx;				// 1
	u8		mFlags;				// 1

	u8		heightoffset;		// 1	Take out						saves 1
	s8		mOverlapCounter;	// 1	Take out						saves 1
	u16		FogTblIndex;		// 2  Put into frame structure			saves 2*/



enum MODEL_FLAGS
{
	MODEL_ANIMATE_FLAG		= (1 << 24),
	MODEL_BUSSTOP_FLAG		= (1 << 25),
	MODEL_BOOTH_FLAG		= (1 << 26),
	MODEL_ENTRANCE_FLAG		= (1 << 27),
	MODEL_OTHERANIM_FLAG	= (1 << 28),
	MODEL_FLAGS_MASK		= (0xff << 24),
};

struct MODEL_PACKET
{
	u32		AnimAndFlags;
	u16		xp, yp, zp, ya;
};


struct MapCTile
{
	float		r,g,b;
	u16			type;
	u16			neighbours;
	u8			object;
	u8			flag;
	float		px,py,pz;
	float		nx,ny,nz;
	u32			frame;
	char		FrameName[128];
	int			PSXFrame;

	int			angle;
	float		u0;
	float		v0;
	float		u1;
	float		v1;
	float		u2;
	float		v2;
	float		u3;
	float		v3;

// *** NOTE: PLEASE ADD NEW VARIABLES HERE ***

	float		ambientr, ambientg, ambientb;

	bool		xflip;
	bool		yflip;
};

struct SPAWNPOS
{
	u8 x, y;
};

struct CObject
{
	CFileName	Name;
	int			Width;
	int			Height;
	int			Depth;
	bool		Mask[MAX_MASKX][MAX_MASKY];
};

struct CModel
{
	float		PosX;
	float		PosY;
	float		PosZ;
	float		AngX;
	float		AngY;
	float		AngZ;

	char		MeshName[512];
	u32			Mesh;
//	MeshFile *	Mesh;

	bool		Highlight;

// *** NOTE: PLEASE ADD NEW VARIABLES HERE AND CHANGE THE VERSION NO. ***

	bool		Snap;
	bool		ShowAxis;

	bool		m_Animates;
	bool		m_Busstop;
	bool		m_Booth;
	bool		m_Entrance;
	bool		m_OtherAnim;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*	Vars
	---- */

/*	Data
	---- */

/*	Class
	----- */

/*------------------------------------------*/
/*											*/
/*------------------------------------------*/

class CChunkFile
{
public:
			CChunkFile()
				{
				}

			~CChunkFile()
				{
				}

		void	Open( const char * file)
			{
			int		fileSize;
			fileSize=FileSize(file);

			if (fileSize < 0)
				GObject::Error(ERR_FATAL,"Error finding size of file %s",file);
			
			Buffer=new u8[fileSize];

			if (!Buffer)
				GObject::Error(ERR_FATAL,"can't alloc mem");

			std::ifstream inFile;

			inFile.open(file,std::ios::binary);

			if (inFile)
				{
				inFile.read((char*)(Buffer),fileSize);
				inFile.close();
				}
			else
				GObject::Error(ERR_FATAL,"Error opening %s",file);
			}


	void	Seek( int l = 0 )					{ Ptr = &Buffer[l];	}
	void	Close()								{ free( Buffer );	}

	void	Skip( int s )						{ Ptr += s;	} 

	void	Read( int * ptr )
	{
		*ptr = *((int *)Ptr);
		Ptr += 4;
	}

	void	Read( void * ptr, int len )
		{
		for (int i=0;i<len;i++)	((u8 *)ptr)[i] = Ptr[i];
		Ptr += len;
		}


protected:

	u8 *	Buffer;
	u8 *	Ptr;
};


/*------------------------------------------*/
/*											*/
/*------------------------------------------*/

class CMapFile
{
public:
			CMapFile()
			{
			m_errorOnMissingFile=true;
			AnimHeaderFile=NULL;
			}

			~CMapFile()
			{
			}

	void	Open( const char * map, const char * rep, const char * anim )
	{
		MapFile.Open( map );
		readRepFile( rep, RepItems );
		readAnimHeaderFile( anim, AnimItems );

		Objects = NULL;
		Tiles = NULL;
		TileNames = NULL;
		Models = NULL;
		AnimHeaderFile = anim;
	}


	void	getTexturesUsed(char const * mapFile,std::vector<GString> & textures)
		{
		m_errorOnMissingFile=false;

		MapFile.Open( mapFile );

		Objects = NULL;
		Tiles = NULL;
		TileNames = NULL;
		Models = NULL;

		Parse();

		for (int f=0;f<nbTiles;f++)
			{
			GString		thisTile;
			thisTile=TileNames[f].Name;
			
			if (thisTile)
				{
				if (std::find(textures.begin(),textures.end(),thisTile)==textures.end())
					textures.push_back(thisTile);
				}
			}

		Close();
		}


	void	Close()
	{
		MapFile.Close();

		if (Objects)	free( Objects );
		if (Tiles)		free( Tiles );
		if (TileNames)	free( TileNames );
		if (Models)		free( Models );
	}

	void	Parse();
	void	Export( const char * file );

	void	GetAllTileNames();
	int		FindFrame( const char * frame );

	int		FindFileEquate( char * filename );

protected:

	int		GetVersion( char * version );

	void	MakeMapPalette(void);
	void	WriteMapPalette(FILE * F);
	void	WriteTileHdrs(FILE * F);


	u8		GetPalNum(uint x,uint y)
		{
		if (!((x+y*MapWidth) < TileToPalNum.size()))
			GObject::Error(ERR_FATAL,"uh?");

		return(TileToPalNum[x+y*MapWidth]);
		}

	void	SetInfoOut( int x, int y, TILE_PACKET * T, OUT_TILE_PACKET * O );


	struct Col
		{
		u8		r,g,b;
		};

	std::vector<Col>		MapPal;
	std::vector<u8>			TileToPalNum;
	
	const char *	AnimHeaderFile;
	
	std::vector<RepItem>	RepItems;
	std::vector<AnimHeaderItem>	AnimItems;


	enum
	{
		NUM_OF_PAL_ENTRIES = 64,
		MAX_SPAWNING_TILES  = 2,
	};


	void	MakeAnimName( char * out, char * in );


	CChunkFile	MapFile;

	u8		VersionText[4];
	int		nbTiles;
	CFileName *	TileNames;

	int		nbObjects;
	CObject *	Objects;

	int		MapWidth;
	int		MapHeight;
	int		TileSize;
	TILE_PACKET *	Tiles;

	int		nbModels;
	int		ModelSize;
	MODEL_PACKET *	Models;

	int		nbSpawnPoints;
	SPAWNPOS	Spawn[MAX_SPAWNING_TILES];

	bool	m_errorOnMissingFile;
};



/*	Functions
	--------- */




/*---------------------------------------------------------------------- */

#endif	/* __EXPORT_H__ */

/*===========================================================================
 end */
