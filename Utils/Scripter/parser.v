    0  $accept : program $end

    1  program : statement_list

    2  statement_list : statement_list statement
    3                 |

    4  statement : END_STMT
    5            | STOP END_STMT
    6            | PAUSE END_STMT
    7            | PRINT OPEN_PAR expression CLOSE_PAR END_STMT
    8            | assign_expression END_STMT
    9            | IF OPEN_PAR conditional CLOSE_PAR statement
   10            | IF OPEN_PAR conditional CLOSE_PAR statement ELSE statement
   11            | WHILE OPEN_PAR conditional CLOSE_PAR statement
   12            | DO statement WHILE OPEN_PAR conditional CLOSE_PAR END_STMT
   13            | BEGIN_CS statement_list END_CS
   14            | function END_STMT

   15  assign_expression : variable ASSIGN expression

   16  conditional : expression EQUAL expression
   17              | expression NOTEQUAL expression
   18              | expression LESSTHAN expression
   19              | expression GREATERTHAN expression

   20  variable : VARIABLE

   21  expression : VALUE
   22             | OPEN_PAR expression CLOSE_PAR
   23             | VARIABLE
   24             | expression PLUS expression
   25             | expression SUBTRACT expression
   26             | expression MULTIPLY expression
   27             | expression DIVIDE expression
   28             | function

   29  $$1 :

   30  function : FUNCTION OPEN_PAR $$1 CLOSE_PAR


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
	statement : IF . OPEN_PAR conditional CLOSE_PAR statement
	statement : IF . OPEN_PAR conditional CLOSE_PAR statement ELSE statement

	OPEN_PAR  shift 18


state 5
	statement : WHILE . OPEN_PAR conditional CLOSE_PAR statement

	OPEN_PAR  shift 19


state 6
	statement : DO . statement WHILE OPEN_PAR conditional CLOSE_PAR END_STMT

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
	statement : PRINT . OPEN_PAR expression CLOSE_PAR END_STMT

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
	variable : VARIABLE .  (20)

	.  reduce 20


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
	assign_expression : variable . ASSIGN expression

	ASSIGN  shift 26


state 16
	statement : function . END_STMT

	END_STMT  shift 27


state 17
	statement : STOP END_STMT .  (5)

	.  reduce 5


state 18
	statement : IF OPEN_PAR . conditional CLOSE_PAR statement
	statement : IF OPEN_PAR . conditional CLOSE_PAR statement ELSE statement

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	conditional goto 31
	expression goto 32
	function goto 33


state 19
	statement : WHILE OPEN_PAR . conditional CLOSE_PAR statement

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	conditional goto 34
	expression goto 32
	function goto 33


state 20
	statement : DO statement . WHILE OPEN_PAR conditional CLOSE_PAR END_STMT

	WHILE  shift 35


state 21
	statement : PAUSE END_STMT .  (6)

	.  reduce 6


state 22
	statement : PRINT OPEN_PAR . expression CLOSE_PAR END_STMT

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	expression goto 36
	function goto 33


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
	END_CS  shift 37
	VARIABLE  shift 11
	FUNCTION  shift 12

	statement goto 13
	assign_expression goto 14
	variable goto 15
	function goto 16


state 24
	function : FUNCTION OPEN_PAR . $$1 CLOSE_PAR
	$$1 : .  (29)

	.  reduce 29

	$$1 goto 38


state 25
	statement : assign_expression END_STMT .  (8)

	.  reduce 8


state 26
	assign_expression : variable ASSIGN . expression

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	expression goto 39
	function goto 33


state 27
	statement : function END_STMT .  (14)

	.  reduce 14


state 28
	expression : OPEN_PAR . expression CLOSE_PAR

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	expression goto 40
	function goto 33


state 29
	expression : VARIABLE .  (23)

	.  reduce 23


state 30
	expression : VALUE .  (21)

	.  reduce 21


state 31
	statement : IF OPEN_PAR conditional . CLOSE_PAR statement
	statement : IF OPEN_PAR conditional . CLOSE_PAR statement ELSE statement

	CLOSE_PAR  shift 41


state 32
	conditional : expression . EQUAL expression
	conditional : expression . NOTEQUAL expression
	conditional : expression . LESSTHAN expression
	conditional : expression . GREATERTHAN expression
	expression : expression . PLUS expression
	expression : expression . SUBTRACT expression
	expression : expression . MULTIPLY expression
	expression : expression . DIVIDE expression

	EQUAL  shift 42
	NOTEQUAL  shift 43
	LESSTHAN  shift 44
	GREATERTHAN  shift 45
	PLUS  shift 46
	SUBTRACT  shift 47
	MULTIPLY  shift 48
	DIVIDE  shift 49


state 33
	expression : function .  (28)

	.  reduce 28


