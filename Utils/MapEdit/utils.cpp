/*************/
/*** Utils ***/
/*************/

#include	"stdafx.h"

#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	"GLEnabledView.h"

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
			glVertex3f( XMin, YMin, Z);
			glVertex3f( XMax, YMin, Z);
			glVertex3f( XMax, YMax, Z);
			glVertex3f( XMin, YMax, Z);
}

/**************************************************************************************/
/**************************************************************************************/
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



void SaveTGA(char *Filename,int W,int H,u8 *Data)
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
void	BGR2RGB(int W,int H,u8 *Data)
{
		for (int Y=0; Y<H; Y++)
			{
			for (int X=0; X<W; X++)
				{
				u8	c0,c1;
				c0=Data[0];
				c1=Data[2];
				Data[0]=c1;
				Data[2]=c0;
				Data+=3;
				}
			}
}

/**************************************************************************************/
void SaveBmp(char *Filename,int Width,int Height,RGBQUAD *Pal,u8 *Image)
{
FILE				*File;
BITMAPFILEHEADER	FileHdr;
BITMAPINFOHEADER	ImageHdr;
int					PaletteSize,ImageSize;

		File=fopen(Filename,"wb");
		
		if (!Pal)
		{
			PaletteSize=0;
			ImageSize=Width*Height*3;
			ImageHdr.biBitCount=24;
		}
		else
		{
			PaletteSize=256*sizeof(RGBQUAD);
			ImageSize=Width*Height;
			ImageHdr.biBitCount=8;

		}

		FileHdr.bfType=19778;
		FileHdr.bfSize=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+PaletteSize+ImageSize;
		FileHdr.bfReserved1=0;
		FileHdr.bfReserved2=0;
		FileHdr.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+PaletteSize;

		ImageHdr.biSize=sizeof(BITMAPINFOHEADER);
		ImageHdr.biWidth=Width;
		ImageHdr.biHeight=Height;
		ImageHdr.biPlanes=1;
// Set Above		ImageHdr.biBitCount=8;	// 24
		ImageHdr.biCompression=BI_RGB;
		ImageHdr.biSizeImage=0;
		ImageHdr.biXPelsPerMeter=0;
		ImageHdr.biYPelsPerMeter=0;
		ImageHdr.biClrUsed=0;
		ImageHdr.biClrImportant=0;
 
		fwrite(&FileHdr,sizeof(BITMAPFILEHEADER),1,File);
		fwrite(&ImageHdr,sizeof(BITMAPINFOHEADER),1,File);

		if (Pal) fwrite(Pal,sizeof(RGBQUAD),256,File);
		
		fwrite(Image,ImageSize,1,File);
		fclose(File);


}

/**************************************************************************************/
void	SetFileExt(char *InName,char *OutName,char *Ext)
{
char	Drive[_MAX_DRIVE];
char	Path[_MAX_DIR];
char	Name[_MAX_FNAME];

		_splitpath(InName,Drive,Path,Name,0);
		sprintf(OutName,"%s%s%s.%s",Drive,Path,Name,Ext);
}

