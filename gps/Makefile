# Project name
NAME=gps


# Directories
INCDIR=include
# LIBDIR=lib
BLDDIR=build
SRCDIR=src
OBJDIR=$(SRCDIR)/obj


# If the first argument is "run"
ifeq (run, $(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2, $(words $(MAKECMDGOALS)), $(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif


DEBUGGER=
DBGFLAGS=-v --leak-check=full --show-leak-kinds=all --read-var-info=yes --track-origins=yes

# Search for source files
SRC=$(wildcard $(SRCDIR)/*.c) 
SRCPP=$(wildcard $(SRCDIR)/*.cpp)

# Search for header files
DEPS=$(wildcard $(INCDIR)/*.h)
DEPSPP=$(wildcard $(INCDIR)/*.hpp)

# Generate .o object files rules
OBJ=$(foreach file, $(SRC), $(file:$(SRCDIR)/%.c=$(OBJDIR)/%.o))
OBJ += $(foreach file, $(SRCPP), $(file:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o))

# Find renamed or removed .o files
DEL_OBJ=$(wildcard $(OBJDIR)/*.o)
DEL_OBJ=$(filter-out %.o, $(OBJC))

CC=g++
CFLAGS=-O3 -I./$(INCDIR) 

USER_LIBS=
DEFAULT_LIBS=-lm
LIBS=$(USER_LIBS) $(DEFAULT_LIBS)


ifdef debug
	CFLAGS += -Wall -Wextra -g -D DEBUG
	DEBUGGER=valgrind $(DBGFLAGS) 
endif

all: checkdirs clean main

# Compile directives
$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	@echo Building $*
	@$(CC) -c -o $@ $< $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(DEPS)
	@echo Building $*
	@$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	@echo Linking object files
	@$(CC) -o $(BLDDIR)/$(NAME) $^ $(CFLAGS) $(LIBS)


.PHONY: run
# Run directives
run:
	$(DEBUGGER) ./$(BLDDIR)/$(NAME) $(RUN_ARGS)

# Utility directives
clean:
	-rm -f $(BLDDIR)/*
	-rm -f $(NAME).zip
	-rm -f $(NAME).tar.gz
	clear
	clear

cleanobj:
	-rm -f $(OBJDIR)/*.o

list:
	clear
	ls -lhR

tar: clean cleanobj
	@echo Compressing files...
	@tar -zcvf $(NAME).tar.gz *
	@echo Done.

zip: clean cleanobj
	@echo Compressing files...
	@zip -r $(NAME).zip *
	@echo Done.

sense:
	$(error Doesnt make sense)

update:
	@echo TODO...

# Check for directory existence and create them if necessary
checkdirs:
	if [ ! -d $(BLDDIR)/ ]; then mkdir -p $(BLDDIR)/; fi
	if [ ! -d $(INCDIR)/ ]; then mkdir -p $(INCDIR)/; fi
	if [ ! -d $(LIBDIR)/ ]; then mkdir -p $(LIBDIR)/; fi
	if [ ! -d $(SRCDIR)/ ]; then mkdir -p $(SRCDIR)/; fi
	if [ ! -d $(OBJDIR)/ ]; then mkdir -p $(OBJDIR)/; fi

create:
# Check if project has a name before creating it
ifeq ($(strip $(NAME)),)
	$(error No project name provided (open this make and set NAME))
else
	mkdir $(NAME) 
	mkdir $(NAME)/$(SRCDIR)
	mkdir $(NAME)/$(INCDIR)
	# mkdir $(NAME)/$(LIBDIR)
	mkdir $(NAME)/$(BLDDIR)
	mkdir $(NAME)/$(OBJDIR)

	# files for git to track
	cp Makefile $(NAME)

	@echo Makefile rules > $(NAME)/README.md
	@echo >> $(NAME)/README.md
	@echo "{all: compile project}" >> $(NAME)/README.md
	@echo "{run: run executable}" >> $(NAME)/README.md
	@echo "{clean: clean object files and zip/tar}" >> $(NAME)/README.md
	@echo "{zip/tar: compress project folder}" >> $(NAME)/README.md
	@echo >> $(NAME)/README.md
	@echo "{======================}" >> $(NAME)/README.md
	@echo >> $(NAME)/README.md
	@echo "{set \'debug=1\' to compile/run in debug mode}" >> $(NAME)/README.md
	@echo "{set \'IN=*filename*\' to feed a file to the program}" >> $(NAME)/README.md
	@echo "{set \'OUT=*filename*\' to write program output to designed file}" >> $(NAME)/README.md
	@echo "{use \'CFLAGS+=*flags*\' to add *flags*}" >> $(NAME)/README.md

endif
