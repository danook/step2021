#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>
#include <algorithm>
#include <ctime>
#include <random>
#include <utility>

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

std::vector<City> read_input(const std::string &);
void print_tour(const std::string &, const std::vector<int> &);
bool check_tour(std::vector<int>, const int &);
std::vector<std::vector<double>> get_distances(const std::vector<City> &);
double get_score(const std::vector<int> &tour, const std::vector<std::vector<double>> &);