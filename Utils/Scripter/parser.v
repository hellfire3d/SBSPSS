    0  $accept : program $end

    1  program : statement_list

    2  statement_list : statement_list statement
    3                 |

    4  statement : END_STMT
    5            | STOP END_STMT
    6            | PAUSE END_STMT
    7            | PRINT OPEN_PAR value CLOSE_PAR END_STMT
    8            | assign_expression END_STMT
    9            | IF OPEN_PAR expression CLOSE_PAR statement
   10            | IF OPEN_PAR expression CLOSE_PAR statement ELSE statement
   11            | WHILE OPEN_PAR expression CLOSE_PAR statement
   12            | DO statement WHILE OPEN_PAR expression CLOSE_PAR END_STMT
   13            | BEGIN_CS statement_list END_CS
   14            | function END_STMT

   15  assign_expression : variable ASSIGN value

   16  expression : equal_expression
   17             | notequal_expression
   18             | lessthan_expression
   19             | greaterthan_expression

   20  equal_expression : value EQUAL value

   21  notequal_expression : value NOTEQUAL value

   22  lessthan_expression : value LESSTHAN value

   23  greaterthan_expression : value GREATERTHAN value

   24  variable : VARIABLE

   25  value : VALUE
   26        | OPEN_PAR value CLOSE_PAR
   27        | VARIABLE
   28        | value PLUS value
   29        | value SUBTRACT value
   30        | value MULTIPLY value
   31        | value DIVIDE value
   32        | function

   33  $$1 :

   34  function : FUNCTION OPEN_PAR $$1 CLOSE_PAR


state 0
	$accept : . program $end
	statement_list : .  (3)

	.  reduce 3

	program goto 1
	statement_list goto 2


state 1
	$accept : program . $end  (0)

	$end  accept


state 2
	program : statement_list .  (1)
	statement_list : statement_list . statement

	STOP  shift 3
	IF  shift 4
	WHILE  shift 5
	DO  shift 6
	PAUSE  shift 7
	PRINT  shift 8
	END_STMT  shift 9
	BEGIN_CS  shift 10
	VARIABLE  shift 11
	FUNCTION  shift 12
	.  reduce 1

	statement goto 13
	assign_expression goto 14
	variable goto 15
	function goto 16


state 3
	statement : STOP . END_STMT

	END_STMT  shift 17


state 4
	statement : IF . OPEN_PAR expression CLOSE_PAR statement
	statement : IF . OPEN_PAR expression CLOSE_PAR statement ELSE statement

	OPEN_PAR  shift 18


state 5
	statement : WHILE . OPEN_PAR expression CLOSE_PAR statement

	OPEN_PAR  shift 19


state 6
	statement : DO . statement WHILE OPEN_PAR expression CLOSE_PAR END_STMT

	STOP  shift 3
	IF  shift 4
	WHILE  shift 5
	DO  shift 6
	PAUSE  shift 7
	PRINT  shift 8
	END_STMT  shift 9
	BEGIN_CS  shift 10
	VARIABLE  shift 11
	FUNCTION  shift 12

	statement goto 20
	assign_expression goto 14
	variable goto 15
	function goto 16


state 7
	statement : PAUSE . END_STMT

	END_STMT  shift 21


state 8
	statement : PRINT . OPEN_PAR value CLOSE_PAR END_STMT

	OPEN_PAR  shift 22


state 9
	statement : END_STMT .  (4)

	.  reduce 4


state 10
	statement : BEGIN_CS . statement_list END_CS
	statement_list : .  (3)

	.  reduce 3

	statement_list goto 23


state 11
	variable : VARIABLE .  (24)

	.  reduce 24


state 12
	function : FUNCTION . OPEN_PAR $$1 CLOSE_PAR

	OPEN_PAR  shift 24


state 13
	statement_list : statement_list statement .  (2)

	.  reduce 2


state 14
	statement : assign_expression . END_STMT

	END_STMT  shift 25


state 15
	assign_expression : variable . ASSIGN value

	ASSIGN  shift 26


state 16
	statement : function . END_STMT

	END_STMT  shift 27


state 17
	statement : STOP END_STMT .  (5)

	.  reduce 5


