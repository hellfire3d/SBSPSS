/**************/
/*** Export ***/
/**************/

#include	"stdafx.h"
#include	<Vector>

#include	"LayerTile.h"
#include	"Map.h"

#include	"utils.h"

#include	"Core.h"
#include	"TexCache.h"
#include	"TileSet.h"
#include	"Tile.h"

#include	"Layer.h"
#include	"LayerTile.h"

#include	"Export.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CExport::CExport(char *Filename)
{
		File=fopen(Filename,"wb");
	
// Write Dummy File Header

		fwrite(&FileHdr,sizeof(sExpFileHdr),1,File);
//		for (int i=0;i<EXPORT_LAYER_COUNT; i++) fwrite(&LayerCount,sizeof(int),1,File);
}

/*****************************************************************************/
CExport::~CExport()
{
int		LayerCount=LayerOfs.size();

		FileHdr.LayerCount=LayerCount;

		for (int i=0;i<LayerCount; i++) 
		{
			FileHdr.LayerOfs[i]=LayerOfs[i];
		}

// ReWrite Main Header
		fseek(File,0,0);
		fwrite(&FileHdr,sizeof(sExpFileHdr),1,File);
		fclose(File);
}

/*****************************************************************************/
/*** Tile Map ****************************************************************/
/*****************************************************************************/
void	CExport::ExportLayerTile(CCore *Core,char *LayerName,int SubType,CMap &Map)
{
sExpLayerHdr	LayerHdr;
int				Width=Map.GetWidth();
int				Height=Map.GetHeight();
int				ThisFilePos=ftell(File);
CTileBank		&TileBank=Core->GetTileBank();
sExpTile		BlankElem={0,0,0,0};

		TRACE1("LayerTile Ofs %i\n",ThisFilePos);
		LayerOfs.push_back(ThisFilePos);

		LayerHdr.Type=LAYER_TYPE_TILE;
		LayerHdr.SubType=SubType;
		LayerHdr.Width=Map.GetWidth();
		LayerHdr.Height=Map.GetHeight();
		fwrite(&LayerHdr,sizeof(sExpLayerHdr),1,File);

		UsedTileList.Add(BlankElem);	// Ensure blank tile is present
		
		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMapElem		&MapElem=Map.Get(X,Y);
				CTile			&ThisTile=Core->GetTile(MapElem.Set,MapElem.Tile);
				sExpLayerTile	OutElem;
				sExpTile		OutTile;

				OutTile.Set=MapElem.Set;
				OutTile.Tile=MapElem.Tile;
				OutTile.TriCount=0;
				OutTile.XOfs=ThisTile.GetTexXOfs();
				OutTile.YOfs=ThisTile.GetTexYOfs();
				
				OutElem.Tile=UsedTileList.Add(OutTile);
				OutElem.Flags=MapElem.Flags;
				fwrite(&OutElem,sizeof(sExpLayerTile),1,File);
			}
		}
}

/*****************************************************************************/
/*** Collision Layer *********************************************************/
/*****************************************************************************/
void	CExport::ExportLayerCollision(CCore *Core,char *LayerName,int SubType,CMap &Map)
{
sExpLayerHdr	LayerHdr;
int				Width=Map.GetWidth();
int				Height=Map.GetHeight();
int				ThisFilePos=ftell(File);
u8				OutElem;

		TRACE1("LayerCollision Ofs %i\n",ThisFilePos);
		LayerOfs.push_back(ThisFilePos);

		LayerHdr.Type=LAYER_TYPE_COLLISION;
		LayerHdr.SubType=SubType;
		LayerHdr.Width=Map.GetWidth();
		LayerHdr.Height=Map.GetHeight();
		fwrite(&LayerHdr,sizeof(sExpLayerHdr),1,File);
	
		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMapElem		&MapElem=Map.Get(X,Y);
				OutElem=0;
				if (MapElem.Tile)
				{
					OutElem=((MapElem.Tile-1)*4)+1;
					OutElem+=MapElem.Flags & TILE_FLAG_MIRROR_XY;
				}
				fwrite(&OutElem,sizeof(u8),1,File);
			}
		}
}

