/*************/
/*** Utils ***/
/*************/

#include	"stdafx.h"

#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	<gl\glaux.h>		// Header File For The Glaux Library
#include	"GLEnabledView.h"
#include	"maths.h"

#include	"Utils.H"

/**************************************************************************************/
void	DbgMsg(const char * pszFmt,...)
{
char szBuf[256];
		if (pszFmt)
		{
			va_list args;
			va_start(args,pszFmt);
			vsprintf(szBuf,pszFmt,args);
			OutputDebugString(szBuf);
			va_end(args);
		}
}

/**************************************************************************************/
/**************************************************************************************/
void	BuildGLBox(float XMin,float XMax,float YMin,float YMax,float ZMin,float ZMax)
{
			// Bottom Face
			glNormal3f( 0.0f,-1.0f, 0.0f);
			glVertex3f( XMin, YMin, ZMin);
			glVertex3f( XMax, YMin, ZMin);
			glVertex3f( XMax, YMin, ZMax);
			glVertex3f( XMin, YMin, ZMax);
			// Front Face
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glVertex3f( XMin, YMin, ZMax);
			glVertex3f( XMax, YMin, ZMax);
			glVertex3f( XMax, YMax, ZMax);
			glVertex3f( XMin, YMax, ZMax);
			// Back Face
			glNormal3f( 0.0f, 0.0f,-1.0f);
			glVertex3f( XMin, YMin, ZMin);
			glVertex3f( XMin, YMax, ZMin);
			glVertex3f( XMax, YMax, ZMin);
			glVertex3f( XMax, YMin, ZMin);
			// Right face
			glNormal3f( 1.0f, 0.0f, 0.0f);
			glVertex3f( XMax, YMin, ZMin);
			glVertex3f( XMax, YMax, ZMin);
			glVertex3f( XMax, YMax, ZMax);
			glVertex3f( XMax, YMin, ZMax);
			// Left Face
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glVertex3f( XMin, YMin, ZMin);
			glVertex3f( XMin, YMin, ZMax);
			glVertex3f( XMin, YMax, ZMax);
			glVertex3f( XMin, YMax, ZMin);
			// Top Face
			glNormal3f( 0.0f, 1.0f, 0.0f);
			glVertex3f( XMin, YMax, ZMin);
			glVertex3f( XMin, YMax, ZMax);
			glVertex3f( XMax, YMax, ZMax);
			glVertex3f( XMax, YMax, ZMin);
}

/**************************************************************************************/
void	BuildGLBoxNoNormals(float XMin,float XMax,float YMin,float YMax,float ZMin,float ZMax)
{
			// Bottom Face
			glVertex3f( XMin, YMin, ZMin);
			glVertex3f( XMax, YMin, ZMin);
			glVertex3f( XMax, YMin, ZMax);
			glVertex3f( XMin, YMin, ZMax);
			// Front Face
			glVertex3f( XMin, YMin, ZMax);
			glVertex3f( XMax, YMin, ZMax);
			glVertex3f( XMax, YMax, ZMax);
			glVertex3f( XMin, YMax, ZMax);
			// Back Face
			glVertex3f( XMin, YMin, ZMin);
			glVertex3f( XMin, YMax, ZMin);
			glVertex3f( XMax, YMax, ZMin);
			glVertex3f( XMax, YMin, ZMin);
			// Right face
			glVertex3f( XMax, YMin, ZMin);
			glVertex3f( XMax, YMax, ZMin);
			glVertex3f( XMax, YMax, ZMax);
			glVertex3f( XMax, YMin, ZMax);
			// Left Face
			glVertex3f( XMin, YMin, ZMin);
			glVertex3f( XMin, YMin, ZMax);
			glVertex3f( XMin, YMax, ZMax);
			glVertex3f( XMin, YMax, ZMin);
			// Top Face
			glVertex3f( XMin, YMax, ZMin);
			glVertex3f( XMin, YMax, ZMax);
			glVertex3f( XMax, YMax, ZMax);
			glVertex3f( XMax, YMax, ZMin);
}

/**************************************************************************************/
void	BuildGLQuad(float XMin,float XMax,float YMin,float YMax,float Z)
{
			// Front Face
			glNormal3f( 0.0f, 0.0f, 1.0f);
			glVertex3f( XMin, YMin, Z);
			glVertex3f( XMax, YMin, Z);
			glVertex3f( XMax, YMax, Z);
			glVertex3f( XMin, YMax, Z);
}

