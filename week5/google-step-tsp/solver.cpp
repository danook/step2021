#include "utils.cpp"

// Returns a two-dimentional vector of distances between two cities.
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

// Gets a tour using greedy algorithm.
std::vector<int> get_greedy_tour(const std::vector<std::vector<double>> &distances)
{
    int num_of_cities = distances.size();
    std::vector<bool> has_visited(num_of_cities, false);
    std::vector<int> greedy_tour;

    int city = 0;
    while (true)
    {
        has_visited[city] = true;
        greedy_tour.push_back(city);

        // Find the nearest unvisited city from |city|.
        int nearest = 0;
        double nearest_distance = 1e9;

        for (int next = 0; next < num_of_cities; ++next)
        {
            if ((!has_visited[next]) && distances[city][next] < nearest_distance)
            {
                nearest = next;
                nearest_distance = distances[city][next];
            }
        }

        if (nearest_distance == 1e9)
        { // When visited all cities (nearest_distance is not renewed)
            break;
        }

        city = nearest;
    }

    return greedy_tour;
}

// Cuts |tour| into fragments.
// Example: tour = {2, 1, 3, 5, 4}, position_to_cut = {0, 3}
// -> returns {{2}, {1, 3, 5}, {4}}
std::vector<std::vector<int>> cut_tour(const std::vector<int> &tour, std::vector<int> positions_to_cut)
{
    std::vector<std::vector<int>> tour_fragments(positions_to_cut.size(), std::vector<int>{});
    std::sort(positions_to_cut.begin(), positions_to_cut.end());
    for (int i = 0; i < positions_to_cut.size(); ++i)
    {
        for (int idx = positions_to_cut[i]; idx != positions_to_cut[(i + 1) % positions_to_cut.size()]; idx = (idx + 1) % tour.size())
        {
            tour_fragments[i].push_back(tour[idx]);
        }
    }
    return tour_fragments;
}

// DFS to find the shortest tour that can be made by arrranging fragments.
// Returns a pair of the order to arrange fragments and the length of the tour.
std::pair<std::deque<int>, double> fragments_dfs(int current, const std::vector<std::vector<int>> &tour_fragments, std::vector<bool> has_visited, const std::vector<std::vector<double>> &distances)
{

    double min_length = 1e9;
    std::deque<int> shortest_tour;
    has_visited[current] = true;

    for (int next = 0; next < tour_fragments.size(); ++next)
    {
        if (!has_visited[next])
        {

            std::pair<std::deque<int>, double> tour = fragments_dfs(next, tour_fragments, has_visited, distances);
            tour.second += distances[tour_fragments[current].back()][tour_fragments[next].front()];

            if (tour.second < min_length)
            {
                shortest_tour = tour.first;
                min_length = tour.second;
            }
        }
    }

    if (min_length == 1e9)
    { // Not renewed; End of the dfs.
        min_length = distances[tour_fragments[current].back()][tour_fragments[0].front()];
    }

    shortest_tour.push_front(current);
    return std::make_pair(shortest_tour, min_length);
}

// Find a way to arrange fragments so that the tour will become shortest.
std::vector<int> arrange_fragments(std::vector<std::vector<int>> tour_fragments, const std::vector<std::vector<double>> &distances)
{
    std::vector<bool> has_visited = std::vector<bool>(tour_fragments.size());
    std::pair<std::deque<int>, double> tour = fragments_dfs(0, tour_fragments, has_visited, distances);
    std::vector<int> answer;
    while (!tour.first.empty())
    {
        int idx = tour.first.back();
        tour.first.pop_back();
        answer.push_back(idx);
    }
    assert(check_tour(answer, tour_fragments.size()));
    return answer;
}

// Calculates the shortest tour to visit all the cities and return to the start.
// |cities|: a vector of coordinate of cities
// If the shortest tour is 0 -> 2 -> 1, returns std::vector{0, 2, 1}.
std::vector<int> get_the_shortest_tour(const std::vector<std::vector<double>> &distances, double time_in_second = 10)
{
    int num_of_cities = distances.size();

    // Apply greedy algorithm
    std::vector<int> shortest_tour = get_greedy_tour(distances);
    std::clock_t start = std::clock();

    while ((std::clock() - start) / CLOCKS_PER_SEC <= time_in_second)
    {

        std::vector<int> city_indices(num_of_cities);
        std::iota(city_indices.begin(), city_indices.end(), 0);
        /*
        std::sort(city_indices.begin(), city_indices.end(),
                  [&shortest_tour, &distances, &num_of_cities](int a, int b) {
                      return distances[shortest_tour[a]][shortest_tour[(a + 1) % num_of_cities]] > distances[shortest_tour[b]][shortest_tour[(b + 1) % num_of_cities]];
                  });
        */
        std::random_device seed_gen;
        std::mt19937 random_engine(seed_gen());
        std::shuffle(city_indices.begin(), city_indices.end(), random_engine);

        int num_of_fragments = std::min(10, num_of_cities);
        std::vector<int> positions_to_cut = std::vector<int>(num_of_fragments);
        for (int position = 0; position < num_of_fragments; ++position)
        {
            positions_to_cut[position] = (city_indices[position] + 1) % num_of_cities;
        }

        std::vector<std::vector<int>> tour_fragments = cut_tour(shortest_tour, positions_to_cut);
        std::vector<int> fragments_order = arrange_fragments(tour_fragments, distances);

        std::vector<int> new_tour;
        for (int fragment : fragments_order)
        {
            for (int city : tour_fragments[fragment])
            {
                new_tour.push_back(city);
            }
        }

        shortest_tour.swap(new_tour);
        assert(check_tour(shortest_tour, num_of_cities));
    }

    return shortest_tour;
}

void test()
{
    std::vector<City> cities = read_input("input_0.csv");
    assert(cities.size() == 5);
    assert(cities[1].x == 1222.0393903625825);
    assert(cities[1].y == 229.56212316547953);

    std::vector<std::vector<double>> distances = get_distances(cities);
    assert(distances[0][1] == distances[1][0]);
    assert(distances[1][1] == 0);
    /*
    std::vector<int> greedy_tour = get_greedy_tour(distances);
    assert(check_tour(greedy_tour, 5));

    std::vector<int> test_tour = {2, 3, 1, 5, 4, 6};
    std::vector<std::vector<int>> test_cut_tour = {{2, 3, 1}, {5, 4, 6}};
    assert(cut_tour(test_tour, std::vector{0, 3}) == test_cut_tour);
    */

    std::vector<int> shortest_tour = get_the_shortest_tour(distances);
    assert(check_tour(shortest_tour, 5));

    print_tour("output_0.csv", shortest_tour);
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
    std::vector<int> shortest_tour = get_the_shortest_tour(distances, 10.0);
    std::vector<int> greedy_tour = get_greedy_tour(distances);
    print_tour(argv[2], shortest_tour);

    std::cout << "Score: " << get_score(shortest_tour, distances) << std::endl;
    std::cout << "Score (greedy): " << get_score(greedy_tour, distances) << std::endl;

    system("pause");
    std::exit(0);
}