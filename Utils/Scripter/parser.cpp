#include <cyacc.h>

#line 1 "C:\\spongebob\\Utils\\scripter\\parser.y"

/****************************************************************************
parser.y
ParserWizard generated YACC file.

Date: 07 December 2000
****************************************************************************/

#include "function.h"
#include "lexer.h"
#include "codegen.h"



#line 19 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
// repeated because of possible precompiled header
#include <cyacc.h>

#include "parser.h"

/////////////////////////////////////////////////////////////////////////////
// constructor

YYPARSENAME::YYPARSENAME()
{
	yytables();
#line 43 "C:\\spongebob\\Utils\\scripter\\parser.y"

	// place any extra initialisation code here

#line 35 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
}

#ifndef YYSTYPE
#define YYSTYPE int
#endif
#ifndef YYSTACK_SIZE
#define YYSTACK_SIZE 100
#endif

// yyattribute
#ifdef YYDEBUG
void YYFAR* YYPARSENAME::yyattribute1(int index) const
{
	YYSTYPE YYFAR* p = &((YYSTYPE YYFAR*)yyattributestackptr)[yytop + index];
	return p;
}
#define yyattribute(index) (*(YYSTYPE YYFAR*)yyattribute1(index))
#else
#define yyattribute(index) (((YYSTYPE YYFAR*)yyattributestackptr)[yytop + (index)])
#endif

#ifdef YYDEBUG
void YYPARSENAME::yyinitdebug(void YYFAR** p, int count) const
{
	yyassert(p != NULL);
	yyassert(count >= 1);

	YYSTYPE YYFAR** p1 = (YYSTYPE YYFAR**)p;
	for (int i = 0; i < count; i++) {
		p1[i] = &((YYSTYPE YYFAR*)yyattributestackptr)[yytop + i - (count - 1)];
	}
}
#endif

