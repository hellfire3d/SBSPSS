/*************************/
/*** Base Level Holder ***/
/*************************/

#ifndef	__MKLEVEL_HEADER__
#define	__MKLEVEL_HEADER__

#include	<vector>
#include	<List.h>
#include	<FaceStore.h>
#include	"IniClass.h"

// Map Editor header files
#include	"..\mapedit\LayerDef.h"
#include	"..\mapedit\ExportHdr.h"

using namespace std;

//***************************************************************************
struct	sMkLevelTex
{
		int		Tile;
		int		Flags;
		int		TexID;

bool	operator ==(sMkLevelTex const &v1)	{return(Tile==v1.Tile && Flags==v1.Flags);}
};

//***************************************************************************
struct	sMkLevelModel
{
		GString	Name;
		int		TriStart;
		int		TriCount;

bool	operator ==(sMkLevelModel const &v1)		{return(Name==v1.Name);}
};

//***************************************************************************
struct	sInfItem
{
	GString	Name;
	int		Val;
bool	operator ==(sInfItem const &v1)		{return(Name==v1.Name);}

};

//***************************************************************************
struct	sUsedTile2d
{
		int	Tile;
		int	Flags;
		int	TexID;

bool	operator ==(sUsedTile2d const &v1)	{return(Tile==v1.Tile && Flags==v1.Flags);}
};

//***************************************************************************
struct	sUsedTile3d
{
		int	Tile;
		int	Flags;

bool	operator ==(sUsedTile3d const &v1)	{return(Tile==v1.Tile);}

};

struct sOutElem3d
{
		sElem3d		Elem3d;
		CFaceStore	FaceStore;
};

//***************************************************************************
struct	sMkLevelLayerThing;
class	CMkLevelLayer;
class	CMkLevel
{
public:
		CMkLevel();
		~CMkLevel();

		void			SetAppDir(const char *Path);
		void			Init(const char *InFilename,const char *OutFilename,const char *IncDir,int TPBase,int TPW,int TPH,int PakW,int PakH);

		void			LoadModels();
		int				AddModel(GString &Filename);
		int				AddModel(const char *Name,int TriStart,int TriCount);

		void			Load();

		void			Process();
		int				AddTile2d(sExpLayerTile &Tile);
		int				AddTile3d(sExpLayerTile &Tile);

		void			AddInfItem(const char *Name,int Val);
		void			Write();

		int				Create2dTile(int Tile,int Flags);
		int				Create3dTile(int Tile,int Flags);
		void			MakeTexName(sExpTile &SrcTile,int Flags,GString &OutStr);
		int				BuildTileTex(sExpTile &SrcTile,int Flags);

		char			*GetConfigStr(const char *Grp,const char *Key)	{return(Config.GetStr(Grp,Key));}
		int				GetConfigInt(const char *Grp,const char *Key)	{return(Config.GetInt(Grp,Key));}
		CIni			&GetConfig()									{return(Config);}
		CTexGrab		&GetTexGrab()									{return(TexGrab);}

		void			SetStart(int X,int Y)				{LevelHdr.PlayerStartX=X; LevelHdr.PlayerStartY=Y;}

		void			GetPakWH(int &W,int &H)				{W=PakW; H=PakH;}
protected:	
		void			BuildModel(CScene &ThisScene,GString &RootPath,int Node);
		CMkLevelLayer	*FindLayer(int Type,int SubType);
		void			LoadStrList(CList<GString> &List,char *TexPtr,int Count);

		void			LoadTiles(sExpFileHdr *FileHdr);
		void			LoadLayers(sExpFileHdr *FileHdr);
		void			LoadLayer(sExpLayerHdr *LayerHdr);

		void			PreProcessLayers();
		void			ProcessElemBanks();
		void			PreProcessElemBank2d();
		void			ProcessElemBank2d();
		void			PreProcessElemBank3d();
		void			ProcessElemBank3d();
		void			ProcessLayers();
		void            SetUpTileUV(sElem2d &Out, sTexOutInfo &Info);

		void			ProcessModels();
		
		void			WriteLevel();
		void			WriteLayers();
		int				WriteLayer(int Type,int SubType,bool Warn=false);
		int				WriteThings(int Type,bool Warn=false);
		int				WriteModelList();
		int				WriteTriList();
		int				WriteQuadList();
		int				WriteVtxList();
		void			WriteElemBanks();
		void			CalcModelBBox(sMkLevelModel &ThisModel,sBBox &BBox);
		void            BuildTiles();

		void			WriteIncFile();

		void			ReportLayers();
		char			*GetLayerName(int Type,int SubType);

		void			ExpTri2Face(sExpTri &In,CFace &Out,bool ImportTex=true);

		FILE					*File;
		GString					InFilename,InPath,LevelName,LevelFullName;
		GString					OutName,OutIncName;
		GString					AppDir;

		int						TileW,TileH;
		CIni					Config;
		
		CList<sExpTri>			InTriList;
		CList<sExpTile>			InTileList;
		CList<GString>			InSetNameList;
		CList<GString>			InTexNameList;


//		CFaceStore				OutFaceList;
		CList<sElem2d>			OutElem2d;
		CList<sOutElem3d>		OutElem3d;

		CList<sUsedTile2d>		UsedTile2dList;
		CList<sUsedTile3d>		UsedTile3dList;

		CList<sMkLevelTex>		Tex2dList;
		CTexGrab				TexGrab;
		CList<Frame>			BmpList;

		vector<CMkLevelLayer*>	LayerList;

		CList<sMkLevelModel>	ModelList;
		CFaceStore				ModelFaceList;

		sLevelHdr				LevelHdr;
		sExpTri					FlatFace[2];

		CList<sInfItem>			InfList;

		int						PakW,PakH;

		vector<sTri>			OutTriList;
		vector<sQuad>			OutQuadList;
		vector<sVtx>			OutVtxList;

};

//***************************************************************************
#endif
