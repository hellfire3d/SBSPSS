/******************/
/*** Layer Tile ***/
/******************/

#include	<DaveLib.h>
#include	<List2d.h>

#include	"MkLevelLayer.h"
#include	"MkLevelLayerTile.h"
#include	"pak.h"



/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CMkLevelLayerTile::CMkLevelLayerTile(sExpLayerHdr *LayerHdr)
{
		Type=LayerHdr->Type;
		SubType=LayerHdr->SubType;
		Width=LayerHdr->Width;
		Height=LayerHdr->Height;

sExpLayerTile	*MapPtr=(sExpLayerTile *)((int)LayerHdr+sizeof(sExpLayerHdr));

		InMap.SetSize(Width,Height);
		
		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sExpLayerTile	ThisTile;
				
				ThisTile.Tile=MapPtr->Tile;
				ThisTile.Flags=MapPtr->Flags;
				InMap.Set(X,Y,ThisTile);
				MapPtr++;
			}
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*** Pre-Process *************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerTile::PreProcess(CMkLevel *Core)
{
int		Width=InMap.GetWidth();
int		Height=InMap.GetHeight();

		OutMap.SetSize(Width,Height);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sExpLayerTile	&InElem=InMap.Get(X,Y);
				sMkLevelElem	&OutElem=OutMap.Get(X,Y);
				
				OutElem.Elem=0;
				if (InElem.Tile)
				{ // Dont process blanks
					OutElem.Elem=Core->AddTile2d(InElem);
				}
			}
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*** Process *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerTile::Process(CMkLevel *Core)
{
}

/*****************************************************************************/
/*****************************************************************************/
/** Write ********************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CMkLevelLayerTile::Write(CMkLevel *Core,FILE *File,const char *LayerName)
{
int			HdrPos=ftell(File);
int			Width=OutMap.GetWidth();
int			Height=OutMap.GetHeight();

		Hdr.Type=Type;
		Hdr.SubType=SubType;
		Hdr.Width=Width;
		Hdr.Height=Height;
		fwrite(&Hdr,sizeof(sLayerHdr),1,File);

//		printf("%s (%i,%i)= %i\n",LayerName,Width,Height,Width*Height*sizeof(sTileMapElem));

		Core->GetPakWH(PakW,PakH);
		if (PakW && PakH)
		{
			int	OrigSize=Width*Height*sizeof(sTileMapElem);
			printf("- PAKing Layer %s (%i,%i).. ",LayerName,Width,Height);
			int	LvlSize=WritePak(Core,File);
			printf("%i bytes - Saved %i Bytes\n",LvlSize,OrigSize-LvlSize);
		}
		else
		{
//			printf("- Writing Layer %s (%i,%i).. ",LayerName,Width,Height);
			int	LvlSize=WriteNormal(Core,File);
//			printf("%i bytes\n",LvlSize);
		}
		PadFile(File);

// ReWrite Hdr
int		RetPos=ftell(File);
		fseek(File,HdrPos,SEEK_SET);
		fwrite(&Hdr,sizeof(sLayerHdr),1,File);
		fseek(File,RetPos,SEEK_SET);

		Size=ftell(File)-HdrPos;
		return(HdrPos);
}

/*****************************************************************************/
void	CMkLevelLayerTile::BuildOutElem(int X,int Y,sTileMapElem *Out)
{
sMkLevelElem	&In=OutMap.Get(X,Y);	

			Out->Tile=In.Elem;

}

/*****************************************************************************/
int		CMkLevelLayerTile::WriteNormal(CMkLevel *Core,FILE *File)
{
int			Width=OutMap.GetWidth();
int			Height=OutMap.GetHeight();
sTileMapElem	OutElem;

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				BuildOutElem(X,Y,&OutElem);
		
				fwrite(&OutElem,sizeof(sTileMapElem),1,File);
			}
		}
	return(Width*Height*sizeof(sTileMapElem));
}

/*****************************************************************************/
int	CMkLevelLayerTile::WritePak(CMkLevel *Core,FILE *File)
{
int				MapW=OutMap.GetWidth();
int				MapH=OutMap.GetHeight();
int				ChunkW=(MapW/PakW)+1;
int				ChunkH=(MapH/PakH)+1;
int				BufferSize=PakW*PakH*sizeof(sTileMapElem);
int				TotalPak=0;

			ChunkBuffer=(sTileMapElem*)malloc(BufferSize);
			PakBuffer=(u8*)malloc(BufferSize*8);	// Allow for bad pak

			for (int ChunkY=0; ChunkY<ChunkH; ChunkY++)
			{
				for (int ChunkX=0; ChunkX<ChunkW; ChunkX++)
				{
					// Create Chunk
					int	ThisChunkW,ThisChunkH,ThisChunkSize;
					int	PakSize;
					ThisChunkSize=BuildPakChunk(ChunkX,ChunkY,ThisChunkW,ThisChunkH);
					
					PakSize=PakChunk(ThisChunkSize);
//					if (PakSize>=PakW*PakH*sizeof(sTileMapElem)) GObject::Error(ERR_WARNING,"BAD PAK: %i>%i\n",PakSize,BufferSize);
					TotalPak+=PakSize;

					fwrite(&PakBuffer,PakSize,1,File);
				}
			}
			free(ChunkBuffer);
			free(PakBuffer);
			return(TotalPak);

}

/*****************************************************************************/
int		CMkLevelLayerTile::BuildPakChunk(int X,int Y,int &ChunkW,int &ChunkH)
{
int		MapW=OutMap.GetWidth();
int		MapH=OutMap.GetHeight();

// Calc Chunk Pos & size
		X=X*PakW;
		MapW-=X;
		ChunkW=__min(MapW,PakW);

		Y=Y*PakH;
		MapH-=Y;
		ChunkH=__min(MapH,PakH);

// Build Chunk
		sTileMapElem	*OutPtr=ChunkBuffer;
		for (int MY=0; MY<ChunkH; MY++)
		{
			for (int MX=0; MX<ChunkW; MX++)
			{
				BuildOutElem(X+MX,Y+MY,OutPtr++);
			}
		}

		return(ChunkW*ChunkH*sizeof(sTileMapElem));
}

/*****************************************************************************/
int		CMkLevelLayerTile::PakChunk(int ChunkSize)
{
int		PakSize=PAK_doPak(PakBuffer,(u8*)ChunkBuffer,ChunkSize);
		
		return(PakSize);
}
