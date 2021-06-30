#include "utils.hpp"

// Gets a tour using greedy algorithm.
// From each city, moves to the nearest unvisited city.
std::vector<int> get_greedy_tour(const int &start_city, const std::vector<std::vector<double>> &distances)
{
    int num_of_cities = distances.size();
    std::vector<bool> has_visited(num_of_cities, false);
    std::vector<int> greedy_tour;

    int current_city = start_city;

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

    // assert(check_tour(greedy_tour, num_of_cities));
    return greedy_tour;
}

// Returns true when two edges are crossed.s
// |index1|, |index2|: indices of starting points of the two edges.
bool are_crossed(const std::vector<int> &tour, const int &index1, const int &index2, const std::vector<std::vector<double>> &distances)
{
    int num_of_cities = distances.size();
    double length_of_edges = distances[tour[index1]][tour[(index1 + 1) % tour.size()]] + distances[tour[index2]][tour[(index2 + 1) % tour.size()]];
    double length_of_rearranged_edges = distances[tour[index1]][tour[index2]] + distances[tour[(index1 + 1) % tour.size()]][tour[(index2 + 1) % tour.size()]];

    return length_of_rearranged_edges < length_of_edges; // When two edges are crossed
}

// Uncrosses crossed two edges.
// |index1|, |index2|: indices of starting points of the two edges.
std::vector<int> &uncross_edges(std::vector<int> &tour, const int &index1, const int &index2)
{
    std::reverse(tour.begin() + index1 + 1, tour.begin() + index2 + 1);
    return tour;
}

// Performs two-opt algorithm.
// When two edges (tour[index1]->tour[index1+1] and tour[index2]->tour[index2+1]) are crossing,
// returns a tour made by uncrossing the two edges.
// Otherwise, returns the given tour.
// NOTE: index1 must be smaller than index2.
std::vector<int> &two_opt(std::vector<int> &tour, const std::vector<std::vector<double>> &distances, const double &time_limit)
{
    int num_of_cities = distances.size();

    std::random_device seed_gen;
    std::mt19937 random_engine(seed_gen());

    // Choose two edges at random, and uncross them if they are crossed.
    std::time_t start = std::time(NULL);
    while ((std::time(NULL) - start) < time_limit)
    {
        // Choose index1 and index2 at random.
        std::pair<int, int> indices = gen_random_indices(num_of_cities, random_engine);
        if (are_crossed(tour, indices.first, indices.second, distances))
        {
            tour = uncross_edges(tour, indices.first, indices.second);
        }
    }

    // assert(check_tour(tour, num_of_cities));
    return tour;
}

// Gets the change of score when inserting |subsequence| between main_tour[edge_start] and main_tour[edge_end].
// |reverses|: whether to reverse the subsequence or not when inserting it to the main_tour.
double get_score_diff(const int &edge_end, const bool &reverses, const std::vector<int> &main_tour, const std::vector<int> &subsequence, const std::vector<std::vector<double>> &distances)
{
    int edge_start = (edge_end + main_tour.size() - 1) % main_tour.size();
    if (reverses)
    {
        return distances[subsequence.back()][main_tour[edge_start]] +
               distances[subsequence.front()][main_tour[edge_end]] -
               distances[main_tour[edge_start]][main_tour[edge_end]];
    }
    else
    {
        return distances[subsequence.front()][main_tour[edge_start]] +
               distances[subsequence.back()][main_tour[edge_end]] -
               distances[main_tour[edge_start]][main_tour[edge_end]];
    }
}

// Cuts out [indices.first, indices.second) from the |tour| and assign it to |subsequence|.
// Rest of the |tour| is assigned to |main_tour|.
void cut_out_subsequence(const std::vector<int> &tour, std::vector<int> &main_tour, std::vector<int> &subsequence, const std::pair<int, int> &indices)
{
    int num_of_cities = tour.size();
    for (int i = 0; i < num_of_cities; ++i)
    {
        if (i == indices.first)
        {
            for (; i < indices.second; ++i)
            {
                subsequence.push_back(tour[i]);
            }
        }
        main_tour.push_back(tour[i]);
    }
    return;
}

// Inserts the subsequence to |main_tour|.
// subsequence is inserted before main_tour[insert_index].
std::vector<int> insert_subsequence(const std::vector<int> &main_tour, const std::vector<int> &subsequence, const int &insert_index, const bool &reverses_subsequence)
{
    int main_tour_size = main_tour.size();
    std::vector<int> new_tour;

    for (int i = 0; i < main_tour_size; ++i)
    {
        if (i == insert_index)
        {
            if (reverses_subsequence)
            {
                for (int j = subsequence.size() - 1; j >= 0; --j)
                {
                    new_tour.push_back(subsequence[j]);
                }
            }
            else
            {
                for (int j = 0; j < subsequence.size(); ++j)
                {
                    new_tour.push_back(subsequence[j]);
                }
            }
        }
        new_tour.push_back(main_tour[i]);
    }

    // assert(check_tour(new_tour, new_tour.size()));
    return new_tour;
}