state 18
	statement : IF OPEN_PAR . expression CLOSE_PAR statement
	statement : IF OPEN_PAR . expression CLOSE_PAR statement ELSE statement

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	expression goto 31
	equal_expression goto 32
	notequal_expression goto 33
	lessthan_expression goto 34
	greaterthan_expression goto 35
	value goto 36
	function goto 37


state 19
	statement : WHILE OPEN_PAR . expression CLOSE_PAR statement

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	expression goto 38
	equal_expression goto 32
	notequal_expression goto 33
	lessthan_expression goto 34
	greaterthan_expression goto 35
	value goto 36
	function goto 37


state 20
	statement : DO statement . WHILE OPEN_PAR expression CLOSE_PAR END_STMT

	WHILE  shift 39


state 21
	statement : PAUSE END_STMT .  (6)

	.  reduce 6


state 22
	statement : PRINT OPEN_PAR . value CLOSE_PAR END_STMT

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	value goto 40
	function goto 37


state 23
	statement_list : statement_list . statement
	statement : BEGIN_CS statement_list . END_CS

	STOP  shift 3
	IF  shift 4
	WHILE  shift 5
	DO  shift 6
	PAUSE  shift 7
	PRINT  shift 8
	END_STMT  shift 9
	BEGIN_CS  shift 10
	END_CS  shift 41
	VARIABLE  shift 11
	FUNCTION  shift 12

	statement goto 13
	assign_expression goto 14
	variable goto 15
	function goto 16


state 24
	function : FUNCTION OPEN_PAR . $$1 CLOSE_PAR
	$$1 : .  (33)

	.  reduce 33

	$$1 goto 42


state 25
	statement : assign_expression END_STMT .  (8)

	.  reduce 8


state 26
	assign_expression : variable ASSIGN . value

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	value goto 43
	function goto 37


state 27
	statement : function END_STMT .  (14)

	.  reduce 14


state 28
	value : OPEN_PAR . value CLOSE_PAR

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	value goto 44
	function goto 37


state 29
	value : VARIABLE .  (27)

	.  reduce 27


state 30
	value : VALUE .  (25)

	.  reduce 25


state 31
	statement : IF OPEN_PAR expression . CLOSE_PAR statement
	statement : IF OPEN_PAR expression . CLOSE_PAR statement ELSE statement

	CLOSE_PAR  shift 45


state 32
	expression : equal_expression .  (16)

	.  reduce 16


state 33
	expression : notequal_expression .  (17)

	.  reduce 17


state 34
	expression : lessthan_expression .  (18)

	.  reduce 18


state 35
	expression : greaterthan_expression .  (19)

	.  reduce 19


state 36
	equal_expression : value . EQUAL value
	notequal_expression : value . NOTEQUAL value
	lessthan_expression : value . LESSTHAN value
	greaterthan_expression : value . GREATERTHAN value
	value : value . PLUS value
	value : value . SUBTRACT value
	value : value . MULTIPLY value
	value : value . DIVIDE value

	EQUAL  shift 46
	NOTEQUAL  shift 47
	LESSTHAN  shift 48
	GREATERTHAN  shift 49
	PLUS  shift 50
	SUBTRACT  shift 51
	MULTIPLY  shift 52
	DIVIDE  shift 53


state 37
	value : function .  (32)

	.  reduce 32


state 38
	statement : WHILE OPEN_PAR expression . CLOSE_PAR statement

	CLOSE_PAR  shift 54


state 39
	statement : DO statement WHILE . OPEN_PAR expression CLOSE_PAR END_STMT

	OPEN_PAR  shift 55


state 40
	statement : PRINT OPEN_PAR value . CLOSE_PAR END_STMT
	value : value . PLUS value
	value : value . SUBTRACT value
	value : value . MULTIPLY value
	value : value . DIVIDE value

	PLUS  shift 50
	SUBTRACT  shift 51
	MULTIPLY  shift 52
	DIVIDE  shift 53
	CLOSE_PAR  shift 56


state 41
	statement : BEGIN_CS statement_list END_CS .  (13)

	.  reduce 13


state 42
	function : FUNCTION OPEN_PAR $$1 . CLOSE_PAR

	CLOSE_PAR  shift 57


state 43
	assign_expression : variable ASSIGN value .  (15)
	value : value . PLUS value
	value : value . SUBTRACT value
	value : value . MULTIPLY value
	value : value . DIVIDE value

	PLUS  shift 50
	SUBTRACT  shift 51
	MULTIPLY  shift 52
	DIVIDE  shift 53
	.  reduce 15


