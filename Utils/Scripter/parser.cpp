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
#line 107 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 111 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(STMT_LIST,yyattribute(1 - 2).treenode,yyattribute(2 - 2).treenode);
#line 95 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 2:
		{
#line 112 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 116 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 117 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 118 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 119 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 120 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 121 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 122 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(IFELSE_STMT,yyattribute(3 - 7).treenode,yyattribute(5 - 7).treenode,yyattribute(7 - 7).treenode);
#line 193 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 10:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[6];
			yyinitdebug((void YYFAR**)yya, 6);
#endif
			{
#line 123 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(WHILE_STMT,yyattribute(3 - 5).treenode,yyattribute(5 - 5).treenode);
#line 206 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 11:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[8];
			yyinitdebug((void YYFAR**)yya, 8);
#endif
			{
#line 124 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(DOWHILE_STMT,yyattribute(2 - 7).treenode,yyattribute(5 - 7).treenode);
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
#line 125 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(STMT_LIST,yyattribute(2 - 3).treenode);
#line 232 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 13:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[3];
			yyinitdebug((void YYFAR**)yya, 3);
#endif
			{
#line 126 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(STMT_LIST,yyattribute(1 - 2).treenode,new CTreeNode(POP_STMT));
#line 245 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 14:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 131 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(ASSIGN_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 258 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 15:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 136 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(2 - 3).treenode;
#line 271 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 16:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 137 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 1).treenode;
#line 284 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 17:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 138 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 1).treenode;
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
#line 139 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 1).treenode;
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
#line 140 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 1).treenode;
#line 323 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 20:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 144 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(EQUAL_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
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
#line 148 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(NOTEQUAL_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 349 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 22:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 152 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(LESSTHAN_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 362 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 23:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 156 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(GREATERTHAN_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 375 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 24:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 161 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(VARIABLE_EXPR,yyattribute(1 - 1).variableIdx);
#line 388 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 25:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 165 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(VALUE_EXPR,yyattribute(1 - 1).value);
#line 401 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 26:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 166 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(VARIABLE_EXPR,yyattribute(1 - 1).variableIdx);
#line 414 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 27:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 169 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(PLUS_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 427 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 28:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 170 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(SUBTRACT_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 440 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 29:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 171 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 1).treenode;
#line 453 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 30:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[3];
			yyinitdebug((void YYFAR**)yya, 3);
#endif
			{
#line 176 "C:\\spongebob\\Utils\\scripter\\parser.y"
if(yyattribute(1 - 2).functionNumber!=-1){m_functionNumber=yyattribute(1 - 2).functionNumber;m_functionArgs=getFunctionArgs(yyattribute(1 - 2).functionNumber);}
#line 466 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 31:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[5];
			yyinitdebug((void YYFAR**)yya, 5);
#endif
			{
#line 177 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(STMT_LIST,m_functionArgs,new CTreeNode(FUNCTION_EXPR,m_functionNumber));
#line 479 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	default:
		yyassert(0);
		break;
	}
}
#line 182 "C:\\spongebob\\Utils\\scripter\\parser.y"



/////////////////////////////////////////////////////////////////////////////
// programs section

#line 495 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
		{ "WHILE", 260 },
		{ "DO", 261 },
		{ "PAUSE", 262 },
		{ "PRINT", 263 },
		{ "ASSIGN", 264 },
		{ "EQUAL", 265 },
		{ "NOTEQUAL", 266 },
		{ "LESSTHAN", 267 },
		{ "GREATERTHAN", 268 },
		{ "PLUS", 269 },
		{ "SUBTRACT", 270 },
		{ "END_STMT", 271 },
		{ "OPEN_PAR", 272 },
		{ "CLOSE_PAR", 273 },
		{ "BEGIN_CS", 274 },
		{ "END_CS", 275 },
		{ "VARIABLE", 277 },
		{ "VALUE", 278 },
		{ "FUNCTION", 279 },
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
		"statement: WHILE OPEN_PAR expression CLOSE_PAR statement",
		"statement: DO statement WHILE OPEN_PAR expression CLOSE_PAR END_STMT",
		"statement: BEGIN_CS statement_list END_CS",
		"statement: function END_STMT",
		"assign_expression: variable ASSIGN value",
		"expression: OPEN_PAR expression CLOSE_PAR",
		"expression: equal_expression",
		"expression: notequal_expression",
		"expression: lessthan_expression",
		"expression: greaterthan_expression",
		"equal_expression: value EQUAL value",
		"notequal_expression: value NOTEQUAL value",
		"lessthan_expression: value LESSTHAN value",
		"greaterthan_expression: value GREATERTHAN value",
		"variable: VARIABLE",
		"value: VALUE",
		"value: VARIABLE",
		"value: value PLUS value",
		"value: value SUBTRACT value",
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
		{ 3, 5, 10 },
		{ 3, 7, 11 },
		{ 3, 3, 12 },
		{ 3, 2, 13 },
		{ 4, 3, 14 },
		{ 5, 3, 15 },
		{ 5, 1, 16 },
		{ 5, 1, 17 },
		{ 5, 1, 18 },
		{ 5, 1, 19 },
		{ 6, 3, 20 },
		{ 7, 3, 21 },
		{ 8, 3, 22 },
		{ 9, 3, 23 },
		{ 10, 1, 24 },
		{ 11, 1, 25 },
		{ 11, 1, 26 },
		{ 11, 3, 27 },
		{ 11, 3, 28 },
		{ 11, 1, 29 },
		{ 13, 0, 30 },
		{ 12, 4, 31 }
	};
	yyreduction = reduction;

	static const yytokenaction_t YYNEARFAR YYBASED_CODE tokenaction[] = {
		{ 67, YYAT_SHIFT, 3 },
		{ 67, YYAT_SHIFT, 4 },
		{ 68, YYAT_SHIFT, 70 },
		{ 67, YYAT_SHIFT, 5 },
		{ 67, YYAT_SHIFT, 6 },
		{ 67, YYAT_SHIFT, 7 },
		{ 67, YYAT_SHIFT, 8 },
		{ 2, YYAT_SHIFT, 3 },
		{ 2, YYAT_SHIFT, 4 },
		{ 65, YYAT_SHIFT, 68 },
		{ 2, YYAT_SHIFT, 5 },
		{ 2, YYAT_SHIFT, 6 },
		{ 2, YYAT_SHIFT, 7 },
		{ 2, YYAT_SHIFT, 8 },
		{ 67, YYAT_SHIFT, 9 },
		{ 40, YYAT_SHIFT, 50 },
		{ 40, YYAT_SHIFT, 51 },
		{ 67, YYAT_SHIFT, 10 },
		{ 57, YYAT_SHIFT, 67 },
		{ 40, YYAT_SHIFT, 54 },
		{ 67, YYAT_SHIFT, 11 },
		{ 2, YYAT_SHIFT, 9 },
		{ 67, YYAT_SHIFT, 12 },
		{ 54, YYAT_SHIFT, 66 },
		{ 2, YYAT_SHIFT, 10 },
		{ 63, YYAT_SHIFT, 50 },
		{ 63, YYAT_SHIFT, 51 },
		{ 2, YYAT_SHIFT, 11 },
		{ 51, YYAT_ERROR, 0 },
		{ 2, YYAT_SHIFT, 12 },
		{ 36, YYAT_SHIFT, 46 },
		{ 36, YYAT_SHIFT, 47 },
		{ 36, YYAT_SHIFT, 48 },
		{ 36, YYAT_SHIFT, 49 },
		{ 53, YYAT_SHIFT, 28 },
		{ 62, YYAT_SHIFT, 50 },
		{ 62, YYAT_SHIFT, 51 },
		{ 44, YYAT_SHIFT, 56 },
		{ 36, YYAT_ERROR, 0 },
		{ 53, YYAT_SHIFT, 29 },
		{ 53, YYAT_SHIFT, 30 },
		{ 53, YYAT_SHIFT, 12 },
		{ 61, YYAT_SHIFT, 50 },
		{ 61, YYAT_SHIFT, 51 },
		{ 60, YYAT_SHIFT, 50 },
		{ 60, YYAT_SHIFT, 51 },
		{ 59, YYAT_SHIFT, 50 },
		{ 59, YYAT_SHIFT, 51 },
		{ 58, YYAT_SHIFT, 50 },
		{ 58, YYAT_SHIFT, 51 },
		{ 43, YYAT_SHIFT, 50 },
		{ 43, YYAT_SHIFT, 51 },
		{ 42, YYAT_SHIFT, 55 },
		{ 39, YYAT_SHIFT, 53 },
		{ 38, YYAT_SHIFT, 52 },
		{ 31, YYAT_SHIFT, 45 },
		{ 23, YYAT_SHIFT, 41 },
		{ 20, YYAT_SHIFT, 39 },
		{ 16, YYAT_SHIFT, 27 },
		{ 15, YYAT_SHIFT, 26 },
		{ 14, YYAT_SHIFT, 25 },
		{ 12, YYAT_SHIFT, 24 },
		{ 8, YYAT_SHIFT, 22 },
		{ 7, YYAT_SHIFT, 21 },
		{ 5, YYAT_SHIFT, 19 },
		{ 4, YYAT_SHIFT, 18 },
		{ 3, YYAT_SHIFT, 17 },
		{ 1, YYAT_ACCEPT, 0 }
	};
	yytokenaction = tokenaction;
	yytokenaction_size = 68;

	static const yystateaction_t YYNEARFAR YYBASED_CODE stateaction[] = {
		{ 0, 0, YYAT_REDUCE, 3 },
		{ 67, 1, YYAT_ERROR, 0 },
		{ -250, 1, YYAT_REDUCE, 1 },
		{ -205, 1, YYAT_DEFAULT, 68 },
		{ -207, 1, YYAT_DEFAULT, 39 },
		{ -208, 1, YYAT_DEFAULT, 39 },
		{ 0, 0, YYAT_DEFAULT, 67 },
		{ -208, 1, YYAT_DEFAULT, 68 },
		{ -210, 1, YYAT_DEFAULT, 39 },
		{ 0, 0, YYAT_REDUCE, 4 },
		{ 0, 0, YYAT_REDUCE, 3 },
		{ 0, 0, YYAT_REDUCE, 25 },
		{ -211, 1, YYAT_DEFAULT, 39 },
		{ 0, 0, YYAT_REDUCE, 2 },
		{ -211, 1, YYAT_DEFAULT, 68 },
		{ -205, 1, YYAT_ERROR, 0 },
		{ -213, 1, YYAT_DEFAULT, 68 },
		{ 0, 0, YYAT_REDUCE, 5 },
		{ 0, 0, YYAT_DEFAULT, 53 },
		{ 0, 0, YYAT_DEFAULT, 53 },
		{ -203, 1, YYAT_ERROR, 0 },
		{ 0, 0, YYAT_REDUCE, 6 },
		{ 0, 0, YYAT_DEFAULT, 51 },
		{ -219, 1, YYAT_DEFAULT, 67 },
		{ 0, 0, YYAT_REDUCE, 31 },
		{ 0, 0, YYAT_REDUCE, 8 },
		{ 0, 0, YYAT_DEFAULT, 51 },
		{ 0, 0, YYAT_REDUCE, 14 },
		{ 0, 0, YYAT_DEFAULT, 53 },
		{ 0, 0, YYAT_REDUCE, 27 },
		{ 0, 0, YYAT_REDUCE, 26 },
		{ -218, 1, YYAT_DEFAULT, 65 },
		{ 0, 0, YYAT_REDUCE, 17 },
		{ 0, 0, YYAT_REDUCE, 18 },
		{ 0, 0, YYAT_REDUCE, 19 },
		{ 0, 0, YYAT_REDUCE, 20 },
		{ -235, 1, YYAT_DEFAULT, 40 },
		{ 0, 0, YYAT_REDUCE, 30 },
		{ -219, 1, YYAT_DEFAULT, 65 },
		{ -219, 1, YYAT_ERROR, 0 },
		{ -254, 1, YYAT_DEFAULT, 65 },
		{ 0, 0, YYAT_REDUCE, 13 },
		{ -221, 1, YYAT_DEFAULT, 65 },
		{ -219, 1, YYAT_REDUCE, 15 },
		{ -236, 1, YYAT_DEFAULT, 65 },
		{ 0, 0, YYAT_DEFAULT, 67 },
		{ 0, 0, YYAT_DEFAULT, 51 },
		{ 0, 0, YYAT_DEFAULT, 51 },
		{ 0, 0, YYAT_DEFAULT, 51 },
		{ 0, 0, YYAT_DEFAULT, 51 },
		{ 0, 0, YYAT_DEFAULT, 51 },
		{ -244, 1, YYAT_DEFAULT, 53 },
		{ 0, 0, YYAT_DEFAULT, 67 },
		{ -238, 1, YYAT_DEFAULT, 39 },
		{ -248, 1, YYAT_DEFAULT, 68 },
		{ 0, 0, YYAT_REDUCE, 32 },
		{ 0, 0, YYAT_REDUCE, 16 },
		{ -241, 1, YYAT_REDUCE, 9 },
		{ -221, 1, YYAT_REDUCE, 21 },
		{ -223, 1, YYAT_REDUCE, 22 },
		{ -225, 1, YYAT_REDUCE, 23 },
		{ -227, 1, YYAT_REDUCE, 24 },
		{ -234, 1, YYAT_REDUCE, 28 },
		{ -244, 1, YYAT_REDUCE, 29 },
		{ 0, 0, YYAT_REDUCE, 11 },
		{ -264, 1, YYAT_ERROR, 0 },
		{ 0, 0, YYAT_REDUCE, 7 },
		{ -257, 1, YYAT_DEFAULT, 68 },
		{ -269, 1, YYAT_ERROR, 0 },
		{ 0, 0, YYAT_REDUCE, 10 },
		{ 0, 0, YYAT_REDUCE, 12 }
	};
	yystateaction = stateaction;

	static const yynontermgoto_t YYNEARFAR YYBASED_CODE nontermgoto[] = {
		{ 53, 65 },
		{ 53, 32 },
		{ 53, 33 },
		{ 53, 34 },
		{ 53, 35 },
		{ 52, 64 },
		{ 53, 36 },
		{ 67, 69 },
		{ 67, 14 },
		{ 51, 63 },
		{ 51, 37 },
		{ 0, 1 },
		{ 0, 2 },
		{ 50, 62 },
		{ 67, 15 },
		{ 49, 61 },
		{ 67, 16 },
		{ 48, 60 },
		{ 47, 59 },
		{ 46, 58 },
		{ 45, 57 },
		{ 28, 44 },
		{ 26, 43 },
		{ 24, 42 },
		{ 23, 13 },
		{ 22, 40 },
		{ 19, 38 },
		{ 18, 31 },
		{ 10, 23 },
		{ 6, 20 }
	};
	yynontermgoto = nontermgoto;
	yynontermgoto_size = 30;

	static const yystategoto_t YYNEARFAR YYBASED_CODE stategoto[] = {
		{ 10, -1 },
		{ 0, -1 },
		{ 0, 23 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 26, 67 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 26, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 22, 53 },
		{ 21, 53 },
		{ 0, -1 },
		{ 0, -1 },
		{ 14, 51 },
		{ 21, 67 },
		{ 10, -1 },
		{ 0, -1 },
		{ 11, 51 },
		{ 0, -1 },
		{ 16, 53 },
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
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 17, 67 },
		{ 8, 51 },
		{ 7, 51 },
		{ 6, 51 },
		{ 4, 51 },
		{ 2, 51 },
		{ -2, -1 },
		{ 2, 67 },
		{ -5, 51 },
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
		{ 0, -1 },
		{ 4, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 }
	};
	yystategoto = stategoto;

	yydestructorptr = NULL;

	yytokendestptr = NULL;
	yytokendest_size = 0;
	yytokendestbase = 0;
}
