/***************************/
/*** 3d Tile Layer Class ***/
/***************************/

#include 	"system\global.h"
#include	"system\vid.h"
#include	<DStructs.h>
#include 	"utils\utils.h"
#include 	"gfx\prim.h"
#include	"game\game.h"

#include	"LayerTile.h"
#include	"LayerTile3d.h"


#if		1
#if		defined(__USER_sbart__) // || defined(__USER_daveo__)
#define	_SHOW_POLYZ_	1
#include	"gfx\font.h"	
static		FontBank		*Font;
int			ShowPolyz=0;
#endif
#endif

static const int	BLOCK_SIZE				=16;
#if defined(__TERRITORY_EUR__)
static const int	SCREEN_TILE_ADJ_U		=2;
static const int	SCREEN_TILE_ADJ_D		=3;	// Extra 2 lines needed, cos of extra height & perspective :o(
static const int	SCREEN_TILE_ADJ_L		=3;
static const int	SCREEN_TILE_ADJ_R		=3;
#else
static const int	SCREEN_TILE_ADJ_U		=2;
static const int	SCREEN_TILE_ADJ_D		=1;
static const int	SCREEN_TILE_ADJ_L		=3;
static const int	SCREEN_TILE_ADJ_R		=3;
#endif
static const int	SCREEN_TILE3D_WIDTH		=(INGAME_SCREENW/BLOCK_SIZE)+SCREEN_TILE_ADJ_L+SCREEN_TILE_ADJ_R;
static const int	SCREEN_TILE3D_HEIGHT	=(INGAME_SCREENH/BLOCK_SIZE)+SCREEN_TILE_ADJ_U+SCREEN_TILE_ADJ_D;

static const int	RENDER_X_OFS			=INGAME_SCREENOFS_X-(SCREEN_TILE_ADJ_L*BLOCK_SIZE)+INGAME_RENDER_OFS_X;
static const int	RENDER_Y_OFS			=INGAME_SCREENOFS_Y-(SCREEN_TILE_ADJ_U*BLOCK_SIZE)+INGAME_RENDER_OFS_Y;

static const int	DeltaTableSizeX=SCREEN_TILE3D_WIDTH+1;
static const int	DeltaTableSizeY=SCREEN_TILE3D_HEIGHT+1;

/*****************************************************************************/
// now uses a single delta table for front and back (interleaved) to reduce register use
// 0 LUF
// 1 RUF
// 2 LDF
// 3 RDF

// 4 LUB
// 5 RUB
// 6 LDB
// 7 RDB

sFlipTable	FlipTable[4]=
{
/*00 <0*/	{{+4096,0,+4096,0},	{	((DVECTOR*)SCRATCH_RAM)+0,
									((DVECTOR*)SCRATCH_RAM)+1,
									((DVECTOR*)SCRATCH_RAM)+2,
									((DVECTOR*)SCRATCH_RAM)+3,
									((DVECTOR*)SCRATCH_RAM)+4,
									((DVECTOR*)SCRATCH_RAM)+5,
									((DVECTOR*)SCRATCH_RAM)+6,
									((DVECTOR*)SCRATCH_RAM)+7,
								}, 0<<31},	
/*01 >0*/	{{-4096,0,+4096,0},	{	((DVECTOR*)SCRATCH_RAM)+1,
									((DVECTOR*)SCRATCH_RAM)+0,
									((DVECTOR*)SCRATCH_RAM)+3,
									((DVECTOR*)SCRATCH_RAM)+2,
									((DVECTOR*)SCRATCH_RAM)+5,
									((DVECTOR*)SCRATCH_RAM)+4,
									((DVECTOR*)SCRATCH_RAM)+7,
									((DVECTOR*)SCRATCH_RAM)+6,
								},1<<31},
/*10 >0*/	{{+4096,0,-4096,0},	{	((DVECTOR*)SCRATCH_RAM)+2,
									((DVECTOR*)SCRATCH_RAM)+3,
									((DVECTOR*)SCRATCH_RAM)+0,
									((DVECTOR*)SCRATCH_RAM)+1,
									((DVECTOR*)SCRATCH_RAM)+6,
									((DVECTOR*)SCRATCH_RAM)+7,
									((DVECTOR*)SCRATCH_RAM)+4,
									((DVECTOR*)SCRATCH_RAM)+5,
								},1<<31},
/*11 <0*/	{{-4096,0,-4096,0},	{	((DVECTOR*)SCRATCH_RAM)+3,
									((DVECTOR*)SCRATCH_RAM)+2,
									((DVECTOR*)SCRATCH_RAM)+1,
									((DVECTOR*)SCRATCH_RAM)+0,
									((DVECTOR*)SCRATCH_RAM)+7,
									((DVECTOR*)SCRATCH_RAM)+6,
									((DVECTOR*)SCRATCH_RAM)+5,
									((DVECTOR*)SCRATCH_RAM)+4,
								},0<<31}
};

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CLayerTile3d::CLayerTile3d(sLevelHdr *LevelHdr,sLayerHdr *Hdr,u8 *_RGBMap,u8 *_RGBTable) : CLayerTile(LevelHdr,Hdr)
{
		ElemBank3d=LevelHdr->ElemBank3d;
		TriList=LevelHdr->TriList;
		QuadList=LevelHdr->QuadList;
		VtxList=LevelHdr->VtxList;
		VtxIdxList=LevelHdr->VtxIdxList;
		RGBMap=_RGBMap;
		RGBTable=_RGBTable;

#if		defined(_SHOW_POLYZ_)
		Font=new ("PrimFont") FontBank;
		Font->initialise( &standardFont );
		Font->setOt( 0 );
		Font->setTrans(1);
#endif

}

