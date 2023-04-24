# Assignment 3: Modularize with make and git
The [3rd assignment](https://education.scinet.utoronto.ca/mod/assign/view.php?id=2169) is an example to show how to modularize with make and git. The program wave1d.cpp (available below) solves the damped 1-dim wave equation. The code (solution) I uploaded is the code given by the [4th assignment](). The author is Ramses van Zon.

On teach cluster, compilation of the code can be done with the commands

```
source teachsetup
make
```

Running the code is, like before, done with the command 

```
make run
```

- The application is divided in five modules: parameters, initialize, output, evolve and simulation.
- The parameters used are stored in 'waveparams.txt'.

- The variables used in the code can be classified in a header file,  'wavetypes.h'.

- Modules loaded

  [Check what can be used on teach cluster.](https://docs.scinet.utoronto.ca/index.php/Teach)

- Compilation options

  ```
  CXXFLAGS=-O2 -g -std=c++17 -Wall -Wfatal-errors -Wconversion
  ```

  - `-O2`: Enables optimization level 2, which can improve program performance.
  - `-g`: Enables debug information, which facilitates debugging when the program goes wrong. (in teachsetup, we loaded [gdb](https://www.sourceware.org/gdb/))
  - `-std=c++17`: Specifies the C++ standard version as C++17, enabling new features in C++17.
  - `-Wall`: Enables all warning messages.
  - `-Wfatal-errors`: Stops compilation as soon as an error is found.
  - `-Wconversion`: Reports warnings for implicit type conversions.

- -c flag

  In lines 8-9

  ```
  wave1d: wave1d.o parameters.o initialize.o output.o evolve.o simulation.o
  	$(CXX) $(LDFLAGS) -o $@ $^
  ```

  in lines 11-12

  ```
  wave1d.o: wave1d.cpp parameters.h wavetypes.h initialize.h output.h evolve.h simulation.h
  	$(CXX) -c $(CXXFLAGS) -o wave1d.o wave1d.cpp
  ```

  The former 'wave1d' is executable (executed in './wave1d waveparams.txt'), while the later 'wave1d.o' is not executable, but an object.

- make clean

  After modifying your code, before re-run it, remember to type 

  ```
  make clean
  ```

  to remove all the '.o' file. Or there might be a bug in your new code and the corresponding '.o' file cannot be generated, while the application uses the former '.o' file and give you a fake result!
