CXX=g++
CXX2=mpicxx
CXXFLAGS=-std=c++17 -O3 -march=native -g
LDFLAGS=-g
LDLIBS=-lnetcdf_c++4 -lnetcdf

all: analyzewave mpianalyzewave

analyzewave.o: analyzewave.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $< 

analyzewave: analyzewave.o 
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

run: analyzewave
	./analyzewave precisewave.nc energies.tsv

mpitest: mpianalyzewave test.sh energies_copy.tsv
	./test.sh

mpianalyzewave.o: mpianalyzewave.cpp
	$(CXX2) -c $(CXXFLAGS) -o $@ $<

mpianalyzewave: mpianalyzewave.o
	$(CXX2) $(LDFLAGS) -o $@ $^ $(LDLIBS)

mpirun: mpianalyzewave
	./mpirun -n 16 ./mpianalyzewave precisewave.nc energies.tsv

clean:
	$(RM) analyzewave.o mpianalyzewave.o analyzewave mpianalyzewave

.PHONY: all run test clean
