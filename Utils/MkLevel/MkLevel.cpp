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
struct	sLayerNameTable
{
	int		Type,SubType;
	char	*Name;
};
sLayerNameTable	LayerNameTable[]=
{
	{LAYER_TYPE_SHADE,LAYER_SUBTYPE_BACK,"Shade"},
	{LAYER_TYPE_TILE,LAYER_SUBTYPE_MID,"Mid"},
	{LAYER_TYPE_TILE,LAYER_SUBTYPE_ACTION,"Action"},
	{LAYER_TYPE_COLLISION,LAYER_SUBTYPE_NONE,"Collision"},
	{LAYER_TYPE_ACTOR,LAYER_SUBTYPE_NONE,"Actor List"},
	{LAYER_TYPE_ITEM,LAYER_SUBTYPE_NONE,"Item List"},
	{LAYER_TYPE_PLATFORM,LAYER_SUBTYPE_NONE,"Platform List"},
	{LAYER_TYPE_TRIGGER,LAYER_SUBTYPE_NONE,"Trigger List"},
	{LAYER_TYPE_FX,LAYER_SUBTYPE_NONE,"FX List"},
	{LAYER_TYPE_HAZARD,LAYER_SUBTYPE_NONE,"Hazard List"},

};
#define		LayerNameTableSize	sizeof(LayerNameTable)/sizeof(sLayerNameTable)

//***************************************************************************
int		TSize,QSize,VSize;
int		Tile2dSize,Tile3dSize;

//***************************************************************************
const GString	ConfigFilename="MkLevel.ini";
sExpLayerTile	BlankTile={0,0};
//sExpLayerTile	BlankTile2d={-1,-1};
//sExpLayerTile	BlankTile3d={0,0};

//***************************************************************************
CMkLevel::CMkLevel()
{
		memset(&LevelHdr,0,sizeof(sLevelHdr));
// Add Blanks
		AddTile2d(BlankTile);
		AddTile3d(BlankTile);
		OutElem3d.resize(1);
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
void	CMkLevel::Init(const char *Filename,const char *OutFilename,const char *IncDir,int TPBase,int TPW,int TPH,int _PakW,int _PakH)
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
			LevelFullName=Path.File();
			LevelFullName.Upper();
			OutName=Path.FullName();
			OutIncName=IncDir;
			OutIncName+=Path.File();
			OutIncName+="_INF.h";

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
		TexGrab.AllowRotate(false);
		TexGrab.ShrinkToFit(false);

// Set up other stuph

		FlatFace[0].vtx[0].x=+0.5f; FlatFace[0].vtx[0].y=+1.0f;	FlatFace[0].vtx[0].z=-4.0f;
		FlatFace[0].vtx[1].x=-0.5f; FlatFace[0].vtx[1].y= 0.0f;	FlatFace[0].vtx[1].z=-4.0f;
		FlatFace[0].vtx[2].x=+0.5f; FlatFace[0].vtx[2].y= 0.0f;	FlatFace[0].vtx[2].z=-4.0f;
		FlatFace[0].uv[0][0]=1;		FlatFace[0].uv[0][1]=1;
		FlatFace[0].uv[1][0]=0;		FlatFace[0].uv[1][1]=0;
		FlatFace[0].uv[2][0]=1;		FlatFace[0].uv[2][1]=0;
		FlatFace[0].Flags=0;

		FlatFace[1].vtx[0].x=-0.5f; FlatFace[1].vtx[0].y= 0.0f;	FlatFace[1].vtx[0].z=-4.0f;
		FlatFace[1].vtx[1].x=+0.5f; FlatFace[1].vtx[1].y=+1.0f;	FlatFace[1].vtx[1].z=-4.0f;
		FlatFace[1].vtx[2].x=-0.5f; FlatFace[1].vtx[2].y=+1.0f;	FlatFace[1].vtx[2].z=-4.0f;
		FlatFace[1].uv[0][0]=0;		FlatFace[1].uv[0][1]=0;
		FlatFace[1].uv[1][0]=1;		FlatFace[1].uv[1][1]=1;
		FlatFace[1].uv[2][0]=0;		FlatFace[1].uv[2][1]=1;
		FlatFace[1].Flags=0;
// Setup Pak Info
		PakW=_PakW;
		PakH=_PakH;
}


