//
// @author: Thilo Kamradt
//
#include <algorithm>
#include <functional>
#include "../utility.hpp"


namespace optimalSort {

    static void merge_Into(const int *source, int *target, unsigned int left, unsigned int middle, unsigned int right) {

        // init pointer
        unsigned int pointer_target = left;
        unsigned int pointer_left = left;
        unsigned int pointer_right = middle;

        // perform merge
        while (pointer_target < right) {
            // something left for merging
            if (pointer_left == middle) {// take rest from right side
                std::move(source + pointer_right, source + right, target + pointer_target);
                break;
            } else if (pointer_right == right) {
                // take rest from left side
                std::move(source + pointer_left, source + middle, target + pointer_target);
                break;
            } else if (source[pointer_left] <= source[pointer_right]) {
                // smaller on left side
                target[pointer_target++] = std::move(source[pointer_left++]);
            } else {
                // smaller on right side
                target[pointer_target++] = std::move(source[pointer_right++]);
            }
        }
    }

   static void mergesort(int *array, unsigned int size) {
        //check before sorting
        if (std::is_sorted(array, array + size)) {
            return;
        }

        // cutoff for small Arrays
        if(size < utility::CUT_OFF_THRESHOLD) {
            utility::insertionSort(array, array + size);
            return;
        }

        // create auxiliary array and copy values
        int *aux = new int[size];
        std::copy(array, array + size - 1, aux);


        // init helper-fields
        // note that source and target are switched because the while block will flip them at first step
        int *source = aux;              // array to fetch values from
        int *target = array;            // array to write to
        int *tmp;                       // auxiliary field to flip source and target
        unsigned int size_target_block = 2;
        unsigned int size_current_block = 1;
        unsigned int full_merges = size / size_target_block;
        unsigned int rest_merge = size % size_target_block;
        unsigned int start;     // number of merges for a specific level

        while (full_merges > 0) {

            // flip source and target
            tmp = source;
            source = target;
            target = tmp;

            // perform full merge-operations
            for (int i = 0; i < full_merges; ++i) {
                start = i * size_target_block;
                merge_Into(source, target, start, start + size_current_block, start + size_target_block);
            }

            // perform the last merge-operation
            start = full_merges * size_target_block;
            if (rest_merge > size_current_block) {
                merge_Into(source, target, start, start + size_current_block, size);
            } else if (rest_merge > 0) {
                // just copy the rest
                std::copy(source + start, source + size - 1, target + start);
            }


            // update block size and number of merges
            size_current_block = size_target_block;
            size_target_block = size_target_block << 1;
            full_merges = size / size_target_block;
            rest_merge = size % size_target_block;
        }

        //final merge-operation
        merge_Into(target, source, 0, size_current_block, size);

        if (source != array) {
            std::copy(source, source + size - 1, array);
        }

        delete[] aux;
    }



    static int config_MedianOfThree(int *array, int left, int right) {
        int mot[3] = {std::move(array[left]), std::move(array[left + 1]), std::move(array[left + 2])};
        utility::insertionSort(mot, mot + 3);
        array[left] = mot[1];
        array[left + 1] = std::move(mot[0]);
        array[left + 2] = std::move(array[right - 1]);
        array[right - 1] = std::move(mot[2]);
        return mot[1];
    }

    /**
    *  3-Way partitioning with left < pivot-Partition < right.
    *  Implementation after Bentley and McIlroy
    */
    static void partition_3_way_Bentley_McIlroy(int *array, int low, int high) {
        if (high <= (low + 3)) { // 3 elements left
            utility::insertionSort(array + low, array + high);
            return;
        }
        if (std::is_sorted(array + low, array + high)) {
            return;
        }
        // Bentley_McIlroy partitioning
        int i = low, j = high;
        int p = low, q = high;
        int pivot = config_MedianOfThree(array, low, high);
        while (true) {
            // find not fitting elements from left and right at once
            while (array[++i] < pivot) {
                if (i >= high) { break; }
            }
            while (pivot < array[--j]) {
                if (j <= low) { break; }
            }

            // pointers cross
            if (i == j && array[i] == pivot) {
                utility::swap(array, ++p, i);
            }
            if (i >= j) {
                break;
            }

            utility::swap(array, i, j);
            if (array[i] == pivot) { utility::swap(array, ++p, i); }
            if (array[j] == pivot) { utility::swap(array, --q, j); }
        }// [== pivot, low...p] [< pivot, p...i] [> pivot, j...q] [==pivot, q...high]

        // place the pivot parts at the center
        for (int k = low; k <= p; k++) {
            utility::swap(array, k, j--);
        }
        for (int k = high - 1; k > q; k--) {
            utility::swap(array, k, i++);
        }// [low...j) < [j...i] < [i...high].

        // sort rest
        partition_3_way_Bentley_McIlroy(array, low, j + 1);
        partition_3_way_Bentley_McIlroy(array, i, high);
    }

    static void quicksort(int *array, unsigned int size) {
        partition_3_way_Bentley_McIlroy(array, 0, size);
    }
}