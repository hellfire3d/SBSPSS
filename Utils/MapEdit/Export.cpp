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
#include	"Elem.h"

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

// Add blank tile

sExpTile	BlankElem={0,0,0,0,0,0,0};
		AddTile(BlankElem);
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
void    CExport::PadFile()
{
int		Pad=ftell(File) & 3;

		if (Pad)
		{
			fwrite(&Pad,Pad,1,File);
		}
}

/*****************************************************************************/
void	CExport::Write(void *Addr,int Len)
{
		fwrite(Addr,Len,1,File);
}

/*****************************************************************************/
int		CExport::ExportLayerHeader(sLayerDef &LayerDef)//(int Type,int SubType,int Width,int Height)
{
		PadFile();
sExpLayerHdr	LayerHdr;
int				ThisFilePos=ftell(File);

		LayerOfs.push_back(ThisFilePos);
		

		LayerHdr.Type=LayerDef.Type;
		LayerHdr.SubType=LayerDef.SubType;
		LayerHdr.Width=LayerDef.Width;
		LayerHdr.Height=LayerDef.Height;
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

//!!		FileHdr.TileW=TileBank->GetTile(0,0).GetElemWidth();
//!!		FileHdr.TileH=TileBank->GetTile(0,0).GetElemHeight();
		FileHdr.TileW=16;
		FileHdr.TileH=16;

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

		if (OutTile.Set==0 && OutTile.Tile==0)
		{
			char	c=0;

			fwrite(&OutTile,sizeof(sExpTile),1,File);
			fwrite(&c,1,16*16*3,File);	// Write RGB
		}
		else
		{
			CElem		&ThisTile=TileBank->GetElem(OutTile.Set,OutTile.Tile);
			GString		SetName=TileBank->GetSetFilename(OutTile.Set);

			ASSERT(ThisTile.GetElemWidth()==FileHdr.TileW);
			ASSERT(ThisTile.GetElemHeight()==FileHdr.TileH);
			if (ThisTile.IsElem3d()) 
			{
				ExportTile3d(Core,ThisTile,OutTile);
			}
			OutTile.Set=SetNames.Add(SetName);
			fwrite(&OutTile,sizeof(sExpTile),1,File);
			fwrite(ThisTile.GetElemRGB(),FileHdr.TileW*FileHdr.TileH*3,1,File);	// Write RGB
		}


}

/*****************************************************************************/
void	CExport::ExportTile3d(CCore *Core,CElem &ThisTile,sExpTile &OutTile)
{
		ExportElem3d(Core,ThisTile,OutTile.TriStart,OutTile.TriCount);
}

/*****************************************************************************/
void	CExport::ExportElem3d(CCore *Core,CElem &ThisElem,int &TriStart,int &TriCount)
{
CTexCache				&TexCache=Core->GetTexCache();
std::vector<sTriFace>	&ElemTriList=ThisElem.GetTriList();

		TriStart=TriList.size();
		TriCount=ElemTriList.size();

		for (int T=0; T<TriCount; T++)
		{
			sTriFace	&InTri=ElemTriList[T];
			sExpTri		OutTri;
			GString		TexName=TexCache.GetTexFilename(InTri.Mat);
			OutTri.TexID=TexNames.Add(TexName);

			for (int p=0; p<3; p++) 
			{
				OutTri.vtx[p]=InTri.vtx[p];
				OutTri.uv[p][0]=InTri.uvs[p].u;
				OutTri.uv[p][1]=InTri.uvs[p].v;
			}
			OutTri.Flags=InTri.Flags;
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
			TRACE2("Set %i:%s\n",i,RelStr);
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
			TRACE2("Tex %i:%s\n",i,RelStr);
		}

}

/*****************************************************************************/
