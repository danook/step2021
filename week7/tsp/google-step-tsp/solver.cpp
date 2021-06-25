#include "utils.hpp"

// Gets a tour using greedy algorithm.
// From each city, moves to the nearest unvisited from the city.
std::vector<int> get_greedy_tour(const std::vector<std::vector<double>> &distances)
{
    int num_of_cities = distances.size();
    std::vector<bool> has_visited(num_of_cities, false);
    std::vector<int> greedy_tour;

    // Starts from city 0.
    int current_city = 0;

    greedy_tour.push_back(current_city);
    has_visited[current_city] = true;

    while (true)
    {
        // Finds the nearest unvisited city.
        int nearest_city = -1;
        for (int next_city = 0; next_city < num_of_cities; ++next_city)
        {
            if (!has_visited[next_city] && (nearest_city == -1 || distances[current_city][next_city] < distances[current_city][nearest_city]))
            {
                nearest_city = next_city;
            }
        }
        if (nearest_city == -1)
        { // When visited all city
            break;
        }

        current_city = nearest_city;
        has_visited[nearest_city] = true;
        greedy_tour.push_back(nearest_city);
    }

    assert(check_tour(greedy_tour, num_of_cities));
    return greedy_tour;
}

// When two edges (tour[index1]->tour[index1+1] and tour[index2]->tour[index2+1]) are crossing,
// returns a tour made by uncrossing the two edges.
// Otherwise, returns the given tour.
// NOTE: index1 must be smaller than index2.
std::vector<int> uncross_edges(const std::vector<int> &tour, const int &index1, const int &index2, const std::vector<std::vector<double>> &distances)
{
    int num_of_cities = distances.size();
    double length_of_edges = distances[tour[index1]][tour[(index1 + 1) % tour.size()]] + distances[tour[index2]][tour[(index2 + 1) % tour.size()]];
    double length_of_rearranged_edges = distances[tour[index1]][tour[index2]] + distances[tour[(index1 + 1) % tour.size()]][tour[(index2 + 1) % tour.size()]];
    std::vector<int> new_tour(tour);

    if (length_of_rearranged_edges < length_of_edges) // When two edges are crossed
    {
        std::reverse(new_tour.begin() + index1 + 1, new_tour.begin() + index2 + 1);
    }

    assert(get_score(new_tour, distances) <= get_score(tour, distances));
    assert(check_tour(new_tour, num_of_cities));
    return new_tour;
}

std::vector<int> two_opt(const std::vector<int> &tour, const std::vector<std::vector<double>> &distances, const double &time_in_second)
{
    int num_of_cities = distances.size();

    std::random_device seed_gen;
    std::mt19937 random_engine(seed_gen());
    std::vector<int> new_tour(tour);

    // Choose two edges at random, and uncross them if they are crossed.
    std::time_t start = std::time(NULL);
    while ((std::time(NULL) - start) < time_in_second)
    {
        // Choose index1 and index2 at random.
        std::pair<int, int> indices = gen_random_indices(num_of_cities, random_engine);
        new_tour = uncross_edges(new_tour, indices.first, indices.second, distances);
    }

    assert(check_tour(new_tour, num_of_cities));
    return new_tour;
}

// NOTE: index1 must be smaller than index2.
std::vector<int> insert_subsequence(const std::vector<int> &main_tour, const std::vector<int> &sub_sequence, const std::vector<std::vector<double>> &distances)
{
    int num_of_cities = distances.size();
    int main_tour_size = main_tour.size();
    std::vector<int> new_tour;
    int min_edge_start = -1;
    int min_edge_end = -1;
    double min_distance = -1;
    bool reverses_subsequence = false;

    // Finds a edge s.t. distance[tour[index1]][edge.from] + distance[tour[index2]][edge.to] is the shortest.
    for (int edge_start = 0; edge_start < main_tour_size; ++edge_start)
    {
        int edge_end = (edge_start + 1) % main_tour_size;
        if (min_edge_start == -1 ||
            (distances[sub_sequence.back()][main_tour[edge_start]] + distances[sub_sequence.front()][main_tour[edge_end]] - distances[main_tour[edge_start]][main_tour[edge_end]] < min_distance))
        {
            min_edge_start = edge_start;
            min_edge_end = edge_end;
            min_distance = distances[sub_sequence.back()][main_tour[min_edge_start]] + distances[sub_sequence.front()][main_tour[min_edge_end]] - distances[main_tour[min_edge_start]][main_tour[min_edge_end]];
            reverses_subsequence = true;
        }

        if (min_edge_start == -1 ||
            (distances[sub_sequence.front()][main_tour[edge_start]] + distances[sub_sequence.back()][main_tour[edge_end]] - distances[main_tour[edge_start]][main_tour[edge_end]] < min_distance))
        {
            min_edge_start = edge_start;
            min_edge_end = edge_end;
            min_distance = distances[sub_sequence.front()][main_tour[min_edge_start]] + distances[sub_sequence.back()][main_tour[min_edge_end]] - distances[main_tour[min_edge_start]][main_tour[min_edge_end]];
            reverses_subsequence = false;
        }
    }

    // Inserts the subsequence to the main tour.

    for (int i = 0; i < main_tour_size; ++i)
    {
        if (i == min_edge_end)
        {
            if (reverses_subsequence)
            {
                for (int j = sub_sequence.size() - 1; j >= 0; --j)
                {
                    new_tour.push_back(sub_sequence[j]);
                }
            }
            else
            {
                for (int j = 0; j < sub_sequence.size(); ++j)
                {
                    new_tour.push_back(sub_sequence[j]);
                }
            }
        }
        new_tour.push_back(main_tour[i]);
    }

    assert(check_tour(new_tour, num_of_cities));
    return new_tour;
}

