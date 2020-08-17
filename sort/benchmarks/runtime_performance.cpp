//
// @author Thilo Kamradt
//

#include <iostream>
#include <chrono>
#include <random>
#include "../../utility.hpp"
#include "sort.h"

long get_runtime(const unsigned int size, int precision) {
    // Generate 'size' random numbers and save them to 'array' using time_since_epoch as seed.
    int *array = new int[size];
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 1000);
    for (int j = 0; j < size; ++j) {
        array[j] = dist(rd);
    }

    // record performance
    auto begin = std::chrono::steady_clock::now();
    sort(array, size);
    auto end = std::chrono::steady_clock::now();

    // delete the array
    delete[] array;

    // show measured time
    if (precision == 0) {
        return  std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count();
    } else if (precision == 1) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    } else {
        return std::chrono::duration_cast<std::chrono::seconds>(end-begin).count();
    }

}

int validate_dataset_size(int size) {
    if (size < 1 || size > utility::DATASETS.size()) {
        size = utility::DATASETS.size();
        std::cout << "Invalid input. Program will run on all datasets." << std::endl;
    }
    return size;
}

/**
 * Casts the userinput to precision represented by a flag
 * @returns 0 for nanoseconds
 *          1 for milliseconds
 *          2 for seconds
 */
int cast_input_2_precision(std::string input) {
    if (input.compare("ns") == 0) {
        return 0;
    } else if (input.compare("s") == 0) {
        return 2;
    } else {
        return 1;
    }
}

void bench_set(int size, int precision) {
    long sum = 0;
    int iterations = size <= utility::DATASETS_SMALL_LIMIT ? 10 : 3;
    for (int j = 0; j < iterations; ++j) {
        sum += get_runtime(utility::DATASETS[size], precision);
    }
    sum = sum/iterations;
    std::cout << utility::DATASETS[size] << " : " << sum << std::endl;
    utility::sleep_for(60);
}

void printSingleRuntime(int dataset, int precision) {
    // validate input
    int size = validate_dataset_size(dataset);

    // start performance test
    bench_set(size, precision);
}

void printRuntimeUpTo(int dataset, int precision) {
    // validate input
    int size = validate_dataset_size(dataset);

    // start performance test
    for (int i = 0; i < size; ++i) {
        bench_set(i, precision);
    }
}

