/*************/
/*** Utils ***/
/*************/

#ifndef	__UTILS_HEADER__
#define	__UTILS_HEADER__


/**************************************************************************************/
void	DbgMsg(const char * pszFmt,...);
void	BuildGLBox(float XMin,float XMax,float YMin,float YMax,float ZMin,float ZMax);
void	BuildGLBoxNoNormals(float XMin,float XMax,float YMin,float YMax,float ZMin,float ZMax);
void	BuildGLQuad(float XMin,float XMax,float YMin,float YMax,float Z);

/**************************************************************************************/

#endif