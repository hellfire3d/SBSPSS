/*************/
/*** Utils ***/
/*************/

#ifndef	__UTILS_HEADER__
#define	__UTILS_HEADER__

#include	"maths.h"
//#include	<gl\gl.h>
//#include	<gl\glu.h>
//#include	<gl\glut.h>
//#include	<gl\glaux.h>		// Header File For The Glaux Library


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
void SaveTGA(char *Filename,int W,int H,char *Data);
void BGR2RGB(int W,int H,char *Data);
		


/**************************************************************************************/

#endif