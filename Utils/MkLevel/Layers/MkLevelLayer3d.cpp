/****************/
/*** Layer 3d ***/
/****************/

#include	<Davelib.h>
#include	<List2d.h>

#include	"..\MkLevel.h"
#include	"MkLevelLayer3d.h"

/*****************************************************************************/
/*****************************************************************************/
/*** Pre-Process *************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayer3d::PreProcess(CMkLevel *Core)
{
int			Width=InMap.GetWidth();
int			Height=InMap.GetHeight();

		OutMap.SetSize(Width,Height);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sExpLayerTile	&InElem=InMap.Get(X,Y);
				sMkLevelElem	&OutElem=OutMap.Get(X,Y);

				OutElem.Elem=0;
				if (InElem.Tile)
				{ // Not Blank Tile
					OutElem.Elem=Core->AddTile3d(InElem);
				}
			}
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*** Process *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayer3d::Process(CMkLevel *Core)
{
/*
int				Width=InMap.GetWidth();
int				Height=InMap.GetHeight();
int				i,ListSize;

		TriList.SetTexGrab(Core->GetTexGrab());
//!!!		TexGrab.AllowRotate(true);
		TexGrab.AllowRotate(false);
//!!!		TexGrab.ShrinkToFit(true);
		TexGrab.ShrinkToFit(false);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sExpLayerTile	&ThisElem=InMap.Get(X,Y);
				sMkLevelElem	&OutElem=OutMap.Get(X,Y);
					
				OutElem.Elem=TriList.GetFaceCount();

				if (ThisElem.Tile)
				{ // Not Blank Tile
					sExpTile		&InTile=Core->GetTile(ThisElem);
// Build Sorted List
					CList<CFace>	SortList;
					CList<float>	ZPosList;
					if (InTile.TriCount)
					{
						for (i=0; i<InTile.TriCount; i++)
						{
							int		ListPos;
							CFace	&ThisFace=CMkLevel::AllTriList[InTile.TriStart+i];
							float	ThisZPos;
							
							ThisZPos=ThisFace.vtx[0].z;
							if (ThisZPos>ThisFace.vtx[1].z) ThisZPos=ThisFace.vtx[1].z;
							if (ThisZPos>ThisFace.vtx[2].z) ThisZPos=ThisFace.vtx[2].z;
							
							ListSize=SortList.size();
							for (ListPos=0; ListPos<ListSize; ListPos++)
							{
								if (ZPosList[ListPos]<ThisZPos) break;
							}
							SortList.insert(ListPos,ThisFace);
							ZPosList.insert(ListPos,ThisZPos);
							// Flip 3d tiles
							CFace	&F=SortList[ListPos];
							bool	SwapPnt=false;

							if (ThisElem.Flags & PC_TILE_FLAG_MIRROR_X)	
							{
								F.vtx[0].x=-F.vtx[0].x;
								F.vtx[1].x=-F.vtx[1].x;
								F.vtx[2].x=-F.vtx[2].x;
								SwapPnt^=1;
							}

							if (ThisElem.Flags & PC_TILE_FLAG_MIRROR_Y)	
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

						}

					}
					else
					{ // create flat tile
						for (int i=0; i<2; i++)
						{
							FlatFace[i].Mat=OutElem.TexID;
							SortList.push_back(FlatFace[i]);
						}
					}

					ListSize=SortList.size();
// Sort out Flipping
					for (i=0; i<ListSize; i++)
					{
						CFace	&F=SortList[i];

// Offset Vtx's
						for (int p=0; p<3; p++) 
						{
							F.vtx[p].x	+=(float)X;
							F.vtx[p].y	-=(float)Y;
						}
						F.ID=i;
//						printf("%i\n",F.ID);
						TriList.AddFace(F);

					}
				}
			}
		}
*/
}

