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
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

bool isVowel(char c) {

	static set<char> vowels;
	if (vowels.empty()) {
		vowels.insert('a');
		vowels.insert('e');
		vowels.insert('i');
		vowels.insert('o');
		vowels.insert('u');
	}

	return vowels.count(c);
}

long long countAll (long long start, long long stop, long long size) {
	return (start + 1) * (size - stop);
}

long long repeated (long long start_prev, long long stop, long long size) {
	return (size - stop) * (start_prev + 1);
}

long long solve (ifstream& fin) {

	// first read the case

	string name;
	int n = 0;

	fin >> name;
	fin >> n;

	// iteration count
	int consecutive_consonants = 0, size = name.size(), start_prev = -1;
	long long nvalue = 0;

	for (int i = 0; i < size; i++) {
		// if it is not a vowel count it
		if (!isVowel(name[i])) consecutive_consonants++;
		// if it is a vowel start counting from 0
		else consecutive_consonants = 0;
		// if the number is equal or grater to n ...
		if (consecutive_consonants >= n) {
			// count all sub string containing this one
			nvalue += countAll(i - n + 1, i, size);
			// deduce those which were already counted
			nvalue -= repeated(start_prev, i, size);
			// update start_prev
			start_prev = i - n + 1;
		}
	}

	// return the size
	return nvalue;
}

void doit () {

	int count = 0;

	ifstream fin ("/home/simonm/Pobrane/A-large-practice(3).in");
//	ifstream fin ("/home/simonm/Pobrane/test.in");
//	ofstream fout("/home/simonm/prog2.out");

	fin >> count;

	for (int i = 1; i <= count; i++) {

		long long result = solve(fin);
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

