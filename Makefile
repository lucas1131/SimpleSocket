# Project name
NAME=

# Project version used for packaging
VERSION=1.0


# Directories
INCDIR=include
# LIBDIR=lib
BLDDIR=build
SRCDIR=src
OBJDIR=$(SRCDIR)/obj


DEBUGGER=
DBGFLAGS=-v --leak-check=full --show-leak-kinds=all --read-var-info=yes --track-origins=yes

# Search for .c source files
SRC=$(wildcard $(SRCDIR)/*.c)
# Search for .h header files
DEPS=$(wildcard $(INCDIR)/*.h)
# Generate .o object files rules
OBJ=$(foreach file, $(SRC), $(file:$(SRCDIR)/%.c=$(OBJDIR)/%.o))


CC=gcc
CFLAGS=-O3 -I./$(INCDIR) 

LIBS=-lm

ifdef debug
	CFLAGS += -Wall -Wextra -g -D DEBUG
	DEBUGGER=valgrind $(DBGFLAGS) 
endif

all: clean main

# Compile directives

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	@echo Building $*
	@$(CC) -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	@echo Linking object files
	@$(CC) -o $(BLDDIR)/$(NAME) $^ $(CFLAGS) $(LIBS)


# Run directives
run:
	$(DEBUGGER) ./$(BLDDIR)/$(NAME) $(INPUT) $(OUTPUT)

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
	@tar -zcvf $(NAME)-$(VERSION).tar.gz *
	@echo Done.

zip: clean cleanobj
	@echo Compressing files...
	@zip -r $(NAME)-$(VERSION).zip *
	@echo Done.

# Check if project has a name before creating it
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
	touch $(NAME)/$(SRCDIR)/.ignore
	touch $(NAME)/$(OBJDIR)/.ignore
	touch $(NAME)/$(BLDDIR)/.ignore
	touch $(NAME)/$(INCDIR)/.ignore
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
