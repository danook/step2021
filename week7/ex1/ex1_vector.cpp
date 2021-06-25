#include <iostream>
#include <ctime>
#include <vector>
#include <cassert>

// Returns the time needed to calculate a product of two matrices.
// |size|: size of matrices.
double get_computation_time(int size){
    std::vector<std::vector<int>> a(size, std::vector<int>(size, 1));
    std::vector<std::vector<int>> b(size, std::vector<int>(size, 1));
    std::vector<std::vector<int>> c(size, std::vector<int>(size, 0));

    std::clock_t start = std::clock();
    for (int j = 0; j < size; ++j){
        for (int k = 0; k < size; ++k){
            for (int i = 0; i < size; ++i){
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }
    std::clock_t end = std::clock();

    // Checks product
    for (int i = 0; i < size; ++i){
        for (int j = 0; j < size; ++j){
            assert(c[i][j] == size);
        }
    }

    return (double)(end - start) / (double)CLOCKS_PER_SEC;
}

int main(){
    const int TRIALS = 5;

    double sum = 0.0;
    for (int i = 0; i < TRIALS; ++i){
        double time = get_computation_time(800);
        std::cout << "Trial " << i + 1 << ": " << time << " sec." << std::endl;
        sum += time;
    }

    double average_time = sum / (double)TRIALS;
    std::cout << "Average time: " << average_time << " sec." << std::endl;
    return 0;
}