state 34
	statement : WHILE OPEN_PAR conditional . CLOSE_PAR statement

	CLOSE_PAR  shift 50


state 35
	statement : DO statement WHILE . OPEN_PAR conditional CLOSE_PAR END_STMT

	OPEN_PAR  shift 51


state 36
	statement : PRINT OPEN_PAR expression . CLOSE_PAR END_STMT
	expression : expression . PLUS expression
	expression : expression . SUBTRACT expression
	expression : expression . MULTIPLY expression
	expression : expression . DIVIDE expression

	PLUS  shift 46
	SUBTRACT  shift 47
	MULTIPLY  shift 48
	DIVIDE  shift 49
	CLOSE_PAR  shift 52


state 37
	statement : BEGIN_CS statement_list END_CS .  (13)

	.  reduce 13


state 38
	function : FUNCTION OPEN_PAR $$1 . CLOSE_PAR

	CLOSE_PAR  shift 53


state 39
	assign_expression : variable ASSIGN expression .  (15)
	expression : expression . PLUS expression
	expression : expression . SUBTRACT expression
	expression : expression . MULTIPLY expression
	expression : expression . DIVIDE expression

	PLUS  shift 46
	SUBTRACT  shift 47
	MULTIPLY  shift 48
	DIVIDE  shift 49
	.  reduce 15


state 40
	expression : OPEN_PAR expression . CLOSE_PAR
	expression : expression . PLUS expression
	expression : expression . SUBTRACT expression
	expression : expression . MULTIPLY expression
	expression : expression . DIVIDE expression

	PLUS  shift 46
	SUBTRACT  shift 47
	MULTIPLY  shift 48
	DIVIDE  shift 49
	CLOSE_PAR  shift 54


state 41
	statement : IF OPEN_PAR conditional CLOSE_PAR . statement
	statement : IF OPEN_PAR conditional CLOSE_PAR . statement ELSE statement

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

	statement goto 55
	assign_expression goto 14
	variable goto 15
	function goto 16


state 42
	conditional : expression EQUAL . expression

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	expression goto 56
	function goto 33


state 43
	conditional : expression NOTEQUAL . expression

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	expression goto 57
	function goto 33


state 44
	conditional : expression LESSTHAN . expression

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	expression goto 58
	function goto 33


state 45
	conditional : expression GREATERTHAN . expression

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	expression goto 59
	function goto 33


state 46
	expression : expression PLUS . expression

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	expression goto 60
	function goto 33


state 47
	expression : expression SUBTRACT . expression

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	expression goto 61
	function goto 33


state 48
	expression : expression MULTIPLY . expression

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	expression goto 62
	function goto 33


state 49
	expression : expression DIVIDE . expression

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	expression goto 63
	function goto 33


state 50
	statement : WHILE OPEN_PAR conditional CLOSE_PAR . statement

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

	statement goto 64
	assign_expression goto 14
	variable goto 15
	function goto 16


state 51
	statement : DO statement WHILE OPEN_PAR . conditional CLOSE_PAR END_STMT

	OPEN_PAR  shift 28
	VARIABLE  shift 29
	VALUE  shift 30
	FUNCTION  shift 12

	conditional goto 65
	expression goto 32
	function goto 33


state 52
	statement : PRINT OPEN_PAR expression CLOSE_PAR . END_STMT

	END_STMT  shift 66


state 53
	function : FUNCTION OPEN_PAR $$1 CLOSE_PAR .  (30)

	.  reduce 30


state 54
	expression : OPEN_PAR expression CLOSE_PAR .  (22)

	.  reduce 22


55: shift-reduce conflict (shift 67, reduce 9) on ELSE
state 55
	statement : IF OPEN_PAR conditional CLOSE_PAR statement .  (9)
	statement : IF OPEN_PAR conditional CLOSE_PAR statement . ELSE statement

	ELSE  shift 67
	.  reduce 9


state 56
	conditional : expression EQUAL expression .  (16)
	expression : expression . PLUS expression
	expression : expression . SUBTRACT expression
	expression : expression . MULTIPLY expression
	expression : expression . DIVIDE expression

	PLUS  shift 46
	SUBTRACT  shift 47
	MULTIPLY  shift 48
	DIVIDE  shift 49
	.  reduce 16


state 57
	conditional : expression NOTEQUAL expression .  (17)
	expression : expression . PLUS expression
	expression : expression . SUBTRACT expression
	expression : expression . MULTIPLY expression
	expression : expression . DIVIDE expression

	PLUS  shift 46
	SUBTRACT  shift 47
	MULTIPLY  shift 48
	DIVIDE  shift 49
	.  reduce 17


