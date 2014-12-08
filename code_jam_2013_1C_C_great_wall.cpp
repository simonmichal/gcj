/*
 * code_jam_2013_1C_C_great_wall.cpp
 *
 *  Created on: 7 Oct 2014
 *      Author: simonm
 */

#include <map>
#include <vector>
#include <tuple>

#include <iostream>
#include <fstream>

#include <limits>
#include <algorithm>

#include <stdlib.h>

// day, west, east, strength
typedef std::tuple<int, int, int, int> attack_t;

class Wall
{

public:
    Wall() : size(0), segment_tree() {}

    void initialize(std::vector<attack_t> & attacks)
    {
        std::vector<int> points;
        points.reserve(2 * attacks.size());
        // put all the points into a vector
        for (auto const & attack : attacks)
        {
            points.push_back(std::get<1>(attack));
            points.push_back(std::get<2>(attack));
        }
        // sort points
        std::sort(points.begin(), points.end());
        // remove duplicates
        auto end = std::unique(points.begin(), points.end());
        // replace each point with a respective index
        for (auto & attack : attacks)
        {
            // use binary search (lower_bound) to find the iterator pointing to the element of interest
            auto it = std::lower_bound(points.begin(), end, std::get<1>(attack));
            std::get<1>(attack) = it - points.begin();
            it = std::lower_bound(points.begin(), end, std::get<2>(attack));
            std::get<2>(attack) = it - points.begin();
        }
        // prepare the wall
        size = end - points.begin();
        segment_tree.resize(calc_size(size), 0);
    }

    bool attack(attack_t const & a) const
    {
        int time, west, east, strength;
        std::tie(time, west, east, strength) = a;
        return query(0, 0, size - 1, west, east - 1) < strength;
    }

    void update(attack_t const & a)
    {
        int time, west, east, strength;
        std::tie(time, west, east, strength) = a;
        update(0, 0, size - 1, west, east - 1, strength);
    }

private:

    static size_t calc_size(size_t size)
    {
        int pow = ceil(log2(size));
        return (1 << (pow + 1)) - 1;
    }

    int query(size_t node, size_t begin, size_t end, size_t q_begin, size_t q_end) const
    {
        // base case: the intervals are not overlapping
        if (q_begin > end || q_end < begin) return std::numeric_limits<int>::max();
        // base case: the interval is inside the query
        if (begin >= q_begin && end <= q_end) return segment_tree[node];
        // recursively check the children
        size_t centre = (begin + end) / 2;
        return std::max(segment_tree[node], // we can do that because new values will always be bigger than the old one
                std::min(query(2 * node + 1, begin, centre, q_begin, q_end), query(2 * node + 2, centre + 1, end, q_begin, q_end)));
    }

    void update(size_t node, size_t begin, size_t end, size_t u_begin, size_t u_end, int value)
    {
        // base case: the intervals are not overlapping
        if (u_begin > end || u_end < begin) return;
        // base case: the interval is inside the query
        // (the children will be updated in a lazy way)
        if (begin >= u_begin && end <= u_end) segment_tree[node] = std::max(value, segment_tree[node]);
        // otherwise ...
        else
        {
            size_t centre = (begin + end) / 2;
            update(2 * node + 1, begin, centre, u_begin, u_end, value);
            update(2 * node + 2, centre + 1, end, u_begin, u_end, value);
            segment_tree[node] = std::max(segment_tree[node], // we can do that because new values will always be bigger than the old ones
                            std::min(segment_tree[2 * node + 1], segment_tree[2 * node + 2]));
        }
    }

    size_t size;
    std::vector<int> segment_tree;
};

class Solution
{
    typedef std::tuple<int, int, int, int, int, int, int, int> tribe_t;
    enum {d = 0, n, w, e, s, delta_d, delta_p, delta_s};

public:

    Solution(std::istream & in)
    {
        int count;
        in >> count;

        for(int i = 0; i < count; ++i)
        {
            tribe_t tribe;
            in >> std::get<d>(tribe) >> std::get<n>(tribe) >> std::get<w>(tribe) >> std::get<e>(tribe);
            in >> std::get<s>(tribe) >> std::get<delta_d>(tribe) >> std::get<delta_p>(tribe) >> std::get<delta_s>(tribe);

            while (std::get<n>(tribe) > 0)
            {
                attacks.push_back(tribe_to_attack(tribe));
                next_tribe(tribe);
            }
        }
        // this will sort attacks by time (since it is the first tuple element)
        std::sort(attacks.begin(), attacks.end(), attack_cmp);
        // prepare interval in the wall
        wall.initialize(attacks);
    }

    int solve()
    {
        int ret = 0;
        int day = std::get<0>(attacks.front());
        std::vector<std::vector<attack_t>::iterator> update;
        std::vector<attack_t>::iterator it;
        for (it = attacks.begin(); it != attacks.end(); ++it)
        {
            if (std::get<0>(*it) != day)
            {
                uppdate_wall(update);
                update.clear();
                day = std::get<0>(*it);
            }
            if (wall.attack(*it))
            {
                ++ret;
                update.push_back(it);
            }
        }
        return ret;
    }

private:

    void uppdate_wall(std::vector<std::vector<attack_t>::iterator> const & update)
    {
        for (auto & itr : update)
        {
            wall.update(*itr);
        }
    }

    attack_t tribe_to_attack(tribe_t const & t)
    {
        return std::make_tuple(std::get<d>(t), std::get<w>(t), std::get<e>(t), std::get<s>(t));
    }

    void next_tribe(tribe_t & tribe)
    {
        std::get<d>(tribe) += std::get<delta_d>(tribe);
        std::get<n>(tribe) -= 1;
        std::get<w>(tribe) += std::get<delta_p>(tribe);
        std::get<e>(tribe) += std::get<delta_p>(tribe);
        std::get<s>(tribe) += std::get<delta_s>(tribe);
    }

    static bool attack_cmp(attack_t const & a1, attack_t const & a2)
    {
        return std::get<0>(a1) < std::get<0>(a2);
    }

    std::vector<attack_t> attacks;
    Wall wall;
};

//int main()
//{
////    std::ifstream in ("/home/simonm/Downloads/C-small-practice.in");
//    std::ifstream in ("/home/simonm/Downloads/C-large-practice.in");
//    int count;
//    in >> count;
//
//    time_t start = time(0);
//
//    for (int i = 1; i <= count; ++i)
//    {
//        int v = Solution(in).solve();
//        std::cout << "Case #" << i << ": " << v << std::endl;
//    }
//
//    time_t stop = time(0);
////    std::cout << (stop - start) << std::endl;
//    return 0;
//}
