/*=========================================================================

	MISC.HPP

	Author:  Gary Liddon @ Watford
	Created: 4th May 1991
	Purpose: Shitey misc stuff

	Copyright (c) 1991 - 1997 Gary Liddon

===========================================================================*/

#ifndef __PC_GLIB_MISC_HPP__
#define __PC_GLIB_MISC_HPP__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <list>
#include <fstream>

/*	Glib
	---- */
#include "gtypes.h"

/*	Local
	----- */
#include "gobject.hpp"
#include "gstring.hpp"
#include "gutils.h"

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Class defintions
	---------------- */
class GLIB_API CommandLine : public GObject
{
public:
	CommandLine(int argc,char **argv,char *(*Func)(char *String,int Num));
	~CommandLine();
	char *GetNextItem();

protected:
	std::ifstream *InStream;
	char *InStreamFile;

	char *GetNextScriptFileItem();
	char *GetNextCommandLineItem();

private:
	int CommandItem;
	int MyArgc;
	char **MyArgv;
	char *LastItem;
};

class GLIB_API Gofstream : public std::ofstream
{
public:
	enum ENDIAN
		{
		BIG_ENDIAN,
		LITTLE_ENDIAN,
		};

	Gofstream(ENDIAN NewEndian=BIG_ENDIAN)
		{SetEndian(NewEndian);}

	void	SetEndian(ENDIAN NewEndian)
			{Endian=NewEndian;}

	void Put16(u16 PVal)
		{
		switch(Endian)
			{
			case BIG_ENDIAN:
				put(u8(PVal>>8));
				put(u8(PVal&0xff));
				break;

			case LITTLE_ENDIAN:
				put(u8(PVal&0xff));
				put(u8(PVal>>8));
				break;
			}
		}

	void Put32(u32 PVal)
		{
		switch(Endian)
			{
			case BIG_ENDIAN:
				put(u8(PVal>>24));
				put(u8(PVal>>16));
				put(u8(PVal>>8));
				put(u8(PVal>>0));
				break;

			case LITTLE_ENDIAN:
				put(u8(PVal>>0));
				put(u8(PVal>>8));
				put(u8(PVal>>16));
				put(u8(PVal>>24));
				break;
			}
		}

	int Align(int Alignment,char const * Pad=NULL)
		{
		int		NewPos;

		int CurrPos;

		CurrPos=tellp();

		if (Alignment && CurrPos)
			{
			NewPos=CurrPos+((CurrPos%Alignment) == 0 ? 0 : Alignment-(CurrPos%Alignment));

			if (NewPos != CurrPos)
				{
				if (Pad)
					{
					int StrSize;

					StrSize=strlen(Pad);

					for (;CurrPos<NewPos;CurrPos++)
						put((u8)Pad[CurrPos%StrSize]);
					}
				else
					{
					for (;CurrPos<NewPos;CurrPos++)
						put((u8)0);
					}
				}
			}
		else
			NewPos=0;

		return NewPos;
		}

protected:
	ENDIAN	Endian;
};


class GLIB_API Gifstream : public std::ifstream
{
public:
	enum ENDIAN
		{
		BIG_ENDIAN,
		LITTLE_ENDIAN,
		};

		Gifstream(ENDIAN NewEndian=BIG_ENDIAN)
			{
			SetEndian(NewEndian);
			}

	void	SetEndian(ENDIAN NewEndian)
			{
			Endian=NewEndian;
			}

	u16		Get16(void)
		{
		u8	Byte0,Byte1;

		switch(Endian)
			{
			case BIG_ENDIAN:
				Byte1=get();
				Byte0=get();
				break;

			case LITTLE_ENDIAN:
				Byte0=get();
				Byte1=get();
				break;
			}

		return(u16(Byte0)|(u16(Byte1)<<8));
		}

	u32		Get32(void)
		{
		u8	Byte0,Byte1,Byte2,Byte3;

		switch(Endian)
			{
			case BIG_ENDIAN:
				Byte3=get();
				Byte2=get();
				Byte1=get();
				Byte0=get();
				break;

			case LITTLE_ENDIAN:
				Byte0=get();
				Byte1=get();
				Byte2=get();
				Byte3=get();
				break;
			}

		return(u32(Byte0)|(u32(Byte1)<<8)|(u32(Byte2)<<16)|(u32(Byte3)<<24));
		}


	void Align(int Alignment)
		{

		int CurrPos;

		CurrPos=tellg();

		if (Alignment && CurrPos)
			{
			int		NewPos;

			NewPos=GU_AlignVal(CurrPos,Alignment);

			seekg(NewPos);
			}
		}


protected:

	ENDIAN	Endian;
};


class FileCycler
{
public:
	virtual int DoCycle(char const *Spec,bool Recurse);

protected:
	virtual void FileCallback(char const * FName,int FileNum)
		{
		}
};

class FileToStrList : FileCycler
{
public:
	FileToStrList(char const * FileSpec,bool Recurse,std::list<GString> & NewList) : List(NewList)
		{
		DoCycle(FileSpec,Recurse);
		}

protected:

	virtual void FileCallback(char const * FName,int FileNum)
		{
		List.push_front(FName);
		}

	std::list<GString> & List;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*	Vars
	---- */

/*	Data
	---- */

/*	Functions
	--------- */
GLIB_API int	CycleFiles(char const *Spec,void (*Func)(char const *Fname,int Num),BOOL Recurse);
GLIB_API BOOL	SwitchInfo(char *String);
GLIB_API bool	FileExists(const char *String);
GLIB_API int	FileSize(const char *String);

GLIB_API bool copyFile(char const * Dest,char const * Source,bool Overwrite = false);

/*---------------------------------------------------------------------- */

#endif	/* __PC_GLIB_MISC_HPP__ */

/*===========================================================================
 end */
