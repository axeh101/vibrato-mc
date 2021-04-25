#TODOs
##
##
##+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

PROJECT_NAME := vibrato
VERSION := 1.0
# Compiler
CC := g++
MAIN := 
# FLAGS
CFLAGS :=  -Wall -std=c++17
BINDIR := bin
# PREREQUISITES & STUFFS

all:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-main $(CFLAGS) src/main.cpp
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-ad $(CFLAGS) src/vibrato_ad.cpp
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-digital $(CFLAGS) src/vibrato_digital.cpp
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-vanilla $(CFLAGS) src/vibrato_vanilla.cpp
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-performances $(CFLAGS) src/performance.cpp
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-heston $(CFLAGS) src/vibrato_vanilla_heston.cpp


perf:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-performances $(CFLAGS) src/performance.cpp

digital:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-digital $(CFLAGS) src/vibrato_digital.cpp

vanilla:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-vanilla $(CFLAGS) src/vibrato_vanilla.cpp

heston:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-heston $(CFLAGS) src/vibrato_vanilla_heston.cpp

main:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-main $(CFLAGS) src/main.cpp

autodiff:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-autodiff $(CFLAGS) src/autodiff_test.cpp

vibrato-ad:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-ad $(CFLAGS) src/vibrato_ad.cpp

clean:
	rm -rf $(BINDIR)/vibrato-*