/*=========================================================================

	func.h

	Author:		PKG
	Created:
	Project:	Spongebob
	Purpose:

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/

#ifndef __CODEGEN_H__
#define	__CODEGEN_H__

/*----------------------------------------------------------------------
	Includes
	-------- */

/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

typedef enum
{
	// Statements
	STMT_LIST,				// list of statements [left-part, right-part] (left-part may be another list)
	EMPTY_STMT,				// empty statement
	PRINT_STMT,				// print [variable]
	STOP_STMT,				// stop
	PAUSE_STMT,				// pause
	IF_STMT,				// if [expression, ifcode]
	IFELSE_STMT,			// if [expression, ifcode, elsecode]
		
	// Expressions
	ASSIGN_EXPR,			// assignment [variable, value]
	EQUAL_EXPR,				// == [variable, value]
	NOTEQUAL_EXPR,			// != [variable, value]
	VARIABLE_EXPR,			// variable
	VALUE_EXPR,				// value
	PLUS_EXPR,				// + [value, value]
}NodeType;


/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

class CTreeNode
{
public:
				CTreeNode(NodeType _type);
				CTreeNode(NodeType _type,CTreeNode *child1);
				CTreeNode(NodeType _type,CTreeNode *child1,CTreeNode *child2);
				CTreeNode(NodeType _type,CTreeNode *child1,CTreeNode *child2,CTreeNode *child3);
				CTreeNode(NodeType _type,int _data);

	int			generateCode(int _write);

	int			getVariableIdx()				{return m_variableIdx;}
	int			getValue()						{return m_value;}
	int			getType()						{return m_type;}

private:

	// Nothing else needs to know these so we may as well protect them
	enum
	{
								//	args				stack data				result
		OP_NOP=0x1100,			//
		OP_STOP,				//
		OP_PAUSE,				//
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

	enum{MAX_CHILD_NODES=3};


	int				emit(unsigned short _data,int _write);
	int				emitValue(CTreeNode *_child,int _write);

	NodeType		m_type;
	CTreeNode		*m_children[MAX_CHILD_NODES];
	int				m_numChildren;

	signed short	m_variableIdx;
	signed short	m_value;
};


/*----------------------------------------------------------------------
	Globals
	------- */

extern CTreeNode	*s_baseTreeNode;


/*----------------------------------------------------------------------
	Functions
	--------- */

extern int parseFile(char *_filename,class CTreeNode *_baseNode);

extern int openOutputFile(char *_filename);
extern int closeOutputFile();


/*---------------------------------------------------------------------- */

#endif	/* __CODEGEN_H__ */

/*===========================================================================
 end */
