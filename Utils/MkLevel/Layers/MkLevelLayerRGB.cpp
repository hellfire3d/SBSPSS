/*******************/
/*** Layer Shade ***/
/*******************/

#include	<DaveLib.h>
#include	<List2d.h>

#include	"MkLevelLayer.h"
#include	"MkLevelLayerShade.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CMkLevelLayerShade::CMkLevelLayerShade(sExpLayerHdr *LayerHdr)
{
int		i,ListSize;
int		*iPtr;
u8		*Ptr=(u8*)LayerHdr;

		Type=LayerHdr->Type;
		SubType=LayerHdr->SubType;
		Width=LayerHdr->Width;
		Height=LayerHdr->Height;

		iPtr=(int*)(Ptr+sizeof(sExpLayerHdr));

		ShadeHdr.BandCount=*iPtr++;
sRGBCol	*RGB=(sRGBCol*)iPtr;
		for (i=0; i<LAYER_SHADE_RGB_MAX; i++)
		{
			ShadeHdr.RGB[i][0]=RGB->R;
			ShadeHdr.RGB[i][1]=RGB->G;
			ShadeHdr.RGB[i][2]=RGB->B;
			RGB++;
		}
		iPtr=(int*)RGB;

		ListSize=*iPtr++;
		GfxList.resize(ListSize);
sLayerShadeGfx	*GfxPtr=(sLayerShadeGfx*)iPtr;
		for (i=0; i<ListSize; i++)
		{
			sLayerShadeGfx	&ThisGfx=GfxList[i];
			GfxList[i]=*GfxPtr++;
		}

		iPtr=(int*)GfxPtr;
		ListSize=*iPtr++;
		TypeNameList.resize(ListSize);
char	*TypePtr=(char*)iPtr;
		for (i=0; i<ListSize; i++)
		{
			TypeNameList[i]=TypePtr;
			TypePtr+=strlen(TypePtr)+1;
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*** Pre-Process *************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// Build unique tiles, including pre-genned flips, and replace tile idx with new one
void	CMkLevelLayerShade::PreProcess(CMkLevel *Core)
{
int			i,ListSize=GfxList.size();
int			Idx;
GString		Path=Core->GetConfigStr("MISC","BackGfxDir");
CTexGrab	&TexGrab=Core->GetTexGrab();

		for (i=0; i<ListSize; i++)
		{
			sLayerShadeGfx	&ThisGfx=GfxList[i];
			sBackGfxList	NewType;
				
			NewType.Name=Path+TypeNameList[ThisGfx.Gfx]+".Bmp";
			Idx=OutTypeList.Find(NewType);

			if (Idx==-1)
			{
				TexGrab.ZeroColZero(true);
				NewType.TexID=TexGrab.AddFile(NewType.Name);
				TexGrab.ZeroColZero(false);

				Idx=OutTypeList.Add(NewType);
			}
			ThisGfx.Gfx=Idx;
		}

}

/*****************************************************************************/
/*****************************************************************************/
/*** Process *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerShade::Process(CMkLevel *Core)
{
int			i,ListSize=OutTypeList.size();
CTexGrab	&TexGrab=Core->GetTexGrab();

		//printf("Process Shade Layer\n");
		for (i=0; i<ListSize; i++)
		{
			sBackGfxList	&ThisType=OutTypeList[i];

			sTexOutInfo	&ThisTex=TexGrab.GetTexInfo()[ThisType.TexID];
			ThisType.Out.TPage=ThisTex.Tpage;
			ThisType.Out.Clut=ThisTex.Clut;
			ThisType.Out.U=ThisTex.u;
			ThisType.Out.V=ThisTex.v;
			ThisType.Out.W=ThisTex.w;
			ThisType.Out.H=ThisTex.h;
//			ThisType.TPage|=Trans[i]<<5;
		}

}

/*****************************************************************************/
/*****************************************************************************/
/** Write ********************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CMkLevelLayerShade::Write(CMkLevel *Core,FILE *File,const char *LayerName)
{
sLayerHdr		Hdr;
int				ThisPos=ftell(File);

		Hdr.Type=Type;
		Hdr.SubType=SubType;
		Hdr.Width=Width;
		Hdr.Height=Height;
		fwrite(&Hdr,sizeof(sLayerHdr),1,File);

int		HdrPos=ftell(File);
		fwrite(&ShadeHdr,sizeof(sLayerShadeHdr),1,File);


// Write Gfx Stuff
		ShadeHdr.GfxList=(sLayerShadeBackGfx*)(WriteGfxList(File)-ThisPos);
		ShadeHdr.TypeList=(sLayerShadeBackGfxType*)(WriteTypeList(File)-ThisPos);

// rewrite header
int		RetPos=ftell(File);
		fseek(File,HdrPos,SEEK_SET);
		fwrite(&ShadeHdr,sizeof(sLayerShadeHdr),1,File);
		fseek(File,RetPos,SEEK_SET);


		Size=ftell(File)-ThisPos;
		return(ThisPos);
}

/*****************************************************************************/
int		CMkLevelLayerShade::WriteTypeList(FILE *File)
{
int		Pos=ftell(File);
int		i,ListSize=OutTypeList.size();

		for (i=0; i<ListSize; i++)
		{
			sBackGfxList	&ThisType=OutTypeList[i];

			fwrite(&ThisType.Out,sizeof(sLayerShadeBackGfxType),1,File);
		}
		return(Pos);
}

/*****************************************************************************/
int		CMkLevelLayerShade::WriteGfxList(FILE *File)
{
int		Pos=ftell(File);
int		i,ListSize=GfxList.size();

		ShadeHdr.GfxCount=ListSize;

		for (i=0; i<ListSize; i++)
		{
			sLayerShadeGfx		&ThisGfx=GfxList[i];
			sLayerShadeBackGfx	Out;
			
			Out.Type=ThisGfx.Gfx;
			Out.PosX=ThisGfx.Pos.x;
			Out.PosY=ThisGfx.Pos.y;
			Out.Trans=ThisGfx.TransMode;
			for (int p=0; p<4; p++)
			{
				Out.Ofs[p][0]=ThisGfx.Ofs[p].x;
				Out.Ofs[p][1]=ThisGfx.Ofs[p].y;
				Out.RGB[p][0]=ThisGfx.RGB[p].R;
				Out.RGB[p][1]=ThisGfx.RGB[p].G;
				Out.RGB[p][2]=ThisGfx.RGB[p].B;
			}
			fwrite(&Out,sizeof(sLayerShadeBackGfx),1,File);
		}
		return(Pos);
}
