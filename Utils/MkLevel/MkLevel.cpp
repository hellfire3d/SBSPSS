/*********************/
/*** MkLevel Class ***/
/*********************/

#include	"stdio.h"
#include	<misc.hpp>
#include	<GFName.hpp>
#include	<conio.h>
#include	<iostream.h>
#include	<vector>
#include	<DaveLib.h>

#include	"MkLevel.h"
#include	"Layers\MkLevelLayer.h"

#include	"Layers\MkLevelLayerTile.h"
#include	"Layers\MkLevelLayer3d.h"
#include	"Layers\MkLevelLayerShade.h"
#include	"Layers\MkLevelLayerCollision.h"

#include	"Layers\MkLevelLayerActor.h"
#include	"Layers\MkLevelLayerItem.h"
#include	"Layers\MkLevelLayerPlatform.h"
#include	"Layers\MkLevelLayerFX.h"
#include	"Layers\MkLevelLayerTrigger.h"

//***************************************************************************
GString			ConfigFilename="MkLevel";
sExpLayerTile	BlankTile={-1,-1};

//***************************************************************************
CMkLevel::CMkLevel()
{
		memset(&LvlHdr,0,sizeof(sLvlHdr));
		Tile2dList.Add(BlankTile);
}

//***************************************************************************
CMkLevel::~CMkLevel()
{
}

//***************************************************************************
void	CMkLevel::Init(const char *Filename,const char *OutDir,int TPBase,int TPW,int TPH)
{
// Setup filenames and paths
GFName		Path=Filename;

			InFilename=Filename;
			LevelName=Path.File();
			Path.File("");
			Path.Ext("");
			InPath=Path.FullName();
			OutPath=OutDir; OutPath.Append('\\');
			OutName=OutPath+LevelName;

// Load ini file
#ifdef	_DEBUG
		ConfigFilename="\\spongebob\\tools\\data\\bin\\mklevel.ini";
#endif
		Config.LoadAndImport(ConfigFilename);

// Setup Texgrab
		if (TPBase==-1 || TPW==-1 || TPH==-1) 
			GObject::Error(ERR_FATAL,"TPage not set\n");

		TexGrab.SetTPage(TPBase,TPW,TPH);
		TexGrab.SetOutFile(GString(OutName+".Tex"));
		TexGrab.SetDebug(DebugOn);
		TexGrab.SetDebugOut(GString(OutName+".Lbm"));
		TexGrab.NoSort();
		TexGrab.AnimatedHeadersOnly(true);
		TexGrab.DontOutputBoxes(true);
	//!!!	TexGrab.AllowRotate(true);
		TexGrab.AllowRotate(false);
	//!!!	TexGrab.ShrinkToFit(true);
		TexGrab.ShrinkToFit(false);

// Setup TriList
		OutTriList.SetTexGrab(TexGrab);

// Set up other stuph

		FlatFace[0].vtx[0].x=+0.5f; FlatFace[0].vtx[0].y=+1.0f;	FlatFace[0].vtx[0].z=-4.0f;
		FlatFace[0].vtx[1].x=-0.5f; FlatFace[0].vtx[1].y= 0.0f;	FlatFace[0].vtx[1].z=-4.0f;
		FlatFace[0].vtx[2].x=+0.5f; FlatFace[0].vtx[2].y= 0.0f;	FlatFace[0].vtx[2].z=-4.0f;
		FlatFace[0].uvs[0].u=1;		FlatFace[0].uvs[0].v=1;
		FlatFace[0].uvs[1].u=0;		FlatFace[0].uvs[1].v=0;
		FlatFace[0].uvs[2].u=1;		FlatFace[0].uvs[2].v=0;

		FlatFace[1].vtx[0].x=-0.5f; FlatFace[1].vtx[0].y= 0.0f;	FlatFace[1].vtx[0].z=-4.0f;
		FlatFace[1].vtx[1].x=+0.5f; FlatFace[1].vtx[1].y=+1.0f;	FlatFace[1].vtx[1].z=-4.0f;
		FlatFace[1].vtx[2].x=-0.5f; FlatFace[1].vtx[2].y=+1.0f;	FlatFace[1].vtx[2].z=-4.0f;
		FlatFace[1].uvs[0].u=0;		FlatFace[1].uvs[0].v=0;
		FlatFace[1].uvs[1].u=1;		FlatFace[1].uvs[1].v=1;
		FlatFace[1].uvs[2].u=0;		FlatFace[1].uvs[2].v=1;

}

