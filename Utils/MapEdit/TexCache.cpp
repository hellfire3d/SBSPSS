/*********************/
/*** Texture Cache ***/
/*********************/

#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	<gl\glaux.h>		// Header File For The Glaux Library
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
//			sprintf(NewTex.Filename,"%s%s",Path,TexName);
			TRACE1("Loading Texture %s\n",NewTex.Filename);
			LoadBMP(NewTex.Filename,ThisRGB);
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
void	CTexCache::LoadBMP(char *Filename,sRGBData &RGBData)
{
FILE			*File=NULL;
AUX_RGBImageRec *Aux;

		Aux=auxDIBImageLoad(Filename);
		RGBData.Width=Aux->sizeX;
		RGBData.Height=Aux->sizeY;
		RGBData.RGB=Aux->data;
		free(Aux);	// Safe to free aux now, contents copied (I HATE AUX)
}

/**************************************************************************************/
void	CTexCache::FreeBMP(sRGBData &RGBData)
{
		if (RGBData.RGB)
		{
			free((unsigned char*)RGBData.RGB);
		}
}

/**************************************************************************************/
/*
		for (int i=0;i<Size;i++)
		{
			u8	R=*RgbPtr++;
			u8	G=*RgbPtr++;
			u8	B=*RgbPtr++;
			u8	A=255;
			if ((R==BlankRGB.rgbRed && G==BlankRGB.rgbGreen && B==BlankRGB.rgbBlue))	// Create alpha for transparent pixels (flagged with PINK!!)
			{
				A=0;
			}

			Buffer[(i*4)+0]=R;
			Buffer[(i*4)+1]=G;
			Buffer[(i*4)+2]=B;
			Buffer[(i*4)+3]=A;
		}
*/		

void	CTexCache::LoadTex(sTex &ThisTex,sRGBData *TexData)
{
std::vector<u8>	Buffer;
int		TexWidth=TexData->Width;
int		TexHeight=TexData->Height;
int		GLWidth=AlignSize(TexWidth);
int		GLHeight=AlignSize(TexHeight);
u8		*Src,*Dst;
u8		R,G,B,A;
// create RGB & alpha texture & ensuse texture is correct size for GL

		Buffer.resize(GLWidth*GLHeight*4);
		Dst=&Buffer[0];
		for (int Y=0; Y<GLHeight; Y++)
		{
			for (int X=0; X<GLWidth; X++)
			{
				
				if (X<=TexWidth && Y<=TexHeight)
				{
					Src=(u8*)&TexData->RGB[((Y*TexWidth)+X)*3];
					R=*Src++;
					G=*Src++;
					B=*Src++;
					A=255;
					if ((R==BlankRGB.rgbRed && G==BlankRGB.rgbGreen && B==BlankRGB.rgbBlue))	// Create alpha for transparent pixels (flagged with PINK!!)
					{
						A=0;
					}

				}
				else
				{
					R=255;
					G=0;
					B=255;
					A=255;
				}
				*Dst++=R;
				*Dst++=G;
				*Dst++=B;
				*Dst++=A;
			}
		}

		ThisTex.TexWidth=TexWidth;
		ThisTex.TexHeight=TexHeight;
		ThisTex.GLWidth=GLWidth;
		ThisTex.GLHeight=GLHeight;
		ThisTex.dW=1.0f/(GLWidth/16);
		ThisTex.dW=((float)TexWidth/(float)GLWidth)/(GLWidth/16);
		ThisTex.dH=((float)TexHeight/(float)GLHeight)/(GLHeight/16);
		
		glGenTextures(1, &ThisTex.TexID);

		glBindTexture(GL_TEXTURE_2D, ThisTex.TexID);
		glTexImage2D(GL_TEXTURE_2D, 0, 4, GLWidth, GLHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, &Buffer[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
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
