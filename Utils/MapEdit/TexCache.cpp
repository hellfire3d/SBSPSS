/*********************/
/*** Texture Cache ***/
/*********************/

#include	"stdafx.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<frame.hpp>
#include	<gfname.hpp>
#include	<Vector>

#include	"TexCache.h"
#include	"utils.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// Checks loaded files for dups, assumes all passed RGB is unique
int		CTexCache::ProcessTexture(const char *Filename,sRGBData *RGBData)
{
int			ListSize=TexList.size();
sTex		NewTex;
sRGBData	ThisRGB;
GFName		FName=Filename;

		NewTex.Name=FName.File();
		NewTex.Filename=Filename;

		if (!RGBData)	// Need to load file
		{
			int	Idx=GetTexIdx(NewTex);	// Is already loaded?
			if (Idx!=-1) return(Idx);

//			TRACE1("Loading Texture %s\n",NewTex.Filename);
			
			if (!LoadBMP(NewTex.Filename,ThisRGB))
			{
				exit(-1);
				return(ListSize);
			}
			RGBData=&ThisRGB;
			LoadTex(NewTex,RGBData);
			FreeBMP(ThisRGB);
			NewTex.Loaded=true;
		}
		else
		{
			LoadTex(NewTex,RGBData);
			NewTex.Loaded=FALSE;
		}

		TexList.push_back(NewTex);
				
		return(ListSize);
}

/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
void	CTexCache::CreateAlignTex(Frame &ThisFrame,sRGBData &RGBData)
{
int		FrameW=ThisFrame.GetWidth();
int		FrameH=ThisFrame.GetHeight();
int		AlignW=AlignSize(FrameW);
int		AlignH=AlignSize(FrameH);

		RGBData.RGB=(u8*)MemAlloc(AlignW*AlignH*3);
		ThisFrame.FlipY();
		RGBData.OldW=FrameW;
		RGBData.OldH=FrameH;
		RGBData.TexW=AlignW;
		RGBData.TexH=AlignH;
		RGBData.ScaleU=(float)FrameW/(float)AlignW;
		RGBData.ScaleV=(float)FrameH/(float)AlignH;

		if (FrameW==AlignW && FrameH==AlignH)
		{
			ThisFrame.MakeRGB(RGBData.RGB);
			return;
		}
// Tex is mis aligned, so align and fill borders with pure pink

u8		*Buffer=(u8*)MemAlloc(FrameW*FrameH*3);
u8		*Src,*Dst;
int		X,Y;
		ThisFrame.MakeRGB(Buffer);

		Dst=RGBData.RGB;
	
		Src=Buffer;
		Dst=RGBData.RGB;
		for (Y=0; Y<FrameH; Y++)
		{
			for (X=0; X<FrameW; X++)
			{
				*Dst++=*Src++; 
				*Dst++=*Src++; 
				*Dst++=*Src++;
			}
			for (X; X<AlignW; X++)
			{
				*Dst++=BlankRGB.rgbRed;
				*Dst++=BlankRGB.rgbGreen;
				*Dst++=BlankRGB.rgbBlue;
			}
		}
		for (Y; Y<AlignH; Y++)
		{
			for (X=0; X<AlignW; X++)
			{
				*Dst++=BlankRGB.rgbRed;
				*Dst++=BlankRGB.rgbGreen;
				*Dst++=BlankRGB.rgbBlue;
			}
		}

		MemFree(Buffer);
}

/**************************************************************************************/
bool	CTexCache::LoadBMP(const char *Filename,sRGBData &RGBData)
{
Frame	ThisFrame;
FILE	*File;
// Check File exists
		File=fopen(Filename,"r");
		
		if (!File)
		{
			CString mexstr;
			mexstr.Format("%s Not Found\n", Filename);
			AfxMessageBox(mexstr,MB_OK | MB_ICONEXCLAMATION);
			exit(EXIT_FAILURE );
			return(false);
		}

		fclose(File);		
		ThisFrame.LoadBMP(Filename);

		CreateAlignTex(ThisFrame,RGBData);	
		return(true);
}

/**************************************************************************************/
void	CTexCache::FreeBMP(sRGBData &RGBData)
{
		if (RGBData.RGB)
		{
			MemFree(RGBData.RGB);
			RGBData.RGB=0;
		}
}

/**************************************************************************************/
void	CTexCache::LoadTex(sTex &ThisTex,sRGBData *RGBData)
{
u8		*Buffer;
u8		*Src,*Dst;

// create RGB & alpha texture & ensuse texture is correct size for GL
		Buffer=(u8*)MemAlloc(RGBData->TexW*RGBData->TexH*4);
		ASSERT(Buffer);

		Src=RGBData->RGB;
		Dst=Buffer;

		for (int Y=0; Y<RGBData->TexH; Y++)
		{
			for (int X=0; X<RGBData->TexW; X++)
			{
				u8	R,G,B,A;

				R=*Src++;
				G=*Src++;
				B=*Src++;
				A=255;
				if ((R==BlankRGB.rgbRed && G==BlankRGB.rgbGreen && B==BlankRGB.rgbBlue))	// Create alpha for transparent pixels (flagged with PINK!!)
					{
						A=0;
					}
				*Dst++=R;
				*Dst++=G;
				*Dst++=B;
				*Dst++=A;
			}
		}

		ThisTex.OldW=RGBData->OldW;
		ThisTex.OldH=RGBData->OldH;
		ThisTex.TexW=RGBData->TexW;
		ThisTex.TexH=RGBData->TexH;
		ThisTex.ScaleU=RGBData->ScaleU;
		ThisTex.ScaleV=RGBData->ScaleV;

		glGenTextures(1, &ThisTex.TexID);
		glBindTexture(GL_TEXTURE_2D, ThisTex.TexID);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, RGBData->TexW, RGBData->TexH, 0, GL_RGBA, GL_UNSIGNED_BYTE, Buffer);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glBindTexture(GL_TEXTURE_2D, 0);
		MemFree(Buffer);
}

/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
void	CTexCache::Purge()
{
int	ListSize=TexList.size();

//	TRACE1("Purging %i textures\n",ListSize);

	for (int i=0; i<ListSize; i++)
	{
		glDeleteTextures(1,&TexList[i].TexID);
	}

	TexList.clear();
}

/**************************************************************************************/
