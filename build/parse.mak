#----------------------------------------------------------------------------
#	PARSE.MAK
#
#	Makes sure all the that need defining to build the game have been defined
#	If not ERR_STR is defined and this cause the makefile to report an error
#
#	Tabbed for 5 9 (Please do not change)
#
#	Created:	
#
#	Copyright (C) 1997-1999 G R Liddon
#----------------------------------------------------------------------------

# Make sure the basics are defined first
# --------------------------------------
ifndef TERRITORY

ERR_STR :=	You must define a territory to build for (USA EUR || JAP)

endif

ifndef VERSION

ERR_STR :=	You must define a version to build

endif


# Now Parse for the dev kit stuff
#	----------------------------------
ifdef PSYQ_DEV_IRQ

 PSYQ_DEV_IRQ_OPT :=  /i$(PSYQ_DEV_IRQ)

endif

ifndef DEV_KIT
 ERR_STR :=	You must define DEV_KIT to either SONY_ISA or SONY_PCI or CLIMAX or TPWART
else
 DEV_OPT :=		-DSONY_ISA=0 \
				-DSONY_PCI=1 \
				-DCLIMAX=2 \
				-DTPWART=3 \
		 		-D__DEV_KIT__=$(DEV_KIT)

DK_SONY_ISA :=		0
DK_SONY_PCI :=		1
DK_CLIMAX :=		2
DK_TPWART :=		3
DK_SONY_H2700 :=	$(DK_SONY_PCI)

LNK_DEV_KIT :=		$(DK_$(DEV_KIT))

endif

# Now sort out how the game loads files
# -------------------------------------
ifndef FILE_SYSTEM
 ERR_STR :=	You must define FILE_SYSTEM to either PC or CD
else
 FILE_OPT :=	-DPC=0 \
				-DCD=1 \
 				-D__FILE_SYSTEM__=$(FILE_SYSTEM)

FS_PC :=	0
FS_CD :=	1

LNK_FILE_SYS :=		$(FS_$(FILE_SYSTEM))

endif

#	Make sure we have everything we need for the sony pci dev kit
#	--------------------------------------------------------------
ifeq ($(DEV_KIT),SONY_PCI)

DEV_KIT_FOUND :=	1

endif

#	Make sure we have everything we need for the climax dev kit
#	-----------------------------------------------------------
ifeq ($(DEV_KIT),CLIMAX)

ifndef DDX_PORT

 ERR_STR :=	You must define DDX_PORT for the Climax dev kit i/o addr in your personal makefile

endif

DEV_KIT_FOUND :=	1

endif


#	Make sure we have everything we need for the climax dev kit
#	-----------------------------------------------------------
ifeq ($(DEV_KIT),TPWART)

ifndef DDX_PORT

 ERR_STR :=	You must define DDX_PORT for the Climax dev kit i/o addr in your personal makefile

endif

DEV_KIT_FOUND :=	1

endif


#	Make sure we have everything we need for the Sony H2000
#	--------------------------------------------------------------------
ifeq ($(DEV_KIT),SONY_ISA)

ifdef SONY_ISA_IRQ

 SONY_ISA_IRQ_OPT :=  /i $(SONY_ISA_IRQ)

endif

ifndef SONY_ISA_ADDR

 ERR_STR :=	You must define SONY_ISA_ADDR for the dtl i/o addr in your personal makefile

endif

DEV_KIT_FOUND :=	1

endif

#	Make sure we have everything we need for the Sony H2700
#	-------------------------------------------------------
ifeq ($(DEV_KIT),SONY_H2700)

ifdef SONY_ISA_IRQ

 SONY_ISA_IRQ_OPT :=  /i $(SONY_ISA_IRQ)

endif

ifndef SONY_ISA_ADDR

 ERR_STR :=	You must define SONY_ISA_ADDR for the dtl i/o addr in your personal makefile

endif

DEV_KIT_FOUND :=	1

endif

#----------------------------------------------------------------------------
#	Macros for dev BIOS load \ unload for all the dev systems
#	All take form LOAD_BIOS_$(DEV_KIT)
#----------------------------------------------------------------------------

#	PCI Bios load \ unload
#	----------------------
define	LOAD_BIOS_SONY_PCI
	h25bios
	RESETPS 1 /n
endef

define	UNLOAD_BIOS_SONY_PCI
	-mess1
	h25bios
endef

#	H2700 Bios load \ unload
#	------------------------
define	LOAD_BIOS_SONY_H2700
	-mess1
	-dexbios /a $(SONY_ISA_ADDR) $(SONY_ISA_IRQ_OPT)
	RESETPS 1 /n
endef

define	UNLOAD_BIOS_SONY_H2700
	-dexbios
	-mess1
endef


#	ISA Bios load \ unload
#	----------------------
define	LOAD_BIOS_SONY_ISA
	-mess1
	-dexbios /a $(SONY_ISA_ADDR) $(SONY_ISA_IRQ_OPT)
	RESETPS 1
	RUN /w4 $(subst /,\,$(PSYQ_BIN_DIR)\snpatch.cpe) > con
	DELAY
endef

define	UNLOAD_BIOS_SONY_ISA
	-mess1
	-dexbios
endef


#	CLIMAX Bios load \ unload
#	------------------------
define	LOAD_BIOS_CLIMAX
	-mess1
	-datbios
endef

define	UNLOAD_BIOS_CLIMAX
	-datbios
	-mess1
endef


#	TPWART Bios load \ unload
#	------------------------
define	LOAD_BIOS_TPWART
	-mess1
	-psy /4
endef

define	UNLOAD_BIOS_TPWART
	-psy
	-mess1
endef

#	Set the LOAD\UNLOAD_BIOS macros correctly
#	-----------------------------------------
LOAD_BIOS :=	$(LOAD_BIOS_$(DEV_KIT))
UNLOAD_BIOS :=	$(UNLOAD_BIOS_$(DEV_KIT))


#	Check to make sure we found a dev kit we know about
#	---------------------------------------------------
ifndef ERR_STR

ifndef DEV_KIT_FOUND
 ERR_STR :=	$(DEV_KIT) is not a recognised dev kit
endif

endif

#	Sort out the compiler we're using
#	---------------------------------
ifdef USE_EGCS

COMPILER :=	  	egcs

else

COMPILER :=		gnu

endif

export  PSYQ_PATH :=			$(PSYQ_BIN_DIR)/$(COMPILER)
export  SN_PATH :=			$(PSYQ_BIN_DIR)/$(COMPILER)


#	Define what we need for the ram size
#	------------------------------------
ifdef RAM_SIZE
MEG :=			$(RAM_SIZE)
else
MEG :=			8
endif

MEG_OPT :=		-D__MEG$(MEG)__


#	Bodge Address for 4 meg dev kit
#	-------------------------------
ifeq ($(VERSION),debug)

GLIB_OPT :=		-D__GL_DEBUG__

else

GLIB_OPT :=

endif


#	Bodge Address for 4 meg dev kit
#	-------------------------------
ifeq ($(RAM_SIZE),4)
PROG_ORG =      0x80210000
else
PROG_ORG =      0x80010000
endif


#----------------------------------------------------------------------------
# end