void YYPARSENAME::yyaction(int action)
{
	switch (action) {
	case 0:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 102 "C:\\spongebob\\Utils\\scripter\\parser.y"
s_baseTreeNode=yyattribute(1 - 1).treenode;
#line 82 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 1:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[3];
			yyinitdebug((void YYFAR**)yya, 3);
#endif
			{
#line 106 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(STMT_LIST,yyattribute(1 - 2).treenode,yyattribute(2 - 2).treenode);
#line 95 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 2:
		{
#line 107 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(EMPTY_STMT);
#line 103 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
		}
		break;
	case 3:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 111 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(EMPTY_STMT);
#line 115 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 4:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[3];
			yyinitdebug((void YYFAR**)yya, 3);
#endif
			{
#line 112 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(STOP_STMT);
#line 128 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 5:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[3];
			yyinitdebug((void YYFAR**)yya, 3);
#endif
			{
#line 113 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(PAUSE_STMT);
#line 141 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 6:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[6];
			yyinitdebug((void YYFAR**)yya, 6);
#endif
			{
#line 114 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(PRINT_STMT,yyattribute(3 - 5).treenode);
#line 154 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 7:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[3];
			yyinitdebug((void YYFAR**)yya, 3);
#endif
			{
#line 115 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 2).treenode;
#line 167 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 8:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[6];
			yyinitdebug((void YYFAR**)yya, 6);
#endif
			{
#line 116 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(IF_STMT,yyattribute(3 - 5).treenode,yyattribute(5 - 5).treenode);
#line 180 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 9:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[8];
			yyinitdebug((void YYFAR**)yya, 8);
#endif
			{
#line 117 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(IFELSE_STMT,yyattribute(3 - 7).treenode,yyattribute(5 - 7).treenode,yyattribute(7 - 7).treenode);
#line 193 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 10:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 118 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(STMT_LIST,yyattribute(2 - 3).treenode);
#line 206 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 11:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[3];
			yyinitdebug((void YYFAR**)yya, 3);
#endif
			{
#line 119 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(STMT_LIST,yyattribute(1 - 2).treenode,new CTreeNode(POP_STMT));
#line 219 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 12:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 124 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(ASSIGN_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 232 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 13:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 129 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(2 - 3).treenode;
#line 245 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 14:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 130 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 1).treenode;
#line 258 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 15:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 131 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 1).treenode;
#line 271 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 16:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 135 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(EQUAL_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 284 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 17:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 139 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(NOTEQUAL_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 297 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 18:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 144 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(VARIABLE_EXPR,yyattribute(1 - 1).variableIdx);
#line 310 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 19:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 148 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(VALUE_EXPR,yyattribute(1 - 1).value);
#line 323 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 20:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 149 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(VARIABLE_EXPR,yyattribute(1 - 1).variableIdx);
#line 336 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 21:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 150 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(PLUS_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 349 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 22:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 151 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 1).treenode;
#line 362 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 23:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[3];
			yyinitdebug((void YYFAR**)yya, 3);
#endif
			{
#line 156 "C:\\spongebob\\Utils\\scripter\\parser.y"
if(yyattribute(1 - 2).functionNumber!=-1){m_functionNumber=yyattribute(1 - 2).functionNumber;m_functionArgs=getFunctionArgs(yyattribute(1 - 2).functionNumber);}
#line 375 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 24:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[5];
			yyinitdebug((void YYFAR**)yya, 5);
#endif
			{
#line 157 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(STMT_LIST,m_functionArgs,new CTreeNode(FUNCTION_EXPR,m_functionNumber));
#line 388 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	default:
		yyassert(0);
		break;
	}
}
#line 162 "C:\\spongebob\\Utils\\scripter\\parser.y"



/////////////////////////////////////////////////////////////////////////////
// programs section

#line 404 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
void YYPARSENAME::yytables()
{
	yyattribute_size = sizeof(YYSTYPE);
	yysstack_size = YYSTACK_SIZE;

#ifdef YYDEBUG
	static const yysymbol_t YYNEARFAR YYBASED_CODE symbol[] = {
		{ "$end", 0 },
		{ "error", 256 },
		{ "STOP", 257 },
		{ "IF", 258 },
		{ "ELSE", 259 },
		{ "PAUSE", 260 },
		{ "PRINT", 261 },
		{ "ASSIGN", 262 },
		{ "EQUAL", 263 },
		{ "NOTEQUAL", 264 },
		{ "PLUS", 265 },
		{ "END_STMT", 266 },
		{ "OPEN_PAR", 267 },
		{ "CLOSE_PAR", 268 },
		{ "BEGIN_CS", 269 },
		{ "END_CS", 270 },
		{ "VARIABLE", 272 },
		{ "VALUE", 273 },
		{ "FUNCTION", 274 },
		{ NULL, 0 }
	};
	yysymbol = symbol;

	static const char* const YYNEARFAR YYBASED_CODE rule[] = {
		"$accept: program",
		"program: statement_list",
		"statement_list: statement_list statement",
		"statement_list:",
		"statement: END_STMT",
		"statement: STOP END_STMT",
		"statement: PAUSE END_STMT",
		"statement: PRINT OPEN_PAR value CLOSE_PAR END_STMT",
		"statement: assign_expression END_STMT",
		"statement: IF OPEN_PAR expression CLOSE_PAR statement",
		"statement: IF OPEN_PAR expression CLOSE_PAR statement ELSE statement",
		"statement: BEGIN_CS statement_list END_CS",
		"statement: function END_STMT",
		"assign_expression: variable ASSIGN value",
		"expression: OPEN_PAR expression CLOSE_PAR",
		"expression: equal_expression",
		"expression: notequal_expression",
		"equal_expression: value EQUAL value",
		"notequal_expression: value NOTEQUAL value",
		"variable: VARIABLE",
		"value: VALUE",
		"value: VARIABLE",
		"value: value PLUS value",
		"value: function",
		"$$1:",
		"function: FUNCTION OPEN_PAR $$1 CLOSE_PAR"
	};
	yyrule = rule;
#endif

	static const yyreduction_t YYNEARFAR YYBASED_CODE reduction[] = {
		{ 0, 1, -1 },
		{ 1, 1, 0 },
		{ 2, 2, 1 },
		{ 2, 0, 2 },
		{ 3, 1, 3 },
		{ 3, 2, 4 },
		{ 3, 2, 5 },
		{ 3, 5, 6 },
		{ 3, 2, 7 },
		{ 3, 5, 8 },
		{ 3, 7, 9 },
		{ 3, 3, 10 },
		{ 3, 2, 11 },
		{ 4, 3, 12 },
		{ 5, 3, 13 },
		{ 5, 1, 14 },
		{ 5, 1, 15 },
		{ 6, 3, 16 },
		{ 7, 3, 17 },
		{ 8, 1, 18 },
		{ 9, 1, 19 },
		{ 9, 1, 20 },
		{ 9, 3, 21 },
		{ 9, 1, 22 },
		{ 11, 0, 23 },
		{ 10, 4, 24 }
	};
	yyreduction = reduction;

	static const yytokenaction_t YYNEARFAR YYBASED_CODE tokenaction[] = {
		{ 49, YYAT_SHIFT, 3 },
		{ 49, YYAT_SHIFT, 4 },
		{ 32, YYAT_SHIFT, 40 },
		{ 49, YYAT_SHIFT, 5 },
		{ 49, YYAT_SHIFT, 6 },
		{ 32, YYAT_SHIFT, 41 },
		{ 47, YYAT_SHIFT, 40 },
		{ 2, YYAT_SHIFT, 3 },
		{ 2, YYAT_SHIFT, 4 },
		{ 49, YYAT_SHIFT, 7 },
		{ 2, YYAT_SHIFT, 5 },
		{ 2, YYAT_SHIFT, 6 },
		{ 49, YYAT_SHIFT, 8 },
		{ 30, YYAT_SHIFT, 38 },
		{ 30, YYAT_SHIFT, 39 },
		{ 49, YYAT_SHIFT, 9 },
		{ 2, YYAT_SHIFT, 7 },
		{ 49, YYAT_SHIFT, 10 },
		{ 30, YYAT_ERROR, 0 },
		{ 2, YYAT_SHIFT, 8 },
		{ 46, YYAT_SHIFT, 40 },
		{ 45, YYAT_SHIFT, 40 },
		{ 2, YYAT_SHIFT, 9 },
		{ 44, YYAT_SHIFT, 49 },
		{ 2, YYAT_SHIFT, 10 },
		{ 40, YYAT_SHIFT, 25 },
		{ 40, YYAT_SHIFT, 26 },
		{ 40, YYAT_SHIFT, 10 },
		{ 41, YYAT_SHIFT, 48 },
		{ 36, YYAT_SHIFT, 43 },
		{ 35, YYAT_SHIFT, 40 },
		{ 34, YYAT_SHIFT, 42 },
		{ 27, YYAT_SHIFT, 37 },
		{ 24, YYAT_SHIFT, 24 },
		{ 19, YYAT_SHIFT, 33 },
		{ 14, YYAT_SHIFT, 23 },
		{ 13, YYAT_SHIFT, 22 },
		{ 12, YYAT_SHIFT, 21 },
		{ 10, YYAT_SHIFT, 20 },
		{ 6, YYAT_SHIFT, 18 },
		{ 5, YYAT_SHIFT, 17 },
		{ 4, YYAT_SHIFT, 16 },
		{ 3, YYAT_SHIFT, 15 },
		{ 1, YYAT_ACCEPT, 0 }
	};
	yytokenaction = tokenaction;
	yytokenaction_size = 44;

	static const yystateaction_t YYNEARFAR YYBASED_CODE stateaction[] = {
		{ 0, 0, YYAT_REDUCE, 3 },
		{ 43, 1, YYAT_ERROR, 0 },
		{ -250, 1, YYAT_REDUCE, 1 },
		{ -224, 1, YYAT_DEFAULT, 41 },
		{ -226, 1, YYAT_DEFAULT, 10 },
		{ -226, 1, YYAT_DEFAULT, 41 },
		{ -228, 1, YYAT_DEFAULT, 10 },
		{ 0, 0, YYAT_REDUCE, 4 },
		{ 0, 0, YYAT_REDUCE, 3 },
		{ 0, 0, YYAT_REDUCE, 19 },
		{ -229, 1, YYAT_ERROR, 0 },
		{ 0, 0, YYAT_REDUCE, 2 },
		{ -229, 1, YYAT_DEFAULT, 41 },
		{ -226, 1, YYAT_ERROR, 0 },
		{ -231, 1, YYAT_DEFAULT, 41 },
		{ 0, 0, YYAT_REDUCE, 5 },
		{ 0, 0, YYAT_DEFAULT, 24 },
		{ 0, 0, YYAT_REDUCE, 6 },
		{ 0, 0, YYAT_DEFAULT, 40 },
		{ -236, 1, YYAT_DEFAULT, 49 },
		{ 0, 0, YYAT_REDUCE, 24 },
		{ 0, 0, YYAT_REDUCE, 8 },
		{ 0, 0, YYAT_DEFAULT, 40 },
		{ 0, 0, YYAT_REDUCE, 12 },
		{ -234, 1, YYAT_DEFAULT, 40 },
		{ 0, 0, YYAT_REDUCE, 21 },
		{ 0, 0, YYAT_REDUCE, 20 },
		{ -236, 1, YYAT_DEFAULT, 36 },
		{ 0, 0, YYAT_REDUCE, 15 },
		{ 0, 0, YYAT_REDUCE, 16 },
		{ -250, 1, YYAT_DEFAULT, 32 },
		{ 0, 0, YYAT_REDUCE, 23 },
		{ -263, 1, YYAT_DEFAULT, 36 },
		{ 0, 0, YYAT_REDUCE, 11 },
		{ -237, 1, YYAT_DEFAULT, 36 },
		{ -235, 1, YYAT_REDUCE, 13 },
		{ -239, 1, YYAT_ERROR, 0 },
		{ 0, 0, YYAT_DEFAULT, 49 },
		{ 0, 0, YYAT_DEFAULT, 40 },
		{ 0, 0, YYAT_DEFAULT, 40 },
		{ -247, 1, YYAT_ERROR, 0 },
		{ -238, 1, YYAT_ERROR, 0 },
		{ 0, 0, YYAT_REDUCE, 25 },
		{ 0, 0, YYAT_REDUCE, 14 },
		{ -236, 1, YYAT_REDUCE, 9 },
		{ -244, 1, YYAT_REDUCE, 17 },
		{ -245, 1, YYAT_REDUCE, 18 },
		{ -259, 1, YYAT_REDUCE, 22 },
		{ 0, 0, YYAT_REDUCE, 7 },
		{ -257, 1, YYAT_DEFAULT, 41 },
		{ 0, 0, YYAT_REDUCE, 10 }
	};
	yystateaction = stateaction;

	static const yynontermgoto_t YYNEARFAR YYBASED_CODE nontermgoto[] = {
		{ 49, 50 },
		{ 49, 12 },
		{ 24, 36 },
		{ 24, 28 },
		{ 24, 29 },
		{ 49, 13 },
		{ 24, 30 },
		{ 49, 14 },
		{ 40, 47 },
		{ 40, 31 },
		{ 0, 1 },
		{ 0, 2 },
		{ 39, 46 },
		{ 38, 45 },
		{ 37, 44 },
		{ 22, 35 },
		{ 20, 34 },
		{ 19, 11 },
		{ 18, 32 },
		{ 16, 27 },
		{ 8, 19 }
	};
	yynontermgoto = nontermgoto;
	yynontermgoto_size = 21;

	static const yystategoto_t YYNEARFAR YYBASED_CODE stategoto[] = {
		{ 9, -1 },
		{ 0, -1 },
		{ 0, 19 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 18, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 14, 24 },
		{ 0, -1 },
		{ 9, 40 },
		{ 14, 49 },
		{ 5, -1 },
		{ 0, -1 },
		{ 6, 40 },
		{ 0, -1 },
		{ -3, 40 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 11, 49 },
		{ 4, 40 },
		{ 3, 40 },
		{ -1, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ -3, -1 },
		{ 0, -1 }
	};
	yystategoto = stategoto;

	yydestructorptr = NULL;

	yytokendestptr = NULL;
	yytokendest_size = 0;
	yytokendestbase = 0;
}
