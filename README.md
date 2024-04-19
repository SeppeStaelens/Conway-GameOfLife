# Conway's Game of Life
### Seppe Staelens, ss3033

This repository was my submission for the coursework of "Advanced Research Computing", part of the MPhil in Data Intensive Science at the University of Cambridge. I took this course as a requirement for my PhD funding, as I am supported by the Centre for Doctoral Training in Data Intensive Science. The original assignment can be found in `coursework.md` -- I was given permission to upload this repository to my own GitHub.

The project as a whole is not completely finished, as I dropped it after the submission deadline, and is therefore not a perfect implementation. It does however display many good coding practices.

## Installation

Upon cloning the repository, the code can be built by running

```bash
$ make clean
$ make build
$ make
```
where the first line is in principle not necessary, but just for safety.

The folder `bin` should now have been created, containing two executables. The first, `play_simple`, is to run the code without any parallelization. To run, for example, the Gosper glider gun one should execute
```bash
$ mkdir examples/Gosper_glider_gun/txt/
$ bin/play_simple examples/Gosper_glider_gun/params.txt
```
Note that, when running (or creating a new) example, one should first make sure the output path in the `params.txt` file exists, which is why we created the folder `txt/` first.

To run the parallelized code, one should run something similar, being
```bash
$ mpirun -n 4 bin/play_simple examples/Gosper_glider_gun/params.txt
```
where in this case the number of MPI ranks is chosen to be 4. This can be adapted (however, see below under parallelization).

A couple of unit tests can be run as
```bash
$ ctest --test-dir build/release
```
to ensure the code is working.

## Parameter file

All the parameters for the code should be given in a `params.txt` file, for which examples can be found in `examples/`. The parameters should be self-explanatory, though there are two important things:
- In the case that the game is initialized from a file (`random_data = 0`), the `board_size` should match the actual board size in the `board_file`.
- The `output_path` should exist, prior to running the code, as mentioned above.

I have saved following example folders:
- Gosper glider gun
- `spaceship` (the glider)
- `simple_board` which is just a small randomly initialized board.
- `large`: a folder in which I saved large grids of length $\mathcal{O}(1000)$.
- `crit_5`, in which I experimented with `N_critical = 5`.
## Parallelization

In the parallelized code, I have made use of a 2D Cartesian communicator for the MPI. 
It can take many different values for the number of ranks, but there is the following two requirements:
- The number of ranks $N$ should be factorizable as $N = a \times b$, where $a,b$ are both divisors of `board_size`, i.e. the width of the board. 
- Furthermore, both $a$ and $b$ need to be smaller than the board size.

The reason is that the programme divides the grid into subgrids, which at this point have to be of equal size. Due to the Cartesian topology, the requirement follows. The second requirement is simply due to the implementation of the iterative procedure, which does not handle one-dimensional or single-cell boards.

OpenMP parallelization is handled in the `params.txt` file, where the number of openMP threads can be indicated. However, openMP parallelisation is not yet optimised at the moment, and seems to slow the code down for the grids I tested. Very large grids could potentially experience a speed-up. This is currently under development in the `dev` branch.

## Supercomputer

The code has been tested on the CSD3 supercomputer at the University of Cambridge. An example submission script is given in the `examples/` folder, for the icelake partition. This example script runs both the serial and parallel code, and times them.

## Post-processing of txt files

The boards are saved as `txt` files that can be converted to PNG with the `post_process/visualize.py` script. This is a very basic script, however.

## Documentation

Documentation for the code can be found in `docs/`, in PDF format.
