#TODOs
##
##
##+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

PROJECT_NAME := vibrato-mc
VERSION := 1.0
# Compiler
CC := g++
MAIN := 
# FLAGS
CFLAGS :=  -Wall -std=c++14 
BINDIR := bin
# PREREQUISITES & STUFFS

all:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME) $(CFLAGS) src/vibrato_digital.cpp

