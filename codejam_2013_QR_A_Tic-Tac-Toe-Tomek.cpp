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

using namespace std;

bool check (unsigned short int& player) {

	unsigned short int mask[] = {0xf, 0xf0, 0xf00, 0xf000, 0x8888, 0x4444, 0x2222, 0x1111, 0x1248, 0x8421};

	for (int i = 0; i < 10; i++) {
		if ( (player & mask[i]) == mask[i]) return true;
	}

	return false;
}

bool fill(unsigned short int& x, unsigned short int& o, ifstream& fin) {

	bool empty_field = false;

	char c = 0;

	for (int i = 0; i < 16; i++) {

		fin >> c;

		if (c == 'X' || c == 'T') {
			x |= 1 << i;
		}

		if (c == 'O' || c == 'T') {
			o |= 1 << i;
		}

		empty_field |= c == '.';
	}

	return empty_field;
}

void solve (unsigned short int x, unsigned short int o, bool empty_field, int index) {

	bool x_won = check(x);
	if (x_won) {
		cout << "Case #" << index << ": X won" << endl;
		return;
	}

	bool o_won = check(o);
	if (o_won) {
		cout << "Case #" << index << ": O won" << endl;
		return;
	}

	if (empty_field) {
		cout << "Case #" << index << ": Game has not completed" << endl;
		return;
	}

	cout << "Case #" << index << ": Draw" << endl;
}

void doit () {

	int count = 0;

	ifstream fin ("/home/simonm/Pobrane/A-large-practice.in");

	fin >> count;

	for (int i = 1; i <= count; i++) {

		unsigned short int x = 0, o = 0;
		bool empty_field = fill(x, o, fin);
		solve(x, o, empty_field, i);
	}

	fin.close();
}


int main() {

	time_t start = time(0);
	doit();
	time_t stop = time(0);
	cout << endl << stop - start << endl;

	return 0;
}
