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
		int		Set;
		int		XOfs,YOfs;
		u8		*RGB;
		int		Flags;
		int		RChk,GChk,BChk;
		int		TexID;

bool	operator ==(sMkLevelTex const &v1)
		{
		if (Set!=v1.Set) return(false);
		if (XOfs!=v1.XOfs) return(false);
		if (YOfs!=v1.YOfs) return(false);
		if (Flags!=v1.Flags) return(false);
		return(true);
		}
};

//***************************************************************************
struct	sMkLevelPlatform
{
		GString	Name;
		int		TriStart;
		int		TriCount;

bool	operator ==(sMkLevelPlatform const &v1)		{return(Name==v1.Name);}
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
		void			Init(const char *InFilename,const char *OutFilename,int TPBase,int TPW,int TPH);

		void			Load();

		void			Process();
		int				AddTile3d(sExpLayerTile &Tile)			{return(Tile3dList.Add(Tile));}
		int				AddTile2d(sExpLayerTile &Tile)			{return(Tile2dList.Add(Tile));}
		int				AddPlatform(sMkLevelLayerThing &ThisThing);

		void			Write();

		int				Create2dTex(int Tile,int Flags);
		int				Create3dTile(sExpLayerTile &ThisTile);
		int				FindRGBMatch(sMkLevelTex &ThisTex);
		bool			IsRGBSame(const sMkLevelTex &Tile0,const sMkLevelTex &Tile1);
		void			MakeTexName(sMkLevelTex &InTex,GString &OutStr);
		int				BuildTileTex(sMkLevelTex &InTex);

		char			*GetConfigStr(const char *Grp,const char *Key)	{return(Config.GetStr(Grp,Key));}
		CIni			&GetConfig()									{return(Config);}
		CTexGrab		&GetTexGrab()									{return(TexGrab);}

		void			SetStart(int X,int Y)				{LevelHdr.PlayerStartX=X; LevelHdr.PlayerStartY=Y;}
protected:	
		CMkLevelLayer	*FindLayer(int Type,int SubType);
		void			LoadStrList(CList<GString> &List,char *TexPtr,int Count);

		void			LoadTiles(sExpFileHdr *FileHdr);
		void			LoadLayers(sExpFileHdr *FileHdr);
		void			LoadLayer(sExpLayerHdr *LayerHdr);

		void			PreProcessLayers();
		void			ProcessTileBanks();
		void			PreProcessTileBank2d();
		void			ProcessTileBank2d();
		void			PreProcessTileBank3d();
		void			ProcessTileBank3d();
		void			ProcessLayers();
		void            SetUpTileUV(sTile2d &Out, sTexOutInfo &Info);

		void			WriteLevel();
		void			WriteLayers();
		int				WriteLayer(int Type,int SubType,const char *LayerName);
		int				WriteThings(int Type,const char *LayerName);
		int				WritePlatformGfx();
		int				WriteTriList();
		int				WriteQuadList();
		int				WriteVtxList();
		void			WriteTileBank();

		void            BuildTiles();

		void			ExpTri2Face(sExpTri &In,CFace &Out,bool ImportTex=true);

		FILE					*File;
		GString					InFilename,InPath,LevelName;
		GString					OutName;
		GString					AppDir;

		int						TileW,TileH;
		CIni					Config;

		CList<sExpTri>			InTriList;
		CList<sExpTile>			InTileList;
		CList<GString>			InSetNameList;
		CList<GString>			InTexNameList;
		CList<GString>			UsedSetNameList;
		CList<GString>			UsedTexNameList;

		CFaceStore				OutFaceList;
		CList<sTile2d>			OutTile2dList;
		CList<sTile3d>			OutTile3dList;

		CList<sExpLayerTile>	Tile2dList;
		CList<sExpLayerTile>	Tile3dList;
		CList<sMkLevelTex>		Tex2dList;
		CTexGrab				TexGrab;
		CList<Frame>			BmpList;

		CList<sMkLevelPlatform>	PlatformList;
		vector<CMkLevelLayer*>	LayerList;

		sLevelHdr				LevelHdr;
		sExpTri					FlatFace[2];

};

//***************************************************************************
#endif
