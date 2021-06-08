#include "utils.hpp"

// Reads a list of cities from a CSV file.
// |filename|: path to the input file.
// Returns a vector of coordinate of cities.
std::vector<City> read_input(const std::string &filename)
{
    std::ifstream ifs(filename);
    if (ifs.fail())
    {
        std::cerr << "Error: failed to open input file." << std::endl;
        std::exit(1);
    }

    double x, y;
    char c;
    std::vector<City> cities;
    std::string first_line;
    // assert(first_line == "x,y");

    ifs >> first_line; // Skip the first line.

    // Stream cannot be split into two decimals without c.
    while (ifs >> x >> c >> y)
    {
        assert(c == ',');
        cities.push_back(City(x, y));
    }

    std::cout << "Input the tour from the file." << std::endl;
    return cities;
}

// Outputs |tour| to a CSV file.
// |filename|: path to the output file.
void print_tour(const std::string &filename, const std::vector<int> &tour)
{
    std::ofstream ofs(filename);
    if (ofs.fail())
    {
        std::cerr << "Error: failed to open output file." << std::endl;
        std::exit(1);
    }
    ofs << "index" << std::endl;
    for (int id : tour)
    {
        ofs << id << std::endl;
    }
    std::cout << "Output the tour to the file." << std::endl;
    return;
}

// Check if every city is in |tour| once.
bool check_tour(std::vector<int> tour, int num_of_cities)
{
    if (tour.size() != num_of_cities)
        return false;
    std::sort(tour.begin(), tour.end());
    for (int i = 0; i < num_of_cities; ++i)
    {
        if (tour[i] != i)
            return false;
    }
    return true;
}