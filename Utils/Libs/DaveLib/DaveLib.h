/*************************/
/*** Dave Lib Routines ***/
/*************************/

#ifndef	__DAVE_LIB_HEADER__
#define	__DAVE_LIB_HEADER__

#include "stdio.h"
#include <misc.hpp>
#include <gfname.hpp>
#include "psxtypes.h"
#include "FaceStore.h"
#include "..\..\..\tools\data\include\dStructs.h"

#include "psxtypes.h"

//***************************************************************************
const float ONE_DEGREE          = 0.0174532925199432957692369076848861271344f;
const float INV_ONE_DEGREE      = 1/ONE_DEGREE;

inline	float DEG2RAD (float dval)		{return(dval*ONE_DEGREE);}
inline	float RAD2DEG (float dval)		{return(dval*(INV_ONE_DEGREE));}

//***************************************************************************
class FileCycle : protected FileCycler
{
public:
	void	AddFile(const char * Name)	
		{
		DoCycle(Name,false);
		}

	std::vector<GString> const & GetFileInfoVector(void) const		{return(AllFileInfos);}

protected:
	void	FileCallback(char const * FName,int FileNum)
		{
		AllFileInfos.resize(AllFileInfos.size()+1);
		AllFileInfos[AllFileInfos.size()-1]=GString(FName);
		}	

	std::vector<GString>	AllFileInfos;

} ;

#define	ASSERT(x)	if (!(x))  GObject::Error(ERR_FATAL,"%s %s %i\n",#x,__FILE__,__LINE__);


//***************************************************************************
GString CheckFileString(const char * Str);
bool	CheckPlusMinus(const char * Str);
int		ZeroAndCountCommas(char * Str);

//***************************************************************************
extern GString		OutStr,IncludeFile;
extern bool			DebugOn;
extern FileCycle	MyFiles;
extern float		Scale;
extern int			StripLength;

//***************************************************************************
void	DebugStr2(char *Txt,int Param);
void	DebugStr(char *Txt,int Param);
void	DebugStr(char *Txt);
void	DebugStr();

int		round(float f);

//void	alignFile( FILE *f, int align );
void    PadFile(FILE *File);


char	*FindFile(const char *Name);
void	SaveTGA(char *Filename,int W,int H,u8 *Data,bool IsBGR=false);
Vector3 CalcNormal(Vector3 const &v0, Vector3 const &v1, Vector3 const &v2 );

//GString  ReplaceBadFileChars(GString s);

//***************************************************************************
#endif