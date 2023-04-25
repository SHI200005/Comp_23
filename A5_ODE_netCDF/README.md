# Assignment 5: Solve ODE and store in NetCDF file

The [5th assignment](https://education.scinet.utoronto.ca/mod/assign/view.php?id=2250) is an example to write a program that uses GSL's odeiv2 sublibrary to solve the systems of ODEs and write out the time series to a NetCDF file.

On teach cluster, compilation of the code can be done with the commands

```
source teachsetup
make
```

Running the code is, like before, done with the command 

```
make run
```

After running, remove the .o files 

```
make clean
```

- hdf5 needed for using NetCDF 4

  ```
  module load gcc/12 gsl/2.7 gdb rarray hdf5 netcdf
  ```

- Link to the gsl and netcdf libraries when compiling

  ```
  LDLIBS=-lgsl -lgslcblas -lm -lnetcdf_c++4 -lnetcdf
  ```

  
