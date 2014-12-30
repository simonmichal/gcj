/*
 * main.cpp
 *
 *  Created on: 12 Sep 2014
 *      Author: simonm
 */

#include <iostream>
#include <fstream>
#include <array>

// count the number of set bits in a byte at compile time
constexpr unsigned bits_set(unsigned char byte, int bits = 8)
{
	return bits == 0 ? 0 : (byte & 1) + bits_set(byte >> 1, bits - 1);
}

// recursive data structure
// with the number of bits set
// for each possible byte
template<int SIZE>
struct bytes_t
{
	// remaining values
    bytes_t<SIZE - 1> other_values;
    // the number of bits set for (SIZE - 1) byte
    // e.g. for SIZE = 8 it is (7)decimal =
    // (111)binary so it equals to 3
    int value;
    // set the value in constructor
    bytes_t() : value(bits_set(SIZE - 1)) {}
    // subscript operator
    int operator[](size_t index)
    {
    	// reinterpret the data structure as an array of ints
    	// (dirty trick but it works ;)
        return reinterpret_cast<int*>(this)[index];
    }
};

// template specialization - the base case
template<>
struct bytes_t<1> 			// it is important that this one ends at 1
{                 			// and at 0 because it has to contain a value (int)
    int value;    			// otherwise we would have an empty struct
    bytes_t() : value(0) {} // whose size is greater than 0 according to c++ standard,
};							// and this in turn would break the subscript operator

// counts the number of bits that are set in a unsigned long long
unsigned count_set_bits(unsigned long long l)
{
    static bytes_t<256> ones;
    static auto size = sizeof(unsigned long long);
    unsigned ret = 0;
    auto bytes = (unsigned char*)&l;
    for(auto i = 0U; i < size; ++i)
        ret += ones[bytes[i]];
    return ret;
}

// gets the highest number that could potentially win the contest
unsigned long long could_win(unsigned long long n, unsigned long long p)
{
	// number of teams
    auto teams = 1ULL << n;
    auto last_winning_score = teams - p;
    // the number of games that have to be won in order to be the last one
    auto games_won = count_set_bits(last_winning_score);
    // find the number with least bits set in range (last_winning_score, teams - 1)
    auto number = teams >> 1; // only the first game has been won
    while (number < teams - 1)
    {
    	// if number if greater that last winning score that's the one we are looking for
        if (number > last_winning_score)
        {
        	// if number has less bits set than games_won it is easier to achieve this result
            if (games_won > count_set_bits(number)) games_won = count_set_bits(number);
            break;
        }
        // get the next number (one bit set more = next game won)
        number = number >> 1 | teams >> 1;
    }
    // calculate the index of the team
    // (there must be 2^(games_won) - 1 teams that can be beaten)
    return teams - (1ULL << games_won);
}

// gets the highest number that will win for sure
unsigned long long will_win(unsigned long long n, unsigned long long p)
{
    auto ret = 0ULL, add = 2ULL;
    // number of teams
    auto teams = 1ULL << n;
    // last team
    auto last = teams - 1;
    auto last_winning_score = teams - p;
    // each round the number of competitor is divided by 2
    while ((teams = teams >> 1) != 0)
    {
    	// determine the first game that has to be won
        if (last_winning_score & teams)
        {
            // if ret is greater than the last
            // team it is the last team
            return ret > last ? last : ret;
        }
        // with each acceptable failure the number
        // grows by the subsequent power of 2
        ret += add;
        add *= 2;
    }
    // if ret is greater than the last
    // team it is the last team
    // the first game will be won for sure by '0', the second at most by '2' (if failed in previous games),
    // the third at most by '6' (if failed in previous games), and so on ...
    return ret > last ? last : ret;
}

std::pair<unsigned long long, unsigned long long> solve(unsigned long long n, unsigned long long p)
{
    return {will_win(n, p), could_win(n, p)};
}

int main()
{
//    std::ifstream in ("/home/simonm/Pobrane/B-small-practice(1).in");
    std::ifstream in ("/home/simonm/Pobrane/B-large-practice(1).in");
    int count;
    in >> count;
    for (int i = 1; i <= count; ++i)
    {
        unsigned long long n, p;
        in >> n >> p;
        auto s = solve(n, p);
        std::cout << "Case #" << i << ": " << s.first << " " << s.second << std::endl;
    }

    return 0;
}
