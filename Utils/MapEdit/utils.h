/*************/
/*** Utils ***/
/*************/

#ifndef	__UTILS_HEADER__
#define	__UTILS_HEADER__

#include	<vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>

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
class	GString;
class	CMapEditDoc;
void	DbgMsg(const char * pszFmt,...);

int		AlignSize(int Size);

void	BuildGLBox(float XMin,float XMax,float YMin,float YMax,float ZMin,float ZMax);
void	BuildGLBoxNoNormals(float XMin,float XMax,float YMin,float YMax,float ZMin,float ZMax);
void	BuildGLQuad(float XMin,float XMax,float YMin,float YMax,float Z);

CPoint	IDToPoint(int ID,int Width);
int		PointToID(CPoint &Pnt,int Width);

//void	SaveTGA(const char *Filename,int W,int H,u8 *Data);
void SaveTGA(const char *Filename,int W,int H,u8 *Data,bool IsBGR=false);

void	SaveBmp(char *Filename,int Width,int Height,RGBQUAD *Pal,u8 *Image);

void	BGR2RGB(int W,int H,u8 *Data);

void	SetFileExt(const char *InName,char *OutName,char *Ext);

void	MakeFullFilename(const char *RelName,GString &Out);
void	MakePathRel2App(const char* In,char *Out);
GString	GetWorkingPath();
void	CheckFilename(GString &Filename);
int		round(float f) ;

void	*_MemAlloc(size_t Size);
void	_MemFree(void *Ptr);
void	CheckMem();

//#ifdef _DEBUG
//#define	MemAlloc(v)	_MemAlloc(v)
//#define	MemFree(v)	_MemFree(v)
//#else
#define	MemAlloc(v)	malloc(v)
#define	MemFree(v)	free(v)
//#endif

void	GetExecPath(GString &Path);


#endif