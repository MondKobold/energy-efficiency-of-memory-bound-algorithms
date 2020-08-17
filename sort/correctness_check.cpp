//
// @author: Thilo Kamradt
//
#include <iostream>
#include <functional>
#include <chrono>
#include <random>
#include "../utility.hpp"
#include "mergesort.cpp"
//#include "timsort.cpp"
//#include "non_comparison.cpp"
//#include "quicksort.cpp"
#define QS_M3
#include "benchmarks/sort.h"

int main() {

    int size = 20;//utility::GB1;//utility::KB32;

    // generate array
    int *array = new int [size];
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 1000);
    for (int j = 0; j < size; ++j) {
        array[j] = dist(rd);
    }

    // sort & measure time
    auto start = std::chrono::steady_clock::now();

    //std::sort(array, array + size);
    //std::stable_sort(array, array + size);

    //utility::insertionSort(array, array + size);

    //mergesort::sort_TopDown(array, size);
    //mergesort::sort_CutOff(array, size);
    //mergesort::sort_PreCheck(array, size);
    //mergesort::sort_BottomUp_iterative(array, size);
    //mergesort::sort_BottomUp_recursive(array, size);
    //mergesort::sort_WorkArray(array, size);
    //mergesort::sort_InPlace(array, 0, size);
    //mergesort::sort_2_parallel(array, size);
    //mergesort::sort_n_parallel(array, size);

    //quicksort::sort_BaseImpl(array, size);
    //quicksort::sort_CutOff(array, size);
    //quicksort::sort_CutOffAndIgnore(array, size);
    //quicksort::sort_MedianOfThree(array, size);
    //quicksort::sort_Shuffle(array, size);
    //quicksort::sort_random_pivot(array, size);
    //quicksort::sort_TailCallElimination(array, size);
    //quicksort::sort_3_way_dijkstra(array, size);
    //quicksort::sort_3_way_Bentley_McIlroy(array, size);

    //timsort::sort(array, size);
    //timsort::sort_2_parallel(array, size);
    //timsort::sort_n_parallel(array, size);

    //non_comp::sort_counting(array, size);
    //non_comp::sort_bucket(array, size);
    //non_comp::sort_radix(array, size);
    //non_comp::sort_radix_exchange(array, size);
    //mergesort::sort_trueInPlace(array, size);
    //sort(array, size);

    auto end = std::chrono::steady_clock::now();

    auto delta = end - start;

    //check for correctness
    std::cout << "Sorting is: ";
    if(std::is_sorted(array, array + size)) {
        std::cout << "correct";
        std::cout << std::endl;
        utility::print_array(array, size);
    } else {
        std::cout << "invalid! ";
        utility::print_array(array, size);
    }
    std::cout << std::endl;


    // print results
    auto duration_ns =  std::chrono::duration_cast<std::chrono::nanoseconds>(delta).count();
    auto duration_ms =  std::chrono::duration_cast<std::chrono::milliseconds>(delta).count();
    auto duration_s =  std::chrono::duration_cast<std::chrono::seconds>(delta).count();


    std::cout << std::endl << "Time needed: " << duration_ns << " ns/ " << duration_ms << " ms/ " << duration_s << " s" << std::endl;

    delete[] array;

    return 0;
}