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
#include <vector>
#include <algorithm>

using namespace std;

int addMote(int A, int x) {
	return pow(2.0, x) * (A - 1) + 1;
}


int howMany(double A, double mote) {
	return floor( log( (mote - 1) / (A - 1) ) / log(2.0) ) + 1;
}

int eat(int A, vector<int>::iterator begin, vector<int>::iterator end) {

	int b = *begin;

	// if it is the end ...
	if(begin == end) return 0;

	// is A is greater than the smallest one, eat it and proceed further
	if (*begin < A) return eat(A + *begin, begin + 1, end);

	// if we would remove the rest we would need to do ...
	int remove = end - begin;

	// if A equals 1 there's nothing else we can do
	if (A == 1) return remove;

	// A equals the next element
	if (*begin == A) {
		// add one mote
		A += A - 1;
		int add = eat(A + *begin, begin + 1, end) + 1;
		// return whatever is smaller
		return add < remove ? add : remove;
	}

	// how many motes do we need to add
	int add = howMany(A, *begin);
	// add ..
	A = addMote(A, add);

	//  and then we still need to eat the rest ...
	add += eat(A + *begin, begin + 1, end);

	// return whatever is smaller
	return add < remove ? add : remove;
}

int solve (ifstream& fin) {

	// first read the case

	int A = 0, size = 0;

	fin >> A;
	fin >> size;

	vector<int> motes;

	for (int i = 0;  i < size; i++) {
		int tmp;
		fin >> tmp;
		motes.push_back(tmp);
	}

	// then sort the motes
	sort(motes.begin(), motes.end());

	// and eat them up ...
	return eat(A, motes.begin(), motes.end());
}

void doit () {

	int count = 0;

	ifstream fin ("/home/simonm/Pobrane/A-large-practice(2).in");
//	ofstream fout("/home/simonm/prog2.out");

	fin >> count;

	for (int i = 1; i <= count; i++) {
		int result = solve(fin);
		cout << "Case #" << i << ": " << result << endl;
//		fout << "Case #" << i << ": " << result << endl;
	}

	fin.close();

//	fout.flush();
//	fout.close();
}


int main() {

	time_t start = time(0);
	doit();
	time_t stop = time(0);
//	cout << endl << stop - start << endl;

	return 0;
}
