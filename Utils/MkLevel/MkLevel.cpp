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
#include	"Layers\MkLevelLayerRGB.h"

#define	PSX_TILE2D_HEIGHT	(12)
#define	PSX_TILE3D_HEIGHT	(16)

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
	{LAYER_TYPE_RGB,LAYER_SUBTYPE_NONE,"RGB"},

};
#define		LayerNameTableSize	sizeof(LayerNameTable)/sizeof(sLayerNameTable)

//***************************************************************************
int			TSize,QSize,VSize;
int			Tile2dSize,Tile3dSize;
const char	*ModelExcludeGroupName="ModelExcludeList";
const char	*ModelOtOfsGroupName="ModelOtOfs";
int			LocalOptCount=0;

//***************************************************************************
const GString	ConfigFilename="MkLevel.ini";
sExpLayerTile	BlankTile={0,0};


const float	InElemXMin=-0.5f;
const float	InElemXMax=+0.5f;
const float	InElemYMin=-0.0f;
const float	InElemYMax=+1.0f;
const float	InElemZMin=-4.0f;
const float	InElemZMax=+4.0f;

const float	OutElemXMin=-0.5f;
const float	OutElemXMax=+0.5f;
const float	OutElemYMin=-0.5f;
const float	OutElemYMax=+0.5f;
const float	OutElemZMin=-4.0f;
const float	OutElemZMax=+4.0f;

Vector3	DefVtxTable[8]=
{
	Vector3(OutElemXMin,OutElemYMin,OutElemZMin),	// FLU
	Vector3(OutElemXMax,OutElemYMin,OutElemZMin),	// FRU
	Vector3(OutElemXMin,OutElemYMax,OutElemZMin),	// FLD
	Vector3(OutElemXMax,OutElemYMax,OutElemZMin),	// FRD

	Vector3(OutElemXMin,OutElemYMin,OutElemZMax),	// BLU
	Vector3(OutElemXMax,OutElemYMin,OutElemZMax),	// BRU
	Vector3(OutElemXMin,OutElemYMax,OutElemZMax),	// BLD
	Vector3(OutElemXMax,OutElemYMax,OutElemZMax),	// BRD
};
#define	DefVtxTableSize	sizeof(DefVtxTable)/sizeof(Vector3)


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
void	CMkLevel::Init(const char *Filename,const char *OutFilename,const char *IncDir,int TPBase,int TPW,int TPH,int _PakW,int _PakH,bool _LocalGeom,float _SnapThresh)
{
// Setup filenames and paths
GFName		Path;
char		*Ptr;
char		Buffer[256];

			strcpy(Buffer,Filename);
			Ptr=Buffer;
			while (*Ptr)
			{
				if (*Ptr=='\\' || *Ptr=='/') *Ptr=' ';
				Ptr++;
			}

			while (*Ptr!=' ') Ptr--;
			*Ptr--=0;
			while (*Ptr!=' ') Ptr--; Ptr--;
			while (*Ptr!=' ') Ptr--; Ptr++;
			LevelName=Ptr;
			InFilename=Filename;
			Path=Filename;
//			LevelName=Path.File();
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

// Setup Extra Info
		PakW=_PakW;
		PakH=_PakH;
		LocalGeom=_LocalGeom;
		SnapThresh=_SnapThresh;
		AddDefVtx(OutVtxList);
}

//***************************************************************************
void	CMkLevel::AddDefVtx(vector<sVtx> &VtxList)
{
CFaceStore	F;
		for (int i=0; i<DefVtxTableSize; i++)
		{
			F.AddVtx(VtxList,DefVtxTable[i]);
		}
}

//***************************************************************************
int		CMkLevel::AddModel(const char *Name,int TriStart,int TriCount)
{
sMkLevelModel	ThisModel;
int				ModelID;
GString			ModelName=Name;

			ThisModel.Name=Name;
			ThisModel.TriStart=TriStart;
			ThisModel.TriCount=TriCount;

			if (Config.FindKey(ModelExcludeGroupName,Name)!=-1)
			{
				ThisModel.TriCount=0;
			}
			ModelID=ModelList.Add(ThisModel);
			return(ModelID);
}

