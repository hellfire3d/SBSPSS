/*=========================================================================

	script.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef	__PAUL_SCRIPT_H__
#define	__PAUL_SCRIPT_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

#ifndef _FILEIO_HEADER_
#include "fileio\fileio.h"
#endif


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CScript
{
public:
	void	initialise(FileEquate _fe);

	void	reset();
	void	run();

	int		isFinished();

private:
	// Global vars
	enum
	{
		LIVES,
		NUM_GLOBAL_VARS,
	};
	
	// Local vars
	enum
	{
		TMP1,
		TMP2,
		TMP3,
		NUM_LOCAL_VARS,
	};

	typedef enum
	{
		RESET,
		PAUSED,
		RUNNING,

		STOPPED,

		STOPPED_STACK_NOT_EMPTY,
		CRASHED_ILLEGAL_OPCODE,
	}ScriptState;

	enum
	{
		STACK_SIZE=20,
	};

	enum
	{
								//	args				stack data				result
		OP_NOP=0x1100,			//
		OP_STOP,				//
		OP_PAUSE,
		OP_PUSHVALUE,			//	value
		OP_PUSHVARVALUE,		//	varidx
		OP_JMP,					//						jump
		OP_JMPF,				//						jump, value
		OP_JMPT,				//						jump, value
		OP_IS_EQUAL_VALUE,		//						value, value			pushes result ( 0 or 1 ) to stack
		OP_IS_NOTEQUAL_VALUE,	//						value, value			pushes result ( 0 or 1 ) to stack
		OP_ASSIGN,				//						varidx, value
		OP_ADD,					//						value, value			pushes result to stack
		OP_PRINT,				//						value
	};


	void			executeNextInstruction();
	unsigned short	readNextInstruction();
	void			jump(signed short _distance);
	void			push(unsigned short _data);
	unsigned short	pop();
	void			setVar(int _varIdx,signed short _value);
	signed short	getVar(int _varIdx);


	unsigned short	*m_code;
	int				m_pc;

	unsigned short	*m_stack;
	int				m_sp;

	static signed short		s_globalVars[NUM_GLOBAL_VARS];
	signed short			m_localVars[NUM_LOCAL_VARS];

	ScriptState		m_state;
};


/*----------------------------------------------------------------------
	Globals
	------- */

/*----------------------------------------------------------------------
	Functions
	--------- */

/*---------------------------------------------------------------------- */

#endif	/* __PAUL_SCRIPT_H__ */

/*===========================================================================
 end */
