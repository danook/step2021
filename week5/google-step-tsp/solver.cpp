#include "utils.cpp"

// Depth first search to find the best |depth|-length tour from the |present|.
// Returns a pair of tour and the score of the tour.
// NOTE: The order of elements in returning vector is reversed;
// If visiting order is 0 -> 2 -> 1, the vector is {1, 2, 0}.
std::pair<std::vector<int>, double> dfs(const int &present, const std::vector<std::vector<double>> &distances, const std::vector<bool> &has_visited, const int &depth)
{

    if (depth == 0)
    {
        return make_pair(std::vector<int>{present}, 0.0);
    }

    int num_of_cities = distances.size();
    std::vector<bool> new_has_visited(has_visited);

    new_has_visited[present] = true;
    std::pair<std::vector<int>, double> best_tour_and_score;
    best_tour_and_score.second = 1e9;

    for (int next = 0; next < num_of_cities; ++next)
    {
        if (!new_has_visited[next])
        {
            std::pair<std::vector<int>, double> tour_and_score = dfs(next, distances, new_has_visited, depth - 1);
            tour_and_score.second += distances[present][next];
            if (tour_and_score.second < best_tour_and_score.second)
            {
                best_tour_and_score = tour_and_score;
            }
        }
    }

    // Not renewed; no unvisited cities left.
    if (best_tour_and_score.second == 1e9)
    {
        return make_pair(std::vector<int>{present}, distances[present][0]);
    }

    best_tour_and_score.first.push_back(present);
    return best_tour_and_score;
}

// Gets a tour using greedy algorithm.
// From each city, moves to the nearest unvisited from the city.
std::vector<int> get_greedy_tour(const std::vector<std::vector<double>> &distances)
{
    int num_of_cities = distances.size();
    std::vector<bool> has_visited(num_of_cities, false);
    std::vector<int> greedy_tour;

    // Starts from city 0.
    int present = 0;
    int num_of_unvisited_cities = num_of_cities - 1;
    double score = 0.0;  // To check if the score is calculated correctly in dfs().

    greedy_tour.push_back(present);
    has_visited[present] = true;

    while (true)
    {
        std::pair<std::vector<int>, double> best_tour_and_score = dfs(present, distances, has_visited, 2);
           
        for (int i = best_tour_and_score.first.size() - 2; i >= 0; --i){
            greedy_tour.push_back(best_tour_and_score.first[i]);
            has_visited[best_tour_and_score.first[i]] = true;
        }
        present = best_tour_and_score.first.front();
        
        num_of_unvisited_cities -= (best_tour_and_score.first.size() - 1);
        score += best_tour_and_score.second;

        // Has visited all the cities
        if (num_of_unvisited_cities == 0) break;

    }
    
    // assert(abs(score - get_score(greedy_tour, distances)) < 1e-4);
    // assert(check_tour(greedy_tour, num_of_cities));
    return greedy_tour;
}

// When two edges (tour[index1]->tour[index1+1] and tour[index2]->tour[index2+1]) are crossing,
// returns a tour made by uncrossing the two edges.
// Otherwise, returns the given tour.
// NOTE: index1 must be smaller than index2.
std::vector<int> uncross_tour(const std::vector<int> tour, const int &index1, const int &index2, const std::vector<std::vector<double>> &distances)
{
    double length_of_edges = distances[tour[index1]][tour[(index1 + 1) % tour.size()]] + distances[tour[index2]][tour[(index2 + 1) % tour.size()]];
    double length_of_rearranged_edges = distances[tour[index1]][tour[index2]] + distances[tour[(index1 + 1) % tour.size()]][tour[(index2 + 1) % tour.size()]];
    std::vector<int> new_tour(tour);

    if (length_of_rearranged_edges < length_of_edges) // When two edges are crossed
    {
        std::reverse(new_tour.begin() + index1 + 1, new_tour.begin() + index2 + 1);
    }

    // assert(get_score(new_tour, distances) <= get_score(tour, distances));
    return new_tour;
}

// Calculates the shortest tour to visit all the cities and return to the start.
// |time_in_second|: Time to execute the function.
// If the shortest tour is 0 -> 2 -> 1, returns std::vector{0, 2, 1}.
std::vector<int> get_the_shortest_tour(const std::vector<std::vector<double>> &distances, const double &time_in_second)
{
    int num_of_cities = distances.size();

    std::random_device seed_gen;
    std::mt19937 random_engine(seed_gen());

    // Apply greedy algorithm
    std::vector<int> shortest_tour = get_greedy_tour(distances);
    std::cout << "Score(greedy): " << get_score(shortest_tour, distances) << std::endl;

    // Choose two edges at random, and uncross them if they are crossed.
    std::time_t start = std::time(NULL);
    while ((std::time(NULL) - start) < time_in_second)
    {
        // Choose index1 and index2 at random.
        int index1, index2;
        do
        {
            index1 = random_engine() % num_of_cities;
            index2 = random_engine() % num_of_cities;
        } while (index1 == index2);

        if (index1 > index2) std::swap(index1, index2); // index1 has to be smaller than index2.
        shortest_tour = uncross_tour(shortest_tour, index1, index2, distances);
    }

    // assert(check_tour(shortest_tour, num_of_cities));
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

    // Test for dfs()
    std::pair<std::vector<int>, double> tour_and_score = dfs(0, distances, std::vector<bool>(4, false), 3);
    assert((tour_and_score.second - 3.0) < 1e-4);

    // Test for uncross_tour()
    std::vector<int> uncrossed_tour = uncross_tour({0, 1, 2, 3}, 1, 3, distances);
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
    //test();
    
    std::vector<City> cities = read_input(argv[1]);
    std::vector<std::vector<double>> distances = get_distances(cities);
    std::vector<int> shortest_tour = get_the_shortest_tour(distances, 300);
    print_tour(argv[2], shortest_tour);
    std::cout << "Score: " << get_score(shortest_tour, distances) << std::endl;
    
    std::exit(0);
}