//***************************************************************************
void	CMkLevel::PreProcessModels()
{
int			i,ListSize=ModelList.size();

			for (i=0; i<ListSize; i++)
			{
				sMkLevelModel	&ThisModel=ModelList[i];
				int		OtOfs=0;
				
				Config.GetInt(ModelOtOfsGroupName,ThisModel.Name,OtOfs);

				if (OtOfs)
					printf("ModelOTOfs %s %i\n",ThisModel.Name,OtOfs);
			
				ThisModel.ElemID=Create3dElem(ThisModel.TriCount,ThisModel.TriStart,false,OtOfs);	// always all models as global for the moment
			}
}

//***************************************************************************
void	CMkLevel::ProcessModels()
{
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
			if (SnapThresh!=0.0f) SnapTiles();

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
			_fullpath( FullName, InName, 1024);
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
			case LAYER_TYPE_RGB:
				LayerList.push_back(new CMkLevelLayerRGB(LayerHdr));
				break;
			default:
				GObject::Error(ERR_FATAL,"Unknown Layer Type\n");
			}
		}
}


//***************************************************************************
void	CMkLevel::SnapTiles()
{
int		i,ListSize=InTileList.size();
float	SnapXMin=InElemXMin+SnapThresh;
float	SnapXMax=InElemXMax-SnapThresh;
float	SnapYMin=InElemYMin+SnapThresh;
float	SnapYMax=InElemYMax-SnapThresh;
float	SnapZMin=InElemZMin+SnapThresh;
float	SnapZMax=InElemZMax-SnapThresh;

		for (i=0;i<ListSize;i++)
		{
			sExpTile	&ThisTile=InTileList[i];
			for (int T=0; T<ThisTile.TriCount; T++)
			{
				sExpTri	&ThisTri=InTriList[ThisTile.TriStart+T];
				for (int p=0;p<3; p++)
				{
					Vector3	&V=ThisTri.vtx[p];

					if (V.x<SnapXMin) V.x=InElemXMin;
					if (V.x>SnapXMax) V.x=InElemXMax;
					if (V.y<SnapYMin) V.y=InElemYMin;
					if (V.y>SnapYMax) V.y=InElemYMax;
					if (V.z<SnapZMin) V.z=InElemZMin;
					if (V.z>SnapZMax) V.z=InElemZMax;
				}
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
		printf("PreProcess ElemBanks\n");
		PreProcessElemBank2d();
		PreProcessElemBank3d();
		printf("PreProcess Models\n");
		PreProcessModels();
		printf("Process Textures\n");
		TexGrab.Process();
		printf("Process ElemBanks\n");
		ProcessElemBank2d();
		ProcessElemBank3d();
		printf("Process Layers\n");
		ProcessLayers();
		printf("Process Models\n");
		ProcessModels();
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
void	CMkLevel::PreProcessElemBank2d()
{
int		i,ListSize=UsedTile2dList.size();

// Extract Tex data from list
		for (i=1; i<ListSize; i++)
		{ // Skip blank
			sUsedTile2d &ThisTile=UsedTile2dList[i];
			ThisTile.TexID=Create2dTile(ThisTile.Tile,ThisTile.Flags,PSX_TILE2D_HEIGHT);
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
int		MaxElemTri=0,MaxElemQuad=0,MaxElemVtx=0;
void	CMkLevel::ProcessElemBank3d()
{
//int		i,ListSize=UsedTile3dList.size();	<--- UsedTile3dList is tiles only!!
int		i,ListSize=OutElem3d.size();

		for (i=0; i<ListSize; i++)
		{
			ProcessElem3d(OutElem3d[i]);
		}
		printf("Max Elem Tri =%i\tMax Elem Quad =%i\tMax Elem Vtx =%i\n",MaxElemTri,MaxElemQuad,MaxElemVtx);
		if (LocalGeom)
		{
			printf("LocalGeom Optimised %i/%i\n",LocalOptCount,ListSize);

		}
}

//***************************************************************************
void	CMkLevel::ProcessElem3d(sOutElem3d &ThisElem)
{
CFaceStore		&ThisList=ThisElem.FaceStore;

			ThisList.setMaxStripLength(StripLength);
			ThisElem.Elem3d.TriStart=OutTriList.size();
			ThisElem.Elem3d.QuadStart=OutQuadList.size();
			if (!LocalGeom)
			{ // Global Geom
				ThisList.Process(OutTriList,OutQuadList,OutVtxList);
				CalcOtOfs(OutTriList,OutVtxList,ThisElem.Elem3d.TriStart,ThisList.GetTriFaceCount());
				CalcOtOfs(OutQuadList,OutVtxList,ThisElem.Elem3d.QuadStart,ThisList.GetQuadFaceCount());
			}
			else
			{ // Local Geom
				vector<sVtx>	LocalVtxList;
				int		VtxStart=0;

				if (!ThisElem.Model)
				{
					AddDefVtx(LocalVtxList);
					VtxStart=8;
				}

				ThisList.Process(OutTriList,OutQuadList,LocalVtxList);
				ThisElem.Elem3d.VtxIdxStart=OutLocalVtxIdxList.size();
				
				int		ListSize=LocalVtxList.size();
				int		LocalVtxCount=0;

				for (int v=VtxStart; v<ListSize; v++)
				{
					u16	Idx=CFaceStore::AddVtx(OutVtxList,LocalVtxList[v]);

					OutLocalVtxIdxList.push_back(Idx);
					LocalVtxCount++;
				}
				if (LocalVtxCount==0) 
				{
					LocalOptCount++;
					LocalVtxCount=0;
				}
				ThisElem.Elem3d.VtxTriCount=LocalVtxCount/3;
				if (LocalVtxCount%3) ThisElem.Elem3d.VtxTriCount++;
				
				CalcOtOfs(OutTriList,LocalVtxList,ThisElem.Elem3d.TriStart,ThisList.GetTriFaceCount());
				CalcOtOfs(OutQuadList,LocalVtxList,ThisElem.Elem3d.QuadStart,ThisList.GetQuadFaceCount());
			}

			ThisElem.Elem3d.TriCount=ThisList.GetTriFaceCount();
			ThisElem.Elem3d.QuadCount=ThisList.GetQuadFaceCount();

		if (!ThisElem.Model)
		{ // Gen max polys per tile (NOT MODEL)
			if (MaxElemTri<ThisElem.Elem3d.TriCount) MaxElemTri=ThisElem.Elem3d.TriCount;
			if (MaxElemQuad<ThisElem.Elem3d.QuadCount) MaxElemQuad=ThisElem.Elem3d.QuadCount;
			if (MaxElemVtx<ThisElem.Elem3d.VtxTriCount*3) MaxElemVtx=ThisElem.Elem3d.VtxTriCount*3;
		}
}

//***************************************************************************
int		OTMin=1 ;
int		OTMax=15-1;

void	CMkLevel::CalcOtOfs(vector<sTri> &PList,vector<sVtx> &VtxList,int Start,int Count)
{
int		ZOfs=+4*Scale;
int		PntCount=3;

		for (int i=0;i<Count;i++)
		{
			sTri	&P=PList[Start+i];
			float	OtOfs=0;
			int		Z[3];
// Get VtxZ
			
			Z[0]=VtxList[P.P0].vz+ZOfs;
			Z[1]=VtxList[P.P1].vz+ZOfs;
			Z[2]=VtxList[P.P2].vz+ZOfs;

			for (int p=0; p<PntCount; p++)	
			{
				OtOfs+=Z[p]*Z[p];
			}
			
			OtOfs=sqrt(OtOfs/PntCount);
			OtOfs/=8;
//			printf("%i\n",P.OTOfs);
			OtOfs+=P.OTOfs;
//			OtOfs++;	// Allow for GUI

			if (OtOfs>OTMax) OtOfs=OTMax;
			if (OtOfs<OTMin) OtOfs=OTMin;

			P.OTOfs=OtOfs;
		}

}

//***************************************************************************
void	CMkLevel::CalcOtOfs(vector<sQuad> &PList,vector<sVtx> &VtxList,int Start,int Count)
{
int		ZOfs=+4*Scale;
int		PntCount=4;

		for (int i=0;i<Count;i++)
		{
			sQuad	&P=PList[Start+i];
			float	OtOfs=0;
			int		Z[4];
// Get VtxZ
			
			Z[0]=VtxList[P.P0].vz+ZOfs;
			Z[1]=VtxList[P.P1].vz+ZOfs;
			Z[2]=VtxList[P.P2].vz+ZOfs;
			Z[3]=VtxList[P.P3].vz+ZOfs;

			for (int p=0; p<PntCount; p++)	
			{
				OtOfs+=Z[p]*Z[p];
			}

			OtOfs=sqrt(OtOfs/PntCount);
			OtOfs/=8;
			OtOfs+=P.OTOfs;

			if (OtOfs>OTMax) OtOfs=OTMax;
			if (OtOfs<OTMin) OtOfs=OTMin;

			P.OTOfs=OtOfs;
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
		Out.TPage=0xe1000000 | (Info.Tpage & 0x9ff);		// fixup TPage for TSPRT
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
sExpTile	&SrcTile=InTileList[Tile];
int			ElemID;
			if (SrcTile.TriCount)
			{
				ElemID=Create3dElem(SrcTile.TriCount,SrcTile.TriStart,true,0);
			}
			else
			{
				ElemID=Create2dElem(Tile);
			}

			return(ElemID);
}

//***************************************************************************
int		CMkLevel::Create3dElem(int TriCount,int TriStart,bool IsTile,int OtOfs)
{
CFace			F;
int				i,ListSize;
CList<sExpTri>	SortList;
CList<float>	ZPosList;
int				ElemID=OutElem3d.size();
				OutElem3d.resize(ElemID+1);

sOutElem3d		&ThisElem=OutElem3d[ElemID];
CFaceStore		&FaceList=ThisElem.FaceStore;
				FaceList.SetTexGrab(TexGrab);

			for (i=0; i<TriCount; i++)
			{
				int		ListPos;
				sExpTri	&ThisTri=InTriList[TriStart+i];
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

// Add sorted list to main list

			for (i=0; i<TriCount; i++)
			{
				sExpTri &ThisTri=SortList[i];
				CFace	F;

				F.TexName=InTexNameList[ThisTri.TexID];
				F.Mat=-1;

				for (int p=0; p<3; p++)
				{
//					F.vtx[p]=ThisTri.vtx[p];
					F.vtx[p].x=+ThisTri.vtx[p].x;
					F.vtx[p].y=-ThisTri.vtx[p].y;
					F.vtx[p].z=+ThisTri.vtx[p].z;
					if (IsTile)
					{
						F.vtx[p].y+=0.5f;
					}
					F.uvs[p].u=ThisTri.uv[p][0];
					F.uvs[p].v=ThisTri.uv[p][1];
				}
				F.OtOfs=OtOfs;
				FaceList.SetTPageFlag(F,ThisTri.Flags);
				FaceList.AddFace(F,true);
				
			}

			ThisElem.Model=!IsTile;

			return(ElemID);			
}

//***************************************************************************
int			CMkLevel::Create2dElem(int Tile)
{
CFace			F;
int				i;
int				TexID=Create2dTile(Tile,0,PSX_TILE3D_HEIGHT);
int				ElemID=OutElem3d.size();
				OutElem3d.resize(ElemID+1);

sOutElem3d		&ThisElem=OutElem3d[ElemID];
CFaceStore		&FaceList=ThisElem.FaceStore;
				FaceList.SetTexGrab(TexGrab);

			for (i=0; i<2; i++)
			{
				sExpTri &ThisTri=FlatFace[i];
				CFace	F;

				F.Mat=TexID;

				for (int p=0; p<3; p++)
				{
					F.vtx[p].x=+ThisTri.vtx[p].x;
					F.vtx[p].y=-ThisTri.vtx[p].y;
					F.vtx[p].z=+ThisTri.vtx[p].z;
					F.vtx[p].y+=0.5f;	// Adjust for tile offset (2dElems Always Tile)
					F.uvs[p].u=ThisTri.uv[p][0];
					F.uvs[p].v=ThisTri.uv[p][1];
				}
				F.OtOfs=0;
				FaceList.SetTPageFlag(F,0);
				FaceList.AddFace(F,true);
			}
			ThisElem.Model=false;
			ThisElem.OTOfs=0;
			return(ElemID);
}

//***************************************************************************
int		CMkLevel::Create2dTile(int Tile,int Flags,int Height)
{
sExpTile	&SrcTile=InTileList[Tile];
sMkLevelTex	InTex;
int			Idx;

		InTex.Tile=Tile;
		InTex.Flags=Flags;
		InTex.Height=Height;
		
		Idx=Tex2dList.Find(InTex);

		if (Idx!=-1)
		{
			return(Tex2dList[Idx].TexID);
		}

// Must be new, add it
		InTex.TexID=BuildTileTex(SrcTile,Flags,InTex.Height);
		Tex2dList.push_back(InTex);
		return(InTex.TexID);
}

//***************************************************************************
int		CMkLevel::BuildTileTex(sExpTile	&SrcTile,int Flags,int Height)
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

				if (Height!=16)
				{
					ThisFrame.Resize(16,Height);
				}
				
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
// VtxIdxList
		LevelHdr.VtxIdxList=(u16*)ftell(File);
		ListSize=OutLocalVtxIdxList.size();
		for (i=0; i<ListSize; i++)
		{
			u16	&ThisIdx=OutLocalVtxIdxList[i];
			fwrite(&ThisIdx,1,sizeof(u16),File);
		}

}


//***************************************************************************
int		CMkLevel::WriteTriList()
{
int			ThisPos=ftell(File);
int			i,ListSize=OutTriList.size();

		for (i=0;i<ListSize;i++)
		{
			sTri	&T=OutTriList[i];
			T.P0*=4;
			T.P1*=4;
			T.P2*=4;
			fwrite(&T,1,sizeof(sTri),File);
		}
		printf("%i Tris\t(%i Bytes)\n",ListSize,ListSize*sizeof(sTri));
		return(ThisPos);
}

//***************************************************************************
int		CMkLevel::WriteQuadList()
{
int			ThisPos=ftell(File);
int			i,ListSize=OutQuadList.size();

		for (i=0;i<ListSize;i++)
		{
			sQuad	&Q=OutQuadList[i];
			Q.P0*=4;
			Q.P1*=4;
			Q.P2*=4;
			Q.P3*=4;
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
sVtx	Min={+100,+100,+100};
sVtx	Max={-100,-100,-100};

		for (i=0; i<ListSize; i++)
		{
			sVtx const	&In=OutVtxList[i];
			sVtx		Out;

			Out.vx=+In.vx;
			Out.vy=+In.vy;
			Out.vz=+In.vz;
			Min.vx=__min(Min.vx,Out.vx);
			Min.vy=__min(Min.vy,Out.vy);
			Min.vz=__min(Min.vz,Out.vz);
			Max.vx=__max(Max.vx,Out.vx);
			Max.vy=__max(Max.vy,Out.vy);
			Max.vz=__max(Max.vz,Out.vz);
//			Out.rgb=(Out.vz+64)/8;
//			printf("%i\n",Out.rgb);
			fwrite(&Out,1,sizeof(sVtx),File);
		}
		printf("%i Vtx\t(%i Bytes)\n",ListSize,ListSize*sizeof(sVtx));

		printf("MinX: %i\tMaxX: %i\n",Min.vx,Max.vx);
		printf("MinY: %i\tMaxY: %i\n",Min.vy,Max.vy);
		printf("MinZ: %i\tMaxZ: %i\n",Min.vz,Max.vz);
//		printf("Min %i %i %i\n",Min.vx,Min.vy,Min.vz);
//		printf("Max %i %i %i\n",Max.vx,Max.vy,Max.vz);

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
// RGB
		LevelHdr.RGBLayer=WriteLayer(LAYER_TYPE_RGB,LAYER_SUBTYPE_NONE);
// Things
int		ThingStart=ftell(File);
		LevelHdr.ActorList=WriteThings(LAYER_TYPE_ACTOR);
		LevelHdr.ItemList=WriteThings(LAYER_TYPE_ITEM);
		LevelHdr.PlatformList=WriteThings(LAYER_TYPE_PLATFORM);
		LevelHdr.TriggerList=WriteThings(LAYER_TYPE_TRIGGER);
		LevelHdr.FXList=WriteThings(LAYER_TYPE_FX);
		LevelHdr.HazardList=WriteThings(LAYER_TYPE_HAZARD);
		LevelHdr.ModelList=(sModel*)WriteModelList();

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
int		i,ListSize=ModelList.size();
int		Ofs=ftell(File);

		for (i=0; i<ListSize; i++)
		{
			sModel				Out;
			sMkLevelModel		&ThisModel=ModelList[i];
			sOutElem3d			&ThisElem=OutElem3d[ThisModel.ElemID];
			sBBox				&ElemBBox=ThisElem.FaceStore.GetBBox();
			Out.ElemID=ThisModel.ElemID;
			Out.BBox=ElemBBox;

			if (ThisModel.TriCount==0)
				printf("Writing Model E:%i - %s - Dummy --\n",Out.ElemID,ThisModel.Name);
			else
				printf("Writing Model E:%i - %s - T:%i Q:%i BBox:%i,%i->%i,%i\n",Out.ElemID,ThisModel.Name,ThisElem.Elem3d.TriCount,ThisElem.Elem3d.QuadCount,Out.BBox.XMin,Out.BBox.YMin,Out.BBox.XMax,Out.BBox.YMax);
			
			fwrite(&Out,1,sizeof(sModel),File);
		}

		return(Ofs);
}

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
