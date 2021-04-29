
PROJECT_NAME := vibrato
CC := g++
MAIN := 
CFLAGS :=  -Wall -std=c++17
BINDIR := bin
TESTDIR := src/tests

all: setup
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-main $(CFLAGS) $(TESTDIR)/main.cpp

	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-digital $(CFLAGS) $(TESTDIR)/vibrato_digital.cpp
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-vanilla $(CFLAGS) $(TESTDIR)/vibrato_vanilla.cpp

	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-heston $(CFLAGS) $(TESTDIR)/vibrato_heston.cpp

	$(CC) -o $(BINDIR)/vad-digital $(CFLAGS) $(TESTDIR)/vad_digital.cpp
	$(CC) -o $(BINDIR)/vad-vanilla $(CFLAGS) $(TESTDIR)/vad_vanilla.cpp
	$(CC) -o $(BINDIR)/vad-heston $(CFLAGS) $(TESTDIR)/vad_heston.cpp
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-perf $(CFLAGS) $(TESTDIR)/performance.cpp


perf: setup
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-performances $(CFLAGS) $(TESTDIR)/performance.cpp
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-speed $(CFLAGS) $(TESTDIR)/speed.cpp

digital: setup
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-digital $(CFLAGS) $(TESTDIR)/vibrato_digital.cpp

vanilla: setup
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-vanilla $(CFLAGS) $(TESTDIR)/vibrato_vanilla.cpp

heston: setup
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-heston $(CFLAGS) $(TESTDIR)/vibrato_heston.cpp

main: setup
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-main $(CFLAGS) $(TESTDIR)/main.cpp

vanilla-ad: setup
	$(CC) -o $(BINDIR)/vad-vanilla $(CFLAGS) $(TESTDIR)/vad_vanilla.cpp

digital-ad: setup
	$(CC) -o $(BINDIR)/vad-digital $(CFLAGS) $(TESTDIR)/vad_digital.cpp

heston-ad: setup
	$(CC) -o $(BINDIR)/vad-heston $(CFLAGS) $(TESTDIR)/vad_heston.cpp


clean:
	rm -rf $(BINDIR)/*
	rm -rf src/python/datasets/*.csv

setup:
	mkdir -p $(BINDIR) src/python/datasets

autodiff:
	$(CC) -o $(BINDIR)/autodiff $(CFLAGS) $(TESTDIR)/autodiff.cpp
