# Compiler
CC			:=	gcc
# Compiler flags
DEFINES		:=	$(if $(filter all-without-colors, $(MAKECMDGOALS)), -DNO_USE_COLORS,)
CC_WFLAGS	:=	-W -Wall -Wextra -pedantic -std=c89
CC_OFLAGS	:=	-O2 -funroll-loops
CC_LFLAGS	:=	-Wl,-s -static
CFLAGS		:=	$(CC_WFLAGS) $(CC_OFLAGS) $(DEFINES)

# Tools
RM			:=	rm
RM_FLAGS	:=	-rf
MKDIR		:=	mkdir
MKDIR_FLAGS	:=	-p
CP			:=	cp
CP_FLAGS	:=	-f
WINDRES		:=	$(shell which windres)

# Directories
LIBSDIR		:=	lib
BUILDDIR	:=	build
BINDIR		:=	bin

SOURCES		:=	src/view/view.c \
				src/application/application.c \
				src/utils/utils.c \
				src/main.c

LIBS		:=	pokem

OBJS		:=	$(BUILDDIR)/view.o \
				$(BUILDDIR)/application.o \
				$(BUILDDIR)/utils.o \
				$(BUILDDIR)/main.o

RC_FILE		:=	resources/manifest.rc
RC_OBJ		:=	$(BUILDDIR)/res.o

# If no `windres` tool available, the RC object file must be skipped
ifeq ($(WINDRES),)
	RC_OBJ	=
endif

EXECUTABLE	:=	$(BINDIR)/pokeM

# Messages
MSG			:=	printf
# Colors
NOCOLOR		:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[0m)
BLACK		:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[0;30m)
DARKGRAY	:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[1;30m)
RED			:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[0;31m)
LIGHTRED	:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[1;31m)
GREEN		:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[0;32m)
LIGHTGREEN	:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[1;32m)
ORANGE		:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[0;33m)
YELLOW		:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[1;33m)
BLUE		:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[0;34m)
LIGHTBLUE	:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[1;34m)
PURPLE		:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[0;35m)
LIGHTPURPLE	:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[1;35m)
CYAN		:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[0;36m)
LIGHTCYAN	:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[1;36m)
LIGHTGRAY	:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[0;37m)
WHITE		:=	$(if $(filter -DNO_USE_COLORS, $(DEFINES)),,\033[1;37m)

# ----------------------------------------------------------------------------------------------------

.DEFAULT_GOAL := all
.PHONY: all all-without-colors clean help

all: $(EXECUTABLE) ## Build Pokem CLI executable (default)
all-without-colors: $(EXECUTABLE) ## Build Pokem CLI executable, without colors

clean: ## Remove all leftovers from the previous build
	@$(MSG) "$(YELLOW)Removing 3rd party build leftovers...$(NOCOLOR)\n"
	@$(MAKE) --directory src/3rdparty/pokem clean
	@$(MSG) "$(YELLOW)Removing intermediate objects files...$(NOCOLOR)\n"
	$(RM) $(RM_FLAGS) $(RC_OBJ) $(OBJS)
	@$(MSG) "$(YELLOW)Removing binaries...$(NOCOLOR)\n"
	$(RM) $(RM_FLAGS) $(EXECUTABLE)
	@$(MSG) "$(YELLOW)Removing directories...$(NOCOLOR)\n"
	$(RM) $(RM_FLAGS) $(BUILDDIR)
	$(RM) $(RM_FLAGS) $(BINDIR)
	@$(MSG) "$(WHITE)PokeM-CLI$(LIGHTGREEN) cleaned.$(NOCOLOR)\n"

help:
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "$(WHITE)%-10s$(NOCOLOR) %s\n", $$1, $$2}'

$(EXECUTABLE): $(LIBS) $(BUILDDIR) $(OBJS) $(RC_OBJ) $(BINDIR)
	@$(MSG) "$(YELLOW)Building and linking executable file...$(NOCOLOR)\n"
	$(CC) -o $@ $(CFLAGS) $(CC_LFLAGS) $(OBJS) $(RC_OBJ) -L$(LIBSDIR) -l$(LIBS)
	@$(MSG) "$(LIGHTGREEN)Done. $(WHITE)PokeM-CLI$(LIGHTGREEN) was built in the $(LIGHTBLUE)$(BINDIR)$(LIGHTGREEN) directory. Enjoy.$(NOCOLOR)\n"

$(LIBS): $(LIBSDIR)
	@$(MSG) "$(YELLOW)Building $(LIGHTGREEN)$@$(YELLOW) library...$(NOCOLOR)\n"
	@$(MAKE) --directory src/3rdparty/$@ $(if $(filter all-without-colors, $(MAKECMDGOALS)), all-without-colors,)
	@$(CP) $(CP_FLAGS) src/3rdparty/$@/binlib/lib$@.a lib/lib$@.a
	@$(CP) $(CP_FLAGS) src/3rdparty/$@/binlib/$@.h lib/$@.h

$(OBJS):
	@$(MSG) "$(GREEN)Compiling $(BLUE)$<$(GREEN)...$(NOCOLOR)\n"
	$(CC) -c $(CFLAGS) $< -o $@

$(RC_OBJ):
	@$(MSG) "$(YELLOW)Building MS Windows's RC file...$(NOCOLOR)\n"
	$(WINDRES) -i $(RC_FILE) $(DEFINES) -o $@
	@$(MSG) "$(YELLOW)Building intermediate objects files...$(NOCOLOR)\n"

$(LIBSDIR):
	@$(MSG) "$(YELLOW)Creating $@ directory...$(NOCOLOR)\n"
	$(MKDIR) $(MKDIR_FLAGS) $(LIBSDIR)

$(BUILDDIR):
	@$(MSG) "$(YELLOW)Creating $@ directory...$(NOCOLOR)\n"
	$(MKDIR) $(MKDIR_FLAGS) $(BUILDDIR)

$(BINDIR):
	@$(MSG) "$(YELLOW)Creating $@ directory...$(NOCOLOR)\n"
	$(MKDIR) $(MKDIR_FLAGS) $(BINDIR)

# Intermediate objects build rules
$(BUILDDIR)/main.o:        		src/main.c
$(BUILDDIR)/application.o:		src/application/application.c
$(BUILDDIR)/view.o:				src/view/view.c
$(BUILDDIR)/utils.o:			src/utils/utils.c