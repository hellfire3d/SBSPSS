#----------------------------------------------------------------------------
#	CCOPT.MAK
#
#	This holds all the command line options for C++
#	compilation
#
#	Tabbed for 5 9 (Please do not change)
#
#	Created:	3rd February 1999
#
#	Copyright (C) 1997-1998 G R Liddon
#----------------------------------------------------------------------------

COMPILER_INI_FILE :=		$(PSYQ_PATH)/sn.ini

ASM_OPTS :=			/l /zd /z /w /q \
					/o c+ /o w+ /o ws+ \
					/j $(PSYQ_INC_DIR)	\
					/j $(SOURCE_DIR) \
					/j ./



COMMON_CCFLAGS :=	-Werror \
					-D__TERRITORY_$(TERRITORY)__\
					-D__VERSION_$(VERSION)__\
					-D__USER_$(USER_NAME)__ \
					-D__VERSION_STR__=$(VERSION)\
					-D__TERRITORY_STR__=$(TERRITORY)\
					-DPSX \
					-D_LANGUAGE_C_PLUS_PLUS \
					$(USER_DEF_OPT) \
					$(GLIB_OPT) \
					$(FILE_OPT) \
					$(DEV_OPT) \
					$(SMALL_OPT) \
					-Wall \
					-Wmissing-prototypes\
					-Wmissing-declarations\
					-Wno-unused \
					-Wbad-function-cast \
					-g \
					-O2\
					-fno-builtin\
					-fomit-frame-pointer\
					-fno-exceptions \
					-fno-rtti \
					-I. \
					-I$(PSYQ_INC_DIR) \
					-I$(DDX_INC_DIR)\
					-I$(MOD_INC_DIR)\
					-I$(SOURCE_DIR)\
					-I$(DATA_INC_DIR) \
					-I$(GAME_DATA_DIR) \
					-I$(INC_DIR)

#					-fno-inline \

#----------------------------------------------------------------------------
# end