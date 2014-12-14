//============================================================================
// Name        : codejam.cpp
// Author      : Michal Simon
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

template <typename T>
class matrix
{
public:
	matrix(const int rows, const int columns) : values(rows * columns), rows(rows), columns(columns) {}

	T& at(const int row, const int column)
	{
		return values.at(row * columns + column);
	}

	T const & at(const int row, const int column) const
	{
		return values.at(row * columns + column);
	}

//	bool check(const int row, const int column)
//	{
//		bool left  = at(row, column) >= at(row, column - 1);
//		bool right = at(row, column) >= at(row, column + 1);
//		bool up    = at(row, column) >= at(row - 1, column);
//		bool down  = at(row, column) >= at(row + 1, column);
//
//		return left | right | up | down;
//	}

	int wight() const {return columns;}
	int hight() const {return rows;}

private:
	vector<T> values;
	const int rows;
	const int columns;
};


bool check(const matrix<int>& lawn, const int row, const int column)
{
	bool horizontal = true;
	for (int i = 0; i < lawn.wight(); ++i)
	{
		if ( !(lawn.at(row, column) >= lawn.at(row, i)) )
		{
			horizontal = false;
			break;
		}
	}

	bool vertical = true;
	for (int i = 0; i < lawn.hight(); ++i)
	{
		if ( !(lawn.at(row, column) >= lawn.at(i, column)) )
		{
			vertical = false;
			break;
		}
	}

	return horizontal | vertical;
}

bool solve(istream& in)
{
	int n = 0, m = 0;
	in >> n;
	in >> m;

	matrix<int> lawn (n, m);

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
		{
			int val = 0;
			in >> val;
			lawn.at(i, j) = val;
		}



	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			if (!check(lawn, i, j))
			{
				return false;
			}

	return true;
}

void doit ()
{

	int count = 0;

	ifstream fin (
			"/home/simonm/Pobrane/B-large-practice.in"
//			"/home/simonm/Pobrane/B-small-practice.in"
		);
//	ifstream fin ("/home/simonm/Pobrane/test.in");
//	ofstream fout("/home/simonm/prog2.out");

	fin >> count;

	for (int i = 1; i <= count; i++) {

		bool result = solve(fin);
		cout << "Case #" << i << ": " << (result ? "YES" : "NO") << endl;
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

