# Makefile for "Brazen" C++ application
# Created by Weston Cook

PROG := Brazen
CC := g++
CPPFLAGS := -g -Wall -Iinclude
CPPS := main.cpp
TESTCPPS := tests/*.cpp

$(PROG) : $(CPPS)
	$(CC) $(CPPFLAGS) -o $(PROG) $(CPPS)

.PHONY: tests $(TESTCPPS)
tests: $(TESTCPPS)
$(TESTCPPS):
	$(CC) $(CPPFLAGS) -o $(basename $@) $@

clean :
	rm -f $(PROG) $(basename $(wildcard $(TESTCPPS)))
