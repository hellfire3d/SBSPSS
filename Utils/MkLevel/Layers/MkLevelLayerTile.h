/******************/
/*** Layer Tile ***/
/******************/

#ifndef	__MKLEVEL_LAYER_TILE_HEADER__
#define	__MKLEVEL_LAYER_TILE_HEADER__

#include	"MkLevelLayer.h"
#include	<List2d.h>

struct	sMkLevelElem
{
	u16				Elem;
	int				TexID;
};

/*****************************************************************************/
class	CMkLevelLayerTile : public CMkLevelLayer
{
public:
		CMkLevelLayerTile(sExpLayerHdr *LayerHdr);

virtual	void	PreProcess(CMkLevel *Core);
virtual	void	Process(CMkLevel *Core);
virtual	int		Write(CMkLevel *Core,FILE *File,const char *LayerName);

protected:
		int		WriteNormal(CMkLevel *Core,FILE *File);
		int		WritePak(CMkLevel *Core,FILE *File);

		void	BuildOutElem(int X,int Y,sTileMapElem *Out);

		CList2d<sExpLayerTile>		InMap;
		CList2d<sMkLevelElem>		OutMap;
		sLayerHdr					Hdr;

// Pak Stuff
		int				BuildPakChunk(int X,int Y,int &PakW,int &PakH);
		int				PakChunk(int ChunkSize);

		int				PakW,PakH;
		sTileMapElem	*ChunkBuffer;
		u8				*PakBuffer;


};

/*****************************************************************************/
#endif
