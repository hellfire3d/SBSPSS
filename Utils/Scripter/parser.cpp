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
#line 109 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 113 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(STMT_LIST,yyattribute(1 - 2).treenode,yyattribute(2 - 2).treenode);
#line 95 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 2:
		{
#line 114 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 118 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 119 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 120 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 121 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 122 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 123 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 124 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 125 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 126 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 127 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 128 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 133 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(ASSIGN_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
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
#line 138 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 1).treenode;
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
#line 139 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 140 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 141 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 1).treenode;
#line 310 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 19:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 145 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(EQUAL_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
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
#line 149 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(NOTEQUAL_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
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
#line 153 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(LESSTHAN_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
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
#line 157 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(GREATERTHAN_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 362 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 23:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 162 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(VARIABLE_EXPR,yyattribute(1 - 1).variableIdx);
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
#line 166 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(VALUE_EXPR,yyattribute(1 - 1).value);
#line 388 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 25:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 167 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(2 - 3).treenode;
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
#line 168 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 171 "C:\\spongebob\\Utils\\scripter\\parser.y"
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
#line 172 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(SUBTRACT_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 440 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 29:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 173 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(MULTIPLY_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 453 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 30:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 174 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(DIVIDE_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 466 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 31:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 175 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 1).treenode;
#line 479 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 32:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[3];
			yyinitdebug((void YYFAR**)yya, 3);
#endif
			{
#line 180 "C:\\spongebob\\Utils\\scripter\\parser.y"
if(yyattribute(1 - 2).functionNumber!=-1){m_functionNumber=yyattribute(1 - 2).functionNumber;m_functionArgs=getFunctionArgs(yyattribute(1 - 2).functionNumber);}
#line 492 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 33:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[5];
			yyinitdebug((void YYFAR**)yya, 5);
#endif
			{
#line 181 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(STMT_LIST,m_functionArgs,new CTreeNode(FUNCTION_EXPR,m_functionNumber));
#line 505 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	default:
		yyassert(0);
		break;
	}
}
#line 186 "C:\\spongebob\\Utils\\scripter\\parser.y"



/////////////////////////////////////////////////////////////////////////////
// programs section

#line 521 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
		{ "MULTIPLY", 271 },
		{ "DIVIDE", 272 },
		{ "END_STMT", 273 },
		{ "OPEN_PAR", 274 },
		{ "CLOSE_PAR", 275 },
		{ "BEGIN_CS", 276 },
		{ "END_CS", 277 },
		{ "VARIABLE", 279 },
		{ "VALUE", 280 },
		{ "FUNCTION", 281 },
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
		"value: OPEN_PAR value CLOSE_PAR",
		"value: VARIABLE",
		"value: value PLUS value",
		"value: value SUBTRACT value",
		"value: value MULTIPLY value",
		"value: value DIVIDE value",
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
		{ 5, 1, 15 },
		{ 5, 1, 16 },
		{ 5, 1, 17 },
		{ 5, 1, 18 },
		{ 6, 3, 19 },
		{ 7, 3, 20 },
		{ 8, 3, 21 },
		{ 9, 3, 22 },
		{ 10, 1, 23 },
		{ 11, 1, 24 },
		{ 11, 3, 25 },
		{ 11, 1, 26 },
		{ 11, 3, 27 },
		{ 11, 3, 28 },
		{ 11, 3, 29 },
		{ 11, 3, 30 },
		{ 11, 1, 31 },
		{ 13, 0, 32 },
		{ 12, 4, 33 }
	};
	yyreduction = reduction;

	static const yytokenaction_t YYNEARFAR YYBASED_CODE tokenaction[] = {
		{ 71, YYAT_SHIFT, 3 },
		{ 71, YYAT_SHIFT, 4 },
		{ 72, YYAT_SHIFT, 74 },
		{ 71, YYAT_SHIFT, 5 },
		{ 71, YYAT_SHIFT, 6 },
		{ 71, YYAT_SHIFT, 7 },
		{ 71, YYAT_SHIFT, 8 },
		{ 2, YYAT_SHIFT, 3 },
		{ 2, YYAT_SHIFT, 4 },
		{ 69, YYAT_SHIFT, 72 },
		{ 2, YYAT_SHIFT, 5 },
		{ 2, YYAT_SHIFT, 6 },
		{ 2, YYAT_SHIFT, 7 },
		{ 2, YYAT_SHIFT, 8 },
		{ 59, YYAT_SHIFT, 71 },
		{ 56, YYAT_SHIFT, 70 },
		{ 71, YYAT_SHIFT, 9 },
		{ 42, YYAT_SHIFT, 57 },
		{ 40, YYAT_SHIFT, 56 },
		{ 71, YYAT_SHIFT, 10 },
		{ 39, YYAT_SHIFT, 55 },
		{ 38, YYAT_SHIFT, 54 },
		{ 71, YYAT_SHIFT, 11 },
		{ 2, YYAT_SHIFT, 9 },
		{ 71, YYAT_SHIFT, 12 },
		{ 31, YYAT_SHIFT, 45 },
		{ 2, YYAT_SHIFT, 10 },
		{ 23, YYAT_SHIFT, 41 },
		{ 20, YYAT_SHIFT, 39 },
		{ 2, YYAT_SHIFT, 11 },
		{ 16, YYAT_SHIFT, 27 },
		{ 2, YYAT_SHIFT, 12 },
		{ 36, YYAT_SHIFT, 46 },
		{ 36, YYAT_SHIFT, 47 },
		{ 36, YYAT_SHIFT, 48 },
		{ 36, YYAT_SHIFT, 49 },
		{ 15, YYAT_SHIFT, 26 },
		{ 44, YYAT_SHIFT, 50 },
		{ 44, YYAT_SHIFT, 51 },
		{ 44, YYAT_SHIFT, 52 },
		{ 44, YYAT_SHIFT, 53 },
		{ 55, YYAT_SHIFT, 28 },
		{ 36, YYAT_ERROR, 0 },
		{ 44, YYAT_SHIFT, 58 },
		{ 14, YYAT_SHIFT, 25 },
		{ 12, YYAT_SHIFT, 24 },
		{ 55, YYAT_SHIFT, 29 },
		{ 55, YYAT_SHIFT, 30 },
		{ 55, YYAT_SHIFT, 12 },
		{ 67, YYAT_SHIFT, 50 },
		{ 67, YYAT_SHIFT, 51 },
		{ 67, YYAT_SHIFT, 52 },
		{ 67, YYAT_SHIFT, 53 },
		{ 66, YYAT_SHIFT, 50 },
		{ 66, YYAT_SHIFT, 51 },
		{ 66, YYAT_SHIFT, 52 },
		{ 66, YYAT_SHIFT, 53 },
		{ 65, YYAT_SHIFT, 50 },
		{ 65, YYAT_SHIFT, 51 },
		{ 65, YYAT_SHIFT, 52 },
		{ 65, YYAT_SHIFT, 53 },
		{ 64, YYAT_SHIFT, 50 },
		{ 64, YYAT_SHIFT, 51 },
		{ 64, YYAT_SHIFT, 52 },
		{ 64, YYAT_SHIFT, 53 },
		{ 63, YYAT_SHIFT, 50 },
		{ 63, YYAT_SHIFT, 51 },
		{ 63, YYAT_SHIFT, 52 },
		{ 63, YYAT_SHIFT, 53 },
		{ 62, YYAT_SHIFT, 50 },
		{ 62, YYAT_SHIFT, 51 },
		{ 62, YYAT_SHIFT, 52 },
		{ 62, YYAT_SHIFT, 53 },
		{ 61, YYAT_SHIFT, 50 },
		{ 61, YYAT_SHIFT, 51 },
		{ 61, YYAT_SHIFT, 52 },
		{ 61, YYAT_SHIFT, 53 },
		{ 60, YYAT_SHIFT, 50 },
		{ 60, YYAT_SHIFT, 51 },
		{ 60, YYAT_SHIFT, 52 },
		{ 60, YYAT_SHIFT, 53 },
		{ 43, YYAT_SHIFT, 50 },
		{ 43, YYAT_SHIFT, 51 },
		{ 43, YYAT_SHIFT, 52 },
		{ 43, YYAT_SHIFT, 53 },
		{ 8, YYAT_SHIFT, 22 },
		{ 7, YYAT_SHIFT, 21 },
		{ 5, YYAT_SHIFT, 19 },
		{ 4, YYAT_SHIFT, 18 },
		{ 3, YYAT_SHIFT, 17 },
		{ 1, YYAT_ACCEPT, 0 }
	};
	yytokenaction = tokenaction;
	yytokenaction_size = 91;

	static const yystateaction_t YYNEARFAR YYBASED_CODE stateaction[] = {
		{ 0, 0, YYAT_REDUCE, 3 },
		{ 90, 1, YYAT_ERROR, 0 },
		{ -250, 1, YYAT_REDUCE, 1 },
		{ -184, 1, YYAT_DEFAULT, 72 },
		{ -186, 1, YYAT_DEFAULT, 39 },
		{ -187, 1, YYAT_DEFAULT, 39 },
		{ 0, 0, YYAT_DEFAULT, 71 },
		{ -187, 1, YYAT_DEFAULT, 72 },
		{ -189, 1, YYAT_DEFAULT, 39 },
		{ 0, 0, YYAT_REDUCE, 4 },
		{ 0, 0, YYAT_REDUCE, 3 },
		{ 0, 0, YYAT_REDUCE, 24 },
		{ -229, 1, YYAT_DEFAULT, 39 },
		{ 0, 0, YYAT_REDUCE, 2 },
		{ -229, 1, YYAT_DEFAULT, 72 },
		{ -228, 1, YYAT_ERROR, 0 },
		{ -243, 1, YYAT_DEFAULT, 72 },
		{ 0, 0, YYAT_REDUCE, 5 },
		{ 0, 0, YYAT_DEFAULT, 55 },
		{ 0, 0, YYAT_DEFAULT, 55 },
		{ -232, 1, YYAT_ERROR, 0 },
		{ 0, 0, YYAT_REDUCE, 6 },
		{ 0, 0, YYAT_DEFAULT, 55 },
		{ -250, 1, YYAT_DEFAULT, 71 },
		{ 0, 0, YYAT_REDUCE, 33 },
		{ 0, 0, YYAT_REDUCE, 8 },
		{ 0, 0, YYAT_DEFAULT, 55 },
		{ 0, 0, YYAT_REDUCE, 14 },
		{ 0, 0, YYAT_DEFAULT, 55 },
		{ 0, 0, YYAT_REDUCE, 27 },
		{ 0, 0, YYAT_REDUCE, 25 },
		{ -250, 1, YYAT_DEFAULT, 69 },
		{ 0, 0, YYAT_REDUCE, 16 },
		{ 0, 0, YYAT_REDUCE, 17 },
		{ 0, 0, YYAT_REDUCE, 18 },
		{ 0, 0, YYAT_REDUCE, 19 },
		{ -233, 1, YYAT_DEFAULT, 44 },
		{ 0, 0, YYAT_REDUCE, 32 },
		{ -254, 1, YYAT_DEFAULT, 69 },
		{ -254, 1, YYAT_ERROR, 0 },
		{ -257, 1, YYAT_DEFAULT, 44 },
		{ 0, 0, YYAT_REDUCE, 13 },
		{ -258, 1, YYAT_DEFAULT, 69 },
		{ -188, 1, YYAT_REDUCE, 15 },
		{ -232, 1, YYAT_DEFAULT, 69 },
		{ 0, 0, YYAT_DEFAULT, 71 },
		{ 0, 0, YYAT_DEFAULT, 55 },
		{ 0, 0, YYAT_DEFAULT, 55 },
		{ 0, 0, YYAT_DEFAULT, 55 },
		{ 0, 0, YYAT_DEFAULT, 55 },
		{ 0, 0, YYAT_DEFAULT, 55 },
		{ 0, 0, YYAT_DEFAULT, 55 },
		{ 0, 0, YYAT_DEFAULT, 55 },
		{ 0, 0, YYAT_DEFAULT, 55 },
		{ 0, 0, YYAT_DEFAULT, 71 },
		{ -233, 1, YYAT_DEFAULT, 39 },
		{ -258, 1, YYAT_DEFAULT, 72 },
		{ 0, 0, YYAT_REDUCE, 34 },
		{ 0, 0, YYAT_REDUCE, 26 },
		{ -245, 1, YYAT_REDUCE, 9 },
		{ -192, 1, YYAT_REDUCE, 20 },
		{ -196, 1, YYAT_REDUCE, 21 },
		{ -200, 1, YYAT_REDUCE, 22 },
		{ -204, 1, YYAT_REDUCE, 23 },
		{ -208, 1, YYAT_REDUCE, 28 },
		{ -212, 1, YYAT_REDUCE, 29 },
		{ -216, 1, YYAT_REDUCE, 30 },
		{ -220, 1, YYAT_REDUCE, 31 },
		{ 0, 0, YYAT_REDUCE, 11 },
		{ -266, 1, YYAT_ERROR, 0 },
		{ 0, 0, YYAT_REDUCE, 7 },
		{ -257, 1, YYAT_DEFAULT, 72 },
		{ -271, 1, YYAT_ERROR, 0 },
		{ 0, 0, YYAT_REDUCE, 10 },
		{ 0, 0, YYAT_REDUCE, 12 }
	};
	yystateaction = stateaction;

	static const yynontermgoto_t YYNEARFAR YYBASED_CODE nontermgoto[] = {
		{ 55, 69 },
		{ 55, 32 },
		{ 55, 33 },
		{ 55, 34 },
		{ 55, 35 },
		{ 54, 68 },
		{ 55, 36 },
		{ 71, 73 },
		{ 71, 14 },
		{ 53, 67 },
		{ 53, 37 },
		{ 0, 1 },
		{ 0, 2 },
		{ 52, 66 },
		{ 71, 15 },
		{ 51, 65 },
		{ 71, 16 },
		{ 50, 64 },
		{ 49, 63 },
		{ 48, 62 },
		{ 47, 61 },
		{ 46, 60 },
		{ 45, 59 },
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
	yynontermgoto_size = 32;

	static const yystategoto_t YYNEARFAR YYBASED_CODE stategoto[] = {
		{ 10, -1 },
		{ 0, -1 },
		{ 0, 23 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 28, 71 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 28, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 24, 55 },
		{ 23, 55 },
		{ 0, -1 },
		{ 0, -1 },
		{ 16, 53 },
		{ 23, 71 },
		{ 12, -1 },
		{ 0, -1 },
		{ 13, 53 },
		{ 0, -1 },
		{ 12, 53 },
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
		{ 19, 71 },
		{ 10, 53 },
		{ 9, 53 },
		{ 8, 53 },
		{ 7, 53 },
		{ 6, 53 },
		{ 4, 53 },
		{ 2, 53 },
		{ -2, -1 },
		{ 2, 71 },
		{ -5, 53 },
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
