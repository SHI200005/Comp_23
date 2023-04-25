# Assignment 6: Using BLAS for the 1d wave

In this [assignment](https://education.scinet.utoronto.ca/mod/assign/view.php?id=2298), you will be taking the one-dimensional wave equation code and re-implementing the one_time_step function using linear algebra calls to BLAS (we could take the two-dimensional case too, but it would be a bit more involved).

One issue of my version: Here I used dense matrix, but usually it is a sparse matrix and we should use sparse matrix to interpret. However, I am a rookie who don't know how to code...

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

## Integrated test

After the run above, you will have an output file called 'results.dat' (name defined in the program). For integrated test, first type

```
mv results.dat original.dat
```

to rename 'results.dat' to 'original.dat'.

After you modified the code, you need to confirm the results you get is the same as before, then 'test.sh' is a code to compare the results given by modified and original program. You need to first modify the permissions of this file to make it executable, then type

```
bash test.sh
```

to run the integrated test. Then you will see 'Test passed!'

## Linear Algebra Formulation

The matrix $\boldsymbol{A}$ I am using is 

$$\begin{matrix}
0 & 0 & 0 & 0 &\cdots & 0 & 0 & 0\\
adjacent & diagonal & adjacent & 0 & \cdots & 0 & 0 & 0 \\
0 & adjacent & diagonal & adjacent & \cdots & 0 & 0 & 0\\
\vdots & \vdots & \vdots & \vdots & \ddots& \vdots & \vdots & \vdots\\0& 0 & 0 & 0&  \cdots& adjacent & diagonal & adjacent  \\0 & 0 & 0 & 0 &\cdots & 0 & 0 & 0\\
\end{matrix}$$

where $$adjacent=2-2\frac{\Delta t^2c^2}{\Delta x^2}-\frac{\Delta t}{\tau}$$, $$diagonal=\frac{\Delta t^2c^2}{\Delta x^2}$$. Then I can throw away the boundary conditions.

I used **cblas_dgemv**, and the new calculated vector will be assigned to the 'previous state'. Then we swap the 'current state' and the 'previous state' (which is now actually the next state). We don't need 'previous', 'current' and 'next'. Just throw away 'next'.