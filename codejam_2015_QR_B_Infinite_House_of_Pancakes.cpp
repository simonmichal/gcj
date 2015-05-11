//============================================================================
// Name        : test.cpp
// Author      : Michal Simon
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include <iostream>
#include <algorithm>
#include <vector>

#include <queue>

std::vector<int> eat(std::vector<int> const & diners)
{
    std::vector<int> ret;
    for (auto it = diners.begin(); it != diners.end(); ++it) 
    {
        if (*it > 1)
            ret.push_back(*it - 1);
    }
    std::make_heap(ret.begin(), ret.end());
    return ret;
}

std::vector<int> split(std::vector<int> const & diners, int first, int second)
{
    // copy input vector
    std::vector<int> ret;
    std::copy(diners.begin(), diners.end(), std::back_inserter(ret));
    // pop the max
    std::pop_heap(ret.begin(), ret.end());
    // replace the max with first
    ret.back() = first;
    std::push_heap(ret.begin(), ret.end());
    // add second
    ret.push_back(second);
    std::push_heap(ret.begin(), ret.end());

    return ret;
}

int solve(std::vector<int> const & diners)
{
    // base case:
    // max diner is 1 => we need only one round to eat everything
    if (diners.front() == 1)
        return 1;
    auto max = diners.front();
    // recurse
    auto ret = solve(eat(diners)) + 1;
    for (int i = 2; i <= max / 2; ++i) 
    {
        auto tmp = solve(split(diners, i, max - i)) + 1;
        if (tmp < ret) ret = tmp;
    }

    return ret;
}

int solve_bfs(std::vector<int> const & diners)
{
    std::queue<std::pair < std::vector<int>, int>> q;
    q.push(std::make_pair(diners, 0));

    while (!q.empty()) 
    {
        // pop the first element in the queue
        auto v = q.front();
        q.pop();
        auto max = v.first.front();
        // base case
        if (max == 1) return v.second + 1;
        // recurse
        // eat part
        q.push(std::make_pair(eat(v.first), v.second + 1));
        // split part
        for (int i = 2; i <= max / 2; ++i) 
        {
            q.push(std::make_pair(split(v.first, i, max - i), v.second + 1));
        }
    }
    // that the worst case
    return diners.front();
}

int main(void)
{
    std::cout << "Start!" << std::endl;

    int count = 0;
    std::cin >> count;

    size_t size;

    for (int i = 0; i < count; ++i) {
        std::cin >> size;
        std::vector<int> diners;
        for (size_t j = 0; j < size; ++j) {
            int tmp = 0;
            std::cin >> tmp;
            diners.push_back(tmp);
        }
        std::make_heap(diners.begin(), diners.end());
        std::cout << "Case #" << (i + 1) << ": " << solve_bfs(diners) << std::endl;
    }
}
