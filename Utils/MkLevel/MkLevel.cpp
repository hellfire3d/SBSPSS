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
#include	"Layers\MkLevelLayerHazard.h"

//***************************************************************************
const GString	ConfigFilename="MkLevel.ini";
sExpLayerTile	BlankTile2d={-1,-1};
sExpLayerTile	BlankTile3d={0,0};

//***************************************************************************
CMkLevel::CMkLevel()
{
		memset(&LevelHdr,0,sizeof(sLevelHdr));
		Tile2dList.Add(BlankTile2d);
		Tile3dList.Add(BlankTile3d);
		OutTile3dList.resize(1);
		OutTile3dList[0].TriCount=0;
		OutTile3dList[0].QuadCount=0;
}

//***************************************************************************
CMkLevel::~CMkLevel()
{
}

//***************************************************************************
void	CMkLevel::SetAppDir(const char *AppPath)
{
#ifdef	_DEBUG
		AppDir="\\spongebob\\tools\\data\\bin\\";
#else
GFName	Path=AppPath;

		Path.File("");
		Path.Ext("");
		AppDir=Path.FullName();
#endif
}

//***************************************************************************
void	CMkLevel::Init(const char *Filename,const char *OutFilename,int TPBase,int TPW,int TPH)
{
// Setup filenames and paths
GFName		Path;

			InFilename=Filename;
			Path=Filename;
			LevelName=Path.File();
			Path.File("");
			Path.Ext("");
			InPath=Path.FullName();
			Path=OutFilename;
			Path.Ext("");
			OutName=Path.FullName();

// Load ini file
			Config.LoadAndImport(GString(AppDir+ConfigFilename));

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
		OutFaceList.SetTexGrab(TexGrab);
// Set up other stuph

		FlatFace[0].vtx[0].x=+0.5f; FlatFace[0].vtx[0].y=+1.0f;	FlatFace[0].vtx[0].z=-4.0f;
		FlatFace[0].vtx[1].x=-0.5f; FlatFace[0].vtx[1].y= 0.0f;	FlatFace[0].vtx[1].z=-4.0f;
		FlatFace[0].vtx[2].x=+0.5f; FlatFace[0].vtx[2].y= 0.0f;	FlatFace[0].vtx[2].z=-4.0f;
		FlatFace[0].uv[0][0]=1;		FlatFace[0].uv[0][1]=1;
		FlatFace[0].uv[1][0]=0;		FlatFace[0].uv[1][1]=0;
		FlatFace[0].uv[2][0]=1;		FlatFace[0].uv[2][1]=0;

		FlatFace[1].vtx[0].x=-0.5f; FlatFace[1].vtx[0].y= 0.0f;	FlatFace[1].vtx[0].z=-4.0f;
		FlatFace[1].vtx[1].x=+0.5f; FlatFace[1].vtx[1].y=+1.0f;	FlatFace[1].vtx[1].z=-4.0f;
		FlatFace[1].vtx[2].x=-0.5f; FlatFace[1].vtx[2].y=+1.0f;	FlatFace[1].vtx[2].z=-4.0f;
		FlatFace[1].uv[0][0]=0;		FlatFace[1].uv[0][1]=0;
		FlatFace[1].uv[1][0]=1;		FlatFace[1].uv[1][1]=1;
		FlatFace[1].uv[2][0]=0;		FlatFace[1].uv[2][1]=1;
}


//***************************************************************************
int		CMkLevel::AddModel(GString &Filename)
{
GFName			Path=Filename;
sMkLevelModel	ThisModel;
int				Idx;
CScene			Scene;

		ThisModel.Name=GFName(Filename).File();
		Idx=ModelList.Find(ThisModel);

		if (Idx!=-1)
		{
			return(Idx);
		}
		Idx=ModelList.size();

		Path.File("");
		Path.Ext("");
GString	RootPath=Path.FullName();
// Load Model and add
int		TriStart=ModelFaceList.GetFaceCount();;
		Scene.Load(Filename);
		BuildModel(Scene,RootPath,0);
		ThisModel.TriStart=TriStart;
		ThisModel.TriCount=ModelFaceList.GetFaceCount()-TriStart;

		ModelList.Add(ThisModel);
		return(Idx);
}

