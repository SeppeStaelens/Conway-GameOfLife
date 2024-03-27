# Conway's Game of Life
### Seppe Staelens, ss3033

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

In the parallelized code, I have made use of a 2D Cartesian communicator for the MPI. At the time of writing, this is not super flexible yet.
- Based on the input number of ranks, the program looks for divisors of this number that are somewhat similar, to avoid making e.g 2x10 Cartesian grids (this is mostly personal preference at this point). For the perfect squares it simply returns the root of the number of ranks.
- This root needs to be a divisor of the `board_size`, as the programme currently only supports subgrids of equal size.

The foundations are in the code to extend this to be more flexible, but there are still MPI bugs at the moment.

OpenMP parallelization is handled in the `params.txt` file, where the number of openMP threads can be indicated.

## Supercomputer

The code has been tested on the CSD3 supercomputer at the University of Cambridge. An example submission script is given in the `examples/` folder, for the icelake partition. This example script runs both the serial and parallel code, and times them.

## Post-processing of txt files

The boards are saved as `txt` files that can be converted to PNG with the `post_process/visualize.py` script. This is a very basic script, however.

## Documentation

Documentation for the code can be found in `docs/`, in PDF format.
