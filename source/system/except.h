/*=========================================================================

	except.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __SYSTEM_EXCEPT_H__
#define __SYSTEM_EXCEPT_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

#ifdef __USER_CDBUILD__
#define __INSTALL_EXCEPTION_HANDLER__
#endif


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Globals
	------- */

extern int *reg_lst;
extern int dev_kit;


/*----------------------------------------------------------------------
	Functions
	--------- */
extern void installExceptionHandler();

extern "C" void exc_c(void);
extern "C" void install_exc(void);
extern "C" void uninstall_exc(void);


/*---------------------------------------------------------------------- */

#endif	/* __SYSTEM_EXCEPT_H__ */

/*===========================================================================
 end */