// Returns the probability to change to the new tour.
// Probability is based on simulated annealing algorithm.
// |score_diff|: (score of the new tour) - (score of the current tour)
double get_transition_probability(const std::time_t &start_time, const double &time_limit, const double &score_diff)
{
    std::time_t current_time = std::time(NULL);
    double start_temp = 1.75;
    double end_temp = 0.05;
    // double temp = start_temp * std::pow(end_temp / start_temp, (double)(current_time - start_time) / time_limit);
    double temp = start_temp + (end_temp - start_temp) * (double)(current_time - start_time) / time_limit;
    return std::exp(-score_diff / temp); // Minus is needed because smaller score is better.
}

// Cuts out subsequences randomly and connects it to another place of rest of the tour.
// Whether to connect subsequence or not is judged using simulated annealing algorithm.
std::vector<int> &move_subsequence(std::vector<int> &tour, const std::vector<std::vector<double>> &distances, const double &time_limit)
{
    int num_of_cities = distances.size();

    std::random_device seed_gen;
    std::mt19937 random_engine(seed_gen());
    std::uniform_real_distribution<double> random_uniform(0, 1);

    // Choose two edges at random, and uncross them if they are crossed.
    std::time_t start = std::time(NULL);
    while ((std::time(NULL) - start) < time_limit)
    {
        // Choose index1 and index2 at random.
        std::pair<int, int> indices = gen_random_indices(num_of_cities, random_engine);
        std::vector<int> main_tour, subsequence;
        cut_out_subsequence(tour, main_tour, subsequence, indices);

        for (int insert_index = 0; insert_index < main_tour.size(); ++insert_index)
        {
            if (insert_index != indices.first)
            {
                for (int j = 0; j <= 1; ++j)
                {
                    bool insert_reverses = (j == 0);

                    // Judge whether to insert the subsequence or not.
                    double score_change = get_score_diff(insert_index, insert_reverses, main_tour, subsequence, distances) - get_score_diff(indices.first, false, main_tour, subsequence, distances);
                    double transition_probability = get_transition_probability(start, time_limit, score_change);

                    if (random_uniform(random_engine) < transition_probability) // This statement is true with probability of min(transition_probability, 1).
                    {
                        tour = insert_subsequence(main_tour, subsequence, insert_index, insert_reverses);
                        // check_tour(tour, num_of_cities);
                        break;
                    }
                }
            }
        }
    }

    // assert(check_tour(tour, num_of_cities));
    return tour;
}

// Calculates the shortest tour to visit all the cities and return to the start.
// If the shortest tour is 0 -> 2 -> 1, returns std::vector{0, 2, 1}.
std::vector<int> get_shortest_tour(const std::vector<std::vector<double>> &distances)
{
    int num_of_cities = distances.size();

    // Try greedy & two-opt algorithm from different start points,
    // and choose the one with the best score.
    int best_start = -1;
    double best_score = -1;

    for (int start = 0; start < num_of_cities; start += 64)
    {
        std::vector<int> tour = get_greedy_tour(start, distances);
        tour = two_opt(tour, distances, 2);
        double score = get_score(tour, distances);
        if (best_start == -1 || score < best_score)
        {
            best_score = score;
            best_start = start;
        }
    }

    // std::cout << "Start: " << best_start << std::endl;
    std::vector<int> shortest_tour = get_greedy_tour(best_start, distances);
    std::cout << "Score(greedy): " << get_score(shortest_tour, distances) << std::endl;
    shortest_tour = two_opt(shortest_tour, distances, 120);
    std::cout << "Score(two-opt): " << get_score(shortest_tour, distances) << std::endl;
    shortest_tour = move_subsequence(shortest_tour, distances, 7200);
    std::cout << "Score(final): " << get_score(shortest_tour, distances) << std::endl;

    assert(check_tour(shortest_tour, num_of_cities));
    return shortest_tour;
}

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        std::cerr << "Designate the input and output files." << std::endl;
        std::exit(1);
    }

    std::vector<City> cities = read_input(argv[1]);
    std::vector<std::vector<double>> distances = get_distances(cities);
    std::vector<int> shortest_tour = get_shortest_tour(distances);
    print_tour(argv[2], shortest_tour);

    std::exit(0);
}