state 44
	value : OPEN_PAR value . CLOSE_PAR
	value : value . PLUS value
	value : value . SUBTRACT value
	value : value . MULTIPLY value
	value : value . DIVIDE value

	PLUS  shift 50
	SUBTRACT  shift 51
	MULTIPLY  shift 52
	DIVIDE  shift 53
	CLOSE_PAR  shift 58


state 45
	statement : IF OPEN_PAR expression CLOSE_PAR . statement
	statement : IF OPEN_PAR expression CLOSE_PAR . statement ELSE statement

	STOP  shift 3
	IF  shift 4
	WHILE  shift 5
	DO  shift 6
	PAUSE  shift 7
	PRINT  shift 8
	END_STMT  shift 9
	BEGIN_CS  shift 10
	VARIABLE  shift 11
	FUNCTION  shift 12

	statement goto 59
	assign_expression goto 14
	variable goto 15
	function goto 16


state 46
	equal_expression : value EQUAL . value

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	value goto 60
	function goto 37


state 47
	notequal_expression : value NOTEQUAL . value

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	value goto 61
	function goto 37


state 48
	lessthan_expression : value LESSTHAN . value

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	value goto 62
	function goto 37


state 49
	greaterthan_expression : value GREATERTHAN . value

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	value goto 63
	function goto 37


state 50
	value : value PLUS . value

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	value goto 64
	function goto 37


state 51
	value : value SUBTRACT . value

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	value goto 65
	function goto 37


state 52
	value : value MULTIPLY . value

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	value goto 66
	function goto 37


state 53
	value : value DIVIDE . value

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	value goto 67
	function goto 37


state 54
	statement : WHILE OPEN_PAR expression CLOSE_PAR . statement

	STOP  shift 3
	IF  shift 4
	WHILE  shift 5
	DO  shift 6
	PAUSE  shift 7
	PRINT  shift 8
	END_STMT  shift 9
	BEGIN_CS  shift 10
	VARIABLE  shift 11
	FUNCTION  shift 12

	statement goto 68
	assign_expression goto 14
	variable goto 15
	function goto 16


state 55
	statement : DO statement WHILE OPEN_PAR . expression CLOSE_PAR END_STMT

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	expression goto 69
	equal_expression goto 32
	notequal_expression goto 33
	lessthan_expression goto 34
	greaterthan_expression goto 35
	value goto 36
	function goto 37


state 56
	statement : PRINT OPEN_PAR value CLOSE_PAR . END_STMT

	END_STMT  shift 70


state 57
	function : FUNCTION OPEN_PAR $$1 CLOSE_PAR .  (34)

	.  reduce 34


state 58
	value : OPEN_PAR value CLOSE_PAR .  (26)

	.  reduce 26


59: shift-reduce conflict (shift 71, reduce 9) on ELSE
state 59
	statement : IF OPEN_PAR expression CLOSE_PAR statement .  (9)
	statement : IF OPEN_PAR expression CLOSE_PAR statement . ELSE statement

	ELSE  shift 71
	.  reduce 9


state 60
	equal_expression : value EQUAL value .  (20)
	value : value . PLUS value
	value : value . SUBTRACT value
	value : value . MULTIPLY value
	value : value . DIVIDE value

	PLUS  shift 50
	SUBTRACT  shift 51
	MULTIPLY  shift 52
	DIVIDE  shift 53
	.  reduce 20


state 61
	notequal_expression : value NOTEQUAL value .  (21)
	value : value . PLUS value
	value : value . SUBTRACT value
	value : value . MULTIPLY value
	value : value . DIVIDE value

	PLUS  shift 50
	SUBTRACT  shift 51
	MULTIPLY  shift 52
	DIVIDE  shift 53
	.  reduce 21


state 62
	lessthan_expression : value LESSTHAN value .  (22)
	value : value . PLUS value
	value : value . SUBTRACT value
	value : value . MULTIPLY value
	value : value . DIVIDE value

	PLUS  shift 50
	SUBTRACT  shift 51
	MULTIPLY  shift 52
	DIVIDE  shift 53
	.  reduce 22


state 63
	greaterthan_expression : value GREATERTHAN value .  (23)
	value : value . PLUS value
	value : value . SUBTRACT value
	value : value . MULTIPLY value
	value : value . DIVIDE value

	PLUS  shift 50
	SUBTRACT  shift 51
	MULTIPLY  shift 52
	DIVIDE  shift 53
	.  reduce 23