//***************************************************************************
//*** Load ******************************************************************
//***************************************************************************
void	CMkLevel::Load()
{
FILE		*File;
sExpFileHdr	*FileHdr;
int			Size;

			printf("Load %s\n",LevelName);

			File=fopen(InFilename,"rb");
			if (!File)
				GObject::Error(ERR_FATAL,"%s Not found\n",InFilename);
// Calc File Size
			fseek(File,0,SEEK_END);
			Size=ftell(File);
			fseek(File,0,SEEK_SET);
			FileHdr=(sExpFileHdr*)malloc(Size);
// Load It
			fread(FileHdr,Size,1,File);
			fclose(File);

			LoadTiles(FileHdr);
			LoadLayers(FileHdr);

			free(FileHdr);
}

//***************************************************************************
void		CMkLevel::LoadStrList(CList<GString> &List,char *TexPtr,int Count)
{
char	FullName[256];
GString	FilePath;

		for (int i=0; i<Count; i++)
		{
			if (!strcmp(TexPtr,"BLANK")) GObject::Error(ERR_FATAL,"Old Format. Please Re-export\n");
			GFName::makeabsolute(InPath,TexPtr,FullName);
			List.push_back(GString(FullName));
			TexPtr+=strlen(TexPtr)+1;
		}
}

//***************************************************************************
void	CMkLevel::LoadTiles(sExpFileHdr *FileHdr)
{
u8		*ByteHdr=(u8*)FileHdr;
int		i,ListSize;
int		RGBSize;

		TileW=FileHdr->TileW;
		TileH=FileHdr->TileH;
		RGBSize=TileW*TileH*3;

// Load SetNames
		LoadStrList(InSetNameList,(char*) &ByteHdr[FileHdr->SetNameOfs],FileHdr->SetNameCount);
// Load TexNames
		LoadStrList(InTexNameList,(char*) &ByteHdr[FileHdr->TexNameOfs],FileHdr->TexNameCount);

// Load Tiles
u8		*TilePtr=(u8*) &ByteHdr[FileHdr->TileOfs];

		InTileList.resize(FileHdr->TileCount);
		for (i=0; i<FileHdr->TileCount; i++)
		{
			sExpTile	*ThisTilePtr=(sExpTile*)TilePtr;
			sExpTile	&InTile=InTileList[i];
			
			InTile=*ThisTilePtr;
			InTile.RGB=(u8*)malloc(RGBSize);
			memcpy(InTile.RGB,TilePtr+sizeof(sExpTile),RGBSize);
			TilePtr+=RGBSize+sizeof(sExpTile);
		}

// Load Tris
sExpTri	*TriPtr=(sExpTri*) &ByteHdr[FileHdr->TriOfs];
		InTriList.resize(FileHdr->TriCount);
		for (i=0; i<FileHdr->TriCount; i++)
		{
			InTriList[i]=TriPtr[i];
		}

// Load 2d bitmaps
		ListSize=InSetNameList.size();
		BmpList.resize(ListSize);
		for (i=0; i<ListSize; i++)
		{
			GString	Ext=GFName(InSetNameList[i]).Ext();
			if (Ext=="BMP")
			{
			BmpList[i].LoadBMP(InSetNameList[i]);
			}
		}

}

