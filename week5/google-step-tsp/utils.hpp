#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <deque>
#include <ctime>
#include <random>

// A struct to maintain the cordinate of a city.
struct City
{
    double x;
    double y;
    City(double x0, double y0)
    {
        x = x0;
        y = y0;
    }
};

std::vector<City> read_input(const std::string&);
void print_tour(const std::string&, const std::vector<int>&);
bool check_tour(std::vector<int>, int);