# Assignment 10: Managing many serial computations

For this [assignment](https://education.scinet.utoronto.ca/mod/assign/view.php?id=2386), your task is to (1) perform a parameter sweep calculation using the mzasolve application which implements a solution of the modified zombie apocalypse equations (see assignment 5), and (2) post-process the results.

One [issue](https://education.scinet.utoronto.ca/mod/forum/discuss.php?d=148) and I am just too lazy to change the code...

Attention! For the serial farming, ntasks-per-node should be 16, which means the maximum number of tasks should be 16.

```
#!/bin/bash
#SBATCH --job-name=mzasolve
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=16
#SBATCH --cpus-per-task=1
#SBATCH --time=02:15:00
```

