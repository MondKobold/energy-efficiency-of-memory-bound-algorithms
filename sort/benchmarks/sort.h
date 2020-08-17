//
// @author Thilo Kamradt
//

#ifndef BENCHMARK_SORT_H
#define BENCHMARK_SORT_H

#include <functional>
#include <random>

#if defined(MS_TD) || defined(MS_CO) || defined(MS_PC) || defined(MS_IP) || defined(MS_IP_SI) || defined(MS_BO_I) || defined(MS_BO_R) || defined(MS_WA) || defined(MS_P2) || defined(MS_P20)
#include "../mergesort.cpp"
#endif

#if defined(QS) || defined(QS_CO) || defined(QS_COI) || defined(QS_SHUFFLE) || defined(QS_3_D) || defined(QS_3_BMI) || defined(QS_TCE) || defined(QS_RP) || defined(QS_M3) || defined(QS_PC)
#include "../quicksort.cpp"
#endif

#if defined(MS_OPT) || defined(QS_OPT)
#include "../optimalSort.cpp"
#endif

#ifdef TIM
#include "../timsort.cpp"
#endif

#if defined(BS) || defined(RADIX) || defined(RADIX_X) || defined(CS)
#include "../non_comparison.cpp"
#endif


static void sort(int *array, unsigned int size) {
#ifdef MS_TD
    mergesort::sort_TopDown(array, size);
#endif
#ifdef MS_CO
    mergesort::sort_CutOff(array, size);
#endif
#ifdef MS_PC
    mergesort::sort_PreCheck(array, size);
#endif
#ifdef MS_IP
    mergesort::sort_InPlace(array, 0, size);
#endif
#ifdef MS_IP_SI
    mergesort::sort_InPlace_single_init(array, size);
#endif
#ifdef MS_BO_I
    mergesort::sort_BottomUp_iterative(array, size);
#endif
#ifdef MS_BO_R
    mergesort::sort_BottomUp_recursive(array, size);
#endif
#ifdef MS_WA
    mergesort::sort_WorkArray(array, size);
#endif
#ifdef MS_P2
    mergesort::sort_2_parallel(array, size);
#endif
#ifdef MS_P20
    mergesort::sort_20_parallel(array, size);
#endif

#ifdef QS
    quicksort::sort_BaseImpl(array, size);
#endif
#ifdef QS_CO
    quicksort::sort_CutOff(array, size);
#endif
#ifdef QS_COI
    quicksort::sort_CutOffAndIgnore(array, size);
#endif
#ifdef QS_SHUFFLE
    quicksort::sort_Shuffle(array, size);
#endif
#ifdef QS_3_D
    quicksort::sort_3_way_dijkstra(array, size);
#endif
#ifdef QS_3_BMI
    quicksort::sort_3_way_Bentley_McIlroy(array, size);
#endif
#ifdef QS_TCE
    quicksort::sort_TailCallElimination(array, size);
#endif
#ifdef QS_RP
    quicksort::sort_random_pivot(array, size);
#endif
#ifdef QS_M3
    quicksort::sort_MedianOfThree(array, size);
#endif
#ifdef QS_PC
    quicksort::sort_Precheck(array, size);
#endif


#ifdef TIM
    timsort::sort(array, size);
#endif
#ifdef MS_OPT
    optimalSort::mergesort(array, size);
#endif
#ifdef QS_OPT
    optimalSort::quicksort(array, size);
#endif

#ifdef BS
    non_comp::sort_bucket(array, size);
#endif
#ifdef RADIX
    non_comp::sort_radix(array, size);
#endif
#ifdef RADIX_X
    non_comp::sort_radix_exchange(array, size);
#endif
#ifdef CS
    non_comp::sort_counting(array, size);
#endif
}

static void sort_x_elements(int size) {
    std::cout << "start benchmark: sort" << std::endl;

    // generate array
    int *array = new int[size];
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 1000);
    for (int j = 0; j < size; ++j) {
        array[j] = dist(rd);
    }

    // sort & measure time
    sort(array, size);

    std::cout << "Success" << std::endl;

    delete[] array;
}

static void generate_x_elements(int size) {
    std::cout << "start benchmark: generate" << std::endl;

    // generate array
    int *array = new int[size];
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0, 1000);
    for (int j = 0; j < size; ++j) {
        array[j] = dist(rd);
    }

    std::cout << "Success" << std::endl;

    delete[] array;
}

#endif //BENCHMARK_SORT_H