std::vector<int> move_subsequence(const std::vector<int> &tour, const std::vector<std::vector<double>> &distances, const double &time_in_second)
{
    int num_of_cities = distances.size();

    std::random_device seed_gen;
    std::mt19937 random_engine(seed_gen());
    std::vector<int> new_tour(tour);

    // Choose two edges at random, and uncross them if they are crossed.
    std::time_t start = std::time(NULL);
    while ((std::time(NULL) - start) < time_in_second)
    {
        // Choose index1 and index2 at random.
        std::pair<int, int> indices = gen_random_indices(num_of_cities, random_engine);
        std::vector<int> main_tour, subsequence;
        for (int i = 0; i < num_of_cities; ++i)
        {
            if (i == indices.first)
            {
                for (; i < indices.second; ++i)
                {
                    subsequence.push_back(new_tour[i]);
                }
            }
            main_tour.push_back(new_tour[i]);
        }

        new_tour = insert_subsequence(main_tour, subsequence, distances);
    }

    assert(check_tour(new_tour, num_of_cities));
    return new_tour;
}

// Calculates the shortest tour to visit all the cities and return to the start.
// |time_in_second|: Time to execute the function.
// If the shortest tour is 0 -> 2 -> 1, returns std::vector{0, 2, 1}.
std::vector<int> get_the_shortest_tour(const std::vector<std::vector<double>> &distances)
{
    int num_of_cities = distances.size();

    // Apply greedy algorithm
    std::vector<int> shortest_tour = get_greedy_tour(distances);
    std::cout << "Score(greedy): " << get_score(shortest_tour, distances) << std::endl;
    shortest_tour = two_opt(shortest_tour, distances, 300);
    std::cout << "Score(two-opt): " << get_score(shortest_tour, distances) << std::endl;
    shortest_tour = move_subsequence(shortest_tour, distances, 300);
    std::cout << "Score(final): " << get_score(shortest_tour, distances) << std::endl;

    assert(check_tour(shortest_tour, num_of_cities));
    return shortest_tour;
}

void test()
{
    // Input
    std::vector<City> cities = read_input("input_0.csv");
    assert(cities.size() == 5);
    assert(cities[1].x == 1222.0393903625825);
    assert(cities[1].y == 229.56212316547953);

    // Calculating distances
    std::vector<std::vector<double>> distances = get_distances({City(0, 0), City(0, 1), City(1, 0), City(1, 1)});
    assert(abs(distances[0][3] * distances[0][3] - 2.0) < 1e-4);
    assert(distances[1][1] == 0);

    // Test for uncross_edges()
    std::vector<int> uncrossed_tour = uncross_edges({0, 1, 2, 3}, 1, 3, distances);
    std::vector<int> answer = std::vector<int>{0, 1, 3, 2};
    assert(uncrossed_tour == answer);
}

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        std::cerr << "Designate the input and output files." << std::endl;
        std::exit(1);
    }
    // test();

    std::vector<City> cities = read_input(argv[1]);
    std::vector<std::vector<double>> distances = get_distances(cities);
    std::vector<int> shortest_tour = get_the_shortest_tour(distances);
    print_tour(argv[2], shortest_tour);

    std::exit(0);
}