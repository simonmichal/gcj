//============================================================================
// Name        : codejam.cpp
// Author      : Michal Simon
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
#include <map>

using namespace std;

//One millilitre of paint is required to cover area π cm^2
int solve (ifstream& fin) {

	long long r = 0, t = 0;

	// first get the data
	fin >> r;
	fin >> t;

	// that's the equation we need to solve:
	// 2n^2 + (2r - 1)n - t <= 0
	// 2n^2 + (2r - 1)n -> sum off all rings (arithmetic progression)

	// now look for the greater root of the equation using the standard formula
	double a = 2.0;
	double b = 2.0 * r - 1.0;
	double c = -t;

	// calculate delta
	double delta = b * b - 4.0 * a * c;
	// calculate the root of the equation
	// we round the result up because of the problems with double precision (e.g. 0.999999999 should be 1)
  // we also don't use the standard formula but a transformed one (by multiplying counter and denominator by 'b + sqrt(delta)')
  // because we want to avoid subtraction of two very big numbers that are almost equal because it leads to precision loss
	long long n = ceil ( (-2.0 * c) / (b + sqrt(delta)) );

	// now we need to make sure the result is OK
	// since we are rounding up it might be to high so we might need to decrement
	while (t - 2 * n * n - (2 * r - 1) * n < 0) --n;

	return n;
}

void doit () {

	int count = 0;

	ifstream fin ("/home/simonm/Pobrane/A-large-practice(1).in");
	ofstream fout("/home/simonm/prog2.out");

	fin >> count;

	for (int i = 1; i <= count; i++) {
		int result = solve(fin);
		cout << "Case #" << i << ": " << result << endl;
//		fout << "Case #" << i << ": " << result << endl;
	}

	fin.close();

	fout.flush();
	fout.close();
}


int main() {

	time_t start = time(0);
	doit();
	time_t stop = time(0);
	cout << endl << stop - start << endl;

	return 0;
}
