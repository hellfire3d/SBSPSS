/*=========================================================================

	script.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "script\script.h"

#ifndef __SCRIPT_FUNCTION_H__
#include "script\function.h"
#endif

#ifndef	__SYSTEM_DBG_H__
#include "system\dbg.h"
#endif

#ifndef	__MEMORY_HEADER__
#include "mem\memory.h"
#endif


/*	Std Lib
	------- */

/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

//#define FULL_CODE_OUTPUT
//#define SHOW_RUN_COUNT


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

signed short	CScript::s_globalVars[NUM_GLOBAL_VARS]=
{
	0,			// LIVES
};


// Buffer for passing arguments to functions
unsigned short	CScript::s_argBuffer[MAX_FUNCTION_ARGS];


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CScript::initialise(FileEquate _fe)
{
	int	i;

	m_code=(unsigned short*)CFileIO::loadFile(_fe);
	m_stack=(unsigned short*)MemAlloc(sizeof(unsigned short)*STACK_SIZE,"ScriptStack");
	reset();
	for(i=0;i<NUM_LOCAL_VARS;i++)
	{
		m_localVars[i]=0;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CScript::dump()
{
	MemFree(m_stack);
	MemFree(m_code);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CScript::run()
{
#ifdef SHOW_RUN_COUNT
	int run=0;
#endif
	if(m_state<=RUNNING)
	{
		m_state=RUNNING;
		do
		{
			executeNextInstruction();
#ifdef SHOW_RUN_COUNT
			run++;			
#endif
		}
		while(m_state==RUNNING);
	}
#ifdef SHOW_RUN_COUNT
	if(run)
	{
		PAUL_DBGMSG("ran %d instructions",run);
	}
#endif
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CScript::reset()
{
	m_pc=0;
	m_sp=0;
	m_state=RESET;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CScript::isFinished()
{
	return !(m_state<=RUNNING);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CScript::executeNextInstruction()
{
	unsigned short	instruction;
	signed short	val1,val2,val3;
	int				i;

#ifdef FULL_CODE_OUTPUT
PAUL_DBGMSG("pc:0x%04d  sp:%03d",m_pc*2,m_sp);
#endif
	instruction=readNextInstruction();
	switch(instruction)
	{
		case OP_NOP:
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("NOP");
#endif
			break;

		case OP_STOP:				//
			PAUL_DBGMSG("STOP");
			if(m_sp==0)
			{
				m_state=STOPPED;
			}
			else
			{
				PAUL_DBGMSG("!STACK NOT EMPTY!");
				m_state=STOPPED_STACK_NOT_EMPTY;
			}
			break;
			
		case OP_PAUSE:				//
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("PAUSE");
#endif
			m_state=PAUSED;
			break;

		case OP_PUSHVALUE:			//	value
			val1=readNextInstruction();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("PUSHVALUE %d",val1);
#endif
			push(val1);
			break;
			
		case OP_PUSHVARVALUE:		//	varidx
			val1=readNextInstruction();
			val2=getVar(val1);
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("PUSHVARVALUE $%d ( %d )",val1,val2);
#endif
			push(val2);
			break;

		case OP_POP:				//								value
			val1=pop();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("POP %d",val1);
#endif
			break;

		case OP_JMP:				//								jump
			val1=pop();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("JMP %d",val1);
#endif
			jump(val1);
			break;
			
		case OP_JMPF:				//								jump, value
			val1=pop();
			val2=pop();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("JMPF %d,%d",val1,val2);
#endif
			if(val2==0)jump(val1);
			break;

		case OP_JMPT:				//								jump, value
			val1=pop();
			val2=pop();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("JMPT %d,%d",val1,val2);
#endif
			if(val2!=0)jump(val1);
			break;

		case OP_IS_EQUAL_VALUE:		//								value, value			pushes result ( 0 or 1 ) to stack
			val1=pop();
			val2=pop();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("IS_EQUAL_VALUE %d,%d",val1,val2);
#endif
			push(val1==val2);
			break;

		case OP_IS_NOTEQUAL_VALUE:	//								value, value			pushes result ( 0 or 1 ) to stack
			val1=pop();
			val2=pop();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("IS_NOTEQUAL_VALUE %d,%d",val1,val2);
#endif
			push(val1!=val2);
			break;

		case OP_IS_LESSTHAN_VALUE:	//								value, value			pushes result ( 0 or 1 ) to stack
			val1=pop();
			val2=pop();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("OP_IS_LESSTHAN_VALUE %d,%d",val1,val2);
#endif
			push(val1<val2);
			break;
			
		case OP_IS_GREATERTHAN_VALUE://								value, value			pushes result ( 0 or 1 ) to stack
			val1=pop();
			val2=pop();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("OP_IS_GREATERTHAN_VALUE %d,%d",val1,val2);
#endif
			push(val1>val2);
			break;
			
		case OP_ASSIGN:				//								varidx, value
			val1=pop();
			val2=pop();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("$%d=%d",val1,val2);
#endif
			setVar(val1,val2);
			break;

		case OP_ADD:				//								value, value			pushes result to stack
			val1=pop();
			val2=pop();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("ADD %d,%d",val1,val2);
#endif
			push(val1+val2);
			break;

		case OP_MULTIPLY:			//								value, value			pushes result to stack
			val1=pop();
			val2=pop();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("MULTIPLY %d,%d",val1,val2);
#endif
			push(val1*val2);
			break;

		case OP_DIVIDE:				//								value, value			pushes result to stack
			val1=pop();
			val2=pop();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("DIVIDE %d,%d",val1,val2);
#endif
			if(val2==0)
			{
				SYSTEM_DBGMSG("[SCRIPT] DIVIDE BY ZERO @%d",m_pc);
				m_state=CRASHED_DIVIDE_BY_ZERO;
			}
			else
			{
				push(val1/val2);
			}
			break;

		case OP_NEG:				//								value					pushes result to stack
			val1=pop();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("NEG %d",val1);
#endif
			push(-val1);
			break;

		case OP_PRINT:				//								value
			val1=pop();
			PAUL_DBGMSG("PRINT %d",val1);
			break;

		case OP_CALL_FUNCTION:		//	functionnumber, argcount	args					pushes return value to stack
			val1=readNextInstruction();
			val2=readNextInstruction();
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("CALL_FUNCTION %d ( %d args )",val1,val2);
#endif
			ASSERT(val2<MAX_FUNCTION_ARGS);		// Too many args.. just increase the #define to fix this
			for(i=0;i<val2;i++)
			{
				s_argBuffer[val2-i-1]=pop();
			}
			val3=callFunction(val1,val2,s_argBuffer);
#ifdef FULL_CODE_OUTPUT
			PAUL_DBGMSG("( return value is %d )",val3);
#endif
			push(val3);
			break;

		default:
			SYSTEM_DBGMSG("[SCRIPT] ILLEGAL OPCODE@%d ( %d )",m_pc,instruction);
			m_state=CRASHED_ILLEGAL_OPCODE;
			break;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
unsigned short CScript::readNextInstruction()
{
	return m_code[m_pc++];
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CScript::jump(signed short _distance)
{
	m_pc+=_distance;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CScript::push(unsigned short _data)
{
	ASSERT(m_sp<=STACK_SIZE-1);		// Stack overflow about to occur :(
	m_stack[m_sp++]=_data;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
unsigned short CScript::pop()
{
	ASSERT(m_sp>=1);				// Stack underflow about to occur :(
	return(m_stack[--m_sp]);
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
void CScript::setVar(int _varIdx,signed short _value)
{
	ASSERT(_varIdx>=0&&_varIdx<=NUM_GLOBAL_VARS+NUM_LOCAL_VARS-1);

	if(_varIdx<NUM_GLOBAL_VARS)
	{
		ASSERT(0);		// Need to update global vars.. (PKG)
		s_globalVars[_varIdx]=_value;
	}
	else if(_varIdx<NUM_GLOBAL_VARS+NUM_LOCAL_VARS)
	{
		m_localVars[_varIdx-NUM_GLOBAL_VARS]=_value;
	}
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
signed short CScript::getVar(int _varIdx)
{
	ASSERT(_varIdx>=0&&_varIdx<=NUM_GLOBAL_VARS+NUM_LOCAL_VARS-1);

	int	ret=0;
	
	if(_varIdx<NUM_GLOBAL_VARS)
	{
		ASSERT(0);		// Need to read global vars.. (PKG)
		ret=s_globalVars[_varIdx];
	}
	else if(_varIdx<NUM_GLOBAL_VARS+NUM_LOCAL_VARS)
	{
		ret=m_localVars[_varIdx-NUM_GLOBAL_VARS];
	}
	return ret;
}


/*===========================================================================
 end */
