//
// @author: Thilo Kamradt
//
#include <algorithm>
#include <functional>
#include <omp.h>
#include "../utility.hpp"


namespace mergesort {
//------------------------ In Place Merging---------------------------------------------------
    static void merge_regular(int *array, int *aux, int aux_size, int left, int middle, int right) {
        int aux_pointer = 0, array_pointer = middle;
        for (int k = left; k < right; k++) {
            if (aux_pointer == aux_size) {
                //everything is in place
                break;
            } else if (array_pointer == right) {
                std::move(aux + aux_pointer, aux + aux_size, array + k);
                break;
            } else if (aux[aux_pointer] <= array[array_pointer]) {
                array[k] = std::move(aux[aux_pointer++]);
            } else {
                array[k] = std::move(array[array_pointer++]);
            }
        }
    }

    static void merge_reverse(int *array, int *aux, int aux_size, int left, int middle, int right) {
        int aux_pointer = aux_size, array_pointer = middle;
        for (int k = right; k < left; k--) {
            if (aux_pointer < 0) {
                //everything is in place
                break;
            } else if (array_pointer < left) {
                std::move(aux, aux + aux_pointer, array);
                break;
            } else if (aux[aux_pointer] >= array[array_pointer]) {
                array[k] = std::move(aux[aux_pointer--]);
            } else {
                array[k] = std::move(array[array_pointer--]);
            }
        }
    }

    static void merge_InPlace(int *array, int left, int middle, int right) {
        int left_size = middle - left;
        int right_size = right - middle;
        int *aux;
        if (left_size <= right_size) {
            aux = new int[left_size];
            std::move(array + left, array + middle, aux);
            merge_regular(array, aux, left_size, left, middle, right);
            delete[] aux;
        } else {
            aux = new int[right_size];
            std::move(array + middle, array + right, aux);
            merge_reverse(array, aux, right_size, left, middle, right);
            delete[] aux;
        }
    }

    static void sort_InPlace(int *origin, unsigned int left, unsigned int right) {

        if (right - left < 2) {
            return;
        }

        // divide
        int middle = (left + right) >> 1;

        // conquer
        sort_InPlace(origin, left, middle);
        sort_InPlace(origin, middle, right);

        // merge
        merge_InPlace(origin, left, middle, right);
    }

//------------------------ Top Down ---------------------------------------------------
    static void sort_TopDown(int *array, int size) {
        // recursion end
        if (size < 2) {
            return;
        }

        // divide array into two
        int middle = size >> 1;
        int size_left = middle;
        int size_right = size - middle;

        // conquer by sort
        sort_TopDown(array, size_left);
        sort_TopDown(array+size_left , size_right);

        int *array_left = new int[size_left];
        int *array_right = new int[size_right];
        std::move(array, array + size_left, array_left);
        std::move(array + middle, array + size, array_right);


        // merge arrays into input
        int pointer_target = 0, pointer_left = 0, pointer_right = 0;
        while (pointer_target < size) {
            if (pointer_left == size_left) {
                // move rest
                std::move(array_right + pointer_right, array_right + size_right, array + pointer_target);
                break;
            } else if (pointer_right == size_right) {
                // move rest
                std::move(array_left + pointer_left, array_left + size_left, array + pointer_target);
                break;
            } else if (array_left[pointer_left] <= array_right[pointer_right]) {
                // left one is smaller
                array[pointer_target++] = std::move(array_left[pointer_left++]);
            } else {
                // right one is smaller
                array[pointer_target++] = std::move(array_right[pointer_right++]);
            }
        }

        delete[] array_left;
        delete[] array_right;
    }

//------------------------ Cut-Off ----------------------------------------------------
    static void sort_CutOff(int *array, int size) {

        // recursion end
        if (size < 2 + utility::CUT_OFF_THRESHOLD) {
            utility::insertionSort(array, array + size);
            return;
        }

        // divide array into two
        int middle = size >> 1;
        int size_left = middle;
        int size_right = size - middle;

        // conquer by sort
        sort_CutOff(array, size_left);
        sort_CutOff(array + size_left, size_right);

        int *array_left = new int[size_left];
        int *array_right = new int[size_right];
        std::move(array, array + size_left, array_left);
        std::move(array + middle, array + size, array_right);

        // merge arrays into input
        int pointer_target = 0, pointer_left = 0, pointer_right = 0;
        while (pointer_target < size) {
            if (pointer_left == size_left) {
                // move rest
                std::move(array_right + pointer_right, array_right + size_right, array + pointer_target);
                break;
            } else if (pointer_right == size_right) {
                // move rest
                std::move(array_left + pointer_left, array_left + size_left, array + pointer_target);
                break;
            } else if (array_left[pointer_left] <= array_right[pointer_right]) {
                // left one is smaller
                array[pointer_target++] = std::move(array_left[pointer_left++]);
            } else {
                // right one is smaller
                array[pointer_target++] = std::move(array_right[pointer_right++]);
            }
        }

        delete[] array_left;
        delete[] array_right;
    }

//------------------------ Pre-Check --------------------------------------------------
    static void sort_PreCheck(int *array, int size) {

        // recursion end
        if (size <= 1 || std::is_sorted(array, array + size)) {
            return;
        }

        // divide array into two
        int middle = size >> 1;
        int size_left = middle;
        int size_right = size - middle;
        int *array_left = new int[size_left];
        int *array_right = new int[size_right];
        std::move(array, array + size_left - 1, array_left);
        std::move(array + middle, array + size - 1, array_right);

        // conquer by sort
        sort_PreCheck(array_left, size_left);
        sort_PreCheck(array_right, size_right);

        // merge arrays into input
        int pointer_target = 0, pointer_left = 0, pointer_right = 0;
        while (pointer_target < size) {
            if (pointer_left == size_left) {
                // move rest
                std::move(array_right + pointer_right, array_right + size_right, array + pointer_target);
                break;
            } else if (pointer_right == size_right) {
                // move rest
                std::move(array_left + pointer_left, array_left + size_left, array + pointer_target);
                break;
            } else if (array_left[pointer_left] <= array_right[pointer_right]) {
                // left one is smaller
                array[pointer_target++] = std::move(array_left[pointer_left++]);
            } else {
                // right one is smaller
                array[pointer_target++] = std::move(array_right[pointer_right++]);
            }
        }

        delete[] array_left;
        delete[] array_right;
    }

//------------------------ Bottom Up --------------------------------------------------
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