//***************************************************************************
void	CMkLevel::LoadLayers(sExpFileHdr *FileHdr)
{
u8		*ByteHdr=(u8*)FileHdr;

		for (int i=0; i<FileHdr->LayerCount; i++)
		{
			sExpLayerHdr *LayerHdr=(sExpLayerHdr *)&ByteHdr[FileHdr->LayerOfs[i]];

			switch(LayerHdr->Type)
			{
			case LAYER_TYPE_TILE:
				switch (LayerHdr->SubType)
				{
				case LAYER_SUBTYPE_ACTION:
					LayerList.push_back(new CMkLevelLayer3d(LayerHdr));
					break;
				case LAYER_SUBTYPE_MID:
					LayerList.push_back(new CMkLevelLayerTile(LayerHdr));
					break;
				case LAYER_SUBTYPE_BACK:
				default:
					break;
				}
				break;
			case LAYER_TYPE_COLLISION:
				LayerList.push_back(new CMkLevelLayerCollision(LayerHdr));
				break;
			case LAYER_TYPE_SHADE:
				LayerList.push_back(new CMkLevelLayerShade(LayerHdr));
				break;
			case LAYER_TYPE_ACTOR:
				LayerList.push_back(new CMkLevelLayerActor(LayerHdr));
				break;
			case LAYER_TYPE_ITEM:
				LayerList.push_back(new CMkLevelLayerItem(LayerHdr));
				break;
			case LAYER_TYPE_PLATFORM:
				LayerList.push_back(new CMkLevelLayerPlatform(LayerHdr));
				break;
			case LAYER_TYPE_TRIGGER:
				LayerList.push_back(new CMkLevelLayerTrigger(LayerHdr));
				break;
			case LAYER_TYPE_FX:
				LayerList.push_back(new CMkLevelLayerFX(LayerHdr));
				break;
			default:
				GObject::Error(ERR_FATAL,"Unknown Layer Type\n");
			}
		}
}

//***************************************************************************
//*** Process ***************************************************************
//***************************************************************************
void	CMkLevel::Process()
{
		printf("PreProcess Layers\n");
		PreProcessLayers();
		printf("Process Tilebank\n");
		ProcessTileBanks();
		printf("Process Layers\n");
		ProcessLayers();
		OutTriList.Process();
}

//***************************************************************************
void	CMkLevel::PreProcessLayers()
{
int		LayerCount=LayerList.size();

		for (int i=0; i<LayerCount; i++)
		{
			LayerList[i]->PreProcess(this);
		}
}

//***************************************************************************
void	CMkLevel::ProcessLayers()
{
int		LayerCount=LayerList.size();

		for (int i=0; i<LayerCount; i++)
		{
			LayerList[i]->Process(this);
		}
}

//***************************************************************************
void	CMkLevel::ProcessTileBanks()
{
		PreProcessTileBank2d();
		PreProcessTileBank3d();
		TexGrab.Process();
		ProcessTileBank2d();
		ProcessTileBank3d();
}

//***************************************************************************
void	CMkLevel::PreProcessTileBank2d()
{
int		i,ListSize=Tile2dList.size();

		Tile2dList[0].Tile=-1;
// Extract Tex data from list
		for (i=1; i<ListSize; i++)
		{ // Skip blank
			sExpLayerTile &ThisTile=Tile2dList[i];
			ThisTile.Tile=Create2dTex(ThisTile);
		}
}

//***************************************************************************
void	CMkLevel::ProcessTileBank2d()
{
int		i,ListSize=Tile2dList.size();
vector<sTexOutInfo>	&TexInfo=TexGrab.GetTexInfo();

		OutTile2dList.resize(ListSize);
		for (i=1; i<ListSize; i++)
		{ // Skip blank
			sExpLayerTile	&InTile=Tile2dList[i];
			sTile2d			&OutTile=OutTile2dList[i];

			SetUpTileUV(OutTile,TexInfo[InTile.Tile]);
		}

}


//***************************************************************************
void	CMkLevel::PreProcessTileBank3d()
{
int		i,ListSize=Tile3dList.size();

		for (i=0; i<ListSize; i++)
		{
			sExpLayerTile &ThisTile=Tile3dList[i];
			ThisTile.Tile=Create3dTile(ThisTile);
		}
}

//***************************************************************************
void	CMkLevel::ProcessTileBank3d()
{
int		i,ListSize=Tile3dList.size();

		for (i=0; i<ListSize; i++)
		{

		}
}

