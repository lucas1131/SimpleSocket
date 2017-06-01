Makefile rules

{all: compile project}
{run: run executable}
{clean: clean object files and zip/tar}
{zip/tar: compress project folder}

{======================}

{set \'debug=1\' to compile/run in debug mode}
{set \'IN=*filename*\' to feed a file to the program}
{set \'OUT=*filename*\' to write program output to designed file}
{use \'CFLAGS+=*flags*\' to add *flags*}