    static void sort_BottomUp_iterative(int *array, int size) {
        // create auxiliary array and copy values
        int *aux = new int[size];
        std::copy(array, array + size - 1, aux);

        /*
         * init helper-fields
         * note that source and target are switched because the while block will flip them at first step
         */
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

    static int *merge_level_and_proceed(int *source, int *target, unsigned int size, unsigned int size_block_target,
                                        unsigned int size_block_current) {

        unsigned int full_merges = size / size_block_target;
        unsigned int rest_merge = size % size_block_target;
        unsigned int start;

        // perform full merge-operations
        if (full_merges > 0) {
            for (int i = 0; i < full_merges; ++i) {
                start = i * size_block_target;
                merge_Into(source, target, start, start + size_block_current, start + size_block_target);
            }
        }

        // perform incomplete merges
        start = full_merges * size_block_target;
        if (rest_merge > size_block_current) {
            merge_Into(source, target, start, start + size_block_current, size);
        } else if (rest_merge > 0) {
            // just copy the rest
            std::copy(source + start, source + size - 1, target + start);
        }

        if (full_merges == 0) {
            return target;
        } else {
            return merge_level_and_proceed(target, source, size, size_block_target << 1, size_block_target);
        }
    }

    static void sort_BottomUp_recursive(int *array, unsigned int size) {
        // create auxiliary array and copy values
        int *aux = new int[size];
        std::copy(array, array + size - 1, aux);

        int *sorted = merge_level_and_proceed(array, aux, size, 2, 1);

        if (sorted != array) {
            std::copy(sorted, sorted + size - 1, array);
        }

        delete[] aux;
    }

//------------------------ Single-Work Array ------------------------------------------
    static void auxiliary_WorkArray(int *origin, int *helper, unsigned int left, unsigned int right) {

        if (right - left < 2) {
            return;
        }

        // sort helper, using origin as scratch
        int middle = (left + right) >> 1;
        auxiliary_WorkArray(helper, origin, left, middle);
        auxiliary_WorkArray(helper, origin, middle, right);

        // merge back into origin
        unsigned int pointer_target = left, pointer_left = left, pointer_right = middle;
        while (pointer_target <= right) {
            if (pointer_left == middle) {
                // left one is merged
                std::move(helper + pointer_right, helper + right, origin + pointer_target);
                break;
            } else if (pointer_right == right) {
                // right one is merged
                std::move(helper + pointer_left, helper + middle, origin + pointer_target);
                break;
            } else if (helper[pointer_left] <= helper[pointer_right]) {
                // take left one
                origin[pointer_target++] = std::move(helper[pointer_left++]);
            } else {
                // take right one
                origin[pointer_target++] = std::move(helper[pointer_right++]);
            }
        }
    }

    static void sort_WorkArray(int *array, unsigned int size) {
        // create copy of A
        int *aux_array = new int[size];
        std::copy(array, array + size - 1, aux_array);

        // sort array with auxiliary Array
        auxiliary_WorkArray(array, aux_array, 0, size);

        delete[] aux_array;
    }

//-----------------Merge IN-Place Single Allocation-----------------
    static void merge_InPlace(int *array, int left, int middle, int right, int * aux) {
        int left_size = middle - left;
        int right_size = right - middle;
        if (left_size <= right_size) {
            std::move(array + left, array + middle, aux);
            merge_regular(array, aux, left_size, left, middle, right);
        } else {
            std::move(array + middle, array + right, aux);
            merge_reverse(array, aux, right_size, left, middle, right);
        }
    }

    static void aux_InPlace_single_init(int *origin, unsigned int left, unsigned int right, int *aux) {

        if (right - left < 2) {
            return;
        }

        // divide
        int middle = (left + right) >> 1;

        // conquer
        aux_InPlace_single_init(origin, left, middle, aux);
        aux_InPlace_single_init(origin, middle, right, aux);

        // merge
        merge_InPlace(origin, left, middle, right, aux);
    }

    static void sort_InPlace_single_init(int *array, unsigned int size) {
        int *aux = new int[size >> 1];
        aux_InPlace_single_init(array, 0, size, aux);
    }

//------------------------ Parallel ---------------------------------------------------
    static void sort_2_parallel(int *array, unsigned int size) {
        int iam, start, split = size >> 1;

        //use specific number of threads
        omp_set_dynamic(0);

#pragma omp parallel default(none) shared(array, size, split) private(iam, start)  num_threads(2)
        {
            iam = omp_get_thread_num();
            start = iam * split;
            if (iam == 0) {
                sort_TopDown(array, split);
            } else {
                sort_TopDown(array + split, size - split);
            }

        }
        merge_InPlace(array, 0, split, size);
    }

    static void sort_20_parallel(int *array, unsigned int size) {
        int iam, start, partition_size, max_threads = 20;

        // use max number of threads
        omp_set_dynamic(0);
        omp_set_num_threads(max_threads);
        partition_size = size / max_threads;
        std::cout << "Threads: " << max_threads << std::endl;
        max_threads = max_threads >> 1;

        //sort in parallel
#pragma omp parallel default(none)  private(iam, start) shared(array, size, partition_size, max_threads) num_threads(20)
        {
            iam = omp_get_thread_num();
            start = iam * partition_size;

            if (iam == (max_threads - 1)) {
                sort_TopDown(array + start, (size - start));
            } else {
                sort_TopDown(array + start, partition_size);
            }

        } // end parallel region

        //merge partitions per level (bottom up)
        while (partition_size < size) {
            unsigned int step = partition_size + partition_size;
            unsigned int start = 0, mid = partition_size, end = step;
            while (mid < size) {
                merge_InPlace(array, start, mid, end);
                start += step;
                mid += step;
                end = std::min(end + step, size);
            }
            partition_size += partition_size;
        }
    }

    //------------------------ TRUE IN-PLACE ---------------------------------------------------
   /* static void sort_trueInPlace(int *array, int size) {
        // recursion end
        if (size < 2) {
            return;
        }

        // divide array into two
        int middle = size >> 1;
        int size_left = middle;
        int size_right = size - middle;

        // conquer by sort
        sort_TopDown(array, size_left);
        sort_TopDown(array+size_left , size_right);

        // merge arrays into input
        int current = 0, left = 0, right = size_left;
        while (current < size) {
            if(array[left] <= array[right]) {
                if(left != current) {
                    utility::swap(array, current, left);
                }
                left++;
            } else {
                utility::swap(array, current, right);
                right++;
            }
            current++;
            //TODO wenn der linke pointer bereits im hilfsbereich ist und dann links gewinnt, dass wird current auf den falschen punkt geschoben :(
            if(left == right) {
               if(left == current) {
                    break;
               } else {
                   left = current;
               }
            }
            if(left < current) {
                left = right - 1;
            }
        }
    }*/
}