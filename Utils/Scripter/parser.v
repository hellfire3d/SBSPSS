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

   12  assign_expression : variable ASSIGN value

   13  expression : OPEN_PAR expression CLOSE_PAR
   14             | equal_expression
   15             | notequal_expression

   16  equal_expression : value EQUAL value

   17  notequal_expression : value NOTEQUAL value

   18  variable : VARIABLE

   19  value : VALUE
   20        | VARIABLE
   21        | value PLUS value


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
	.  reduce 1

	statement goto 10
	assign_expression goto 11
	variable goto 12


state 3
	statement : STOP . END_STMT

	END_STMT  shift 13


state 4
	statement : IF . OPEN_PAR expression CLOSE_PAR statement
	statement : IF . OPEN_PAR expression CLOSE_PAR statement ELSE statement

	OPEN_PAR  shift 14


state 5
	statement : PAUSE . END_STMT

	END_STMT  shift 15


state 6
	statement : PRINT . OPEN_PAR value CLOSE_PAR END_STMT

	OPEN_PAR  shift 16


state 7
	statement : END_STMT .  (4)

	.  reduce 4


state 8
	statement : BEGIN_CS . statement_list END_CS
	statement_list : .  (3)

	.  reduce 3

	statement_list goto 17


state 9
	variable : VARIABLE .  (18)

	.  reduce 18


state 10
	statement_list : statement_list statement .  (2)

	.  reduce 2


state 11
	statement : assign_expression . END_STMT

	END_STMT  shift 18


state 12
	assign_expression : variable . ASSIGN value

	ASSIGN  shift 19


state 13
	statement : STOP END_STMT .  (5)

	.  reduce 5


state 14
	statement : IF OPEN_PAR . expression CLOSE_PAR statement
	statement : IF OPEN_PAR . expression CLOSE_PAR statement ELSE statement

	OPEN_PAR  shift 20
	VARIABLE  shift 21
	VALUE  shift 22

	expression goto 23
	equal_expression goto 24
	notequal_expression goto 25
	value goto 26


state 15
	statement : PAUSE END_STMT .  (6)

	.  reduce 6


state 16
	statement : PRINT OPEN_PAR . value CLOSE_PAR END_STMT

	VARIABLE  shift 21
	VALUE  shift 22

	value goto 27


state 17
	statement_list : statement_list . statement
	statement : BEGIN_CS statement_list . END_CS

	STOP  shift 3
	IF  shift 4
	PAUSE  shift 5
	PRINT  shift 6
	END_STMT  shift 7
	BEGIN_CS  shift 8
	END_CS  shift 28
	VARIABLE  shift 9

	statement goto 10
	assign_expression goto 11
	variable goto 12


state 18
	statement : assign_expression END_STMT .  (8)

	.  reduce 8


state 19
	assign_expression : variable ASSIGN . value

	VARIABLE  shift 21
	VALUE  shift 22

	value goto 29


state 20
	expression : OPEN_PAR . expression CLOSE_PAR

	OPEN_PAR  shift 20
	VARIABLE  shift 21
	VALUE  shift 22

	expression goto 30
	equal_expression goto 24
	notequal_expression goto 25
	value goto 26


state 21
	value : VARIABLE .  (20)

	.  reduce 20


state 22
	value : VALUE .  (19)

	.  reduce 19


state 23
	statement : IF OPEN_PAR expression . CLOSE_PAR statement
	statement : IF OPEN_PAR expression . CLOSE_PAR statement ELSE statement

	CLOSE_PAR  shift 31


state 24
	expression : equal_expression .  (14)

	.  reduce 14


state 25
	expression : notequal_expression .  (15)

	.  reduce 15


state 26
	equal_expression : value . EQUAL value
	notequal_expression : value . NOTEQUAL value
	value : value . PLUS value

	EQUAL  shift 32
	NOTEQUAL  shift 33
	PLUS  shift 34


state 27
	statement : PRINT OPEN_PAR value . CLOSE_PAR END_STMT
	value : value . PLUS value

	PLUS  shift 34
	CLOSE_PAR  shift 35


state 28
	statement : BEGIN_CS statement_list END_CS .  (11)

	.  reduce 11


state 29
	assign_expression : variable ASSIGN value .  (12)
	value : value . PLUS value

	PLUS  shift 34
	.  reduce 12


state 30
	expression : OPEN_PAR expression . CLOSE_PAR

	CLOSE_PAR  shift 36


state 31
	statement : IF OPEN_PAR expression CLOSE_PAR . statement
	statement : IF OPEN_PAR expression CLOSE_PAR . statement ELSE statement

	STOP  shift 3
	IF  shift 4
	PAUSE  shift 5
	PRINT  shift 6
	END_STMT  shift 7
	BEGIN_CS  shift 8
	VARIABLE  shift 9

	statement goto 37
	assign_expression goto 11
	variable goto 12


state 32
	equal_expression : value EQUAL . value

	VARIABLE  shift 21
	VALUE  shift 22

	value goto 38


state 33
	notequal_expression : value NOTEQUAL . value

	VARIABLE  shift 21
	VALUE  shift 22

	value goto 39


state 34
	value : value PLUS . value

	VARIABLE  shift 21
	VALUE  shift 22

	value goto 40


state 35
	statement : PRINT OPEN_PAR value CLOSE_PAR . END_STMT

	END_STMT  shift 41


state 36
	expression : OPEN_PAR expression CLOSE_PAR .  (13)

	.  reduce 13


37: shift-reduce conflict (shift 42, reduce 9) on ELSE
state 37
	statement : IF OPEN_PAR expression CLOSE_PAR statement .  (9)
	statement : IF OPEN_PAR expression CLOSE_PAR statement . ELSE statement

	ELSE  shift 42
	.  reduce 9


state 38
	equal_expression : value EQUAL value .  (16)
	value : value . PLUS value

	PLUS  shift 34
	.  reduce 16


state 39
	notequal_expression : value NOTEQUAL value .  (17)
	value : value . PLUS value

	PLUS  shift 34
	.  reduce 17


40: shift-reduce conflict (shift 34, reduce 21) on PLUS
state 40
	value : value PLUS value .  (21)
	value : value . PLUS value

	PLUS  shift 34
	.  reduce 21


state 41
	statement : PRINT OPEN_PAR value CLOSE_PAR END_STMT .  (7)

	.  reduce 7


state 42
	statement : IF OPEN_PAR expression CLOSE_PAR statement ELSE . statement

	STOP  shift 3
	IF  shift 4
	PAUSE  shift 5
	PRINT  shift 6
	END_STMT  shift 7
	BEGIN_CS  shift 8
	VARIABLE  shift 9

	statement goto 43
	assign_expression goto 11
	variable goto 12


state 43
	statement : IF OPEN_PAR expression CLOSE_PAR statement ELSE statement .  (10)

	.  reduce 10


State 37 contains 1 shift-reduce conflict
State 40 contains 1 shift-reduce conflict


18 tokens, 10 nonterminals
22 grammar rules, 44 states