/*****************************************************************************/
/*****************************************************************************/
/** Write ********************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CMkLevelLayer3d::Write(FILE *File,const char *LayerName,const char *MapName)
{
int				ThisPos=ftell(File);
/*
int				Width=OutMap.GetWidth();
int				Height=OutMap.GetHeight();
sLayerHdr		LayerHdr;
sLayer3d		Hdr3d;
vector<sVtx>	OutVtxList;

		TriList.Process();
		ProcessVtxList(TriList.GetVtxList(),OutVtxList);

		LayerHdr.Type=Type;
		LayerHdr.SubType=SubType;
		LayerHdr.Width=Width;
		LayerHdr.Height=Height;
		fwrite(&LayerHdr,sizeof(sLayerHdr),1,File);

int	Pos3d=ftell(File);
		fwrite(&Hdr3d,sizeof(sLayer3d),1,File);

		for (int Y=0; Y<Height; Y++)
		{
			for (int X=0; X<Width; X++)
			{
				sMkLevelElem	&OutElem=OutMap.Get(X,Y);	

				fwrite(&OutElem.Elem,sizeof(u16),1,File);
			}
		}

		PadFile(File);
		Hdr3d.TriCount=TriList.GetFaceCount();
		Hdr3d.TriList=TriList.WriteTriList(File)-ThisPos;
		Hdr3d.QuadCount=0;
		Hdr3d.QuadList=TriList.WriteQuadList(File)-ThisPos;
		Hdr3d.VtxCount=OutVtxList.size();
		Hdr3d.VtxList=TriList.WriteVtxList(File,OutVtxList)-ThisPos;

		printf("T:%i V:%i\t",Hdr3d.TriCount,Hdr3d.VtxCount);
		printf("sTri %i\tFT3 %i\n",Hdr3d.TriCount*16,Hdr3d.TriCount*34);
int	RetPos=ftell(File);
		fseek(File,Pos3d,SEEK_SET);
		fwrite(&Hdr3d,sizeof(sLayer3d),1,File);
		fseek(File,RetPos,SEEK_SET);
*/
		return(ThisPos);
}

/*****************************************************************************/
void	CMkLevelLayer3d::ProcessVtxList(vector<sVtx> const &InList,vector<sVtx> &OutList)
{
int		i,ListSize=InList.size();
//int		XMin,XMax,YMin,YMax;
sVtx	Ofs;
		OutList.resize(ListSize);
		if (!ListSize) return;
/*
		XMin=XMax=InList[0].vx;
		YMin=YMax=InList[0].vy;
// Find Min/Max
		for (i=1; i<ListSize; i++)
		{
//			printf("%i %i %i\n",InList[i].vx,InList[i].vy,InList[i].vz);
			if (XMin>InList[i].vx) XMin=InList[i].vx;
			if (XMax<InList[i].vx) XMax=InList[i].vx;
			if (YMin>InList[i].vy) YMin=InList[i].vy;
			if (YMax<InList[i].vy) YMax=InList[i].vy;
		}

//		Ofs.vx=-32768-XMin;
//		Ofs.vy=-32768-YMin;
		Ofs.vx=-XMin;
		Ofs.vy=-YMin;
*/
		Ofs.vx=-0;
		Ofs.vy=-0;
		Ofs.vz=-4*Scale;

// Adjust Vtx`
		for (i=0; i<ListSize;i++)
		{
			sVtx const	&In=InList[i];
			sVtx		&Out=OutList[i];

			Out.vx=+(In.vx+Ofs.vx);
			Out.vy=-(In.vy+Ofs.vy);
			Out.vz=+(In.vz+Ofs.vz);
//			printf("%i %i\n",In.vx,In.vy);

		}
/*
		XMin=XMax=OutList[0].vx;
		YMin=YMax=OutList[0].vy;
// Find Min/Max
		for (i=1; i<ListSize; i++)
		{
			if (XMin>OutList[i].vx) XMin=OutList[i].vx;
			if (XMax<OutList[i].vx) XMax=OutList[i].vx;
			if (YMin>OutList[i].vy) YMin=OutList[i].vy;
			if (YMax<OutList[i].vy) YMax=OutList[i].vy;
		}
		printf("MinXY %i,%i  %i,%i\n",XMin,YMin,XMax,YMax);
*/
}

/*****************************************************************************/
