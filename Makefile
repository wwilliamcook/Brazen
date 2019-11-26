# Weston Cook

#--------------------------------------------------------------------------------
# Makefile for "Brazen" C++ application
#
#	make Brazen	Builds main application
#	make tests	Builds tests
#	make		Builds tests and main application
#	make clean	Removes all compiled binaries
#--------------------------------------------------------------------------------

PROG := Brazen
CC := g++
CCFLAGS := -Iinclude -std=c++11 -g3 -Wall
CPPS := src/*.cpp
DEPS := include/* src/*

TESTDIR := test

$(PROG): $(DEPS)
	$(CC) -o $@.out $(CPPS) $(CCFLAGS)

.PHONY: all
all: tests $(PROG)

.PHONY: tests
tests:

.PHONY: clean
clean:
	rm -f $(PROG).out $(TESTDIR)/*.out