/*****************************************************************************/
CLayerTile3d::~CLayerTile3d()
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

void	CLayerTile3d::init(DVECTOR &MapPos,int Shift)
{
		CLayerTile::init(MapPos,Shift);
		CalcDelta();
}

/*****************************************************************************/
void	CLayerTile3d::shutdown()
{
#if		defined(_SHOW_POLYZ_)
		Font->dump();
		delete Font;
#endif
		for (int i=0; i<16; i++)
		{
			MemFree(DeltaTableX[i]);
			MemFree(DeltaTableY[i]);
		}

}

/*****************************************************************************/
void	CLayerTile3d::CalcDelta()
{
VECTOR	BlkPos;
SVECTOR	Pnt={-BLOCK_SIZE/2,-BLOCK_SIZE/2,-BLOCK_SIZE*4};
s16		*Tab;
		CGameScene::setCameraMtx();

		for (int i=0; i<16; i++)
		{
			Tab=(s16*)MemAlloc(DeltaTableSizeX*2*sizeof(s16),"DeltaTableXTable");
			DeltaTableX[i]=Tab;
			ASSERT(Tab);
			BlkPos.vx=RENDER_X_OFS-i;
			BlkPos.vy=RENDER_Y_OFS;
			for (int x=0; x<DeltaTableSizeX; x++)
			{
				s32		Tmp;
				DVECTOR	O;
				CMX_SetTransMtxXY(&BlkPos);
				Pnt.vz=-BLOCK_SIZE*4;
				RotTransPers(&Pnt,(s32*)&O,&Tmp,&Tmp);
				*Tab++=O.vx;
				Pnt.vz=+BLOCK_SIZE*4;
				RotTransPers(&Pnt,(s32*)&O,&Tmp,&Tmp);
				*Tab++=O.vx;
				BlkPos.vx+=BLOCK_SIZE;
			}

			Tab=(s16*)MemAlloc(DeltaTableSizeY*2*sizeof(s16),"DeltaYTable");
			DeltaTableY[i]=Tab;
			ASSERT(Tab);
			BlkPos.vx=RENDER_X_OFS;
			BlkPos.vy=RENDER_Y_OFS-i;
			for (int y=0; y<DeltaTableSizeY; y++)
			{
				s32		Tmp;
				DVECTOR	O;
				CMX_SetTransMtxXY(&BlkPos);
				Pnt.vz=-BLOCK_SIZE*4;
				RotTransPers(&Pnt,(s32*)&O,&Tmp,&Tmp);
				*Tab++=O.vy;
				Pnt.vz=+BLOCK_SIZE*4;
				RotTransPers(&Pnt,(s32*)&O,&Tmp,&Tmp);
				*Tab++=O.vy;
				BlkPos.vy+=BLOCK_SIZE;
			}
		}
		DeltaF=DeltaTableX[0][(1*2)+0]-DeltaTableX[0][(0*2)+0];
		DeltaB=DeltaTableY[0][(1*2)+1]-DeltaTableY[0][(0*2)+1];

}

