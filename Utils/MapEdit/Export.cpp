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
CExport::CExport(char *Filename,int _LayerCount)
{
		LayerCount=_LayerCount;
		File=fopen(Filename,"wb");
	
// Write Dummy File Header

		fwrite(&FileHdr,sizeof(sExpFileHdr),1,File);
		for (int i=0;i<LayerCount; i++) fwrite(&LayerCount,sizeof(int),1,File);
}

/*****************************************************************************/
CExport::~CExport()
{
		ASSERT(LayerCount==LayerOfs.size());
		FileHdr.LayerCount=LayerCount;
// ReWrite Main Header
		fseek(File,0,0);
		fwrite(&FileHdr,sizeof(sExpFileHdr),1,File);

		for (int i=0;i<LayerCount; i++) 
		{
			TRACE1("LayerOfs %i\n",LayerOfs[i]);
			fwrite(&LayerOfs[i],sizeof(int),1,File);
		}
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
sExpMapElem		BlankElem={0,0,0};

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
				sExpMapElem		TileElem;

				TileElem.Tile=MapElem.Tile;
				TileElem.Set=MapElem.Set;
				TileElem.Flags=MapElem.Flags;
				sExpLayerTile	OutElem;
				OutElem.Tile=UsedTileList.Add(TileElem);
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

// Write Tiles
		ListSize=UsedTileList.size();
		FileHdr.TileCount=ListSize;
		FileHdr.TileOfs=ftell(File);

		for (i=0; i<ListSize; i++)
		{
			sExpMapElem		&ThisElem=UsedTileList[i];
			CTile			&ThisTile=Core->GetTile(ThisElem.Set,ThisElem.Tile);
			sExpTile		OutTile;

			OutTile.Flags=ThisElem.Flags;

			TRACE3("%i %i %i\n",ThisElem.Set,ThisElem.Tile,ThisElem.Flags);
			ExportTile(Core,ThisTile,OutTile);
		}

// Write Tris
		ListSize=TriList.size();
		FileHdr.TriCount=ListSize;
		FileHdr.TriOfs=ftell(File);
		for (i=0; i<ListSize; i++)
		{
			fwrite(&TriList[i],sizeof(sTriFace),1,File);
		}
		TRACE1("TriCount=%i\n",ListSize);
}

/*****************************************************************************/
void	CExport::ExportTile(CCore *Core,CTile &ThisTile,sExpTile &OutTile)
{
int			RGBW=ThisTile.GetPCTexW();
int			RGBH=ThisTile.GetPCTexH();
u8			*RGB=ThisTile.GetPCTexRGB();

		OutTile.W=RGBW;
		OutTile.H=RGBH;

		if (ThisTile.IsTile3d())
		{
			ExportTile3d(Core,ThisTile,OutTile);
		}
		else
		{
			ExportTile2d(Core,ThisTile,OutTile);
		}

		fwrite(&OutTile,sizeof(sExpTile),1,File);
		fwrite(RGB,RGBW*RGBH*3,1,File);

}

/*****************************************************************************/
void	CExport::ExportTile2d(CCore *Core,CTile &ThisTile,sExpTile &OutTile)
{
CTexCache	&TexCache=Core->GetTexCache();

		OutTile.TriStart=-1;
		OutTile.TriCount=0;
		OutTile.XOfs=ThisTile.GetTexXOfs();
		OutTile.YOfs=ThisTile.GetTexYOfs();
		
// Texture
int		TexID=ThisTile.GetTexID();
		if (TexID==-1)	// Blank Tile
		{
			OutTile.TexId=-1;
		}
		else
		{
			sExpTex	OutTex;
			sTex	&ThisTex=TexCache.GetTex(TexID);
			OutTex.Filename=ThisTex.Filename;
			OutTile.TexId=TexList.Add(OutTex);
		}

}

/*****************************************************************************/
void	CExport::ExportTile3d(CCore *Core,CTile &ThisTile,sExpTile &OutTile)
{
CTexCache				&TexCache=Core->GetTexCache();
std::vector<sTriFace>	&TileTriList=ThisTile.GetTriList();

int		TriCount=TileTriList.size();

		OutTile.TriStart=TriList.size();
		OutTile.TriCount=TriCount;
		OutTile.XOfs=-1;
		OutTile.YOfs=-1;
		OutTile.TexId=-1;

		for (int T=0; T<TriCount; T++)
		{
			sTriFace	&TileTri=TileTriList[T];
			sTriFace	OutTri;
			sExpTex		OutTex;

			for (int p=0; p<3; p++) 
			{
				OutTri=TileTri;
// Texture
				sTex	&TriTex=TexCache.GetTex(TileTri.Mat);
				OutTex.Filename=TriTex.Filename;
				OutTri.Mat=TexList.Add(OutTex);
			}
			TriList.push_back(OutTri);
		}
//		return(TriCount);
}

/*****************************************************************************/
/*** Textures ****************************************************************/
/*****************************************************************************/

void	CExport::ExportTexList(CCore *Core)
{
int		TexCount=TexList.size();

		FileHdr.TexCount=TexCount;
		FileHdr.TexOfs=ftell(File);

		for (int i=0; i<TexCount; i++)
		{
			sExpTex	&ThisTex=TexList[i];
			char	TexFilename[256];

			MakePathRel2App(ThisTex.Filename,TexFilename);

			int	Len=strlen(TexFilename);
			fwrite(TexFilename,Len+1,1,File);
			TRACE2("%i:\t%s\n",i,TexFilename);
		}
}

/*****************************************************************************/
