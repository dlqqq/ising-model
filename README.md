# ising-model

A collection of small Monte Carlo simulations of the Ising model.

## C++/Arduino Implementation

See `ising.ino`. This is the most accurate, optimized, and fully-featured
simulation thus far. This is meant to be flashed on a Peggy 2LE LED matrix
board. The buttons can toggle ferromagnetic/anti-ferromagnetic simulation,
temperature, and the external magnetic field.

## Jupyter/Python3

The Jupyter notebook contains a brief overview of the Ising model and explains
the simulation. However, the code in both the python and jupyter script is not
entirely correct, as it includes an extraneous term (explained in the jupyter
notebook). This leads to the same observations (temperature-dependent phase
transitions, ferromagnetic/anti-ferromagnetic patterns, etc.), but is not
precise enough for numerical analysis. I have corrected the equations in the
introduction, but will not finish correcting and re-compiling all the code.
