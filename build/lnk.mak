#----------------------------------------------------------------------------
#	LNK.MAK
#
#	This creates the macro that finally makes the linker script file
#	It also holds any linker options needed
#
#	Tabbed for 5 9 (Please do not change)
#
#	Created:	3rd February 1999
#
#	Copyright (C) 1997-1998 G R Liddon
#----------------------------------------------------------------------------

ifndef STACK_SIZE
STACK_SIZE := 3000
endif

LNK_FLAGS:=		-m -psx -c -v -z -o $(PROG_ORG) -entry __SN_ENTRY_POINT \
				-cpemunge \
				-strip \
				-nsf \
				$(ALL_ASM_OBJS) \
				$(PSYQ_LIB_DIR)/libsn.lib \
				-nsf \
				-we -wm -wo \
				-nostrip \
				stup0 \
				stup1 \
				stup2 \
				VectorNormalS \
				VectorNormal \
				VectorNormalSS\
				cqsrt \
				-nostrip

LINKER_FILE_MACRO :=	   				\
';nostripfile=$(PSYQ_LIB_DIR)/libsn.lib'	\
'text				group '						\
'extractors			group '						\
'extradtors			group '						\
'data				group '						\
'sdata				group '						\
'sbss				group 	bss'				\
'bss				group	bss '				\
'cached				group	cache(0x20000000) '	\
'' 										\
'' 										\
'memend		group	bss'				\
'' 										\
'' 										\
'	section .text,text' 				\
'	section .cached,cached'				\
'	section .sdata,sdata'				\
'	section .ctors,text' 				\
'	section .dtors,text ' 				\
'	section .data,data'					\
'	section .rdata,text'				\
'	section .sbss,sbss'					\
'	section .bss,bss'					\
'	section .memend,memend'				\
'	section .last,memend'				\
'' 										\
'' 										\
'' 										\
'' 										\
'LNK_RamSize		equ	$(RAM_SIZE)'		\
'LNK_StackSize		equ $(STACK_SIZE)'		\
'LNK_OrgAddress		equ $(PROG_ORG)'		\
'LNK_FileSys		equ $(LNK_FILE_SYS)'	\
'LNK_DevKit			equ $(LNK_DEV_KIT)'		\
'$(LNK_OVL) ' \
'' 										\
'$(TAB)regs$(TAB)pc=__SN_ENTRY_POINT'	\
'; $(FILE_SYSTEM) Start-Up Objects'							\
''										\
$(foreach FILE,$($(FILE_SYSTEM)_STARTUP_OBJ), '$(TAB)startstats "$(FILE)" ' '$(TAB)include $(PSYQ_LIB_DIR)/$(FILE).obj' '$(TAB)endstats') \
'' 										\
'; Main Game object files'				\
$(foreach FILE,$(CODE_OBJS_TO_MAKE), '$(TAB)startstats "$(subst /,.,$(FILE:$(OBJ_DIR)/%.o=%))" ' '$(TAB)include $(FILE) $(ovl_$(subst /,_,$(FILE:$(OBJ_DIR)/%.o=%)))' '$(TAB)endstats') \
$(foreach FILE,$(EXTRA_OBJS), '$(TAB)startstats "$(subst /,.,$(FILE:$(OBJ_DIR)/%.o=%))" ' '$(TAB)include $(FILE) $(ovl_$(subst /,_,$(FILE:$(OBJ_DIR)/%.o=%)))' '$(TAB)endstats') \
'' 										\
'; Libs'								\
$(foreach FILE,$(ALL_LIBS_FULL),'$(TAB)startstats "$(FILE)"' '$(TAB)inclib $(FILE)' '$(TAB)endstats') \
''

	
#----------------------------------------------------------------------------
# end