64: shift-reduce conflict (shift 50, reduce 28) on PLUS
64: shift-reduce conflict (shift 51, reduce 28) on SUBTRACT
64: shift-reduce conflict (shift 52, reduce 28) on MULTIPLY
64: shift-reduce conflict (shift 53, reduce 28) on DIVIDE
state 64
	value : value PLUS value .  (28)
	value : value . PLUS value
	value : value . SUBTRACT value
	value : value . MULTIPLY value
	value : value . DIVIDE value

	PLUS  shift 50
	SUBTRACT  shift 51
	MULTIPLY  shift 52
	DIVIDE  shift 53
	.  reduce 28


65: shift-reduce conflict (shift 50, reduce 29) on PLUS
65: shift-reduce conflict (shift 51, reduce 29) on SUBTRACT
65: shift-reduce conflict (shift 52, reduce 29) on MULTIPLY
65: shift-reduce conflict (shift 53, reduce 29) on DIVIDE
state 65
	value : value . PLUS value
	value : value SUBTRACT value .  (29)
	value : value . SUBTRACT value
	value : value . MULTIPLY value
	value : value . DIVIDE value

	PLUS  shift 50
	SUBTRACT  shift 51
	MULTIPLY  shift 52
	DIVIDE  shift 53
	.  reduce 29


66: shift-reduce conflict (shift 50, reduce 30) on PLUS
66: shift-reduce conflict (shift 51, reduce 30) on SUBTRACT
66: shift-reduce conflict (shift 52, reduce 30) on MULTIPLY
66: shift-reduce conflict (shift 53, reduce 30) on DIVIDE
state 66
	value : value . PLUS value
	value : value . SUBTRACT value
	value : value MULTIPLY value .  (30)
	value : value . MULTIPLY value
	value : value . DIVIDE value

	PLUS  shift 50
	SUBTRACT  shift 51
	MULTIPLY  shift 52
	DIVIDE  shift 53
	.  reduce 30


67: shift-reduce conflict (shift 50, reduce 31) on PLUS
67: shift-reduce conflict (shift 51, reduce 31) on SUBTRACT
67: shift-reduce conflict (shift 52, reduce 31) on MULTIPLY
67: shift-reduce conflict (shift 53, reduce 31) on DIVIDE
state 67
	value : value . PLUS value
	value : value . SUBTRACT value
	value : value . MULTIPLY value
	value : value DIVIDE value .  (31)
	value : value . DIVIDE value

	PLUS  shift 50
	SUBTRACT  shift 51
	MULTIPLY  shift 52
	DIVIDE  shift 53
	.  reduce 31


state 68
	statement : WHILE OPEN_PAR expression CLOSE_PAR statement .  (11)

	.  reduce 11


state 69
	statement : DO statement WHILE OPEN_PAR expression . CLOSE_PAR END_STMT

	CLOSE_PAR  shift 72


state 70
	statement : PRINT OPEN_PAR value CLOSE_PAR END_STMT .  (7)

	.  reduce 7


state 71
	statement : IF OPEN_PAR expression CLOSE_PAR statement ELSE . statement

	STOP  shift 3
	IF  shift 4
	WHILE  shift 5
	DO  shift 6
	PAUSE  shift 7
	PRINT  shift 8
	END_STMT  shift 9
	BEGIN_CS  shift 10
	VARIABLE  shift 11
	FUNCTION  shift 12

	statement goto 73
	assign_expression goto 14
	variable goto 15
	function goto 16


state 72
	statement : DO statement WHILE OPEN_PAR expression CLOSE_PAR . END_STMT

	END_STMT  shift 74


state 73
	statement : IF OPEN_PAR expression CLOSE_PAR statement ELSE statement .  (10)

	.  reduce 10


state 74
	statement : DO statement WHILE OPEN_PAR expression CLOSE_PAR END_STMT .  (12)

	.  reduce 12


State 59 contains 1 shift-reduce conflict
State 64 contains 4 shift-reduce conflicts
State 65 contains 4 shift-reduce conflicts
State 66 contains 4 shift-reduce conflicts
State 67 contains 4 shift-reduce conflicts


26 tokens, 14 nonterminals
35 grammar rules, 75 states
