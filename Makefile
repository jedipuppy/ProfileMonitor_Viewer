# Makefile for Basler pylon sample program
.PHONY: all clean

# The program to build
NAME       := ProfileMonitor_viewer

# Installation directories for pylon
PYLON_ROOT ?= /opt/pylon5

# Build tools and flags
LD         := $(CXX)
CPPFLAGS   := $(shell $(PYLON_ROOT)/bin/pylon-config --cflags)
CXXFLAGS   := #e.g., CXXFLAGS=-g -O0 for debugging

LDFLAGS    := $(shell $(PYLON_ROOT)/bin/pylon-config --libs-rpath)
LDLIBS     := $(shell $(PYLON_ROOT)/bin/pylon-config --libs)
OPENCV  :=`pkg-config opencv --cflags` `pkg-config opencv --libs`
ROOTCFLAGS := $(shell root-config --cflags)
ROOTLIBS := $(shell root-config --libs)
# Rules for building
all: $(NAME)

$(NAME): $(NAME).o 
	$(LD) $(LDFLAGS)   -o $@ $^ $(LDLIBS) $(OPENCV) 

$(NAME).o: $(NAME).cpp
	 $(CXX)  $(CPPFLAGS) $(CXXFLAGS)-c -o $@ $< 

clean:
	$(RM) $(NAME).o $(NAME)
