//============================================================================
// Name        : training.cpp
// Author      : Michal Simon
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <tuple>

#include <algorithm>
#include <iterator>

#include <cmath>

struct solution
{
    solution(int x, int y) : x(x), y(y), _x(fabs(x)), _y(fabs(y)) {}

    int arithmetic_progression (int n)
    {
        return (n * n + n) / 2;
    }

    int calc_n()
    {
        double const a = 1;
        double const b = 1;
        double const c = -2 * (_x + _y);
        double delta = sqrt(b * b - 4 * a * c);
        return ceil((-2 * c) / (delta + b));
    }

    void calc_negative(int neg, int size)
    {
        while (neg > 0)
        {
            if (neg <= size)
            {
                negative.insert(neg);
                return;
            }

            negative.insert(size);
            neg -= size;
            --size;
        }
    }

    void calc_moves()
    {
        int n = calc_n();
        int sum = arithmetic_progression(n);

        while (sum % 2 != (_x + _y) % 2)
        {
            ++n;
            sum += n;
        }

        int neg = (sum - (_x + _y)) / 2;
        calc_negative(neg, n);

        for (int i = 1; i <= n; ++i) moves.push_back(negative.count(i) ? -i : i);
    }

    int next_max(int max)
    {
        while(x_moves.count(moves[max - 1]))
        {
            --max;
        }

        return max;
    }

    void calc_x_moves()
    {
        int sum_x = 0, max = moves.size();

        while (sum_x != _x)
        {
            if (_x - sum_x <= max)
            {
                int move = moves[_x - sum_x - 1];
                sum_x += move;
                x_moves.insert(move);
                max = next_max(max);

            }
            else
            {
                sum_x += moves[max - 1];
                x_moves.insert(moves[max - 1]);
                max = next_max(max);
            }
        }
    }

    std::string solve()
    {
        calc_moves();
        calc_x_moves();

        std::string ret;

        std::vector<int>::const_iterator it;
        for (it = moves.begin(); it != moves.end(); ++it)
        {
            if (x_moves.count(*it)) ret += get_horisontal(*it >= 0);
            else ret += get_vertical(*it >= 0);
        }

        return ret;
    }

    char get_horisontal(bool positive)
    {
        if ( (x >= 0 && positive) || (x < 0 && !positive) ) return 'E';
        return 'W';
    }

    char get_vertical(bool positive)
    {
        if ( (y >= 0 && positive) || (y < 0 && !positive) ) return 'N';
        return 'S';
    }

    int x;
    int y;
    int _x;
    int _y;

    std::vector<int> moves;
    std::set<int> x_moves;
    std::set<int> negative;
};

std::tuple<int, int> check(std::string const & solution)
{
    int x = 0, y = 0;
    int counter = 1;

    std::string::const_iterator it;
    for (it = solution.begin(); it != solution.end(); ++it)
    {
        switch (*it)
        {
        case 'E':
            x += counter;
            break;
        case 'W':
            x -= counter;
            break;
        case 'S':
            y -= counter;
            break;
        case 'N':
            y += counter;
            break;
        }
        ++counter;
    }

    return std::make_tuple(x, y);
}


//int main()
//{
//    int count, x, y, _x, _y;
//    std::ifstream fin("/home/simonm/Downloads/B-large-practice.in");
//    std::ofstream fout("/home/simonm/large.out");
//
//    fin >> count;
//
//    for (int i = 0; i < count; ++i)
//    {
//        fin >> x;
//        fin >> y;
//        std::cout << "Case #" << (i + 1) << ": ";
//        fout << "Case #" << (i + 1) << ": ";
//        std::string result = solution(x, y).solve();
//        std::cout << result << std::endl;
//        fout << result << std::endl;
//        std::tie(_x, _y) = check(result);
//
//        if (x != _x || y != _y)
//        {
//            std::cout << "PROBLEM" << std::endl;
//            break;
//        }
//    }
//
//    fout.flush();
//    fout.close();
//
//	return 0;
//}