/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
void	TNormalise(TVECTOR &V)
{
float	SqMag = V.length2();// v.x * v.x + v.y * v.y + v.z * v.z;

		if (SqMag> 0.001f)
		{
			float	Mag = (float)sqrt( SqMag);

			V/=Mag;

		} 
}

/**************************************************************************************/
TVECTOR	TCrossProduct(TVECTOR const &V0,TVECTOR const &V1,const TVECTOR &V2 )
{
TVECTOR	DV1, DV2;
TVECTOR	Out;

		DV1.X() = V1.X() - V0.X();
		DV1.Y() = V1.Y() - V0.Y();
		DV1.Z() = V1.Z() - V0.Z();

		DV2.X() = V2.X() - V0.X();
		DV2.Y() = V2.Y() - V0.Y();
		DV2.Z() = V2.Z() - V0.Z();

		Out.SetX( (DV1.Z() * DV2.Y()) - (DV1.Y() * DV2.Z()) );
		Out.SetY( (DV1.X() * DV2.Z()) - (DV1.Z() * DV2.X()) );
		Out.SetZ( (DV1.Y() * DV2.X()) - (DV1.X() * DV2.Y()) );

		TNormalise(Out);
		return Out;
}

/**************************************************************************************/
CPoint	IDToPoint(int ID,int Width)
{
CPoint	XY;
		
		XY.x=ID%Width;
		XY.y=ID/Width;
		return(XY);
}

/**************************************************************************************/
int		PointToID(CPoint &Pnt,int Width)
{
int		ID;
		ID=(Pnt.y*Width)+Pnt.x;
		
		return(ID);
}

/**************************************************************************************/
/**************************************************************************************/
/**************************************************************************************/
#if	0
AUX_RGBImageRec *LoadBMP(char *Filename)
{
FILE	*File=NULL;

	File=fopen(Filename,"r");

	if (File)
	{
		fclose(File);
		return auxDIBImageLoad(Filename);
	}

	return NULL;
}

/**************************************************************************************/
void	FreeBMP(AUX_RGBImageRec *TextureImage)
{
	if (TextureImage)									// If Texture Exists
	{
		if (TextureImage->data)							// If Texture Image Exists
		{
			free(TextureImage->data);					// Free The Texture Image Memory
		}

		free(TextureImage);								// Free The Image Structure
	}

}

/**************************************************************************************/
int		LoadGLTexture(char *FileName, GLuint &Text,int &Width,int &Height)
{
AUX_RGBImageRec	*TextureImage;
int				Status=FALSE;

	memset(&TextureImage,0,sizeof(void *)*1);           // Init Buffer

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	if (TextureImage=LoadBMP(FileName))
	{
		Width=TextureImage->sizeX;
		Height=TextureImage->sizeY;
		Status=TRUE;									// Set The Status To TRUE

		glGenTextures(1, &Text);						// Create The Texture

		// Typical Texture Generation Using Data From The Bitmap
		glBindTexture(GL_TEXTURE_2D, Text);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage->sizeX, TextureImage->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	FreeBMP(TextureImage);

	return Status;										// Return The Status
}
#endif

/**************************************************************************************/
struct	sTgaHdr
{
	char	id;			   // 0
	char	colmaptype;	   // 1
	char	imagetype;	   // 2
	char	fei[2];		   // 3
	char	cml[2];		   // 5
	char	cmes;		   // 7
	short	xorig;		   // 8
	short	yorig;		   // 10
	short	width;		   // 12
	short	height;		   // 14
	char	depth;		   // 15
	char	imagedesc;	   // 16
};



void SaveTGA(char *Filename,int W,int H,char *Data)
{
FILE			*File;
sTgaHdr			FileHdr;

		File=fopen(Filename,"wb");
		
		memset(&FileHdr,0 ,sizeof(sTgaHdr));
	
		FileHdr.imagetype= 2;  //imagetype
		FileHdr.width = W;
		FileHdr.height= H;
		FileHdr.depth=24;

		fwrite(&FileHdr,sizeof(sTgaHdr),1,File);

		fwrite(Data,W*H*3,1,File);

		fclose(File);


}

/**************************************************************************************/
void	BGR2RGB(int W,int H,char *Data)
{
		for (int Y=0; Y<H; Y++)
			{
			for (int X=0; X<W; X++)
				{
				unsigned char	c0,c1;
				c0=Data[0];
				c1=Data[2];
				Data[0]=c1;
				Data[2]=c0;
				Data+=3;
				}
			}
}