//***************************************************************************
void	CMkLevel::BuildModel(CScene &Scene,GString &RootPath,int Node)
{
CNode					&ThisNode=Scene.GetNode(Node);
vector<sGinTri> const	&NodeTriList =	ThisNode.GetTris();
vector<Vector3> const	&NodeVtxList =	ThisNode.GetPts();
vector<int>	const		&NodeMatList =	ThisNode.GetTriMaterial();
vector<sUVTri> const	&NodeUVList =	ThisNode.GetUVTris();
vector<GString> const	&SceneTexList=	Scene.GetTexList();
vector<int> const		&SceneUsedMatList=Scene.GetUsedMaterialIdx();
vector<Material> const	&SceneMaterials=Scene.GetMaterials();

int						TriCount=NodeTriList.size();

			for (int T=0; T<TriCount; T++)
			{
				int		Mat=SceneUsedMatList[NodeMatList[T]];
				if (Mat>SceneTexList.size()) GObject::Error(ERR_FATAL,"Crap Material ID, wanted %i, only have %i\n",Mat,SceneTexList.size());
				GString	TexName=RootPath+SceneTexList[Mat];
				

				CFace &F=ModelFaceList.AddFace( NodeVtxList, NodeTriList[T], NodeUVList[T], TexName,SceneMaterials[Mat].Flags,false);
//				ModelFaceList.SetTPageFlag(F,SceneMaterials[Mat].Flags);
			}

int		ChildCount=ThisNode.GetPruneChildCount();
		for (int Loop=0;Loop<ChildCount ; Loop++) BuildModel(Scene,RootPath,ThisNode.PruneChildList[Loop]);
}

//***************************************************************************
int		CMkLevel::AddModel(const char *Name,int TriStart,int TriCount)
{
sMkLevelModel	ThisModel;
int				Idx;

		ThisModel.Name=Name;
		Idx=ModelList.Find(ThisModel);

		if (Idx!=-1)
		{
			return(Idx);
		}
		Idx=ModelList.size();
		ThisModel.TriStart=ModelFaceList.GetFaceCount();
		ThisModel.TriCount=TriCount;


// Add tri data
		for (int i=0;i<TriCount; i++)
		{
			sExpTri	&ThisTri=InTriList[TriStart+i];
			CFace	F;

			ExpTri2Face(ThisTri,F);
			ModelFaceList.SetTPageFlag(F,ThisTri.Flags);
			ModelFaceList.AddFace(F,false);
		}

		ModelList.Add(ThisModel);
		return(Idx);
}

