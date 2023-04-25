CXX=g++
CXXFLAGS=-Wall -O3
LDLIBS=-lgsl -lgslcblas -lm -lnetcdf_c++4 -lnetcdf
all: mzasolve

mzasolve: mzasolve.cpp
	$(CXX) $(CFLAGS) -o $@ $^ $(LDLIBS)

run: mzasolve
	./mzasolve 0.018 0.004 timeseries.nc

clean:
	$(RM) mzasolve.o

.PHONY: all clean run
