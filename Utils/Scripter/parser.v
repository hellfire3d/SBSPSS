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
   11            | BEGIN_CS statement_list END_CS
   12            | function END_STMT

   13  assign_expression : variable ASSIGN value

   14  expression : OPEN_PAR expression CLOSE_PAR
   15             | equal_expression
   16             | notequal_expression

   17  equal_expression : value EQUAL value

   18  notequal_expression : value NOTEQUAL value

   19  variable : VARIABLE

   20  value : VALUE
   21        | VARIABLE
   22        | value PLUS value
   23        | function

   24  $$1 :

   25  function : FUNCTION OPEN_PAR $$1 CLOSE_PAR


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
	PAUSE  shift 5
	PRINT  shift 6
	END_STMT  shift 7
	BEGIN_CS  shift 8
	VARIABLE  shift 9
	FUNCTION  shift 10
	.  reduce 1

	statement goto 11
	assign_expression goto 12
	variable goto 13
	function goto 14


state 3
	statement : STOP . END_STMT

	END_STMT  shift 15


state 4
	statement : IF . OPEN_PAR expression CLOSE_PAR statement
	statement : IF . OPEN_PAR expression CLOSE_PAR statement ELSE statement

	OPEN_PAR  shift 16


state 5
	statement : PAUSE . END_STMT

	END_STMT  shift 17


state 6
	statement : PRINT . OPEN_PAR value CLOSE_PAR END_STMT

	OPEN_PAR  shift 18


state 7
	statement : END_STMT .  (4)

	.  reduce 4


state 8
	statement : BEGIN_CS . statement_list END_CS
	statement_list : .  (3)

	.  reduce 3

	statement_list goto 19


state 9
	variable : VARIABLE .  (19)

	.  reduce 19


state 10
	function : FUNCTION . OPEN_PAR $$1 CLOSE_PAR

	OPEN_PAR  shift 20


state 11
	statement_list : statement_list statement .  (2)

	.  reduce 2


state 12
	statement : assign_expression . END_STMT

	END_STMT  shift 21


state 13
	assign_expression : variable . ASSIGN value

	ASSIGN  shift 22


state 14
	statement : function . END_STMT

	END_STMT  shift 23


state 15
	statement : STOP END_STMT .  (5)

	.  reduce 5


state 16
	statement : IF OPEN_PAR . expression CLOSE_PAR statement
	statement : IF OPEN_PAR . expression CLOSE_PAR statement ELSE statement

	OPEN_PAR  shift 24
	VARIABLE  shift 25
	VALUE  shift 26
	FUNCTION  shift 10

	expression goto 27
	equal_expression goto 28
	notequal_expression goto 29
	value goto 30
	function goto 31


state 17
	statement : PAUSE END_STMT .  (6)

	.  reduce 6


state 18
	statement : PRINT OPEN_PAR . value CLOSE_PAR END_STMT

	VARIABLE  shift 25
	VALUE  shift 26
	FUNCTION  shift 10

	value goto 32
	function goto 31


state 19
	statement_list : statement_list . statement
	statement : BEGIN_CS statement_list . END_CS

	STOP  shift 3
	IF  shift 4
	PAUSE  shift 5
	PRINT  shift 6
	END_STMT  shift 7
	BEGIN_CS  shift 8
	END_CS  shift 33
	VARIABLE  shift 9
	FUNCTION  shift 10

	statement goto 11
	assign_expression goto 12
	variable goto 13
	function goto 14


state 20
	function : FUNCTION OPEN_PAR . $$1 CLOSE_PAR
	$$1 : .  (24)

	.  reduce 24

	$$1 goto 34


state 21
	statement : assign_expression END_STMT .  (8)

	.  reduce 8


state 22
	assign_expression : variable ASSIGN . value

	VARIABLE  shift 25
	VALUE  shift 26
	FUNCTION  shift 10

	value goto 35
	function goto 31


state 23
	statement : function END_STMT .  (12)

	.  reduce 12


state 24
	expression : OPEN_PAR . expression CLOSE_PAR

	OPEN_PAR  shift 24
	VARIABLE  shift 25
	VALUE  shift 26
	FUNCTION  shift 10

	expression goto 36
	equal_expression goto 28
	notequal_expression goto 29
	value goto 30
	function goto 31


state 25
	value : VARIABLE .  (21)

	.  reduce 21


