#-------------------------------------------------------------------------------
# Makefile for "Brazen" C++ application
# Weston Cook
#
#	make [all]	Builds test and main application
#	make Brazen	Builds main application
#	make tests	Builds tests
#	make clean	Removes all compiled binaries and object files
#
# References:
#   https://stackoverflow.com/questions/1950926/create-directories-using-make-file
#-------------------------------------------------------------------------------

PROG=Brazen
CC=g++
CCFLAGS=-Iinclude -std=c++11 -g3 -Wall
CPPS=src/*.cpp
DEPS=include/* src/*
DIR_GUARD=@mkdir -p $(@D)

.PHONY: all
all: tests $(PROG)

$(PROG): $(DEPS)
	$(DIR_GUARD)
	$(CC) -o bin/$@ $(CPPS) $(CCFLAGS)

test/%.cpp: $(DEPS)
	$(DIR_GUARD)
	$(CC) -o bin/$(basename $@) $@ $(CPPS) $(CCFLAGS)

.PHONY: tests
tests: test/*.cpp

.PHONY: clean
clean:
	rm -rf bin/*
