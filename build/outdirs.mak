#----------------------------------------------------------------------------
#  Create all the output directories the game needs
#----------------------------------------------------------------------------

#	Macros for ouput dirs
#	---------------------
OUT_DIR :=				out/$(TERRITORY)

OBJ_DIR :=				$(OUT_DIR)/$(VERSION)/$(FILE_SYSTEM)/objs
OBJ_DIRS_TO_MAKE :=		$(foreach SRC_DIR,$(SRC_DIRS),$(OBJ_DIR)/$(SRC_DIR))
VERSION_DIR :=			$(OUT_DIR)/$(VERSION)/version/$(FILE_SYSTEM)

DEPS_DIR :=				$(OUT_DIR)/$(VERSION)/deps/$(FILE_SYSTEM)
DEP_DIRS_TO_MAKE :=		$(foreach SRC_DIR,$(SRC_DIRS),$(DEPS_DIR)/$(SRC_DIR))

TEMP_DIR :=				$(OUT_DIR)/$(VERSION)/$(FILE_SYSTEM)/temp
TEMP_BUILD_DIR :=		$(OUT_DIR)/$(VERSION)/build
DATA_OUT :=				$(OUT_DIR)/data
INC_DIR :=				$(OUT_DIR)/include
REPORT_DIR :=			$(OUT_DIR)/report
SYSTEM_GEN_DIR :=		$(SOURCE_DIR)/system/$(VERSION)/$(TERRITORY)/$(FILE_SYSTEM)	
SYSTEM_GEN_DIR_O :=		$(OBJ_DIR)/system/$(VERSION)/$(TERRITORY)/$(FILE_SYSTEM)
SYSTEM_GEN_DIR_D :=		$(DEPS_DIR)/system/$(VERSION)/$(TERRITORY)/$(FILE_SYSTEM)

DIRS_TO_MAKE :=			$(OBJ_DIR) $(VERSION_DIR) $(TEMP_DIR) $(DEPS_DIR) $(OBJ_DIRS_TO_MAKE) $(DEP_DIRS_TO_MAKE) $(TEMP_BUILD_DIR) $(DATA_OUT) $(INC_DIR) $(REPORT_DIR) $(SYSTEM_GEN_DIR) $(SYSTEM_GEN_DIR_O) $(SYSTEM_GEN_DIR_D)

# DIRS_TO_MAKE :=		$(OBJ_DIR) $(VERSION_DIR) $(DEPS_DIR) $(OBJ_DIRS_TO_MAKE) $(TEMP_DIR) $(DEP_DIRS_TO_MAKE) $(TEMP_BUILD_DIR) $(DATA_OUT) $(INC_DIR) $(REPORT_DIR) $(SIZES_DIR) $(SYSTEM_GEN_DIR) $(SYSTEM_GEN_DIR_O) $(SYSTEM_GEN_DIR_D)

#	Stuff needed to make output dirs
#	--------------------------------
.PHONY: dirs

$(DIRS_TO_MAKE) :
	@$(MKDIR) -p $(DIRS_TO_MAKE)
	@$(ECHO) Created directories $(DIRS_TO_MAKE)

dirs : $(DIRS_TO_MAKE)

#	Some more exports to do after we've decided what the temp dir is
#	----------------------------------------------------------------
export TEMP := 					$(TEMP_DIR)
export TMPDIR := 				$(TEMP_DIR)
export DEPENDENCIES_OUTPUT :=	$(TEMP_DIR)/dep.tmp