//***************************************************************************
void	CMkLevel::SetUpTileUV(sTile2d &Out, sTexOutInfo &Info)
{
int		W = Info.w-1;
int		H = Info.h-1;
float	Inx0, Iny0;
u8		Outx0, Outy0;

// Allow for upside textures now :oP
		Inx0=0;
		Iny0=1-0;

		if (Info.Rotated)
		{
			GObject::Error(ERR_FATAL,"Rotated Texure, cant cope!\n");
		} 

		Outx0 = Info.u +		round(Inx0 * W);
		Outy0 = (Info.v + H) -  round(Iny0 * H);

		Out.u0=Outx0; Out.v0=Outy0;
		Out.TPage=Info.Tpage;
		Out.Clut=Info.Clut;
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
CMkLevelLayer	*CMkLevel::FindLayer(int Type,int SubType)
{
int		ListSize=LayerList.size();

		for (int i=0; i<ListSize; i++)
		{
			if (LayerList[i]->IsType(Type,SubType)) return(LayerList[i]);
		}
		return(0);
}

//***************************************************************************
int		CMkLevel::Create3dTile(sExpLayerTile &InTile)
{
sExpTile		&SrcTile=InTileList[InTile.Tile];
CFace			F;
int				i,ListSize;
CList<sExpTri>	SortList;
CList<float>	ZPosList;

			if (SrcTile.TriCount)
			{
				for (i=0; i<SrcTile.TriCount; i++)
				{
					int		ListPos;
					sExpTri	ThisTri=InTriList[SrcTile.TriStart+i];
					float	ThisZPos;
					

					ThisZPos=ThisTri.vtx[0].z;
					if (ThisZPos>ThisTri.vtx[1].z) ThisZPos=ThisTri.vtx[1].z;
					if (ThisZPos>ThisTri.vtx[2].z) ThisZPos=ThisTri.vtx[2].z;
					
					ListSize=SortList.size();
					for (ListPos=0; ListPos<ListSize; ListPos++)
					{
						if (ZPosList[ListPos]<ThisZPos) break;
					}
					// Flip 3d tiles
					bool	SwapPnt=false;

					if (InTile.Flags & PC_TILE_FLAG_MIRROR_X)	
					{
						ThisTri.vtx[0].x=-ThisTri.vtx[0].x;
						ThisTri.vtx[1].x=-ThisTri.vtx[1].x;
						ThisTri.vtx[2].x=-ThisTri.vtx[2].x;
						SwapPnt^=1;
					}

					if (InTile.Flags & PC_TILE_FLAG_MIRROR_Y)	
					{
						ThisTri.vtx[0].y	=1.0-ThisTri.vtx[0].y;
						ThisTri.vtx[1].y	=1.0-ThisTri.vtx[1].y;
						ThisTri.vtx[2].y	=1.0-ThisTri.vtx[2].y;
						SwapPnt^=1;
					}
					if (SwapPnt)
					{
						Vector3	TmpV=ThisTri.vtx[0];
						ThisTri.vtx[0]=ThisTri.vtx[1];
						ThisTri.vtx[1]=TmpV;
						float	TmpUVu=ThisTri.uv[0][0];
						float	TmpUVv=ThisTri.uv[0][1];
						ThisTri.uv[0][0]=ThisTri.uv[1][0];
						ThisTri.uv[0][1]=ThisTri.uv[1][1];
						ThisTri.uv[1][0]=TmpUVu;
						ThisTri.uv[1][1]=TmpUVv;
					}
					SortList.insert(ListPos,ThisTri);
					ZPosList.insert(ListPos,ThisZPos);
				}

// Add sorted list to main list

				for (i=0; i<SrcTile.TriCount; i++)
				{
					sExpTri &ThisTri=SortList[i];
					CFace	F;
					F.TexName=InTexNameList[ThisTri.TexID];
					F.Mat=-1;

					for (int p=0; p<3; p++)
					{
						F.vtx[p]=ThisTri.vtx[p];
						F.uvs[p].u=ThisTri.uv[p][0];
						F.uvs[p].v=ThisTri.uv[p][1];
					}
					OutTriList.AddFace(F,true);
				}
			}
			else
			{ // create flat tile
				int		TexID=Create2dTex(InTile);
				for (int i=0; i<2; i++)
				{
					FlatFace[i].Mat=TexID;
					OutTriList.AddFace(FlatFace[i],true);
				}
			}

		return(0);
}


//***************************************************************************
int		CMkLevel::Create2dTex(sExpLayerTile &InTile)
{
sExpTile	&SrcTile=InTileList[InTile.Tile];
int			Idx;
sMkLevelTex	InTex;

//		InTex.Set=UsedSetNameList.Add(InSetNameList[SrcTile.Set]);
		InTex.Set=SrcTile.Set;//UsedSetNameList.Add(InSetNameList[SrcTile.Set]);
		InTex.Flags=InTile.Flags;
		InTex.XOfs=SrcTile.XOfs;
		InTex.YOfs=SrcTile.YOfs;
		InTex.RGB=SrcTile.RGB;

		Idx=Tex2dList.Find(InTex);
		if (Idx!=-1) return(Idx);

// Try and find RGB data match
		Idx=FindRGBMatch(InTex);
		if (Idx!=-1) return(Idx);

// Must be new, add it
		BuildTileTex(InTex);
		Tex2dList.push_back(InTex);
		return(InTex.TexID);
}

//***************************************************************************
int		CMkLevel::BuildTileTex(sMkLevelTex &InTex)
{
		ASSERT(InTex.Set>=0 && InTex.Set<InSetNameList.size())
Frame			&InFrame=BmpList[InTex.Set];
Frame			ThisFrame;
Rect			ThisRect;
GString			Name=GFName(InSetNameList[InTex.Set]).File();
GString			TexName;
int				BmpW=InFrame.GetWidth();
int				BmpH=InFrame.GetHeight();

				TexGrab.ShrinkToFit(false);
				TexGrab.AllowRotate(false);

				if (InTex.XOfs*16>BmpW) 
				{
					printf("AARGH!!! %s(%i) wants X=%i,tile is only %i Wide\n",Name,InTex.Set,InTex.XOfs*16,BmpW);
					InTex.XOfs=0;
				}
				if (InTex.YOfs*16>BmpH) 
				{
					printf("AARGH!!! %s(%i) wants Y=%i,tile is only %i High\n",Name,InTex.Set,InTex.YOfs*16,BmpH);
					InTex.YOfs=0;
				}

				MakeTexName(InTex,TexName);

				ThisRect.X=InTex.XOfs*16;
				ThisRect.Y=InTex.YOfs*16;
				ThisRect.W=16;
				ThisRect.H=16;

				ThisFrame.Grab(InFrame,ThisRect);

				if (InTex.Flags& PC_TILE_FLAG_MIRROR_X)		ThisFrame.FlipX();
				if (InTex.Flags & PC_TILE_FLAG_MIRROR_Y)	ThisFrame.FlipY();
				
				InTex.TexID=TexGrab.AddMemFrame(TexName,ThisFrame);

#ifdef	_DEBUG
				if (0)
				{
					if (!ThisFrame.IsBlank())
					{
					char	DbgName[256];
					sprintf(DbgName,"/x/%s.lbm",TexName);
					ThisFrame.SaveLbm(DbgName);
					}
				}
#endif
			return(InTex.TexID);
}

//***************************************************************************
void	CMkLevel::MakeTexName(sMkLevelTex &InTex,GString &OutStr)
{
char			NewName[256];
GString			Name=GFName(InSetNameList[InTex.Set]).File();
			
		sprintf(NewName,"%s_%02d_%02d_%01d_",Name,InTex.XOfs,InTex.YOfs,InTex.Flags&PC_TILE_FLAG_MIRROR_XY);
		OutStr=NewName;
}

//***************************************************************************
int		CMkLevel::FindRGBMatch(sMkLevelTex &ThisTex)
{
int		i,ListSize=Tex2dList.size();
int		Size=TileW*TileH;
u8		*RGBPtr=ThisTex.RGB;

// Create Checksum for this tile
		ThisTex.RChk=0;
		ThisTex.GChk=0;
		ThisTex.BChk=0;
		for (i=0; i<Size; i++)
		{
			ThisTex.RChk+=*RGBPtr++;
			ThisTex.GChk+=*RGBPtr++;
			ThisTex.BChk+=*RGBPtr++;
		}
// Check all others for match
		for (i=0; i<ListSize; i++)
		{
			sMkLevelTex &ChkTex=Tex2dList[i];
			if (ChkTex.RGB)
			{
// Checksum first
				if (ThisTex.RChk==ChkTex.RChk && ThisTex.GChk==ChkTex.GChk && ThisTex.BChk==ChkTex.BChk) 
				{
					if (IsRGBSame(ThisTex,ChkTex)) return(i);
				}
			}
		}
		return(-1);
}

//***************************************************************************
bool	CMkLevel::IsRGBSame(const sMkLevelTex &Tex0,const sMkLevelTex &Tex1)
{
int		W=TileW;
int		H=TileH;
int		XOfs0,YOfs0;
int		XOfs1,YOfs1;
u8		*RGB0=Tex0.RGB;
u8		*RGB1=Tex1.RGB;

		XOfs0=YOfs0=0;
		XOfs1=YOfs1=0;

		if (Tex0.Flags & PC_TILE_FLAG_MIRROR_X) XOfs0=W-1;
		if (Tex0.Flags & PC_TILE_FLAG_MIRROR_Y) YOfs0=H-1;
		if (Tex1.Flags & PC_TILE_FLAG_MIRROR_X) XOfs1=W-1;
		if (Tex1.Flags & PC_TILE_FLAG_MIRROR_Y) YOfs1=H-1;

		for (int Y=0; Y<H; Y++)
		{
			for (int X=0; X<W; X++)
			{
				int	X0=abs(XOfs0-X);
				int	Y0=abs(YOfs0-Y);
				int	X1=abs(XOfs1-X);
				int	Y1=abs(YOfs1-Y);
				int	Idx0=X0+(Y0*W);
				int	Idx1=X1+(Y1*W);

				if (RGB0[Idx0+0]!=RGB1[Idx1+0]) return(false);
				if (RGB0[Idx0+1]!=RGB1[Idx1+1]) return(false);
				if (RGB0[Idx0+2]!=RGB1[Idx1+2]) return(false);
			}
		}
		return(true);
}

//***************************************************************************
//*** Write *****************************************************************
//***************************************************************************
void	CMkLevel::Write()
{
		WriteLevel();
		WriteTileBank();
}

//***************************************************************************
void	CMkLevel::WriteTileBank()
{
GString	OutFilename=OutName+".Tbk";
int		i,ListSize;
		File=fopen(OutFilename,"wb");

		fwrite(&TileBankHdr,1,sizeof(sTileBankHdr),File);

// 2d Tilebank
		TileBankHdr.TileBank2d=(sTile2d*)ftell(File);
		ListSize=OutTile2dList.size();
		for (i=0; i<ListSize; i++)
		{
			sTile2d	&OutTile=OutTile2dList[i];
			fwrite(&OutTile,1,sizeof(sTile2d),File);
		}
// 3d Tilebank
		TileBankHdr.TileBank3d=(sTile3d*)ftell(File);
		ListSize=OutTile3dList.size();
		for (i=0; i<ListSize; i++)
		{
			sTile3d	&OutTile=OutTile3dList[i];
			fwrite(&OutTile,1,sizeof(sTile3d),File);
		}
// TriList
		TileBankHdr.TriList=(sTri*)WriteTriList();
		
// QuadList

// VtxList
		TileBankHdr.VtxList=(sVtx*)ftell(File);
		OutTriList.WriteVtxList(File);

// rewrite header
		fseek(File,0,SEEK_SET);
		fwrite(&TileBankHdr,1,sizeof(sTileBankHdr),File);

		fclose(File);
}


//***************************************************************************
int		CMkLevel::WriteTriList()
{
vector<sTri>	&TriList=OutTriList.GetOutTriList();
vector<sVtx> const	&VtxList=OutTriList.GetVtxList();
int				ThisPos=ftell(File);
int				i,ListSize=TriList.size();
int				MaxOT=0;		
		for (i=0;i<ListSize;i++)
		{
			sTri	&T=TriList[i];
			int		Z[3];
			int		OtOfs=0;
// Calc OtOfs
			Z[0]=abs(VtxList[T.P0].vz);
			Z[1]=abs(VtxList[T.P1].vz);
			Z[2]=abs(VtxList[T.P2].vz);
			
			for (int p=0; p<3; p++)
			{
				if (OtOfs<Z[p]) OtOfs=Z[p];
			}
			if (MaxOT<OtOfs) MaxOT=OtOfs;
			if (OtOfs>63) OtOfs=63;
// Write It			
			fwrite(&T,1,sizeof(sTri),File);
		}
		printf("MAXOT = %i\n",MaxOT);

		return(ThisPos);

}

//***************************************************************************
//*** Write *****************************************************************
//***************************************************************************
void	CMkLevel::WriteLevel()
{
GString	OutFilename=OutName+".Lvl";

		File=fopen(OutFilename,"wb");

		fwrite(&LvlHdr,1,sizeof(sLvlHdr),File);

		WriteLayers();

// rewrite header
		fseek(File,0,SEEK_SET);
		fwrite(&LvlHdr,1,sizeof(sLvlHdr),File);

		fclose(File);
}

//***************************************************************************
void	CMkLevel::WriteLayers()
{
// Back (Shade)
		LvlHdr.BackLayer=WriteLayer(LAYER_TYPE_SHADE,LAYER_SUBTYPE_BACK,0);//"Shade");
// Mid
		LvlHdr.MidLayer=WriteLayer(LAYER_TYPE_TILE,LAYER_SUBTYPE_MID,"Mid");
// Action
		LvlHdr.ActionLayer=WriteLayer(LAYER_TYPE_TILE,LAYER_SUBTYPE_ACTION,"Action");
// Fore
//		LvlHdr.ForeLayer=WriteLayer(LAYER_TYPE_TILE,LAYER_SUBTYPE_FORE,"Fore");
// Collision
		LvlHdr.CollisionLayer=WriteLayer(LAYER_TYPE_COLLISION,LAYER_SUBTYPE_NONE,"Collision");

// Things
		LvlHdr.ActorList=WriteThings(LAYER_TYPE_ACTOR,"Actor List");
		LvlHdr.ItemList=WriteThings(LAYER_TYPE_ITEM,"Item List");
		LvlHdr.PlatformList=WriteThings(LAYER_TYPE_PLATFORM,"Platform List");
		LvlHdr.TriggerList=WriteThings(LAYER_TYPE_TRIGGER,"FX List");
		LvlHdr.FXList=WriteThings(LAYER_TYPE_FX,"FX List");
}

//***************************************************************************
int		CMkLevel::WriteLayer(int Type,int SubType,const char *LayerName)
{
CMkLevelLayer	*ThisLayer=FindLayer(Type,SubType);
int		Ofs;

		if (!ThisLayer)
		{
			if (LayerName) GObject::Error(ERR_WARNING,"No %s Layer Found in %s!!\n",LayerName,LevelName);
			return(0);
		}
		Ofs=ThisLayer->Write(File,LayerName,LevelName);

		PadFile(File);
		return(Ofs);
}

//***************************************************************************
int		CMkLevel::WriteThings(int Type,const char *LayerName)
{
CMkLevelLayer	*ThisLayer=FindLayer(Type,LAYER_SUBTYPE_NONE);
int		Ofs;

		if (!ThisLayer)
		{
			GFName		Name=InFilename;
			if (LayerName) GObject::Error(ERR_WARNING,"No %s Layer Found in %s!!\n",LayerName,Name.File());
			return(0);
		}
		Ofs=ThisLayer->Write(File,LayerName,LevelName);
		PadFile(File);
		return(Ofs);
}
