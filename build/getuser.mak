#----------------------------------------------------------------------------
#	GETUSER.MAK
#
#	Get's the user vars in from their makefile
#
#	Their makefile is at $(USERS_DIR)/<user name>/makefile
#	The user name is whatever they logged onto
#	their computer as (in lower case)
#
#	Tabbed for 5 9 (Please do not change)
#
#	Created:	3rd February 1999
#
#	Copyright (C) 1997-1998 G R Liddon
#----------------------------------------------------------------------------

# Now that's all sorted include the users vars
# --------------------------------------------

ifndef USER_NAME

USER_NAME := $(shell $(LOG_NAME))

endif


USER_VAR_FILE :=	$(USERS_DIR)/$(USER_NAME)/makefile

$(USERS_DIR)/$(USER_NAME)/makefile :
	@$(ECHO) You Need a user file. See gary liddon

include $(USER_VAR_FILE)

#----------------------------------------------------------------------------
# end