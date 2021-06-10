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
    return;
}

// Check if every city is in |tour| once.
bool check_tour(std::vector<int> tour, const int &num_of_cities)
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

// Returns a two-dimensional vector of distances between two cities.
// |cities|: a vector of coordinate of cities.
std::vector<std::vector<double>> get_distances(const std::vector<City> &cities)
{
    int num_of_cities = cities.size();
    std::vector<std::vector<double>> distances =
        std::vector<std::vector<double>>(num_of_cities, std::vector<double>(num_of_cities, 0.0));
    for (int i = 0; i < num_of_cities; ++i)
    {
        for (int j = 0; j < num_of_cities; ++j)
        {
            distances[i][j] = std::sqrt(
                (cities[i].x - cities[j].x) * (cities[i].x - cities[j].x) + (cities[i].y - cities[j].y) * (cities[i].y - cities[j].y));
        }
    }
    return distances;
}

// Returns the score(total distance) of |tour|.
double get_score(const std::vector<int> &tour, const std::vector<std::vector<double>> &distances)
{
    double score = 0.0;
    for (int i = 0; i < tour.size(); ++i)
    {
        score += distances[tour[i]][tour[(i + 1) % tour.size()]];
    }
    return score;
}
