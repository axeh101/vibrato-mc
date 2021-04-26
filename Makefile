
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

	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-heston $(CFLAGS) $(TESTDIR)/vibrato_vanilla_heston.cpp

	$(CC) -o $(BINDIR)/$(PROJECT_NAME)ad-digital $(CFLAGS) $(TESTDIR)/vibrato_ad_digital.cpp
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)ad-vanilla $(CFLAGS) $(TESTDIR)/vibrato_ad_vanilla.cpp

	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-performances $(CFLAGS) $(TESTDIR)/performance.cpp


perf:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-performances $(CFLAGS) $(TESTDIR)/performance.cpp

digital:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-digital $(CFLAGS) $(TESTDIR)/vibrato_digital.cpp

vanilla:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-vanilla $(CFLAGS) $(TESTDIR)/vibrato_vanilla.cpp

heston:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-heston $(CFLAGS) $(TESTDIR)/vibrato_vanilla_heston.cpp

main:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-main $(CFLAGS) $(TESTDIR)/main.cpp

autodiff:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)-autodiff $(CFLAGS) $(TESTDIR)/autodiff_test.cpp

vibrato-ad-vanilla:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)ad-vanilla $(CFLAGS) $(TESTDIR)/vibrato_ad_vanilla.cpp

vibrato-ad-digital:
	$(CC) -o $(BINDIR)/$(PROJECT_NAME)ad-digital $(CFLAGS) $(TESTDIR)/vibrato_ad_digital.cpp
clean:
	rm -rf $(BINDIR)/vibrato-*