/*********************/
/*** Texture Cache ***/
/*********************/

#include	"stdafx.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<frame.hpp>
#include	<Vector>

#include	"TexCache.h"
#include	"utils.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CTexCache::GetTexIdx(char *Filename,int Flags)
{
sTex	Tex;
		strcpy(Tex.Filename,Filename);
		Tex.Flags=Flags;
		return(TexList.Find(Tex));
}

/*****************************************************************************/
// Checks loaded files for dups, assumes all passed RGB is unique
int		CTexCache::ProcessTexture(char *Filename,int Flags,sRGBData *RGBData)
{
int		ListSize=TexList.size();

sTex		NewTex;
sRGBData	ThisRGB;

		strcpy(NewTex.Filename,Filename);
		NewTex.Flags=Flags;

		NewTex.Flags=Flags;

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
			NewTex.Loaded=TRUE;
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
const int	TexAlignTable[]={1,2,4,8,16,32,64,128,256};
const int	TexAlignTableSize=sizeof(TexAlignTable)/sizeof(int);
int		CTexCache::AlignSize(int Size)
{
		for (int i=0;i<TexAlignTableSize-1; i++)
		{
			if (Size>TexAlignTable[i] && Size<TexAlignTable[i+1]) return(TexAlignTable[i+1]);
		}

		return(Size);
}


/**************************************************************************************/
bool	CTexCache::LoadBMP(char *Filename,sRGBData &RGBData)
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
				
		RGBData.Width=ThisFrame.GetWidth();
		RGBData.Height=ThisFrame.GetHeight();
		RGBData.RGB=(u8*)malloc(RGBData.Width*RGBData.Height*3);
		ThisFrame.FlipY();
		ThisFrame.MakeRGB(RGBData.RGB);
		return(true);
}

/**************************************************************************************/
void	CTexCache::FreeBMP(sRGBData &RGBData)
{
		if (RGBData.RGB)
		{
			free(RGBData.RGB);
		}
}

/**************************************************************************************/
void	CTexCache::LoadTex(sTex &ThisTex,sRGBData *TexData)
{
u8		*Buffer;
int		TexWidth=TexData->Width;
int		TexHeight=TexData->Height;
int		GLWidth=AlignSize(TexWidth);
int		GLHeight=AlignSize(TexHeight);
u8		*Src,*Dst;

// create RGB & alpha texture & ensuse texture is correct size for GL

		Buffer=(u8*)malloc(GLWidth*GLHeight*4);
		Src=TexData->RGB;
		Dst=&Buffer[0];
		for (int Y=0; Y<TexHeight; Y++)
		{
			for (int X=0; X<TexWidth; X++)
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
			Dst+=(GLWidth-TexWidth)*4;
		}

		ThisTex.TexWidth=TexWidth;
		ThisTex.TexHeight=TexHeight;

		ThisTex.dW=(1.0f)/(float)(GLWidth/16.0f);
		ThisTex.dH=(1.0f)/(float)(GLHeight/16.0f);

		glGenTextures(1, &ThisTex.TexID);
		glBindTexture(GL_TEXTURE_2D, ThisTex.TexID);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, GLWidth, GLHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &Buffer[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glBindTexture(GL_TEXTURE_2D, 0);
		free(Buffer);
}

/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
void	CTexCache::Purge()
{
int	ListSize=TexList.size();

	TRACE1("Purging %i textures\n",ListSize);

	for (int i=0; i<ListSize; i++)
	{
		glDeleteTextures(1,&TexList[i].TexID);
	}

	TexList.clear();
}

/**************************************************************************************/
