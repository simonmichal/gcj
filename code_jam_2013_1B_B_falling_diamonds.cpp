/*
 * code_jam_2013_1C_C_great_wall.cpp
 *
 *  Created on: 7 Oct 2014
 *      Author: simonm
 */

#include <fstream>
#include <iostream>
#include <map>
#include <tuple>

#include <math.h>

typedef std::pair<size_t, size_t> lookup_t;
typedef std::tuple<size_t, size_t, size_t, size_t, int, int, double> param_t;

void add_to_map(std::map<lookup_t, param_t> & m, lookup_t const & k, param_t const & v)
{
    // find item with the same key in the map
    auto it = m.find(k);
    // if it is not there
    if (it == m.end())
    {
        m.insert({k, v});
    }
    // if it is already in the map
    else
    {
        // just update the probability
        std::get<6>(it->second) += std::get<6>(v);
    }
}

double probability(size_t count, size_t count_left, size_t count_right, size_t n, int x, int y, double p)
{
    // sum up the probabilities for all the cases
    // when the point of interest was encountered
    double ret = 0.0;
    // use own map for recursion (this way redundant elements are eliminated)
    std::map<lookup_t, param_t> m;
    // add the first set of parameters
    add_to_map(m, lookup_t{count_left, count_right}, param_t{count, count_left, count_right, n, x, y, p});
    // loop until the stack is empty
    while(!m.empty())
    {
        auto it = m.begin();
        // get the parameters from the stack
        std::tie(count, count_left, count_right, n, x, y, p) = it->second;
        m.erase(it);
        // check if the point of interest has been encountered
        if (x < 0 && count_left > 0)
        {
            int x_new = -(n - 1) + (count_left - 1), y_new = count_left - 1;
            // base case 1: the point of interest has been encountered
            if (x == x_new && y == y_new)
            {
                ret += p;
                continue;
            }
        }
        else if (x > 0 && count_right > 0)
        {
            int x_new = n - 1 - (count_right - 1), y_new = count_right - 1;
            // base case 1: the point of interest has been encountered
            if (x == x_new && y == y_new)
            {
                ret += p;
                continue;
            }
        }
        // base case 2: all diamonds have fallen down
        if (count == 0) continue;
        // there are still diamonds left ...
        // if the diamond can slide both ways
        if (count_left < n - 1 && count_right < n - 1)
        {
            add_to_map(m, lookup_t{count_left + 1, count_right},
                    param_t{count - 1, count_left + 1, count_right, n, x, y, 0.5 * p});
            add_to_map(m, lookup_t{count_left, count_right + 1},
                    param_t{count - 1, count_left, count_right + 1, n, x, y, 0.5 * p});
        }
        // if the diamond can slide only towards right
        else if (count_left == n - 1)
        {
            add_to_map(m, lookup_t{count_left, count_right + 1},
                    param_t{count - 1, count_left, count_right + 1, n, x, y, p});
        }
        // if the diamond can slide only towards left
        else /*if (count_right == n - 1)*/
        {
            add_to_map(m, lookup_t{count_left + 1, count_right},
                    param_t{count - 1, count_left + 1, count_right, n, x, y, p});
        }
    }

    return ret;
}

double solve(size_t count, int x, int y)
{
    // first figure out how many diamonds have certain positions
    // look for a sum of arithmetic progression closest to 'count'
    // solve the following equation: n^2 + n - 2 * count = 0
    double sqrt_delta = sqrt(1 + 8 * count);
    // we are only interested in positive root
    // also transform the formula to -2*c / (sqrt_delta + b)
    // in order to avoid precision loss
    double root = 4 * count / (sqrt_delta + 1);
    // our number is the greatest odd number
    // smaller than or equal to root
    int n = floor(root);
    if (!(n % 2)) --n;
    // the diamonds form a triangle with the following vertices
    // A(-n + 1, 0), B(n - 1, 0), C(0, n - 1)
    // if the diamond is within the triangle the probability is 1.0
    // following inequalities have to be met:
    // y <= x + n - 1 and y <= -x + n - 1
    if (y - x <= n - 1 && y + x <= n - 1) return 1.0;
    // calculate how many diamonds are left
    int spare = count - n * (1 + n) / 2;
    // if it is not in the triangle check if it is possible at all
    // in order to be possible it has to be in a trapezium:
    // A(-n - 1, 0), B(n + 1, 0), C(-n + high, high), D(n - high, high)
    // the high of trapezium may by at most equal to the high of the enclosing triangle - 1
    int high = spare - 1 >= n + 1 ? n : spare - 1;
    // so following inequalities have to be met:
    // y <= x + n + 1 and y <= -x + n + 1 and y <= high
    if (y > high || y - x > n + 1 || y + x > n + 1) return 0.0;
    // calculate the probability
    return probability(spare, 0, 0, n + 2, x, y, 1.0);
}

int main()
{
//    std::ifstream in ("/home/simonm/Downloads/B-small-practice(1).in");
    std::ifstream in ("/home/simonm/Downloads/B-large-practice(1).in");
    int x, y;
    size_t size, count;
    in >> size;

//    time_t start = time(0);

    for (size_t i = 1; i <= size; ++i)
    {
        in >> count >> x >> y;
        std::cout << "Case #" << i << ": " << solve(count, x, y) << std::endl;
    }

//    time_t stop = time(0);
//    std::cout << (stop - start) << std::endl;
    return 0;
}
