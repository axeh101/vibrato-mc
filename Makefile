
PROJECT_NAME := vibrato
CC := g++
MAIN := 
CFLAGS :=  -Wall -std=c++17
BINDIR := bin
TESTDIR := src/tests

all:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-main $(CFLAGS) $(TESTDIR)/main.cpp
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-autodiff $(CFLAGS) $(TESTDIR)/autodiff_test.cpp

	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-digital $(CFLAGS) $(TESTDIR)/vibrato_digital.cpp
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-vanilla $(CFLAGS) $(TESTDIR)/vibrato_vanilla.cpp

	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-heston $(CFLAGS) $(TESTDIR)/vibrato_heston.cpp

	$(CC) -o $(BINDIR)/vad-digital $(CFLAGS) $(TESTDIR)/vad_digital.cpp
	$(CC) -o $(BINDIR)/vad-vanilla $(CFLAGS) $(TESTDIR)/vad_vanilla.cpp
	$(CC) -o $(BINDIR)/vad-heston $(CFLAGS) $(TESTDIR)/vad_heston.cpp
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-perf $(CFLAGS) $(TESTDIR)/performance.cpp


perf:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-performances $(CFLAGS) $(TESTDIR)/performance.cpp
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-speed $(CFLAGS) $(TESTDIR)/speed.cpp

digital:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-digital $(CFLAGS) $(TESTDIR)/vibrato_digital.cpp

vanilla:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-vanilla $(CFLAGS) $(TESTDIR)/vibrato_vanilla.cpp

heston:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-heston $(CFLAGS) $(TESTDIR)/vibrato_heston.cpp

main:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-main $(CFLAGS) $(TESTDIR)/main.cpp

autodiff:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-autodiff $(CFLAGS) $(TESTDIR)/autodiff_test.cpp

vanilla-ad:
	$(CC) -o $(BINDIR)/vad-vanilla $(CFLAGS) $(TESTDIR)/vad_vanilla.cpp

digital-ad:
	$(CC) -o $(BINDIR)/vad-digital $(CFLAGS) $(TESTDIR)/vad_digital.cpp

heston-ad:
	$(CC) -o $(BINDIR)/vad-heston $(CFLAGS) $(TESTDIR)/vad_heston.cpp


clean:
	rm -rf $(BINDIR)/vibrato-*