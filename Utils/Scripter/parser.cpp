#include <cyacc.h>

#line 1 "C:\\spongebob\\Utils\\scripter\\parser.y"

/****************************************************************************
parser.y
ParserWizard generated YACC file.

Date: 07 December 2000
****************************************************************************/

#include "lexer.h"
#include "codegen.h"



#line 18 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
// repeated because of possible precompiled header
#include <cyacc.h>

#include "parser.h"

/////////////////////////////////////////////////////////////////////////////
// constructor

YYPARSENAME::YYPARSENAME()
{
	yytables();
#line 27 "C:\\spongebob\\Utils\\scripter\\parser.y"

	// place any extra initialisation code here

#line 34 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 80 "C:\\spongebob\\Utils\\scripter\\parser.y"
s_baseTreeNode=yyattribute(1 - 1).treenode;
#line 81 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 84 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(STMT_LIST,yyattribute(1 - 2).treenode,yyattribute(2 - 2).treenode);
#line 94 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 2:
		{
#line 85 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(EMPTY_STMT);
#line 102 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
		}
		break;
	case 3:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 89 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(EMPTY_STMT);
#line 114 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 90 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(STOP_STMT);
#line 127 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 91 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(PAUSE_STMT);
#line 140 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 92 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(PRINT_STMT,yyattribute(3 - 5).treenode);
#line 153 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 93 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 2).treenode;
#line 166 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 94 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(IF_STMT,yyattribute(3 - 5).treenode,yyattribute(5 - 5).treenode);
#line 179 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 95 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(IFELSE_STMT,yyattribute(3 - 7).treenode,yyattribute(5 - 7).treenode,yyattribute(7 - 7).treenode);
#line 192 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 96 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(STMT_LIST,yyattribute(2 - 3).treenode);
#line 205 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 11:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[4];
			yyinitdebug((void YYFAR**)yya, 4);
#endif
			{
#line 101 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(ASSIGN_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 218 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 106 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(2 - 3).treenode;
#line 231 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	case 13:
		{
#ifdef YYDEBUG
			YYSTYPE YYFAR* yya[2];
			yyinitdebug((void YYFAR**)yya, 2);
#endif
			{
#line 107 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 1).treenode;
#line 244 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 108 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=yyattribute(1 - 1).treenode;
#line 257 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 112 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(EQUAL_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 270 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 116 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(NOTEQUAL_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 283 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 121 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(VARIABLE_EXPR,yyattribute(1 - 1).variableIdx);
#line 296 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 125 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(VALUE_EXPR,yyattribute(1 - 1).value);
#line 309 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 126 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(VARIABLE_EXPR,yyattribute(1 - 1).variableIdx);
#line 322 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
#line 127 "C:\\spongebob\\Utils\\scripter\\parser.y"
(*(YYSTYPE YYFAR*)yyvalptr).treenode=new CTreeNode(PLUS_EXPR,yyattribute(1 - 3).treenode,yyattribute(3 - 3).treenode);
#line 335 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
			}
		}
		break;
	default:
		yyassert(0);
		break;
	}
}
#line 130 "C:\\spongebob\\Utils\\scripter\\parser.y"



/////////////////////////////////////////////////////////////////////////////
// programs section

int main(void)
{
	int n=1;

openOutputFile("test.dat");
	mylexer lexer;
	myparser parser;
	if(parser.yycreate(&lexer))
	{
		if(lexer.yycreate(&parser))
		{
			n=parser.yyparse();
		}
	}

if(s_baseTreeNode)
{
	s_baseTreeNode->generateCode(true);
}
closeOutputFile();

	return n;
}


