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


/*	Std Lib
	------- */

#include <stdio.h>


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
			if(s_lexer.openInputFile(_filename)==true)
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
			
		case ASSIGN_EXPR:		// assign [ variable, number ]
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=emit(OP_PUSHVALUE,_write);
			codeSize+=emit(m_children[0]->getVariableIdx(),_write);
			codeSize+=emit(OP_ASSIGN,_write);
			break;
			
		case EQUAL_EXPR:		// == [variable, value]
			codeSize+=m_children[0]->generateCode(_write);
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=emit(OP_IS_EQUAL_VALUE,_write);
			break;
			
		case NOTEQUAL_EXPR:		// != [variable, value]
			codeSize+=m_children[0]->generateCode(_write);
			codeSize+=m_children[1]->generateCode(_write);
			codeSize+=emit(OP_IS_NOTEQUAL_VALUE,_write);
			break;

		case VARIABLE_EXPR:		// variable
		case VALUE_EXPR:		// value
			if(m_numChildren)
				codeSize+=emitValue(m_children[0],_write);
			else
				codeSize+=emitValue(this,_write);
			break;

		case PLUS_EXPR:			// + [value, value]
			codeSize+=emitValue(m_children[0],_write);
			codeSize+=emitValue(m_children[1],_write);
			codeSize+=emit(OP_ADD,_write);
			break;

		default:
			printf("UNHANDLED CASE %d\n",m_type);
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
	int codeSize;

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
			codeSize=0;
			break;
	}
	return codeSize;
}


/*===========================================================================
 end */
