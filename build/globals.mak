#----------------------------------------------------------------------------
#   globals.mak
#
#	Global defintions for all makefiles
#	Directory definitions and user preference inclusion
#
#	Tabbed for 5 9 (Please do not change)
#
#	Created:	19th April 1997 by G R Liddon @ Fareham
#
#	Copyright (C) 1997 G R Liddon
#----------------------------------------------------------------------------

ifndef __GLOBALS_MAK__
__GLOBALS_MAK__ := 1

#----------------------------------------------------------------------------
#	Useful Macros
#	-------------

NULL		:=
SPACE		:=	$(NULL) $(NULL)
TAB			:=	$(NULL)	$(NULL)
COMMA		:=	,
LBRACKET	:=	(
RBRACKET	:=	)
DOLLAR		:=	$$
SQUOTE		:=	'
DQUOTE		:=	"

define	NEWLINE


endef


#----------------------------------------------------------------------------
#	Directory Definitions
#	---------------------


#	Miscellaneous Dirs
#	------------------
TOOL_DIR :=				tools
GRAF_DIR :=				graphics
GAME_DATA_DIR :=		data
USERS_DIR :=			users
SOURCE_DIR :=			source
SOUND :=				sounds

#	Psyq Utilities and libraries
#	----------------------------
PSYQ_DIR :=				$(TOOL_DIR)/psyq
PSYQ_BIN_DIR :=			$(PSYQ_DIR)/bin
PSYQ_LIB_DIR :=			$(PSYQ_DIR)/lib
PSYQ_INC_DIR :=			$(PSYQ_DIR)/include

#	DDX Dodgy test kit
#	------------------
DDX_DIR :=				$(TOOL_DIR)/climax
DDX_BIN_DIR :=			$(DDX_DIR)/exes
DDX_INC_DIR :=			$(DDX_DIR)/libs
DDX_LIB_DIR :=			$(DDX_DIR)/libs

#	MOD - mod playing lib
#	---------------------
MOD_DIR :=				$(TOOL_DIR)/mod
MOD_INC_DIR :=			$(MOD_DIR)/include
MOD_LIB_DIR :=			$(MOD_DIR)/lib

#	VLC - FMV decoder
#	---------------------
VLC_DIR :=				$(TOOL_DIR)/vlc
VLC_INC_DIR :=			$(VLC_DIR)/include
VLC_LIB_DIR :=			$(VLC_DIR)/lib

#	Data tools and includes
#	---------------------
DATA_TOOL_DIR :=		$(TOOL_DIR)/data
DATA_BIN_DIR :=			$(DATA_TOOL_DIR)/bin
DATA_INC_DIR :=			$(DATA_TOOL_DIR)/include

MKLEVEL :=				$(DATA_BIN_DIR)/MkLevel
MKACTOR3D :=			$(DATA_BIN_DIR)/MkActor3d
MKANIM3D :=				$(DATA_BIN_DIR)/MkAnim3d

# Cygwin
# ------
CYG_DIR :=				$(TOOL_DIR)/cygwin
CYG_BIN_DIR :=			$(CYG_DIR)

# Perl
# ----
PERL_DIR :=				$(TOOL_DIR)/perl
PERL_BIN_DIR :=			$(PERL_DIR)/bin
PERL_SCRIPT_DIR :=		$(PERL_DIR)/pl

# Cygwin tools
# ------------
ECHO :=					$(CYG_BIN_DIR)/echo
RMDIR :=				$(CYG_BIN_DIR)/rmdir
MKDIR :=				$(CYG_BIN_DIR)/mkdir
REDIR :=				glecho
TOUCH :=				touch
RM :=					rm
CP :=					cp
SED :=					sed
LS :=					$(CYG_BIN_DIR)/ls
MV :=					$(CYG_BIN_DIR)/mv
DATE :=					$(CYG_BIN_DIR)/date
SED :=					$(CYG_BIN_DIR)/sed

LOG_NAME :=				uname
PARKGRAB :=				parkgrab
BANKER :=				banker
BIGLUMP :=				bl2

# PSX and misc tools
# ------------------
LNK :=					slink
PL :=					perl
CC :=					ccpsx
ASM :=					asmpsx
CPE2EXE :=				cpe2exe
CPE2BIN :=				cpe2bin
LZNP :=					lznp -Q
SIZEAPP :=				sizeapp
MAKEDATA :=				MkData

#----------------------------------------------------------------------------
#	Vars that need exporting
#	------------------------

#	Path
#	----
export PATH =		$(TOOL_DIR);$(CYG_BIN_DIR);$(PSYQ_BIN_DIR);$(PERL_BIN_DIR)
export Path =		$(TOOL_DIR);$(CYG_BIN_DIR);$(PSYQ_BIN_DIR);$(PERL_BIN_DIR)

#	PsyQ Vars overidden for those who've installed PSYQ on their machine
#	--------------------------------------------------------------------
export	COMPILER_PATH :=
export  PSYQ_PATH :=
export	ASSEMBLER_PATH :=
export  LIBRARY_PATH :=
export  C_PLUS_INCLUDE_PATH :=
export  C_INCLUDE_PATH :=

#	These vars are also used by GCC
#	-------------------------------
export  GCC_EXEC_PREFIX :=
export  COMPILER_PATH :=
export  LIBRARY_PATH  :=
export  C_INCLUDE_PATH  :=
export  CPLUS_INCLUDE_PATH  :=
export	OBJC_INCLUDE_PATH  :=

#----------------------------------------------------------------------------
endif		# __GLOBALS_MAK__
#----------------------------------------------------------------------------
# end

