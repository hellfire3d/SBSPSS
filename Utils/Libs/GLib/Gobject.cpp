/*=========================================================================

	GOBJECT.CPP

	Author:  Gary Liddon @ Watford
	Created: 4th May 1991
	Purpose: Base object

	Copyright (c) 1991 - 1997 Gary Liddon

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */
#include <stdlib.h>
#include <stdio.h>
#include <fstream.h>
#include <stdarg.h>

/*	Local
	----- */
#include "gobject.hpp"

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Positional Vars
	--------------- */

/*----------------------------------------------------------------------
	Static Vars
	----------- */
int GObject::NumOfErrors=0;
int GObject::MaxErrors=3;
int GObject::NumOfGobjs=0;
int GObject::NumOfWarnings=0;

unsigned int GObject::BigObjFlags=0;

/*----------------------------------------------------------------------
	Data
	---- */
char const * const GObject::ErrorText[]=
{
	"Too Many Errors",
	"Out of Memory",
};

/*----------------------------------------------------------------------
	Function:	Gobject Constructor
  ---------------------------------------------------------------------- */
GObject::GObject()
{
	NumOfGobjs++;
}

/*----------------------------------------------------------------------
	Function:	Gobject Destructor
  ---------------------------------------------------------------------- */
GObject::~GObject()
{
	NumOfGobjs--;
}


/*----------------------------------------------------------------------
	Function:	Flag a Gobject error
  ---------------------------------------------------------------------- */
void GObject::Error(int ErrNum)
{
	cerr<<"Internal GObject Error\n";
	Error(ERR_FATAL,ErrorText[ErrNum]);
}

/*----------------------------------------------------------------------
	Function:	Flag General purpose error
  ---------------------------------------------------------------------- */
void GObject::Error(int Etype,char const *e, ...)
{
	if (e)
		{
		switch (Etype)
			{
			case ERR_FATAL:
				cerr<<"Fatal Error: ";
				break;
			case ERR_SERIOUS:
				cerr<<"Serious Error: ";
				break;
			case ERR_WARNING:
				NumOfWarnings++;
				cerr<<"Warning: ";
				break;
			}
		
		va_list argptr;
		va_start(argptr,e);
		vprintf(e,argptr);
		va_end(argptr);
		}

	switch (Etype)
		{
		case ERR_FATAL:
			exit(10);
			break;
		case ERR_SERIOUS:
			if (NumOfErrors==MaxErrors)
				Error(ERM_TOOMANYERRORS);
			else
				NumOfErrors++;
		}

}



/*===========================================================================
 end */




