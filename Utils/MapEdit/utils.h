/*************/
/*** Utils ***/
/*************/

#ifndef	__UTILS_HEADER__
#define	__UTILS_HEADER__

#include	"maths.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	<gl\glaux.h>		// Header File For The Glaux Library

/**************************************************************************************/
typedef signed char				s8;
typedef signed short            s16;
typedef signed long				s32;
//typedef long long				s64;

typedef unsigned char			u8;
typedef unsigned short			u16;
typedef unsigned long			u32;
//typedef unsigned long long		u64;

typedef s8						S8;
typedef s16						S16;
typedef s32						S32;
//typedef s64						S64;

typedef u8						U8;
typedef u16						U16;
typedef u32						U32;
//typedef u64						U64;

/**************************************************************************************/
void	DbgMsg(const char * pszFmt,...);

void	BuildGLBox(float XMin,float XMax,float YMin,float YMax,float ZMin,float ZMax);
void	BuildGLBoxNoNormals(float XMin,float XMax,float YMin,float YMax,float ZMin,float ZMax);
void	BuildGLQuad(float XMin,float XMax,float YMin,float YMax,float Z);
int		LoadGLTexture(char *FileName, GLuint &Text,int &Width,int &Height);

void	TNormalise(TVECTOR &V);
TVECTOR	TCrossProduct(TVECTOR const &V0,TVECTOR const &V1,const TVECTOR &V2 );

CPoint	IDToPoint(int ID,int Width);
int		PointToID(CPoint &Pnt,int Width);

//AUX_RGBImageRec *LoadBMP(char *Filename);
//void	FreeBMP(AUX_RGBImageRec *TextureImage);


//void	SaveTGA(char *Filename,int SX,int SY,int SW,int SH);
void SaveTGA(char *Filename,int W,int H,u8 *Data);
void SaveBmp(char *Filename,int Width,int Height,RGBQUAD *Pal,u8 *Image);

void BGR2RGB(int W,int H,u8 *Data);

void	SetFileExt(char *InName,char *OutName,char *Ext);
		
#endif