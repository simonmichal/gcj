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
#include <stack>
#include <map>
#include <algorithm>

using namespace std;

long long mod = 1000002013;

struct ticket
{
	ticket(long long start, long long count) : start(start), count(count) {}

	long long start;
	long long count;
};

long long pay (long long base, long long stops)
{
	return stops * (2 * base + (1 - stops)) / 2 % mod;
}

long long getOff(long long stop, long long count, long long nStops, stack<ticket>& tickets)
{
	// if no one is getting off there is nothing to pay
	if (count <= 0) return 0;
	// the most recent ticket
	ticket& t = tickets.top();
	// cover ass many passengers with the most recent ticket as possible
	long long sum = min(count, t.count) * pay(nStops, stop - t.start);
	// check how many passengers where cover
	count -= t.count;
	// if not all passengers where covered ...
	if (count >= 0)
	{
		// remove empty ticket
		tickets.pop();
		// calc for the next one on stack
		sum += getOff(stop, count, nStops, tickets);
	}
	else
	{
		// set the number of passengers for the most recent ticket
		t.count = -count;
	}

	return sum % mod;
}

long long solve (ifstream& fin)
{
	int lines = 0, nStops = 0;

	fin >> nStops; // get the number of lines
	fin >> lines; // get the number of stops

	long long sumHonest = 0, sumCheating = 0;

	map<long long, long long> stops;

	stack<ticket> tickets;

	long long start, stop, count;
	for (int i = 0; i < lines; i++)
	{
		// get the data from file
		fin >> start;
		fin >> stop;
		fin >> count;
		// calculate the sum if passengers were honest
		sumHonest += count * pay(nStops, stop - start) % mod;
		// fill in the 'stops' vector
		stops[start] += count;
		stops[stop]  -= count;
	}

	map<long long, long long>::iterator itr;
	for (itr = stops.begin(); itr != stops.end(); ++itr)
	{
		if (itr->second > 0)
		{
			tickets.push(ticket(itr->first, itr->second));
		}
		else if (itr->second < 0)
		{
			sumCheating += getOff(itr->first, -itr->second, nStops, tickets);
		}
	}



	long long ret = (sumHonest - sumCheating) % mod;
	return ret < 0 ? ret + mod : ret;
}

void doit ()
{

	int count = 0;

	ifstream fin (
			"/home/simonm/Pobrane/A-large-practice(4).in"
//			"/home/simonm/Pobrane/A-small-practice(4).in"
		);
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

