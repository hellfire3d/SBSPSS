/*=========================================================================

	EXPORT.CPP	

	Author:  
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

#include "mapread.h"


/*	Glib
	---- */
#include <tquant.h>

/*	Local
	----- */

/*	Graphics
	-------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

#define	TILE_SHIFT		8

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Positional Vars
	--------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Data
	---- */

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

int CMapFile::GetVersion( char * version )
{
	int		ret = 0;
	int		len;
	char	whole[8];
	char	frac[8];

	len = strlen(version);
	if (len)
	{
		int		pos = 0;
		int		opos = 0;

		while (version[pos] != '.')
		{
			whole[pos] = version[pos];
			pos++;
			if (pos > len)
				GObject::Error(ERR_FATAL,"Corrupt Version - %s\n", version);
		}

		pos++;
		while (version[pos])
		{
			frac[opos] = version[pos];
			pos++;
			opos++;
			if (pos > len)
				GObject::Error(ERR_FATAL,"Corrupt Version - %s\n", version);
		}
	}

	ret =  atoi( whole );
	ret *= 100;
	ret += atoi( frac );

	GObject::Error(ERR_WARNING, "Map Version %d\n", ret );

	return ret;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

int CMapFile::FindFrame( const char * frame )
{
	char	InFile[_MAX_FNAME];
	char	CompFile[_MAX_FNAME];

	_splitpath( frame, NULL, NULL, InFile, NULL );

	for (int f=0;f<RepItems.size();f++)
	{
		_splitpath( RepItems[f].m_texName, NULL, NULL, CompFile, NULL );
		if (!strcmp( InFile, CompFile ))	return f;
	}

	if (m_errorOnMissingFile)
		GObject::Error(ERR_FATAL,"Missing bmp - %s\n", frame);
	return -1;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void CMapFile::Parse()
{
	int		i;
	int		version;
	int		x, y;
	float	lowestY;
	TILE_PACKET *	t;

	
	MapFile.Seek( 0 );
	MapFile.Read( VersionText, 4 );

	version = GetVersion( (char *)VersionText );
	

	// READ GLOBAL TILE NAMES

	MapFile.Read( &nbTiles );
	TileNames = (CFileName *)malloc( nbTiles * sizeof(CFileName) );

	for (i=0;i<nbTiles;i++)
	{
		MapFile.Read( &TileNames[i], sizeof(CFileName) );
	}


	// READ GLOBAL OBJECT INFO

	MapFile.Read( &nbObjects );
	Objects = (CObject *)malloc( nbObjects * sizeof(CObject) );
	for (i=0;i<nbObjects;i++)
	{
		MapFile.Read( &Objects[i].Name, sizeof(CFileName) );

		if (version < 104)
		{
			MapFile.Read( &Objects[i].Width );
			MapFile.Read( &Objects[i].Height );
			MapFile.Read( &Objects[i].Depth );
			MapFile.Read( &Objects[i].Mask, (sizeof(bool) * MAX_MASKX * MAX_MASKY) );
		}
	}


	// READ TILE MAP

	MapFile.Read( &MapWidth );
	MapFile.Read( &MapHeight );
	MapFile.Read( &TileSize );
	if (TileSize > sizeof(MapCTile))	GObject::Error(ERR_FATAL, "MapCTile STRUCT MISMATCH" );
	Tiles = (TILE_PACKET *)malloc( (MapWidth * MapHeight * sizeof( TILE_PACKET )) );

	if (!Tiles)
		GObject::Error(ERR_FATAL, "can't allocate tiles" );


	t = Tiles;
	nbSpawnPoints=0;
	lowestY = 65536;
	for (y=0;y<MapHeight;y++)
	{
		for (x=0;x<MapWidth;x++)
		{
			MapCTile	tile;
			int		frame = 0;
			int		angle = 0;

			MapFile.Read( &tile, TileSize );

			strlwr( tile.FrameName );
			for (int l=0;l<strlen(tile.FrameName);l++)
			{
				if (tile.FrameName[l] == '\\')	tile.FrameName[l] = '/';
			}
			if (tile.frame)		frame = FindFrame( tile.FrameName );
			else				frame = 0;

			angle = (int)tile.angle;
			angle++;
			angle &= 0x3;
			if (angle == 2)			angle = 0;
			else if (angle == 0)	angle = 2;

			t->frame = tile.frame;
			t->r = (u8)(tile.r * 255.f);
			t->g = (u8)(tile.g * 255.f);
			t->b = (u8)(tile.b * 255.f);
			t->pad = 0;

			if (tile.flag&TF_SPAWN)
			{
				if (nbSpawnPoints>=MAX_SPAWNING_TILES)
				{
					GObject::Error(ERR_FATAL, "Max Spawning Positions exceeded for level" );
				}
				Spawn[nbSpawnPoints].x = x;
				Spawn[nbSpawnPoints].y = y;
				nbSpawnPoints++;

			}

			t->angle = angle;
			t->flags = (int)(tile.flag&(~TF_SPAWN));
			t->height = (int)tile.py;
			t->frame = frame;
			t->xflip = tile.xflip;
			t->yflip = tile.yflip;
			t->type  = tile.type;

//			if (t->flags & TF_NO_BUILD)
//				GObject::Error(ERR_WARNING, "No build flag found\n" );

			if (frame < 0)	t->flags |= TF_HIDDEN;

			if (!(tile.flag & TF_HIDDEN) && (x < MapWidth-1) && (y < MapHeight-1))
			{
				if (lowestY > tile.py)	lowestY = tile.py;
			}

			t++;
		}
	}

	GObject::Error( ERR_WARNING, "Lowest land height = %f\n", lowestY );

	t = Tiles;
	for (y=0;y<MapHeight;y++)
	{
		for (x=0;x<MapWidth;x++)
		{
			t->height -= (int)lowestY;
			if (t->height < 0)	t->height = 0;

//			if (t->height > sizeof(u16))
//			{
//				GObject::Error( ERR_FATAL, "Land height overflow - %d\n", t->height );
//			}

			t++;
		}
	}


	// READ OBJECT LIST

	MapFile.Read( &nbModels );
	MapFile.Read( &ModelSize );
	if (ModelSize > sizeof(CModel))	GObject::Error(ERR_FATAL, "Struct mismatch in CModel" );

	Models = (MODEL_PACKET *)malloc( nbModels * sizeof(MODEL_PACKET) );

	if (!Models)
		GObject::Error(ERR_FATAL, "Can't allocate models\n" );


	MODEL_PACKET *	m = Models;
	for (i=0;i<nbModels;i++)
	{
		u32		animflag;
		u16		angle;
		CModel	model;
		char	MyDir[_MAX_DIR];
		char	MyFile[_MAX_FNAME];


		MapFile.Read( &model, ModelSize );


		_splitpath( model.MeshName, NULL, MyDir, MyFile, NULL );

		angle = (u16)model.AngY;
		if (angle == 1)			angle = 3;
		else if (angle == 3)	angle = 1;

		animflag = FindFileEquate( MyFile );
		animflag &= ~MODEL_FLAGS_MASK;
		if (model.m_Animates)	animflag |= MODEL_ANIMATE_FLAG;
		if (model.m_Busstop)	animflag |= MODEL_BUSSTOP_FLAG;
		if (model.m_Booth)		animflag |= MODEL_BOOTH_FLAG;
		if (model.m_OtherAnim)	animflag |= MODEL_OTHERANIM_FLAG;
		if (model.m_Entrance)	animflag |= MODEL_ENTRANCE_FLAG;


		m->AnimAndFlags = animflag;
		m->xp = (u16)(model.PosX / (1 << TILE_SHIFT));
		m->yp = (u16)(model.PosY - lowestY);
		m->zp = (u16)(model.PosZ / (1 << TILE_SHIFT));
		m->ya = (u16)angle;

//		if (m->yp > sizeof(u16))
//		{
//			GObject::Error( ERR_FATAL, "Object Y position overflow - %d\n", m->yp );
//		}

		m++;
	}
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

int CMapFile::FindFileEquate( char * name )
{
	strupr( name );

	for (int i=0;i<AnimItems.size();i++)
	{
		if (!strcmp( AnimItems[i].m_AnimName, name ))	return( AnimItems[i].m_Frame );
	}

	if (AnimHeaderFile)
	{
		GObject::Error( ERR_WARNING, "Could not find anim %s in anim header %s\n", name, AnimHeaderFile );
	}
	else
	{
		GObject::Error( ERR_WARNING, "Could not find anim %s - anim header not loaded\n", name );
	}

	return (-1);
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void CMapFile::MakeMapPalette(void)
{
	using namespace std;

	vector<u8>	SourcePal;
	vector<u8>	DestPal;

	SourcePal.resize(MapWidth*MapHeight*3);
	TileToPalNum.resize(MapWidth*MapHeight);
	DestPal.resize(NUM_OF_PAL_ENTRIES*3);
	MapPal.resize(NUM_OF_PAL_ENTRIES);

	for (int f=0;f<MapWidth*MapHeight;f++)
		{
		SourcePal[f*3+0]=Tiles[f].r;
		SourcePal[f*3+1]=Tiles[f].g;
		SourcePal[f*3+2]=Tiles[f].b;
		}

	tquant(&SourcePal[0],&TileToPalNum[0],&DestPal[0],NUM_OF_PAL_ENTRIES,MapWidth*MapHeight);

	for (int c=0;c<NUM_OF_PAL_ENTRIES;c++)
		{
		MapPal[c].r=DestPal[c*3+0];
		MapPal[c].g=DestPal[c*3+1];
		MapPal[c].b=DestPal[c*3+2];
		}
}

void CMapFile::WriteMapPalette(FILE * F)
{
	int	count = NUM_OF_PAL_ENTRIES;

	fwrite( &count,	sizeof(u32), 1,F);

	for (int f=0;f<count;f++)
		{
		u8		Dummy = 0;


		fwrite( &MapPal[f].r,	sizeof(u8), 1,F);
		fwrite( &MapPal[f].g,	sizeof(u8), 1,F);
		fwrite( &MapPal[f].b,	sizeof(u8), 1,F);
		fwrite( &Dummy,sizeof(u8),1,F);
		}
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void CMapFile::SetInfoOut( int x, int y, TILE_PACKET * T, OUT_TILE_PACKET * O )
{


	O->mType = 0;

	if (T->height<0)
	{
		O->ypos = 0;
//		printf("low height %d\n", T->height);
	} else
	if (T->height>255*4.f)
	{
		O->ypos = 255;
//		printf("hi height %d\n", T->height);
	} else
	{
		O->ypos = (T->height/4.f);
	}


	O->mNeighbours = 0;
	O->mDirection = 0;

	if (T->frame > 4095)	GObject::Error(ERR_FATAL, "Tile index overflow" );

	O->mTileIndex = T->frame;
	O->mTileIndex |= (T->angle << 12);				// STUFF ANGLE IN TOP BITS OF TILE INDEX
	O->ColIdx = GetPalNum( x, y );
	O->mFlags = T->flags;

	if (T->xflip)	O->mTileIndex |= (1 << 15);		// SET X/Y FLIP IN TOP BITS OF TILE INDEX
	if (T->yflip)	O->mTileIndex |= (1 << 14);

	switch(T->type)
	{
	case TT_VOID:
		O->mType = (u8)CELL_Void;
		break;
	case TT_PATH:
		O->mType = (u8)CELL_Path;
		break;
	}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void CMapFile::Export( const char * file )
{
	FILE *	level;

	MakeMapPalette();

	level = fopen( file, "wb" );
	if (level)
	{
		WriteMapPalette( level );

		fwrite( &MapWidth,	sizeof( int ), 1, level );
		fwrite( &MapHeight,	sizeof( int ), 1, level );

		for (int y=0;y<MapHeight;y++)
		{
			for (int x=0;x<MapWidth;x++)
			{
				TILE_PACKET *	tile;
				OUT_TILE_PACKET	out;

				tile = &Tiles[x + (y * MapWidth)];

				SetInfoOut( x, y, tile, &out );

				fwrite( &out, sizeof(OUT_TILE_PACKET), 1, level );
			}
		}

		fwrite( &nbModels,	sizeof( int ), 1, level );
		for (int i=0;i<nbModels;i++)
		{
			fwrite( &Models[i], sizeof(MODEL_PACKET), 1, level );
		}
		fwrite( &nbSpawnPoints, sizeof(int), 1, level );
		for (i=0; i<nbSpawnPoints; i++)
		{
			fwrite( &Spawn[i], sizeof(SPAWNPOS), 1, level);
		}
//		printf( "WRITTEN %s\n", file );
	}
	else
	{
		GObject::Error(ERR_FATAL, "Could not save lvl file - %s", file );
	}
}


/*===========================================================================
 end */