//***************************************************************************
int		CMkLevel::AddModel(GString &Filename)
{
/*
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
*/
		return(0);
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

int			TriCount=NodeTriList.size();

			for (int T=0; T<TriCount; T++)
			{
				int		Mat=SceneUsedMatList[NodeMatList[T]];
				if (Mat>SceneTexList.size()) GObject::Error(ERR_FATAL,"Crap Material ID, wanted %i, only have %i\n",Mat,SceneTexList.size());
				GString	TexName=RootPath+SceneTexList[Mat];
				
				CFace &F=ModelFaceList.AddFace( NodeVtxList, NodeTriList[T], NodeUVList[T], TexName,SceneMaterials[Mat].Flags,false);
			}

int		ChildCount=ThisNode.GetPruneChildCount();
		for (int Loop=0;Loop<ChildCount ; Loop++) BuildModel(Scene,RootPath,ThisNode.PruneChildList[Loop]);
}

//***************************************************************************
int		CMkLevel::AddModel(const char *Name,int TriStart,int TriCount)
{
/*
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
*/
		return(0);
}

//***************************************************************************
void	CMkLevel::ProcessModels()
{
/*
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
//			printf("%s = %i %i\n",ModelList[i].Name,ModelList[i].TriStart,ModelList[i].TriCount);
			ModelList[i].TriStart+=TriStart;
		}
*/
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
//			sprintf(FullName,"%s",InName);
			_fullpath( FullName, InName, 1024);
//			GFName::makeabsolute(InPath,InName,FullName);
//			InName=FullName;
//			_fullpath( FullName, FullName, 1024);
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

// Load Tris
sExpTri	*TriPtr=(sExpTri*) &ByteHdr[FileHdr->TriOfs];
		InTriList.resize(FileHdr->TriCount);
		for (i=0; i<FileHdr->TriCount; i++)
		{
			InTriList[i]=TriPtr[i];
		}

// Load Tiles
u8		*TilePtr=(u8*) &ByteHdr[FileHdr->TileOfs];
		InTileList.resize(FileHdr->TileCount);
		for (i=0; i<FileHdr->TileCount; i++)
		{
			sExpTile	*ThisTilePtr=(sExpTile*)TilePtr;
			sExpTile	&InTile=InTileList[i];
			
			InTile=*ThisTilePtr;
// Skip RGB Image, not needed (and too buggy)
//			InTile.RGB=(u8*)malloc(RGBSize);
//			memcpy(InTile.RGB,TilePtr+sizeof(sExpTile),RGBSize);
			InTile.RGB=0;
			TilePtr+=RGBSize+sizeof(sExpTile);
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
		printf("Process ElemBanks\n");
		ProcessElemBanks();
		printf("Process Layers\n");
		ProcessLayers();
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
int		CMkLevel::AddTile2d(sExpLayerTile &InTile)
{
sUsedTile2d	ThisTile;

			ThisTile.Tile=InTile.Tile;
			ThisTile.Flags=InTile.Flags;

			return(UsedTile2dList.Add(ThisTile));
}

//***************************************************************************
int		CMkLevel::AddTile3d(sExpLayerTile &InTile)
{
sUsedTile3d	ThisTile;

			ThisTile.Tile=InTile.Tile;
			ThisTile.Flags=InTile.Flags;
int			TileID=UsedTile3dList.Add(ThisTile);
int			TileIdx=(TileID<<2) | (InTile.Flags & PC_TILE_FLAG_MIRROR_XY);
			return(TileIdx);
}

//***************************************************************************
void	CMkLevel::ProcessElemBanks()
{
		PreProcessElemBank2d();
		PreProcessElemBank3d();
		TexGrab.Process();
		ProcessElemBank2d();
		ProcessElemBank3d();
}

//***************************************************************************
void	CMkLevel::PreProcessElemBank2d()
{
int		i,ListSize=UsedTile2dList.size();

//		UsedTile2dList[0].Tile=-1;
// Extract Tex data from list
		for (i=1; i<ListSize; i++)
		{ // Skip blank
			sUsedTile2d &ThisTile=UsedTile2dList[i];
			ThisTile.TexID=Create2dTile(ThisTile.Tile,ThisTile.Flags);
		}
}

//***************************************************************************
void	CMkLevel::ProcessElemBank2d()
{
int		i,ListSize=UsedTile2dList.size();
vector<sTexOutInfo>	&TexInfo=TexGrab.GetTexInfo();

		OutElem2d.resize(ListSize);
		for (i=1; i<ListSize; i++)
		{ // Skip blank
			sUsedTile2d	&InTile=UsedTile2dList[i];
			sElem2d		&OutElem=OutElem2d[i];

			SetUpTileUV(OutElem,TexInfo[InTile.TexID]);
		}
}


//***************************************************************************
void	CMkLevel::PreProcessElemBank3d()
{
int		i,ListSize=UsedTile3dList.size();
		
		for (i=1; i<ListSize; i++)
		{ // Skip Blank
			sUsedTile3d &ThisTile=UsedTile3dList[i];
			ThisTile.Tile=Create3dTile(ThisTile.Tile,ThisTile.Flags);
		}
}

//***************************************************************************
void	CMkLevel::ProcessElemBank3d()
{
int		i,ListSize=UsedTile3dList.size();
int		MaxElemTri=0,MaxElemQuad=0;
		for (i=0; i<ListSize; i++)
		{
			sOutElem3d	&ThisElem=OutElem3d[i];
			CFaceStore	&ThisList=ThisElem.FaceStore;

			ThisList.setMaxStripLength(StripLength);
			ThisElem.Elem3d.TriStart=OutTriList.size();
			ThisElem.Elem3d.QuadStart=OutQuadList.size();
			ThisList.Process(OutTriList,OutQuadList,OutVtxList);
			ThisElem.Elem3d.TriCount=ThisList.GetTriFaceCount();
			ThisElem.Elem3d.QuadCount=ThisList.GetQuadFaceCount();
			if (MaxElemTri<ThisElem.Elem3d.TriCount) MaxElemTri=ThisElem.Elem3d.TriCount;
			if (MaxElemQuad<ThisElem.Elem3d.QuadCount) MaxElemQuad=ThisElem.Elem3d.QuadCount;
		}
		printf("Max Tile Tri =%i\tMax Tile Quad =%i\n",MaxElemTri,MaxElemQuad);
		for (i=0; i<ListSize; i++)
		{
			sOutElem3d	&ThisElem=OutElem3d[i];
			CFaceStore	&ThisList=ThisElem.FaceStore;
			for (int c=i; c<ListSize; c++)
			{
				sOutElem3d	&CheckElem=OutElem3d[c];
				CFaceStore	&CheckList=CheckElem.FaceStore;
			}
		}
}

//***************************************************************************
void	CMkLevel::SetUpTileUV(sElem2d &Out, sTexOutInfo &Info)
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
int		CMkLevel::Create3dTile(int Tile,int Flags)
{
sExpTile		&SrcTile=InTileList[Tile];
CFace			F;
int				i,ListSize,p;
CList<sExpTri>	SortList;
CList<float>	ZPosList;
int				TileID=OutElem3d.size();

			if (SrcTile.TriCount)
			{
				for (i=0; i<SrcTile.TriCount; i++)
				{
					int		ListPos;
					sExpTri	&ThisTri=InTriList[SrcTile.TriStart+i];
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
			{
				int		TexID=Create2dTile(Tile,0);

				for (i=0; i<2; i++)
					{
						FlatFace[i].Flags=0;
						FlatFace[i].TexID=TexID;
						SortList.push_back(FlatFace[i]);
					}
			}

// Add sorted list to main list
			OutElem3d.resize(TileID+1);
CFaceStore	&FaceList=OutElem3d[TileID].FaceStore;
			FaceList.SetTexGrab(TexGrab);

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
				FaceList.SetTPageFlag(F,ThisTri.Flags);
				FaceList.AddFace(F,true);
			}
			
			return(TileID);
}

/*
int		CMkLevel::Create3dTile(int Tile,int Flags)
{
sExpTile		&SrcTile=InTileList[Tile];
CFace			F;
int				i,ListSize,p;
CList<sExpTri>	SortList;
CList<float>	ZPosList;
sTileBank3d		ThisTile;
int				TileID=OutTileBank3d.size();
CFaceStore		FaceStore;


			ThisTile.TriStart=OutFaceList.GetFaceCount();
			ThisTile.QuadCount=0;
			ThisTile.QuadStart=0;

			if (SrcTile.TriCount)
			{
				ThisTile.TriCount=SrcTile.TriCount;
				for (i=0; i<SrcTile.TriCount; i++)
				{
					int		ListPos;
					sExpTri	&ThisTri=InTriList[SrcTile.TriStart+i];
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
			{
				int		TexID=Create2dTile(Tile,0);

				ThisTile.TriCount=2;
				
				for (int i=0; i<2; i++)
					{
						FlatFace[i].Flags=0;
						FlatFace[i].TexID=TexID;
						SortList.push_back(FlatFace[i]);
					}
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

				if (Flags & PC_TILE_FLAG_MIRROR_X)
				{
					F.vtx[0].x=-F.vtx[0].x;
					F.vtx[1].x=-F.vtx[1].x;
					F.vtx[2].x=-F.vtx[2].x;
					SwapPnt^=1;
				}

				if (Flags & PC_TILE_FLAG_MIRROR_Y)
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

			OutTileBank3d.push_back(ThisTile);
			return(TileID);
}

  */
//***************************************************************************
int		CMkLevel::Create2dTile(int Tile,int Flags)
{
sExpTile	&SrcTile=InTileList[Tile];
sMkLevelTex	InTex;
int			Idx;

		InTex.Tile=Tile;
		InTex.Flags=Flags;
		
		Idx=Tex2dList.Find(InTex);

		if (Idx!=-1)
		{
			return(Tex2dList[Idx].TexID);
		}

// Must be new, add it
		InTex.TexID=BuildTileTex(SrcTile,Flags);
		Tex2dList.push_back(InTex);
		return(InTex.TexID);
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
//*** Write *****************************************************************
//***************************************************************************
void	CMkLevel::Write()
{
GString	OutFilename=OutName+".Lvl";

		File=fopen(OutFilename,"wb");

		fwrite(&LevelHdr,1,sizeof(sLevelHdr),File);

		WriteLevel();
		WriteElemBanks();

// rewrite header
		fseek(File,0,SEEK_SET);
		fwrite(&LevelHdr,1,sizeof(sLevelHdr),File);

		fclose(File);
// Write Info header File
		WriteIncFile();

// Write Sizes
		ReportLayers();
}

//***************************************************************************
int		MinOT=123456,MaxOT=0;		

void	CMkLevel::WriteElemBanks()
{
int		i,ListSize;

// 2d Elem
		LevelHdr.ElemBank2d=(sElem2d*)ftell(File);
		ListSize=OutElem2d.size();
		printf("%i 2d Elems\t(%i Bytes)\n",ListSize,ListSize*sizeof(sElem2d));
		for (i=0; i<ListSize; i++)
		{
			sElem2d	&OutElem=OutElem2d[i];
			fwrite(&OutElem,1,sizeof(sElem2d),File);
		}
// 3d Elem

		LevelHdr.ElemBank3d=(sElem3d*)ftell(File);
		ListSize=OutElem3d.size();
		printf("%i 3d Elems\t(%i Bytes)\n",ListSize,ListSize*sizeof(sElem3d));
		for (i=0; i<ListSize; i++)
		{
			sElem3d		&OutElem=OutElem3d[i].Elem3d;
			fwrite(&OutElem,1,sizeof(sElem3d),File);
		}
// TriList
		LevelHdr.TriList=(sTri*)WriteTriList();
		
// QuadList
		LevelHdr.QuadList=(sQuad*)WriteQuadList();

// VtxList
		LevelHdr.VtxList=(sVtx*)WriteVtxList();
}


//***************************************************************************
int		ZMin=9999,ZMax=0;
int		CMkLevel::WriteTriList()
{
int				ThisPos=ftell(File);
int				i,ListSize=OutTriList.size();
int				ZOfs=+4*Scale;

		for (i=0;i<ListSize;i++)
		{
			sTri	&T=OutTriList[i];
			int		OtOfs=0;
			int		Z[3];

// Calc OtOfs
			Z[0]=OutVtxList[T.P0].vz+ZOfs;
			Z[1]=OutVtxList[T.P1].vz+ZOfs;
			Z[2]=OutVtxList[T.P2].vz+ZOfs;
			
			for (int p=0; p<3; p++)
			{
				if (ZMin>Z[p]) ZMin=Z[p];
				if (ZMax<Z[p]) ZMax=Z[p];
				OtOfs+=Z[p]*Z[p];
			}
			OtOfs=(int)sqrt(OtOfs/3);
			
			OtOfs/=8;
			if (MinOT>OtOfs) MinOT=OtOfs;
			if (MaxOT<OtOfs) MaxOT=OtOfs;
			if (OtOfs>15) OtOfs=15;
			if (OtOfs<0) OtOfs=0;

			T.OTOfs=OtOfs;
// Write It			
			fwrite(&T,1,sizeof(sTri),File);
		}
		printf("%i Tris\t(%i Bytes)\n",ListSize,ListSize*sizeof(sTri));
		return(ThisPos);
}

//***************************************************************************
int		CMkLevel::WriteQuadList()
{
int				ThisPos=ftell(File);
int				i,ListSize=OutQuadList.size();
int				ZOfs=+4*Scale;

		for (i=0;i<ListSize;i++)
		{
			sQuad	&Q=OutQuadList[i];
			int		OtOfs=0;
			int		Z[4];

// Calc OtOfs
			Z[0]=OutVtxList[Q.P0].vz+ZOfs;
			Z[1]=OutVtxList[Q.P1].vz+ZOfs;
			Z[2]=OutVtxList[Q.P2].vz+ZOfs;
			Z[3]=OutVtxList[Q.P3].vz+ZOfs;
			
			for (int p=0; p<4; p++)
			{
				if (ZMin>Z[p]) ZMin=Z[p];
				if (ZMax<Z[p]) ZMax=Z[p];
				OtOfs+=Z[p]*Z[p];
			}
			OtOfs=(int)sqrt(OtOfs/4);
			
			OtOfs/=8;
			if (MinOT>OtOfs) MinOT=OtOfs;
			if (MaxOT<OtOfs) MaxOT=OtOfs;
			if (OtOfs>15) OtOfs=15;
			if (OtOfs<0) OtOfs=0;

			Q.OTOfs=OtOfs;
// Write It			
			fwrite(&Q,1,sizeof(sQuad),File);
		}
		printf("%i Quads\t(%i Bytes)\n",ListSize,ListSize*sizeof(sQuad));
		return(ThisPos);
}

//***************************************************************************
int		CMkLevel::WriteVtxList()
{
int		i,ListSize=OutVtxList.size();
int		Pos=ftell(File);

		for (i=0; i<ListSize; i++)
		{
			sVtx const	&In=OutVtxList[i];
			sVtx		Out;

			Out.vx=+In.vx;
			Out.vy=-In.vy+(Scale/2);	// Offset it so the origin is centre centre
			Out.vz=+In.vz;
			fwrite(&Out,1,sizeof(sVtx),File);
		}
		printf("%i Vtx\t(%i Bytes)\n",ListSize,ListSize*sizeof(sVtx));

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
		LevelHdr.BackLayer=WriteLayer(LAYER_TYPE_SHADE,LAYER_SUBTYPE_BACK);
// Mid
		LevelHdr.MidLayer=WriteLayer(LAYER_TYPE_TILE,LAYER_SUBTYPE_MID);
// Action
		LevelHdr.ActionLayer=WriteLayer(LAYER_TYPE_TILE,LAYER_SUBTYPE_ACTION);
// Collision
		LevelHdr.CollisionLayer=WriteLayer(LAYER_TYPE_COLLISION,LAYER_SUBTYPE_NONE);

// Things
int		ThingStart=ftell(File);
		LevelHdr.ActorList=WriteThings(LAYER_TYPE_ACTOR);
		LevelHdr.ItemList=WriteThings(LAYER_TYPE_ITEM);
		LevelHdr.PlatformList=WriteThings(LAYER_TYPE_PLATFORM);
		LevelHdr.TriggerList=WriteThings(LAYER_TYPE_TRIGGER);
		LevelHdr.FXList=WriteThings(LAYER_TYPE_FX);
		LevelHdr.HazardList=WriteThings(LAYER_TYPE_HAZARD);
//		LevelHdr.ModelList=(sModel*)WriteModelList();
		printf("Things =\t(%i Bytes)\n",ftell(File)-ThingStart);


}

//***************************************************************************
int		CMkLevel::WriteLayer(int Type,int SubType,bool Warn)
{
CMkLevelLayer	*ThisLayer=FindLayer(Type,SubType);
int		Ofs;
char	*LayerName=GetLayerName(Type,SubType);

		if (!ThisLayer)
		{
			if (Warn) GObject::Error(ERR_WARNING,"No %s Layer Found in %s!!\n",LayerName,LevelName);
			return(0);
		}
		Ofs=ThisLayer->Write(this,File,LayerName);

		PadFile(File);
		return(Ofs);
}

//***************************************************************************
int		CMkLevel::WriteThings(int Type,bool Warn)
{
CMkLevelLayer	*ThisLayer=FindLayer(Type,LAYER_SUBTYPE_NONE);
int		Ofs;
char	*LayerName=GetLayerName(Type,LAYER_SUBTYPE_NONE);

		if (!ThisLayer)
		{
			GFName		Name=InFilename;
			if (Warn) GObject::Error(ERR_WARNING,"No %s Layer Found in %s!!\n",LayerName,Name.File());
			return(0);
		}
		Ofs=ThisLayer->Write(this,File,LayerName);
//		printf("%s %i\n",LayerName,Ofs);
		PadFile(File);
		return(Ofs);
}

//***************************************************************************
int		CMkLevel::WriteModelList()
{
/*
int		i,ListSize=ModelList.size();
int		Ofs=ftell(File);

		for (i=0; i<ListSize; i++)
		{
			sModel				Out;
			sMkLevelModel		&ThisModel=ModelList[i];

			Out.TriCount=ThisModel.TriCount;
			Out.TriStart=ThisModel.TriStart;
			CalcModelBBox(ThisModel,Out.BBox);
			printf("Writing Model %s (%i/%i) (%i Tris) (BBox %i,%i->%i,%i)\n",ThisModel.Name,i+1,ListSize,Out.TriCount,Out.BBox.XMin,Out.BBox.YMin,Out.BBox.XMax,Out.BBox.YMax);
			fwrite(&Out,1,sizeof(sModel),File);
		}

		return(Ofs);
*/
		return(0);
}

//***************************************************************************
/*
void	CMkLevel::CalcModelBBox(sMkLevelModel &ThisModel,sBBox &BBox)
{
vector<sTri>		&TriList=OutFaceList.GetOutTriList();
vector<sVtx> const	&VtxList=OutFaceList.GetVtxList();
int					Vtx[3];

		BBox.XMin=+32000;
		BBox.XMax=-32000;
		BBox.YMin=+32000;
		BBox.YMax=-32000;

		for (int T=0; T<ThisModel.TriCount; T++)
		{
			sTri	&ThisTri=TriList[T+ThisModel.TriStart];
			Vtx[0]=ThisTri.P0;
			Vtx[1]=ThisTri.P1;
			Vtx[2]=ThisTri.P2;

			for (int V=0; V<3; V++)
			{
				sVtx	const &ThisVtx=VtxList[Vtx[V]];
				if (BBox.XMin>+ThisVtx.vx) BBox.XMin=+ThisVtx.vx;
				if (BBox.XMax<+ThisVtx.vx) BBox.XMax=+ThisVtx.vx;
				if (BBox.YMin>-ThisVtx.vy) BBox.YMin=-ThisVtx.vy;
				if (BBox.YMax<-ThisVtx.vy) BBox.YMax=-ThisVtx.vy;
			}
		}
}
*/
//***************************************************************************
//*** Inf File **************************************************************
//***************************************************************************
void	CMkLevel::AddInfItem(const char *Name,int Val)
{
sInfItem	Item;
GString  ReplaceBadFileChars(GString s);	// Dodgy extern from TexGrab Lib!
			Item.Name=ReplaceBadFileChars(Name);
			Item.Name.Upper();
			Item.Val=Val;

			InfList.Add(Item);
}

//***************************************************************************
void	CMkLevel::WriteIncFile()
{
GString	DefStr;

		DefStr=LevelFullName+"_INF";
		File=fopen(OutIncName,"wt");

		fprintf(File,"// %s Info Header\n",LevelFullName);
		fprintf(File,"\n");
		fprintf(File,"#ifndef\t__%s_INF_HEADER__\n",LevelFullName);
		fprintf(File,"#define\t__%s_INF_HEADER__\n",LevelFullName);
		fprintf(File,"\n");
		fprintf(File,"\n");
		fprintf(File,"enum\t%s\n",DefStr);
		fprintf(File,"{\n");

int		ListSize=InfList.size();
		for (int i=0; i<ListSize; i++)
		{
			sInfItem	&ThisItem=InfList[i];
			
			fprintf(File,"\t%s_%s\t\t=%i,\n",DefStr,ThisItem.Name,ThisItem.Val);
		}

		fprintf(File,"};\n");
		fprintf(File,"\n");
		fprintf(File,"#endif\n");

		fclose(File);
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
void	CMkLevel::ReportLayers()
{
int		i,ListSize=LayerList.size();

		for (i=0; i<ListSize; i++)
		{
			CMkLevelLayer	*ThisLayer=LayerList[i];
			char	*LayerName=GetLayerName(ThisLayer->GetType(),ThisLayer->GetSubType());

			printf("Layer %s= %i bytes\n",LayerName,ThisLayer->GetSize());

		}
}

//***************************************************************************
char	*CMkLevel::GetLayerName(int Type,int SubType)
{
		for (int i=0; i<LayerNameTableSize; i++)
		{
			if (LayerNameTable[i].Type==Type && LayerNameTable[i].SubType==SubType) return(LayerNameTable[i].Name);
		}
		return(0);
}
