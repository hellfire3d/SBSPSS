/*=========================================================================

	codegen.cpp

	Author:		PKG
	Created: 
	Project:	Spongebob
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "codegen.h"

#ifndef _LEXER_H
#include "lexer.h"
#endif

#ifndef _PARSER_H
#include "parser.h"
#endif

#ifndef __FUNCTION_H__
#include "function.h"
#endif


/*	Std Lib
	------- */

#include <stdio.h>
#include <stdlib.h>


/*	Data
	---- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

static FILE	*s_fhOutput=NULL;

CTreeNode	*s_baseTreeNode=NULL;

mylexer		s_lexer;
myparser	s_parser;



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern int parseFile(char *_filename,CTreeNode *_baseNode)
{
	int		ret;

	ret=YYEXIT_FAILURE;
	if(s_parser.yycreate(&s_lexer))
	{
		if(s_lexer.yycreate(&s_parser))
		{
			if(s_lexer.openInputFile(_filename)==(int)true)
			{
				s_parser.setCurrentLexer(&s_lexer);
				s_parser.setBaseNode(_baseNode);
				ret=s_parser.yyparse();
//				s_lexer.closeInputFile();
			}
		}
	}

	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern int openOutputFile(char *_filename)
{
	int	ret=1;

	if((s_fhOutput=fopen(_filename,"wb"))==NULL)
	{
		ret=0;
	}
	return ret;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
extern int closeOutputFile()
{
	fclose(s_fhOutput);
	return 0;
}








/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
CTreeNode::CTreeNode(NodeType _type)
{
	m_type=_type;
	m_children[0]=NULL;
	m_children[1]=NULL;
	m_children[2]=NULL;
	m_numChildren=0;
}
CTreeNode::CTreeNode(NodeType _type,CTreeNode *child1)
{
	m_type=_type;
	m_children[0]=child1;
	m_children[1]=NULL;
	m_children[2]=NULL;
	m_numChildren=1;
}
CTreeNode::CTreeNode(NodeType _type,CTreeNode *child1,CTreeNode *child2)
{
	m_type=_type;
	m_children[0]=child1;
	m_children[1]=child2;
	m_children[2]=NULL;
	m_numChildren=2;
}
CTreeNode::CTreeNode(NodeType _type,CTreeNode *child1,CTreeNode *child2,CTreeNode *child3)
{
	m_type=_type;
	m_children[0]=child1;
	m_children[1]=child2;
	m_children[2]=child3;
	m_numChildren=3;
}
CTreeNode::CTreeNode(NodeType _type,int _data)
{
	m_type=_type;
	m_children[0]=NULL;
	m_children[1]=NULL;
	m_children[2]=NULL;
	m_numChildren=0;
	switch(_type)
	{
		case VARIABLE_EXPR:
			m_variableIdx=_data;
			break;
		case VALUE_EXPR:
			m_value=_data;
			break;
		case FUNCTION_EXPR:
			m_functionNumber=_data;
			break;
		default:
			printf("ARSE\n");
			break;
	}
}



/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CTreeNode::generateCode(int _write)
{
	int	codeSize=0;
	int	tmp1,tmp2;
	
	switch(m_type)
	{
		case STMT_LIST:			// list of statements [left-part, right-part] (left-part may be another list)
			if(m_children[0])codeSize+=m_children[0]->generateCode(_write);
			if(m_children[1])codeSize+=m_children[1]->generateCode(_write);
			break;
			
		case EMPTY_STMT:		// empty statement
			break;
			
		case PRINT_STMT:		// print [variable]
			codeSize+=m_children[0]->generateCode(_write);
			codeSize+=emit(OP_PRINT,_write);
			break;
			
		case STOP_STMT:			// stop
			codeSize+=emit(OP_STOP,_write);
			break;
			
		case PAUSE_STMT:		// pause
			codeSize+=emit(OP_PAUSE,_write);
			break;
			
		case IF_STMT:			// if [expression, ifcode]
			codeSize+=m_children[0]->generateCode(_write);
			codeSize+=emit(OP_PUSHVALUE,_write);
			codeSize+=emit((signed short)m_children[1]->generateCode(false),_write);
			codeSize+=emit(OP_JMPF,_write);
			codeSize+=m_children[1]->generateCode(_write);
			break;
			
		case IFELSE_STMT:		// if [expression, ifcode, elsecode]
			codeSize+=m_children[0]->generateCode(_write);
			codeSize+=emit(OP_PUSHVALUE,_write);
			codeSize+=emit((signed short)(m_children[1]->generateCode(false)+emit(OP_PUSHVALUE,false)+emit(0,false)+emit(OP_JMP,false)),_write);
			codeSize+=emit(OP_JMPF,_write);
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=emit(OP_PUSHVALUE,_write);
			codeSize+=emit((signed short)m_children[2]->generateCode(false),_write);
			codeSize+=emit(OP_JMP,_write);
			codeSize+=m_children[2]->generateCode(_write);
			break;

		case WHILE_STMT:		// while [expression, code]
			tmp1=m_children[1]->generateCode(false);
			tmp2=codeSize;
			codeSize=m_children[0]->generateCode(_write);
			codeSize+=emit(OP_PUSHVALUE,_write);
			codeSize+=emit((signed short)tmp1+3,_write);
			codeSize+=emit(OP_JMPF,_write);
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=emit(OP_PUSHVALUE,_write);
			codeSize+=emit((signed short)-(codeSize-tmp2+2),_write);
			codeSize+=emit(OP_JMP,_write);
			break;

		case DOWHILE_STMT:		// do [code] while [expression]
			tmp1=m_children[0]->generateCode(false)+m_children[1]->generateCode(false)+3;
			codeSize=m_children[0]->generateCode(_write);
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=emit(OP_PUSHVALUE,_write);
			codeSize+=emit((signed short)-tmp1,_write);
			codeSize+=emit(OP_JMPT,_write);
			break;
			
		case POP_STMT:			// pop
			codeSize+=emit(OP_POP,_write);
			break;

		case ASSIGN_EXPR:		// assign [variable, number]
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=emit(OP_PUSHVALUE,_write);
			codeSize+=emit(m_children[0]->getVariableIdx(),_write);
			codeSize+=emit(OP_ASSIGN,_write);
			break;
			
		case EQUAL_EXPR:		// == [variable, value]
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=m_children[0]->generateCode(_write);
			codeSize+=emit(OP_IS_EQUAL_VALUE,_write);
			break;
			
		case NOTEQUAL_EXPR:		// != [variable, value]
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=m_children[0]->generateCode(_write);
			codeSize+=emit(OP_IS_NOTEQUAL_VALUE,_write);
			break;

		case LESSTHAN_EXPR:		//  [variable, value]
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=m_children[0]->generateCode(_write);
			codeSize+=emit(OP_IS_LESSTHAN_VALUE,_write);
			break;

		case GREATERTHAN_EXPR:	//  [variable, value]
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=m_children[0]->generateCode(_write);
			codeSize+=emit(OP_IS_GREATERTHAN_VALUE,_write);
			break;
			
		case VARIABLE_EXPR:		// variable
		case VALUE_EXPR:		// value
			if(m_numChildren)
				codeSize+=emitValue(m_children[0],_write);
			else
				codeSize+=emitValue(this,_write);
			break;

		case PLUS_EXPR:			// + [value, value]
			codeSize+=m_children[0]->generateCode(_write);
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=emit(OP_ADD,_write);
			break;
			
		case SUBTRACT_EXPR:		// - [value, value]
			codeSize+=m_children[0]->generateCode(_write);
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=emit(OP_NEG,_write);
			codeSize+=emit(OP_ADD,_write);
			break;
			
		case MULTIPLY_EXPR:		// * [value, value]
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=m_children[0]->generateCode(_write);
			codeSize+=emit(OP_MULTIPLY,_write);
			break;
			
		case DIVIDE_EXPR:		// / [value, value]
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=m_children[0]->generateCode(_write);
			codeSize+=emit(OP_DIVIDE,_write);
			break;
			
		case FUNCTION_EXPR:		// function [functionNumber]
			codeSize+=emit(OP_CALL_FUNCTION,_write);
			codeSize+=emit(getFunctionNumber(),_write);
			codeSize+=emit(getFunctionArgCount(getFunctionNumber()),_write);
			break;

		default:
			printf("UNHANDLED CASE %d\n",m_type);
			exit(-2);
			break;
	}
	
	return codeSize;
}


/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int CTreeNode::emit(unsigned short _data,int _write)
{
	if(_write)
	{
		fwrite(&_data,sizeof(unsigned short),1,s_fhOutput);
	}
	return 1;
}
int CTreeNode::emitValue(CTreeNode *_child,int _write)
{
	int codeSize=0;

	switch(_child->getType())
	{
		case VARIABLE_EXPR:
			codeSize=emit(OP_PUSHVARVALUE,_write)+emit(_child->getVariableIdx(),_write);
			break;
		case VALUE_EXPR:
			codeSize=emit(OP_PUSHVALUE,_write)+emit(_child->getValue(),_write);
			break;
		default:
			printf("INTERNAL ERROR IN emitValue() :(\n");
			exit(-2);
			break;
	}
	return codeSize;
}


/*===========================================================================
 end */