state 58
	conditional : expression LESSTHAN expression .  (18)
	expression : expression . PLUS expression
	expression : expression . SUBTRACT expression
	expression : expression . MULTIPLY expression
	expression : expression . DIVIDE expression

	PLUS  shift 46
	SUBTRACT  shift 47
	MULTIPLY  shift 48
	DIVIDE  shift 49
	.  reduce 18


state 59
	conditional : expression GREATERTHAN expression .  (19)
	expression : expression . PLUS expression
	expression : expression . SUBTRACT expression
	expression : expression . MULTIPLY expression
	expression : expression . DIVIDE expression

	PLUS  shift 46
	SUBTRACT  shift 47
	MULTIPLY  shift 48
	DIVIDE  shift 49
	.  reduce 19


60: shift-reduce conflict (shift 46, reduce 24) on PLUS
60: shift-reduce conflict (shift 47, reduce 24) on SUBTRACT
60: shift-reduce conflict (shift 48, reduce 24) on MULTIPLY
60: shift-reduce conflict (shift 49, reduce 24) on DIVIDE
state 60
	expression : expression PLUS expression .  (24)
	expression : expression . PLUS expression
	expression : expression . SUBTRACT expression
	expression : expression . MULTIPLY expression
	expression : expression . DIVIDE expression

	PLUS  shift 46
	SUBTRACT  shift 47
	MULTIPLY  shift 48
	DIVIDE  shift 49
	.  reduce 24


61: shift-reduce conflict (shift 46, reduce 25) on PLUS
61: shift-reduce conflict (shift 47, reduce 25) on SUBTRACT
61: shift-reduce conflict (shift 48, reduce 25) on MULTIPLY
61: shift-reduce conflict (shift 49, reduce 25) on DIVIDE
state 61
	expression : expression . PLUS expression
	expression : expression SUBTRACT expression .  (25)
	expression : expression . SUBTRACT expression
	expression : expression . MULTIPLY expression
	expression : expression . DIVIDE expression

	PLUS  shift 46
	SUBTRACT  shift 47
	MULTIPLY  shift 48
	DIVIDE  shift 49
	.  reduce 25


62: shift-reduce conflict (shift 46, reduce 26) on PLUS
62: shift-reduce conflict (shift 47, reduce 26) on SUBTRACT
62: shift-reduce conflict (shift 48, reduce 26) on MULTIPLY
62: shift-reduce conflict (shift 49, reduce 26) on DIVIDE
state 62
	expression : expression . PLUS expression
	expression : expression . SUBTRACT expression
	expression : expression MULTIPLY expression .  (26)
	expression : expression . MULTIPLY expression
	expression : expression . DIVIDE expression

	PLUS  shift 46
	SUBTRACT  shift 47
	MULTIPLY  shift 48
	DIVIDE  shift 49
	.  reduce 26


63: shift-reduce conflict (shift 46, reduce 27) on PLUS
63: shift-reduce conflict (shift 47, reduce 27) on SUBTRACT
63: shift-reduce conflict (shift 48, reduce 27) on MULTIPLY
63: shift-reduce conflict (shift 49, reduce 27) on DIVIDE
state 63
	expression : expression . PLUS expression
	expression : expression . SUBTRACT expression
	expression : expression . MULTIPLY expression
	expression : expression DIVIDE expression .  (27)
	expression : expression . DIVIDE expression

	PLUS  shift 46
	SUBTRACT  shift 47
	MULTIPLY  shift 48
	DIVIDE  shift 49
	.  reduce 27


state 64
	statement : WHILE OPEN_PAR conditional CLOSE_PAR statement .  (11)

	.  reduce 11


state 65
	statement : DO statement WHILE OPEN_PAR conditional . CLOSE_PAR END_STMT

	CLOSE_PAR  shift 68


state 66
	statement : PRINT OPEN_PAR expression CLOSE_PAR END_STMT .  (7)

	.  reduce 7


state 67
	statement : IF OPEN_PAR conditional CLOSE_PAR statement ELSE . statement

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

	statement goto 69
	assign_expression goto 14
	variable goto 15
	function goto 16


state 68
	statement : DO statement WHILE OPEN_PAR conditional CLOSE_PAR . END_STMT

	END_STMT  shift 70


state 69
	statement : IF OPEN_PAR conditional CLOSE_PAR statement ELSE statement .  (10)

	.  reduce 10


state 70
	statement : DO statement WHILE OPEN_PAR conditional CLOSE_PAR END_STMT .  (12)

	.  reduce 12


State 55 contains 1 shift-reduce conflict
State 60 contains 4 shift-reduce conflicts
State 61 contains 4 shift-reduce conflicts
State 62 contains 4 shift-reduce conflicts
State 63 contains 4 shift-reduce conflicts


26 tokens, 10 nonterminals
31 grammar rules, 71 states
