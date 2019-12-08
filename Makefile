#-------------------------------------------------------------------------------
# Makefile for "Brazen" C++ application
# Weston Cook
#
#	make [all]	Builds tests, main application, and examples
#	make Brazen	Builds main application
#	make tests	Builds tests
#	make examples	Builds examples
#	make clean	Removes all compiled binaries and object files
#
# References:
#   https://stackoverflow.com/questions/1950926/create-directories-using-make-file
#-------------------------------------------------------------------------------

PROG=Brazen
CC=g++
CCFLAGS=-Iinclude -lSDL2 -pthread -std=c++11 -g3 -Wall -Wno-switch -Wno-narrowing
CPPS = $(wildcard src/core/**.cpp)
DEPS=include/* src/**

.PHONY: all
all: tests $(PROG)

$(PROG): $(DEPS)
	@mkdir -p bin
	$(CC) -o bin/$@ $(CPPS) $(CCFLAGS)

bin/test/%: $(DEPS)
	@mkdir -p bin/test
	$(CC) -o $@ test/$(notdir $@.cpp) $(CPPS) $(CCFLAGS)

.PHONY: tests
tests: $(patsubst %, bin/%, $(basename $(wildcard test/*.cpp)))

bin/example/%: $(DEPS)
	@mkdir -p bin/example
	$(CC) -o $@ example/$(notdir $@.cpp) $(CPPS) $(CCFLAGS)

.PHONY: examples
examples: $(patsubst %, bin/%, $(basename $(wildcard example/*.cpp)))

.PHONY: clean
clean:
	rm -rf bin/*