#line 376 "C:\\spongebob\\Utils\\scripter\\parser.cpp"
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
		{ "VARIABLE", 271 },
		{ "VALUE", 272 },
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
		"assign_expression: variable ASSIGN value",
		"expression: OPEN_PAR expression CLOSE_PAR",
		"expression: equal_expression",
		"expression: notequal_expression",
		"equal_expression: value EQUAL value",
		"notequal_expression: value NOTEQUAL value",
		"variable: VARIABLE",
		"value: VALUE",
		"value: VARIABLE",
		"value: value PLUS value"
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
		{ 4, 3, 11 },
		{ 5, 3, 12 },
		{ 5, 1, 13 },
		{ 5, 1, 14 },
		{ 6, 3, 15 },
		{ 7, 3, 16 },
		{ 8, 1, 17 },
		{ 9, 1, 18 },
		{ 9, 1, 19 },
		{ 9, 3, 20 }
	};
	yyreduction = reduction;

	static const yytokenaction_t YYNEARFAR YYBASED_CODE tokenaction[] = {
		{ 42, YYAT_SHIFT, 3 },
		{ 42, YYAT_SHIFT, 4 },
		{ 27, YYAT_SHIFT, 34 },
		{ 42, YYAT_SHIFT, 5 },
		{ 42, YYAT_SHIFT, 6 },
		{ 27, YYAT_SHIFT, 35 },
		{ 40, YYAT_SHIFT, 34 },
		{ 2, YYAT_SHIFT, 3 },
		{ 2, YYAT_SHIFT, 4 },
		{ 42, YYAT_SHIFT, 7 },
		{ 2, YYAT_SHIFT, 5 },
		{ 2, YYAT_SHIFT, 6 },
		{ 42, YYAT_SHIFT, 8 },
		{ 39, YYAT_SHIFT, 34 },
		{ 42, YYAT_SHIFT, 9 },
		{ 38, YYAT_SHIFT, 34 },
		{ 2, YYAT_SHIFT, 7 },
		{ 26, YYAT_SHIFT, 32 },
		{ 26, YYAT_SHIFT, 33 },
		{ 2, YYAT_SHIFT, 8 },
		{ 37, YYAT_SHIFT, 42 },
		{ 2, YYAT_SHIFT, 9 },
		{ 26, YYAT_ERROR, 0 },
		{ 34, YYAT_SHIFT, 21 },
		{ 34, YYAT_SHIFT, 22 },
		{ 35, YYAT_SHIFT, 41 },
		{ 30, YYAT_SHIFT, 36 },
		{ 29, YYAT_SHIFT, 34 },
		{ 23, YYAT_SHIFT, 31 },
		{ 20, YYAT_SHIFT, 20 },
		{ 17, YYAT_SHIFT, 28 },
		{ 12, YYAT_SHIFT, 19 },
		{ 11, YYAT_SHIFT, 18 },
		{ 6, YYAT_SHIFT, 16 },
		{ 5, YYAT_SHIFT, 15 },
		{ 4, YYAT_SHIFT, 14 },
		{ 3, YYAT_SHIFT, 13 },
		{ 1, YYAT_ACCEPT, 0 }
	};
	yytokenaction = tokenaction;
	yytokenaction_size = 38;

	static const yystateaction_t YYNEARFAR YYBASED_CODE stateaction[] = {
		{ 0, 0, YYAT_REDUCE, 3 },
		{ 37, 1, YYAT_ERROR, 0 },
		{ -250, 1, YYAT_REDUCE, 1 },
		{ -230, 1, YYAT_DEFAULT, 35 },
		{ -232, 1, YYAT_DEFAULT, 6 },
		{ -232, 1, YYAT_DEFAULT, 35 },
		{ -234, 1, YYAT_ERROR, 0 },
		{ 0, 0, YYAT_REDUCE, 4 },
		{ 0, 0, YYAT_REDUCE, 3 },
		{ 0, 0, YYAT_REDUCE, 18 },
		{ 0, 0, YYAT_REDUCE, 2 },
		{ -234, 1, YYAT_DEFAULT, 35 },
		{ -231, 1, YYAT_ERROR, 0 },
		{ 0, 0, YYAT_REDUCE, 5 },
		{ 0, 0, YYAT_DEFAULT, 20 },
		{ 0, 0, YYAT_REDUCE, 6 },
		{ 0, 0, YYAT_DEFAULT, 34 },
		{ -240, 1, YYAT_DEFAULT, 42 },
		{ 0, 0, YYAT_REDUCE, 8 },
		{ 0, 0, YYAT_DEFAULT, 34 },
		{ -238, 1, YYAT_DEFAULT, 34 },
		{ 0, 0, YYAT_REDUCE, 20 },
		{ 0, 0, YYAT_REDUCE, 19 },
		{ -240, 1, YYAT_DEFAULT, 30 },
		{ 0, 0, YYAT_REDUCE, 14 },
		{ 0, 0, YYAT_REDUCE, 15 },
		{ -246, 1, YYAT_DEFAULT, 27 },
		{ -263, 1, YYAT_DEFAULT, 30 },
		{ 0, 0, YYAT_REDUCE, 11 },
		{ -238, 1, YYAT_REDUCE, 12 },
		{ -242, 1, YYAT_ERROR, 0 },
		{ 0, 0, YYAT_DEFAULT, 42 },
		{ 0, 0, YYAT_DEFAULT, 34 },
		{ 0, 0, YYAT_DEFAULT, 34 },
		{ -248, 1, YYAT_ERROR, 0 },
		{ -241, 1, YYAT_ERROR, 0 },
		{ 0, 0, YYAT_REDUCE, 13 },
		{ -239, 1, YYAT_REDUCE, 9 },
		{ -250, 1, YYAT_REDUCE, 16 },
		{ -252, 1, YYAT_REDUCE, 17 },
		{ -259, 1, YYAT_REDUCE, 21 },
		{ 0, 0, YYAT_REDUCE, 7 },
		{ -257, 1, YYAT_DEFAULT, 35 },
		{ 0, 0, YYAT_REDUCE, 10 }
	};
	yystateaction = stateaction;

	static const yynontermgoto_t YYNEARFAR YYBASED_CODE nontermgoto[] = {
		{ 20, 30 },
		{ 20, 24 },
		{ 20, 25 },
		{ 34, 40 },
		{ 20, 26 },
		{ 42, 43 },
		{ 42, 11 },
		{ 0, 1 },
		{ 0, 2 },
		{ 33, 39 },
		{ 42, 12 },
		{ 32, 38 },
		{ 31, 37 },
		{ 19, 29 },
		{ 17, 10 },
		{ 16, 27 },
		{ 14, 23 },
		{ 8, 17 }
	};
	yynontermgoto = nontermgoto;
	yynontermgoto_size = 18;

	static const yystategoto_t YYNEARFAR YYBASED_CODE stategoto[] = {
		{ 6, -1 },
		{ 0, -1 },
		{ 0, 17 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 15, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 11, 20 },
		{ 0, -1 },
		{ 6, -1 },
		{ 11, 42 },
		{ 0, -1 },
		{ 4, -1 },
		{ -5, -1 },
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
		{ 9, 42 },
		{ 2, -1 },
		{ 0, -1 },
		{ -6, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 0, -1 },
		{ 2, -1 },
		{ 0, -1 }
	};
	yystategoto = stategoto;

	yydestructorptr = NULL;

	yytokendestptr = NULL;
	yytokendest_size = 0;
	yytokendestbase = 0;
}