/*****************************************************************************/
/*** Tiles *******************************************************************/
/*****************************************************************************/
void	CExport::ExportTiles(CCore *Core)
{
int		ListSize,i;

		FileHdr.TileW=Core->GetTile(1,0).GetPCTexW();
		FileHdr.TileH=Core->GetTile(1,0).GetPCTexH();

// Write Tiles
		ListSize=UsedTileList.size();
		FileHdr.TileCount=ListSize;
		FileHdr.TileOfs=ftell(File);

		for (i=0; i<ListSize; i++)
		{
			ExportTile(Core,UsedTileList[i]);
		}

// Write Tris
		ListSize=TriList.size();
		FileHdr.TriCount=ListSize;
		FileHdr.TriOfs=ftell(File);
		for (i=0; i<ListSize; i++)
		{
			fwrite(&TriList[i],sizeof(sExpTri),1,File);
		}
}

/*****************************************************************************/
void	CExport::ExportTile(CCore *Core,sExpTile &OutTile)
{
CTile		&ThisTile=Core->GetTile(OutTile.Set,OutTile.Tile);
CTileBank	&TileBank=Core->GetTileBank();
int			RGBW=ThisTile.GetPCTexW();
int			RGBH=ThisTile.GetPCTexH();
u8			*RGB=ThisTile.GetPCTexRGB();
GString		SetName=TileBank.GetSet(OutTile.Set).GetFilename();

		if (ThisTile.IsTile3d())
		{
			ExportTile3d(Core,ThisTile,OutTile);
		}
		else
		{
		}

// change set name to set mappping
		if (OutTile.Set==0) 
		{
			SetName="BLANK";
		}

		OutTile.Set=SetNames.Add(SetName);
		fwrite(&OutTile,sizeof(sExpTile),1,File);
// Write RGB
		ASSERT(RGBW==FileHdr.TileW);
		ASSERT(RGBH==FileHdr.TileH);
		fwrite(RGB,RGBW*RGBH*3,1,File);

}

/*****************************************************************************/
void	CExport::ExportTile3d(CCore *Core,CTile &ThisTile,sExpTile &OutTile)
{
CTexCache				&TexCache=Core->GetTexCache();
std::vector<sTriFace>	&TileTriList=ThisTile.GetTriList();

int		TriCount=TileTriList.size();

		OutTile.TriStart=TriList.size();
		OutTile.TriCount=TriCount;

		for (int T=0; T<TriCount; T++)
		{
			sTriFace	&InTri=TileTriList[T];
			sExpTri		OutTri;
			GString		TexName=TexCache.GetTexFilename(InTri.Mat);
			OutTri.TexID=TexNames.Add(TexName);

			for (int p=0; p<3; p++) 
			{
				OutTri.vtx[p]=InTri.vtx[p];
				OutTri.uv[p][0]=InTri.uvs[p].u;
				OutTri.uv[p][1]=InTri.uvs[p].v;
			}
			TriList.push_back(OutTri);
		}
}

/*****************************************************************************/
void	CExport::ExportStrList(CCore *Core)
{
int			ListSize,i;
GString		SavePath=GetWorkingPath();
char		RelStr[256];

			SavePath.Upper();

// Set List
		ListSize=SetNames.size();
		FileHdr.SetNameCount=ListSize;
		FileHdr.SetNameOfs=ftell(File);

		for (i=0; i<ListSize; i++)
		{
			GString	&ThisFile=SetNames[i];
			ThisFile.Upper();
			GFName::makerelative(SavePath,ThisFile,RelStr);
			fwrite(RelStr,strlen(RelStr)+1,1,File);
		}
// Tex List
		ListSize=TexNames.size();
		FileHdr.TexNameCount=ListSize;
		FileHdr.TexNameOfs=ftell(File);

		for (i=0; i<ListSize; i++)
		{
			GString	&ThisFile=TexNames[i];
			ThisFile.Upper();
			GFName::makerelative(SavePath,ThisFile,RelStr);
			fwrite(RelStr,strlen(RelStr)+1,1,File);
		}

}

/*****************************************************************************/