/*****************************************************************************/
void	CLayerTile3d::think(DVECTOR &MapPos)
{
			MapXY.vx=MapPos.vx>>4;
			MapXY.vy=MapPos.vy>>4;
			
			MapXY.vx-=SCREEN_TILE_ADJ_L;
			MapXY.vy-=SCREEN_TILE_ADJ_U;

			ShiftX=(MapPos.vx & 15);
			ShiftY=(MapPos.vy & 15);

			RenderOfs.vx=RenderOfs.vy=0;
			DeltaFOfs.vx=DeltaFOfs.vy=0;
			DeltaBOfs.vx=DeltaBOfs.vy=0;
			if (MapXY.vx<0) 
			{
				RenderOfs.vx=-MapXY.vx*BLOCK_SIZE;
				DeltaFOfs.vx=-MapXY.vx*DeltaF;
				DeltaBOfs.vx=-MapXY.vx*DeltaB;
				MapXY.vx=0;
			}
			if (MapXY.vy<0) 
			{
				RenderOfs.vy=-MapXY.vy*BLOCK_SIZE;
				DeltaFOfs.vy=-MapXY.vy*DeltaF;
				DeltaBOfs.vy=-MapXY.vy*DeltaB;
				MapXY.vy=0;
			}

			if (MapXY.vx+SCREEN_TILE3D_WIDTH<=MapWidth)
				RenderW=SCREEN_TILE3D_WIDTH;
			else
				RenderW=MapWidth-MapXY.vx;

			if (MapXY.vy+SCREEN_TILE3D_HEIGHT<=MapHeight)
				RenderH=SCREEN_TILE3D_HEIGHT;
			else
				RenderH=MapHeight-MapXY.vy;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CLayerTile3d::CacheElemVtx(sElem3d *Elem)
{
int		Count=Elem->VtxTriCount;
sVtx	*V0,*V1,*V2;
u16		*IdxTable=&VtxIdxList[Elem->VtxIdxStart];
s32		*OutVtx=(s32*)SCRATCH_RAM;
s32		*OutPtr;

		OutVtx+=8;

		V0=&VtxList[*IdxTable++];
		V1=&VtxList[*IdxTable++];
		V2=&VtxList[*IdxTable++];
		gte_ldv3(V0,V1,V2);

		while (Count--)
		{
			gte_rtpt_b(); // 22 cycles
// Preload next (when able) - Must check this
			V0=&VtxList[*IdxTable++];
			V1=&VtxList[*IdxTable++];
			V2=&VtxList[*IdxTable++];
			OutPtr=OutVtx;
			OutVtx+=3;
			gte_ldv3(V0,V1,V2);
			gte_stsxy3c(OutPtr);	// read XY back
		}
}

/*****************************************************************************/
void	CLayerTile3d::render()
{
int				MapOfs=GetMapOfs();
sTileMapElem	*MapPtr=Map+MapOfs;
u8				*RGBMapPtr=RGBMap+MapOfs;
u8				*PrimPtr=GetPrimPtr();
u8	const		*XYList=(u8*)SCRATCH_RAM;
u32				T0,T1,T2,T3;
s32				ClipZ;
sOT				*ThisOT;
VECTOR			BlkPos;
DVECTOR			*DP0,*DP1,*DP2,*DP3;
s16				*DeltaTabY=DeltaTableY[ShiftY];

#if		defined(_SHOW_POLYZ_)
s16				TCount=0,QCount=0;
#endif

// Setup Trans Matrix
		BlkPos.vx=RENDER_X_OFS-(ShiftX)+RenderOfs.vx;
		BlkPos.vy=RENDER_Y_OFS-(ShiftY)+RenderOfs.vy;

		for (int Y=0; Y<RenderH; Y++)
		{
			sTileMapElem	*MapRow=MapPtr;
			u8				*RGBRow=RGBMapPtr;
			s32				BlkXOld=BlkPos.vx;
			s16				*DeltaTabX=DeltaTableX[ShiftX];

			for (int X=0; X<RenderW; X++)
			{
				u16			Tile=MapRow->Tile;
				u16			TileIdx=Tile>>2;
				sElem3d		*Elem=&ElemBank3d[TileIdx];

				int			TriCount=Elem->TriCount;				
				int			QuadCount=Elem->QuadCount;				
				int			RGBOfs=*RGBRow++;

				if (TriCount || QuadCount)	// Blank tiles rejected here, to prevent over processing (as no tri-count)
				{
					sFlipTable	*FTab=&FlipTable[Tile&3];
					u8			*RGB=&RGBTable[RGBOfs*(16*4)];

					CMX_SetTransMtxXY(&BlkPos);
					CMX_SetRotMatrixXY(&FTab->Mtx);

// --- Cache Vtx ----------
					{
					int		Count=Elem->VtxTriCount;
					sVtx	*V0,*V1,*V2;
					u16		*IdxTable=&VtxIdxList[Elem->VtxIdxStart];
					s32		*OutVtx=(s32*)SCRATCH_RAM;
					s32		*OutPtr;

							OutVtx+=8;

							V0=&VtxList[*IdxTable++];
							V1=&VtxList[*IdxTable++];
							V2=&VtxList[*IdxTable++];
							gte_ldv3(V0,V1,V2);

							while (Count--)
							{
								gte_rtpt_b(); // 22 cycles
					// Preload next (when able) - Must check this
								V0=&VtxList[*IdxTable++];
								V1=&VtxList[*IdxTable++];
								V2=&VtxList[*IdxTable++];
								OutPtr=OutVtx;
								OutVtx+=3;
								gte_ldv3(V0,V1,V2);
								gte_stsxy3c(OutPtr);	// read XY back
							}
					}

					s16	FL=DeltaTabX[(0*2)+0]+DeltaFOfs.vx;
					s16	FR=DeltaTabX[(1*2)+0]+DeltaFOfs.vx;
					s16	FU=DeltaTabY[(0*2)+0]+DeltaFOfs.vy;
					s16	FD=DeltaTabY[(1*2)+0]+DeltaFOfs.vy;
					DP0=FTab->DeltaTab[0];
					DP1=FTab->DeltaTab[1];
					DP2=FTab->DeltaTab[2];
					DP3=FTab->DeltaTab[3];
					DP0->vx=FL;
					DP0->vy=FU;
					DP1->vx=FR;
					DP1->vy=FU;
					DP2->vx=FL;
					DP2->vy=FD;
					DP3->vx=FR;
					DP3->vy=FD;

					s16	BL=DeltaTabX[(0*2)+1]+DeltaBOfs.vx;
					s16	BR=DeltaTabX[(1*2)+1]+DeltaBOfs.vx;
					s16	BU=DeltaTabY[(0*2)+1]+DeltaBOfs.vy;
					s16	BD=DeltaTabY[(1*2)+1]+DeltaBOfs.vy;
					DP0=FTab->DeltaTab[4];
					DP1=FTab->DeltaTab[5];
					DP2=FTab->DeltaTab[6];
					DP3=FTab->DeltaTab[7];
					DP0->vx=BL;
					DP0->vy=BU;
					DP1->vx=BR;
					DP1->vy=BU;
					DP2->vx=BL;
					DP2->vy=BD;
					DP3->vx=BR;
					DP3->vy=BD;

// --- Render Tri's -------------
					sTri	*TList=&TriList[Elem->TriStart];
					while (TriCount--)
					{
						POLY_GT3	*ThisPrim=(POLY_GT3*)PrimPtr;

						T0=*(u32*)(XYList+TList->P0); 
						T1=*(u32*)(XYList+TList->P1); 
						T2=*(u32*)(XYList+TList->P2);
						gte_ldsxy0(T0);
						gte_ldsxy1(T1);
						gte_ldsxy2(T2);
						
						setlen(ThisPrim, GPU_PolyGT3Tag);
						gte_nclip_b();	// 8 cycles

						*(u32*)&ThisPrim->x0=T0;	// Set XY0
						*(u32*)&ThisPrim->x1=T1;	// Set XY1
						*(u32*)&ThisPrim->x2=T2;	// Set XY2
						T0=*(u32*)&TList->uv0;		// Get UV0 & TPage
						T1=*(u32*)&TList->uv1;		// Get UV1 & Clut
						T2=*(u32*)&TList->uv2;		// Get UV2
						gte_stopz(&ClipZ);
						ThisOT=OtPtr+TList->OTOfs;
						ClipZ^=FTab->ClipCode;
						if (ClipZ<0)
						{
							*(u32*)&ThisPrim->u0=T0;	// Set UV0
							*(u32*)&ThisPrim->u1=T1;	// Set UV1
							*(u32*)&ThisPrim->u2=T2;	// Set UV2
							{ // lighting
								T0=*(u32*)&RGB[TList->C0];
								T1=*(u32*)&RGB[TList->C1];
								T2=*(u32*)&RGB[TList->C2];
								*(u32*)&ThisPrim->r0=T0;
								*(u32*)&ThisPrim->r1=T1;
								*(u32*)&ThisPrim->r2=T2;
							}
#if		defined(_SHOW_POLYZ_)	
							if (ShowPolyz)	{setRGB0(ThisPrim,127,0,0); setRGB1(ThisPrim,255,0,0); setRGB2(ThisPrim,255,0,0); TCount++;}	
#endif
							ThisPrim->code=TList->PolyCode;
							addPrim(ThisOT,ThisPrim);
							PrimPtr+=sizeof(POLY_GT3);
						}
						TList++;
					}

// --- Render Quads -----------
					sQuad	*QList=&QuadList[Elem->QuadStart];
					while (QuadCount--)
					{
						POLY_GT4	*ThisPrim=(POLY_GT4*)PrimPtr;

						T0=*(u32*)(XYList+QList->P0); 
						T1=*(u32*)(XYList+QList->P1); 
						T2=*(u32*)(XYList+QList->P2);
						gte_ldsxy0(T0);
						gte_ldsxy1(T1);
						gte_ldsxy2(T2);
						
						setlen(ThisPrim, GPU_PolyGT4Tag);
						gte_nclip_b();	// 8 cycles
						T3=*(u32*)(XYList+QList->P3);

						*(u32*)&ThisPrim->x0=T0;	// Set XY0
						*(u32*)&ThisPrim->x1=T1;	// Set XY1
						*(u32*)&ThisPrim->x2=T2;	// Set XY2
						*(u32*)&ThisPrim->x3=T3;	// Set XY3
						T0=*(u32*)&QList->uv0;		// Get UV0 & TPage
						T1=*(u32*)&QList->uv1;		// Get UV1 & Clut
						T2=*(u32*)&QList->uv2;		// Get UV2
						T3=*(u32*)&QList->uv3;		// Get UV2
						gte_stopz(&ClipZ);
						ThisOT=OtPtr+QList->OTOfs;
						ClipZ^=FTab->ClipCode;
						if (ClipZ<0)
						{
							*(u32*)&ThisPrim->u0=T0;	// Set UV0
							*(u32*)&ThisPrim->u1=T1;	// Set UV1
							*(u32*)&ThisPrim->u2=T2;	// Set UV2
							*(u32*)&ThisPrim->u3=T3;	// Set UV2
							{ // Lighting
								T0=*(u32*)&RGB[QList->C0];
								T1=*(u32*)&RGB[QList->C1];
								T2=*(u32*)&RGB[QList->C2];
								T3=*(u32*)&RGB[QList->C3];
								*(u32*)&ThisPrim->r0=T0;
								*(u32*)&ThisPrim->r1=T1;
								*(u32*)&ThisPrim->r2=T2;
								*(u32*)&ThisPrim->r3=T3;
							}
#if		defined(_SHOW_POLYZ_)	
							if (ShowPolyz)	{setRGB0(ThisPrim,0,127,0);setRGB1(ThisPrim,0,255,0); setRGB2(ThisPrim,0,255,0); setRGB3(ThisPrim,0,255,0); QCount++;}	
#endif
							ThisPrim->code=QList->PolyCode;

							addPrim(ThisOT,ThisPrim);
							PrimPtr+=sizeof(POLY_GT4);
						}
						QList++;
					}

				}
				MapRow++;
				BlkPos.vx+=BLOCK_SIZE;
				DeltaTabX+=2;
			}
			MapPtr+=MapWidth;
			RGBMapPtr+=MapWidth;
			BlkPos.vx=BlkXOld;
			BlkPos.vy+=BLOCK_SIZE;
			DeltaTabY+=2;
		}

		SetPrimPtr((u8*)PrimPtr);

#if		defined(_SHOW_POLYZ_)
char	Txt[256];
		sprintf(Txt,"TC %i\nQC %i",TCount,QCount);
		Font->print( 128, 32, Txt);
#endif

}
