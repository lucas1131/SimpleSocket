# Project name
NAME=FreeGluteos3D


# Directories
INCDIR=include
# LIBDIR=lib
BLDDIR=build
SRCDIR=src
OBJDIR=$(SRCDIR)/obj


DEBUGGER=
DBGFLAGS=-v --leak-check=full --show-leak-kinds=all --read-var-info=yes --track-origins=yes

# Search for source files
SRC=$(wildcard $(SRCDIR)/*.c) 
SRC += $(wildcard $(SRCDIR)/*.cpp)

# Search for header files
DEPS=$(wildcard $(INCDIR)/*.h)
DEPS += $(wildcard $(INCDIR)/*.hpp)

# Generate .o object files rules
# Find filter out .cpp from .c for correct capture
_OBJC=$(foreach file, $(SRC), $(file:$(SRCDIR)/%.c=$(OBJDIR)/%.o))
OBJC=$(filter-out %.cpp, $(_OBJC))
# Find filter out .c from .cpp for correct capture
_OBJCPP=$(foreach file, $(SRC), $(file:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o))
OBJCPP=$(filter-out %.c, $(_OBJCPP))
# Combine both C and C++ objects
OBJ=$(OBJC) $(OBJCPP)


CC=g++
CFLAGS=-O3 -I./$(INCDIR) 

USER_LIBS=-lGL -lglut -lGLU
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


# Run directives
run:
	$(DEBUGGER) ./$(BLDDIR)/$(NAME) 

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

# Check if project has a name before creating it

# Check for directory existence and create them if necessary
checkdirs:
	if [ ! -d $(BLDDIR)/ ]; then mkdir -p $(BLDDIR)/; fi
	if [ ! -d $(INCDIR)/ ]; then mkdir -p $(INCDIR)/; fi
	if [ ! -d $(LIBDIR)/ ]; then mkdir -p $(LIBDIR)/; fi
	if [ ! -d $(SRCDIR)/ ]; then mkdir -p $(SRCDIR)/; fi
	if [ ! -d $(OBJDIR)/ ]; then mkdir -p $(OBJDIR)/; fi

create:
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
 