state 26
	value : VALUE .  (20)

	.  reduce 20


state 27
	statement : IF OPEN_PAR expression . CLOSE_PAR statement
	statement : IF OPEN_PAR expression . CLOSE_PAR statement ELSE statement

	CLOSE_PAR  shift 37


state 28
	expression : equal_expression .  (15)

	.  reduce 15


state 29
	expression : notequal_expression .  (16)

	.  reduce 16


state 30
	equal_expression : value . EQUAL value
	notequal_expression : value . NOTEQUAL value
	value : value . PLUS value

	EQUAL  shift 38
	NOTEQUAL  shift 39
	PLUS  shift 40


state 31
	value : function .  (23)

	.  reduce 23


state 32
	statement : PRINT OPEN_PAR value . CLOSE_PAR END_STMT
	value : value . PLUS value

	PLUS  shift 40
	CLOSE_PAR  shift 41


state 33
	statement : BEGIN_CS statement_list END_CS .  (11)

	.  reduce 11


state 34
	function : FUNCTION OPEN_PAR $$1 . CLOSE_PAR

	CLOSE_PAR  shift 42


state 35
	assign_expression : variable ASSIGN value .  (13)
	value : value . PLUS value

	PLUS  shift 40
	.  reduce 13


state 36
	expression : OPEN_PAR expression . CLOSE_PAR

	CLOSE_PAR  shift 43


state 37
	statement : IF OPEN_PAR expression CLOSE_PAR . statement
	statement : IF OPEN_PAR expression CLOSE_PAR . statement ELSE statement

	STOP  shift 3
	IF  shift 4
	PAUSE  shift 5
	PRINT  shift 6
	END_STMT  shift 7
	BEGIN_CS  shift 8
	VARIABLE  shift 9
	FUNCTION  shift 10

	statement goto 44
	assign_expression goto 12
	variable goto 13
	function goto 14


state 38
	equal_expression : value EQUAL . value

	VARIABLE  shift 25
	VALUE  shift 26
	FUNCTION  shift 10

	value goto 45
	function goto 31


state 39
	notequal_expression : value NOTEQUAL . value

	VARIABLE  shift 25
	VALUE  shift 26
	FUNCTION  shift 10

	value goto 46
	function goto 31


state 40
	value : value PLUS . value

	VARIABLE  shift 25
	VALUE  shift 26
	FUNCTION  shift 10

	value goto 47
	function goto 31


state 41
	statement : PRINT OPEN_PAR value CLOSE_PAR . END_STMT

	END_STMT  shift 48


state 42
	function : FUNCTION OPEN_PAR $$1 CLOSE_PAR .  (25)

	.  reduce 25


state 43
	expression : OPEN_PAR expression CLOSE_PAR .  (14)

	.  reduce 14


44: shift-reduce conflict (shift 49, reduce 9) on ELSE
state 44
	statement : IF OPEN_PAR expression CLOSE_PAR statement .  (9)
	statement : IF OPEN_PAR expression CLOSE_PAR statement . ELSE statement

	ELSE  shift 49
	.  reduce 9


state 45
	equal_expression : value EQUAL value .  (17)
	value : value . PLUS value

	PLUS  shift 40
	.  reduce 17


state 46
	notequal_expression : value NOTEQUAL value .  (18)
	value : value . PLUS value

	PLUS  shift 40
	.  reduce 18


47: shift-reduce conflict (shift 40, reduce 22) on PLUS
state 47
	value : value PLUS value .  (22)
	value : value . PLUS value

	PLUS  shift 40
	.  reduce 22


state 48
	statement : PRINT OPEN_PAR value CLOSE_PAR END_STMT .  (7)

	.  reduce 7


state 49
	statement : IF OPEN_PAR expression CLOSE_PAR statement ELSE . statement

	STOP  shift 3
	IF  shift 4
	PAUSE  shift 5
	PRINT  shift 6
	END_STMT  shift 7
	BEGIN_CS  shift 8
	VARIABLE  shift 9
	FUNCTION  shift 10

	statement goto 50
	assign_expression goto 12
	variable goto 13
	function goto 14


state 50
	statement : IF OPEN_PAR expression CLOSE_PAR statement ELSE statement .  (10)

	.  reduce 10


State 44 contains 1 shift-reduce conflict
State 47 contains 1 shift-reduce conflict


19 tokens, 12 nonterminals
26 grammar rules, 51 states
