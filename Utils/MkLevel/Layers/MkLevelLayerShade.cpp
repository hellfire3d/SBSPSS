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
int		*iPtr;
u8		*Ptr=(u8*)LayerHdr;
u8		*RGB;

		Type=LayerHdr->Type;
		SubType=LayerHdr->SubType;
		Width=LayerHdr->Width;
		Height=LayerHdr->Height;

		iPtr=(int*)(Ptr+sizeof(sExpLayerHdr));

		Count=*iPtr++;
		List.resize(LAYER_SHADE_RGB_MAX);
		for (int i=0; i<LAYER_SHADE_RGB_MAX; i++)
		{
			List[i].Pos=*iPtr++;
			RGB=(u8*)(iPtr);
			List[i].RGB[0]=RGB[0];
			List[i].RGB[1]=RGB[1];
			List[i].RGB[2]=RGB[2];
			iPtr+=4/sizeof(int);
		}

		Trans[0]=*iPtr++;
		Flags[0]=*iPtr++;
		Trans[1]=*iPtr++;
		Flags[1]=*iPtr++;

// Load back gfx
char	*c=(char*)iPtr;
		BackGfx[0]=c;  
		c+=strlen(c)+1;
		BackGfx[1]=c;  

}


/*****************************************************************************/
/*****************************************************************************/
/*** Pre-Process *************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// Build unique tiles, including pre-genned flips, and replace tile idx with new one
void	CMkLevelLayerShade::PreProcess(CMkLevel *Core)
{
GString	Path=Core->GetConfigStr("MISC","BackGfxDir");

	for (int i=0; i<2; i++)
	{
		if (!BackGfx[i].Empty())
		{
			TexID[i]=AddBackGfx(Core,Path+BackGfx[i]+".Bmp");
		}
		else
		{
			TexID[i]=-1;
		}
	}
}

/*****************************************************************************/
int		CMkLevelLayerShade::AddBackGfx(CMkLevel *Core,const char *Filename)
{
sBackGfxList	NewGfx;
CTexGrab		&TexGrab=Core->GetTexGrab();

			NewGfx.Name=Filename;
			NewGfx.TexID=-1;

int			Idx=BackGfxList.Add(NewGfx);

			if (BackGfxList[Idx].TexID==-1)
			{
				TexGrab.ZeroColZero(true);
				BackGfxList[Idx].TexID=TexGrab.AddFile(BackGfxList[Idx].Name);
				TexGrab.ZeroColZero(false);
			}
			
			return(BackGfxList[Idx].TexID);

}

/*****************************************************************************/
/*****************************************************************************/
/*** Process *****************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CMkLevelLayerShade::Process(CMkLevel *Core)
{
CTexGrab	&TexGrab=Core->GetTexGrab();

		//printf("Process Shade Layer\n");
		for (int i=0; i<2; i++)
		{
			sLayerShadeGfx	&ThisGfx=Data.BackGfx[i];
			if (TexID[i]==-1)
			{
				ThisGfx.TPage=0;
			}
			else
			{
				sTexOutInfo	&ThisTex=TexGrab.GetTexInfo()[TexID[i]];
				ThisGfx.TPage=ThisTex.Tpage;
				ThisGfx.Clut=ThisTex.Clut;
				ThisGfx.U=ThisTex.u;
				ThisGfx.V=ThisTex.v;
				ThisGfx.W=ThisTex.w;
				ThisGfx.H=ThisTex.h;
				ThisGfx.TPage|=Trans[i]<<5;
				ThisGfx.Flags=Flags[i];
			}

		}
}

/*****************************************************************************/
/*****************************************************************************/
/** Write ********************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CMkLevelLayerShade::Write(FILE *File,const char *LayerName,const char *MapName)
{
sLayerHdr		Hdr;
int				ThisPos=ftell(File);

		Hdr.Type=Type;
		Hdr.SubType=SubType;
		Hdr.Width=Width;
		Hdr.Height=Height;
		fwrite(&Hdr,sizeof(sLayerHdr),1,File);

		Data.Count=Count;
		for (int i=0; i<Count; i++)
		{
			Data.Data[i].Ofs=List[i].Pos;
			Data.Data[i].RGB[0]=List[i].RGB[0];
			Data.Data[i].RGB[1]=List[i].RGB[1];
			Data.Data[i].RGB[2]=List[i].RGB[2];
		}

		fwrite(&Data,sizeof(sLayerShadeHdr),1,File);
		PadFile(File);

		return(ThisPos);
}

/*****************************************************************************/
