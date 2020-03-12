#include <Peggy2.h>
#include <math.h>
#include <limits.h>

#define MAX_INDEX 24

Peggy2 frame;

int b1_pin = A0;
int b2_pin = A1;
int b3_pin = A2;
int b4_pin = A3;
int b5_pin = A4;
int s2_pin = 8;

double rand_double(){
	return (double) random(0, INT_MAX) / INT_MAX;
}

void setup() {

	// hardware init
	frame.HardwareInit();
	frame.Clear();
	pinMode(b1_pin, INPUT_PULLUP);
	pinMode(b2_pin, INPUT_PULLUP);
	pinMode(b3_pin, INPUT_PULLUP);
	pinMode(b4_pin, INPUT_PULLUP);
	pinMode(b5_pin, INPUT_PULLUP);
	pinMode(s2_pin, INPUT_PULLUP);

	// generate seed via random input noise
	randomSeed(analogRead(0) + analogRead(1) + analogRead(2)
	           + analogRead(3) + analogRead(4) + analogRead(5));

	// generate pseudorandom lattice
	for(int i = 0; i < 25; i++)
		for(int j = 0; j < 25; j++)
			if(random(0, INT_MAX) % 2 == 0) frame.SetPoint(i,j);

}

int top, bottom, left, right, point;
int top_i, bottom_i, left_j, right_j;
int i, j;
int b1, b2, b3, b4, b5, s2;
int dE;

// thermodynamic beta
double beta = 10;

// dipole interaction constant
int K = 100000;

// external magnetic field strength
int H = 0;

int val;

void loop() {

	// read input buttons
	b1 = digitalRead(b1_pin);
	b2 = digitalRead(b2_pin);
	b3 = digitalRead(b3_pin);
	b4 = digitalRead(b4_pin);
	b5 = digitalRead(b5_pin);
	s2 = digitalRead(s2_pin);

	// change temperature
	if(b3 == LOW)
		beta *= 2;
	if(b4 == LOW)
		beta /= 2;

	// change external magnetic field
	if(b5 == LOW)
		H = (H > 0) ? H * 2 : 1;
	if(b2 == LOW)
		H = (H < 0) ? H * 2 : -1;

	// toggle ferromagnetism/anti-ferromagnetism
	if(b1 == LOW)
		K = -100000;
	if(s2 == LOW)
		K = 100000;

	// pick a random lattice point
	i = random(0, 25);
	j = random(0, 25);

	top_i = i - 1;
	bottom_i = i + 1;
	left_j = j - 1;
	right_j = j + 1;

	// assume periodic boundary conditions
	if(i == 0)
		top_i = MAX_INDEX;
	else if(i == MAX_INDEX)
		bottom_i = 0;
	if(j == 0)
		left_j = MAX_INDEX;
	else if(j == MAX_INDEX)
		right_j = 0;

	// determine spin states of this point and its neighbors
	point = frame.GetPoint(i, j) ? 1 : -1;
	top = frame.GetPoint(top_i, j) ? 1 : -1;
	bottom = frame.GetPoint(bottom_i, j) ? 1 : -1;
	left = frame.GetPoint(i, left_j) ? 1 : -1;
	right = frame.GetPoint(i, right_j) ? 1 : -1;

	// calculate changes in energy
	dE = 2 * point * K * (top + bottom + left + right) + 2 * H * point;

	// flip if dE < 0, otherwise use Boltzmann distribution and flip
	// accordingly.
	if(dE < 0){
		frame.WritePoint(i, j, point - 1);
	} else {
		if(rand_double() < exp(- beta * dE))
			frame.WritePoint(i, j, point - 1);
	}

	// draw the frame
	frame.RefreshAll(1);
}