//***************************************************************************
void	CMkLevel::ProcessModels()
{
int		i,ListSize;
int		TriStart=OutFaceList.GetFaceCount();

// Add faces
		ListSize=ModelFaceList.GetFaceCount();
		for (i=0; i<ListSize; i++)
		{
			OutFaceList.AddFace(ModelFaceList[i],true);
		}

// Update models
		ListSize=ModelList.size();
		for (i=0; i<ListSize; i++)
		{
			printf("%s = %i %i\n",ModelList[i].Name,ModelList[i].TriStart,ModelList[i].TriCount);
			ModelList[i].TriStart+=TriStart;
		}
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
char	FullName[1024];
GString	FilePath;

		for (int i=0; i<Count; i++)
		{
			GString	InName=InPath;
			InName+=TexPtr;
			GFName::makeabsolute(InPath,InName,FullName);
			InName=FullName;
			_fullpath( FullName, FullName, 1024);
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
//			InTile.RGB=(u8*)malloc(RGBSize);
//			memcpy(InTile.RGB,TilePtr+sizeof(sExpTile),RGBSize);
			InTile.RGB=0;
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
			case LAYER_TYPE_HAZARD:
				LayerList.push_back(new CMkLevelLayerHazard(LayerHdr));
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
		printf("Process Models\n");
		ProcessModels();
		printf("Process Tilebank\n");
		ProcessTileBanks();
		printf("Process Layers\n");
		ProcessLayers();
		OutFaceList.Process();
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
			ThisTile.Tile=Create2dTex(ThisTile.Tile,ThisTile.Flags);
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
		
		for (i=1; i<ListSize; i++)
		{ // Skip Blank
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
void	CMkLevel::ExpTri2Face(sExpTri &ThisTri,CFace &F,bool ImportTex)
{
		if (ImportTex)
		{
			F.TexName=InTexNameList[ThisTri.TexID];
			F.Mat=-1;
		}
		else
		{
			F.Mat=ThisTri.TexID;
		}

		for (int p=0; p<3; p++)
		{
			F.vtx[p]=ThisTri.vtx[p];
			F.vtx[p].y=F.vtx[p].y;
			F.uvs[p].u=ThisTri.uv[p][0];
			F.uvs[p].v=ThisTri.uv[p][1];
		}
		

}
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
int				i,ListSize,p;
CList<sExpTri>	SortList;
CList<float>	ZPosList;
sTile3d			ThisTile;
int				TileID=OutTile3dList.size();

			ThisTile.TriStart=OutFaceList.GetFaceCount();
			ThisTile.QuadCount=0;
			ThisTile.QuadStart=0;

			if (SrcTile.TriCount)
			{
				ThisTile.TriCount=SrcTile.TriCount;
				for (i=0; i<SrcTile.TriCount; i++)
				{
					int		ListPos;
					sExpTri	ThisTri=InTriList[SrcTile.TriStart+i];
					float	ThisZPos;

					ThisZPos=ThisTri.vtx[0].z;
					if (ThisZPos<ThisTri.vtx[1].z) ThisZPos=ThisTri.vtx[1].z;
					if (ThisZPos<ThisTri.vtx[2].z) ThisZPos=ThisTri.vtx[2].z;
					
					ListSize=SortList.size();
					for (ListPos=0; ListPos<ListSize; ListPos++)
					{
						if (ZPosList[ListPos]>ThisZPos) break;
					}
					SortList.insert(ListPos,ThisTri);
					ZPosList.insert(ListPos,ThisZPos);
				}

			}
			else
			{ // create flat tile (This is WRONG, flip tex are gen, need to flip goem)
				int		TexID=Create2dTex(InTile.Tile,InTile.Flags);

				ThisTile.TriCount=2;
				for (int i=0; i<2; i++)
					{
						FlatFace[i].TexID=TexID;
						SortList.push_back(FlatFace[i]);
					}
				InTile.Flags=0;
			}

// Add sorted list to main list
			ListSize=SortList.size();
			for (i=0; i<ListSize; i++)
			{
				sExpTri &ThisTri=SortList[i];
				CFace	F;
				bool	SwapPnt=false;

				if (SrcTile.TriCount)
				{
					F.TexName=InTexNameList[ThisTri.TexID];
					F.Mat=-1;
				}
				else
				{
					F.Mat=ThisTri.TexID;
				}

				for (p=0; p<3; p++)
				{
					F.vtx[p]=ThisTri.vtx[p];
					F.vtx[p].y=F.vtx[p].y;
					F.uvs[p].u=ThisTri.uv[p][0];
					F.uvs[p].v=ThisTri.uv[p][1];
				}

				if (InTile.Flags & PC_TILE_FLAG_MIRROR_X)
				{
					F.vtx[0].x=-F.vtx[0].x;
					F.vtx[1].x=-F.vtx[1].x;
					F.vtx[2].x=-F.vtx[2].x;
					SwapPnt^=1;
				}

				if (InTile.Flags & PC_TILE_FLAG_MIRROR_Y)
				{
					F.vtx[0].y	=1.0-F.vtx[0].y;
					F.vtx[1].y	=1.0-F.vtx[1].y;
					F.vtx[2].y	=1.0-F.vtx[2].y;
					SwapPnt^=1;
				}

				if (SwapPnt)
				{
					Vector3	TmpV=F.vtx[0];
					F.vtx[0]=F.vtx[1];
					F.vtx[1]=TmpV;
					sUV		TmpUV=F.uvs[0];
					F.uvs[0]=F.uvs[1];
					F.uvs[1]=TmpUV;
				}
				OutFaceList.SetTPageFlag(F,ThisTri.Flags);
				OutFaceList.AddFace(F,true);
			}

			OutTile3dList.push_back(ThisTile);

			return(TileID);
}


//***************************************************************************
int		CMkLevel::Create2dTex(int Tile,int Flags)
{
sExpTile	&SrcTile=InTileList[Tile];
//sMkLevelTex	InTex;

//		InTex.RGB=SrcTile.RGB;

// Try and find RGB data match
// not working
//		Idx=FindRGBMatch(InTex);
//		if (Idx!=-1) return(Idx);

// Must be new, add it
//		InTex.Set=SrcTile.Set;
//		InTex.Flags=Flags;
//		InTex.XOfs=SrcTile.XOfs;
//		InTex.YOfs=SrcTile.YOfs;

int		TexID=BuildTileTex(SrcTile,Flags);
//		Tex2dList.push_back(InTex);
		return(TexID);
}

//***************************************************************************
int		CMkLevel::BuildTileTex(sExpTile	&SrcTile,int Flags)
{
Frame			&InFrame=BmpList[SrcTile.Set];
Frame			ThisFrame;
Rect			ThisRect;
GString			Name=GFName(InSetNameList[SrcTile.Set]).File();
GString			TexName;
int				BmpW=InFrame.GetWidth();
int				BmpH=InFrame.GetHeight();
int				TexID;
				TexGrab.ShrinkToFit(false);
				TexGrab.AllowRotate(false);

				if (SrcTile.XOfs*16>BmpW) 
				{
					printf("AARGH!!! %s(%i) wants X=%i,tile is only %i Wide\n",Name,SrcTile.Set,SrcTile.XOfs*16,BmpW);
					SrcTile.XOfs=0;
				}
				if (SrcTile.YOfs*16>BmpH) 
				{
					printf("AARGH!!! %s(%i) wants Y=%i,tile is only %i High\n",Name,SrcTile.Set,SrcTile.YOfs*16,BmpH);
					SrcTile.YOfs=0;
				}

				MakeTexName(SrcTile,Flags,TexName);

				ThisRect.X=SrcTile.XOfs*16;
				ThisRect.Y=SrcTile.YOfs*16;
				ThisRect.W=16;
				ThisRect.H=16;

				ThisFrame.Grab(InFrame,ThisRect);

				if (Flags& PC_TILE_FLAG_MIRROR_X)		ThisFrame.FlipX();
				if (Flags & PC_TILE_FLAG_MIRROR_Y)	ThisFrame.FlipY();
				
				TexID=TexGrab.AddMemFrame(TexName,ThisFrame);

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
			return(TexID);
}

//***************************************************************************
void	CMkLevel::MakeTexName(sExpTile &SrcTile,int Flags,GString &OutStr)
{
char			NewName[256];
GString			Name=GFName(InSetNameList[SrcTile.Set]).File();
			
		sprintf(NewName,"%s_%02d_%02d_%01d_",Name,SrcTile.XOfs,SrcTile.YOfs,Flags&PC_TILE_FLAG_MIRROR_XY);
		OutStr=NewName;
}

//***************************************************************************
/*
int		CMkLevel::FindRGBMatch(sMkLevelTex &ThisTex)
{
int		i,ListSize=Tex2dList.size();
int		Size=TileW*TileH;
u8		*RGBPtr=ThisTex.RGB;

		if (!RGBPtr) printf("HA HA\n");
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
					if (ThisTex.Flags==ChkTex.Flags)
					{
						if (IsRGBSame(ThisTex,ChkTex)) return(i);
					}
				}
			}
		}
		return(-1);
}
*/
//***************************************************************************
/*
bool	CMkLevel::IsRGBSame(const sMkLevelTex &Tex0,const sMkLevelTex &Tex1)
{
int		Size=TileW*TileH*3;
int		H=TileH;
u8		*RGB0=Tex0.RGB;
u8		*RGB1=Tex1.RGB;

int	Res=memcmp(RGB0,RGB1,Size);

	return(Res==0);
}
*/
/*
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

*/
//***************************************************************************
//*** Write *****************************************************************
//***************************************************************************
void	CMkLevel::Write()
{
GString	OutFilename=OutName+".Lvl";

		File=fopen(OutFilename,"wb");

		fwrite(&LevelHdr,1,sizeof(sLevelHdr),File);

		WriteLevel();
		WriteTileBank();

// rewrite header
		fseek(File,0,SEEK_SET);
		fwrite(&LevelHdr,1,sizeof(sLevelHdr),File);

		fclose(File);

}

//***************************************************************************
int		MinOT=123456,MaxOT=0;		

void	CMkLevel::WriteTileBank()
{
int		i,ListSize;

// 2d Tilebank
		LevelHdr.TileBank2d=(sTile2d*)ftell(File);
		ListSize=OutTile2dList.size();
		printf("%i 2d tiles\n",ListSize);
		for (i=0; i<ListSize; i++)
		{
			sTile2d	&OutTile=OutTile2dList[i];
			fwrite(&OutTile,1,sizeof(sTile2d),File);
		}
// 3d Tilebank
		LevelHdr.TileBank3d=(sTile3d*)ftell(File);
		ListSize=OutTile3dList.size();
		printf("%i 3d tiles\n",ListSize);
		for (i=0; i<ListSize; i++)
		{
			sTile3d	&OutTile=OutTile3dList[i];
			fwrite(&OutTile,1,sizeof(sTile3d),File);
		}
// TriList
		LevelHdr.TriList=(sTri*)WriteTriList();
		
// QuadList
		LevelHdr.QuadList=(sQuad*)WriteQuadList();
		printf("OT %i -> %i\n",MinOT,MaxOT);

// VtxList
		LevelHdr.VtxList=(sVtx*)WriteVtxList();
//		LevelHdr.VtxList=(sVtx*)OutFaceList.WriteVtxList(File);
}


//***************************************************************************
int	ZMin=9999,ZMax=0;
int		CMkLevel::WriteTriList()
{
vector<sTri>	&TriList=OutFaceList.GetOutTriList();
vector<sVtx> const	&VtxList=OutFaceList.GetVtxList();
int				ThisPos=ftell(File);
int				i,ListSize=TriList.size();
int				ZOfs=+4*Scale;

		for (i=0;i<ListSize;i++)
		{
			sTri	&T=TriList[i];
			int		OtOfs=0;
			int		Z[3];

// Calc OtOfs
			Z[0]=VtxList[T.P0].vz+ZOfs;
			Z[1]=VtxList[T.P1].vz+ZOfs;
			Z[2]=VtxList[T.P2].vz+ZOfs;
			
			for (int p=0; p<3; p++)
			{
				if (ZMin>Z[p]) ZMin=Z[p];
				if (ZMax<Z[p]) ZMax=Z[p];
				OtOfs+=Z[p]*Z[p];
			}
			OtOfs=(int)sqrt(OtOfs/3);
			
			OtOfs/=8;
//			printf("%i\n",OtOfs);
			if (MinOT>OtOfs) MinOT=OtOfs;
			if (MaxOT<OtOfs) MaxOT=OtOfs;
			if (OtOfs>15) OtOfs=15;
			if (OtOfs<0) OtOfs=0;

//			if (OtOfs>15) OtOfs=15;
			T.OTOfs=OtOfs;

// Write It			
			fwrite(&T,1,sizeof(sTri),File);
		}
		printf("Tri %i\n",ListSize);
		printf("ZMin %i ZMax %i\n",ZMin,ZMax);

		return(ThisPos);

}

//***************************************************************************
int		CMkLevel::WriteQuadList()
{
vector<sQuad>	&QuadList=OutFaceList.GetOutQuadList();
vector<sVtx> const	&VtxList=OutFaceList.GetVtxList();
int				ThisPos=ftell(File);
int				i,ListSize=QuadList.size();

		for (i=0;i<ListSize;i++)
		{
			sQuad	&Q=QuadList[i];
			int		Z[4];
			int		OtOfs=0;
// Calc OtOfs
			Z[0]=abs(VtxList[Q.P0].vz);
			Z[1]=abs(VtxList[Q.P1].vz);
			Z[2]=abs(VtxList[Q.P2].vz);
			Z[3]=abs(VtxList[Q.P3].vz);
			
			for (int p=0; p<4; p++)
			{
				if (OtOfs<Z[p]) OtOfs=Z[p];
			}
			if (MinOT>OtOfs) MinOT=OtOfs;
			if (MaxOT<OtOfs) MaxOT=OtOfs;
			if (OtOfs>63) OtOfs=63;
// Write It			
			fwrite(&Q,1,sizeof(sQuad),File);
		}
		printf("Quad %i\n",ListSize,MaxOT);
		return(ThisPos);

}

//***************************************************************************
int		CMkLevel::WriteVtxList()
{
vector<sVtx> const	&VtxList=OutFaceList.GetVtxList();
int		i,ListSize=VtxList.size();
int		Pos=ftell(File);
//sVtx	Ofs;

//		Ofs.vx=-0;
//		Ofs.vy=-0;
//		Ofs.vz=-4*Scale;

		for (i=0; i<ListSize; i++)
		{
			sVtx const	&In=VtxList[i];
			sVtx		Out;

			Out.vx=+In.vx;
			Out.vy=-In.vy;
			Out.vz=+In.vz;
//			printf("%i\n",Out.vz);
			fwrite(&Out,1,sizeof(sVtx),File);
		}
		return(Pos);
}

//***************************************************************************
//*** Write *****************************************************************
//***************************************************************************
void	CMkLevel::WriteLevel()
{
		WriteLayers();

}

//***************************************************************************
void	CMkLevel::WriteLayers()
{
// Back (Shade)
		LevelHdr.BackLayer=WriteLayer(LAYER_TYPE_SHADE,LAYER_SUBTYPE_BACK,0);//"Shade");
// Mid
		LevelHdr.MidLayer=WriteLayer(LAYER_TYPE_TILE,LAYER_SUBTYPE_MID,"Mid");
// Action
		LevelHdr.ActionLayer=WriteLayer(LAYER_TYPE_TILE,LAYER_SUBTYPE_ACTION,"Action");
// Collision
		LevelHdr.CollisionLayer=WriteLayer(LAYER_TYPE_COLLISION,LAYER_SUBTYPE_NONE,"Collision");

// Things
		LevelHdr.ActorList=WriteThings(LAYER_TYPE_ACTOR,"Actor List");
		LevelHdr.ItemList=WriteThings(LAYER_TYPE_ITEM,"Item List");
		LevelHdr.PlatformList=WriteThings(LAYER_TYPE_PLATFORM,"Platform List");
		LevelHdr.TriggerList=WriteThings(LAYER_TYPE_TRIGGER,"Trigger List");
		LevelHdr.FXList=WriteThings(LAYER_TYPE_FX,"FX List");
		LevelHdr.HazardList=WriteThings(LAYER_TYPE_HAZARD,"Hazard List");
		LevelHdr.ModelList=(sModel*)WriteModelList();
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
//		printf("%s %i\n",LayerName,Ofs);
		PadFile(File);
		return(Ofs);
}

//***************************************************************************
int		CMkLevel::WriteModelList()
{
int		i,ListSize=ModelList.size();
int		Ofs=ftell(File);

		for (i=0; i<ListSize; i++)
		{
			sModel				Out;
			sMkLevelModel		&ThisModel=ModelList[i];

			Out.TriCount=ThisModel.TriCount;
			Out.TriStart=ThisModel.TriStart;
			printf("Writing Model %s (%i/%i)- %i Tris\n",ThisModel.Name,i+1,ListSize,Out.TriCount);
			fwrite(&Out,1,sizeof(sModel),File);
		}

		return(Ofs);
}
