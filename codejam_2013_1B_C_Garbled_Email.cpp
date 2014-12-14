//============================================================================
// Name        : codejam.cpp
// Author      : Michal Simon
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <queue>
#include <unordered_set>
#include <map>
#include <algorithm>

using namespace std;


class word_set
{
public:

	word_set()
	{
		s.reserve(521196);
	}

	void insert (std::string const & word)
	{
		add_all(word);
	}

	std::pair<std::string, int> find(std::string word, std::string::size_type start)
	{
		std::queue< std::pair<std::string, std::string::size_type> > q;
		q.push(std::make_pair(word, start));

		while (!q.empty())
		{
			// get the parameters from the queue
			std::tie(word, start) = q.front(); q.pop();
			// check if it is the one
			if (s.find(word) != s.end())
			{
				int index = start > word.size() ? start - word.size() : 0;
				return std::make_pair(word, index);
			}
			// generate new words
			for (std::string::size_type i = start; i < word.size(); ++i)
			{
				std::string s = word;
				s[i] = '*';
				q.push(std::make_pair(s, i + 5));
			}
		}

		return std::make_pair("", 0);
	}

private:

	void add_all(std::string const & word, int start = 0)
	{
		s.insert(word);

		for (std::string::size_type i = start; i < word.size(); ++i)
		{
			std::string s = word;
			s[i] = '*';
			add_all(s, i + 5);
		}
	}

	std::unordered_set<std::string> s;
};

struct result
{
	result() : index(0), not_matching(0) {}
	result(result const & copy) : index(copy.index), str(copy.str), not_matching(copy.not_matching) {}

	static bool cmp(result const & r1, result const & r2)
	{
		if (r1.not_matching == r2.not_matching) return r1.str.size() < r2.str.size();
		return r1.not_matching > r2.not_matching;
	}

	typedef vector<result> mem_type;
	typedef bool(*cmp_type)(result const &, result const &);

	static int find(word_set & dict, string const & line, string::size_type max_length)
	{
		memo.clear();
		priority_queue<result, mem_type, cmp_type> q(result::cmp);
		q.push(result());

		while(!q.empty())
		{
			// get the top element
			result const top = q.top(); q.pop();
			// check if it is the solution
			if (top.str.size() == line.size()) return top.not_matching;

			for (string::size_type length = 1; length <= max_length; ++length)
			{
				if (top.str.size() + length > line.size()) break;
				std::string s = line.substr(top.str.size(), length);

				std::pair<std::string, int> ret = dict.find(s, top.index);
				if (!ret.first.empty())
				{
					result r = top;
					r.not_matching += std::count(ret.first.begin(), ret.first.end(), '*');
					r.str += ret.first;
					r.index = ret.second;

					std::pair<int, int> key = std::make_pair(r.str.size(), r.index);
					std::map< std::pair<int, int>, int >::iterator it = memo.find(key);
					if (it == memo.end() || r.not_matching < it->second)
					{
						memo.insert(std::make_pair(key, r.not_matching));
						q.push(r);
					}
				}
			}
		}

		return -1;
	}

	static std::map< std::pair<int, int>, int > memo;

	int index;
	std::string str;
	int not_matching;
};

std::map< std::pair<int, int>, int > result::memo;


int solve(istream& in, word_set & dict, string::size_type max_length)
{
	string line;
	in >> line;

	return result::find(dict, line, max_length);
}

void doit ()
{
	ifstream din ("/home/simonm/Pobrane/garbled_email_dictionary.txt");

	string line;
	word_set dict;
	string::size_type max_length = 0;

	while((din >> line))
	{
		dict.insert(line);
		if (line.size() > max_length) max_length = line.size();
	}

	std::cout << "Dict ready" << std::endl;

	ifstream fin ("/home/simonm/Pobrane/C-large-practice.in");
//	ifstream fin ("/home/simonm/Pobrane/C-small-practice(1).in");
//	ifstream fin ("/home/simonm/Pobrane/input.txt");
	ofstream fout("/home/simonm/prog.out");

	int count = 0;
	fin >> count;

	for (int i = 1; i <= count; i++) {

		int result = solve(fin, dict, max_length);
		cout << "Case #" << i << ": " << result << endl;
		fout << "Case #" << i << ": " << result << endl;
	}

	fin.close();

	fout.flush();
	fout.close();
}


int main() {

	doit();

	return 0;
}
