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
void	CExport::Write(void *Addr,int Len)
{
		fwrite(Addr,Len,1,File);
}

/*****************************************************************************/
int		CExport::ExportLayerHeader(int Type,int SubType,int Width,int Height)
{
sExpLayerHdr	LayerHdr;
int				ThisFilePos=ftell(File);

		LayerOfs.push_back(ThisFilePos);

		LayerHdr.Type=Type;
		LayerHdr.SubType=SubType;
		LayerHdr.Width=Width;
		LayerHdr.Height=Height;
		fwrite(&LayerHdr,sizeof(sExpLayerHdr),1,File);

		return(ThisFilePos);
}

/*****************************************************************************/
/*** Tiles *******************************************************************/
/*****************************************************************************/
void	CExport::ExportTiles(CCore *Core)
{
int		ListSize,i;
CTileBank	*TileBank=Core->GetTileBank();

		FileHdr.TileW=TileBank->GetTile(1,0).GetPCTexW();
		FileHdr.TileH=TileBank->GetTile(1,0).GetPCTexH();

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
CTileBank	*TileBank=Core->GetTileBank();
CTile		&ThisTile=TileBank->GetTile(OutTile.Set,OutTile.Tile);
int			RGBW=ThisTile.GetPCTexW();
int			RGBH=ThisTile.GetPCTexH();
u8			*RGB=ThisTile.GetPCTexRGB();
GString		SetName=TileBank->GetSet(OutTile.Set).GetFilename();

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
