#!/usr/bin/python

import numpy as np
from PIL import Image

def gen_lattice(N, M):
	return np.random.choice([-1, 1], size = (N, M))

def graph_lattice(lattice):
	return Image.fromarray(np.uint8((lattice + 1) * 0.5 * 255))

def compute_step(lattice, BETA, H):
	N = lattice.shape[0]
	M = lattice.shape[1]
	i = np.random.randint(N)
	j = np.random.randint(M)
	dE = test_flip(i, j, lattice, BETA, H)

	if (dE < 0):
		lattice[i][j] = - lattice[i][j]
	else:
		if (np.random.ranf() < np.exp(- dE * BETA)):
			lattice[i][j] = - lattice[i][j]
	return lattice

# returns computed change in energy
def test_flip(i, j, lattice, BETA, H):
	max_i = lattice.shape[0] - 1
	max_j = lattice.shape[1] - 1

	top = i - 1
	bottom = i + 1
	left = j - 1
	right = j + 1

	if (i == 0):
		top = max_i
	elif (i == max_i):
		bottom = 0

	if (j == 0):
		left = max_j
	elif (j == max_j):
		right = 0

	dE = 2 * BETA * lattice[i][j] * (lattice[top][j] + lattice[bottom][j] + lattice[i][left] + lattice[i][right]) + 2 * H * lattice[i][j]
	return dE

main_lattice = gen_lattice(200,200)
print(main_lattice.shape)
image_file = graph_lattice(main_lattice)
image_file.save("./lattice.png",format = "PNG")

for i in range(1000000):
	main_lattice = compute_step(main_lattice, 2, 0)
image_file = graph_lattice(main_lattice)
image_file.save("./lattice_equilibrium.png",format = "PNG")
