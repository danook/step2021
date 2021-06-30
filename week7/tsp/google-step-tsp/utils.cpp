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

// Checks if every city is in |tour| exactly once.
bool check_tour(std::vector<int> tour, const int &num_of_cities)
{
    if (tour.size() != num_of_cities)
        return false;
    std::vector<bool> is_in_tour(num_of_cities, false);
    for (int city: tour){
        is_in_tour[city] = true;
    }
    for (bool in_tour: is_in_tour){
        if (!in_tour) return false;
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

// Generates two random integers in [0, num_of_cities).
// The first integer is smaller than the second one.
std::pair<int, int> gen_random_indices(const int &num_of_cities, std::mt19937 &random_engine)
{
    int index1, index2;
    do
    {
        index1 = random_engine() % num_of_cities;
        index2 = random_engine() % num_of_cities;
    } while (index1 == index2);

    if (index1 > index2)
        std::swap(index1, index2); // index1 has to be smaller than index2.

    return std::make_pair(index1, index2);
    
}
