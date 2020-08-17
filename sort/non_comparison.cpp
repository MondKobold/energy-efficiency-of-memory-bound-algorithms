//
// @author: Thilo Kamradt
//

#include <cmath>
#include <vector>

namespace non_comp {


    const int optimal_bin_size = 32;

    void sort_bucket(int array[], int size) {
        int i;

        // allocate Buckets
        if(size < optimal_bin_size * 2) {
            utility::insertionSort(array, array + size);
            return;
        }
        int total_Buckets = std::ceil(size / optimal_bin_size);
        std::vector<int> *bucket =  new std::vector<int>[total_Buckets];

        // find max and min
        unsigned int offset = *array;
        unsigned int devider = *array;

        for (i = 1; i < size; ++i) {
            if(array[i] > devider) {
                devider = array[i];
            }
            if (array[i] < offset) {
                offset = array[i];
            }
        }
        devider = std::ceil(float (devider - offset + 1) / total_Buckets);

        // fill buckets
        int pos;
        for (i = 0; i < size; i++) {
            pos = std::floor((array[i] - offset) / devider);
            bucket[pos].push_back(array[i]);
        }

        // sort the buckets
        int * start;
        for (i = 0; i < total_Buckets; i++) {
            start = bucket[i].data();
            utility::insertionSort(start, start + bucket[i].size());
        }

        // copy sorted buckets back to array
        int *ptr = array;
        for (i = 0; i < total_Buckets; i++) {
            std::copy(bucket[i].begin(), bucket[i].end(), ptr);
            ptr += bucket[i].size();
        }
    }

    /** Sorts array[0..n)
      * O(n + m). m = max - min
      * */
    static void sort_counting(int *array, unsigned int size) {

        int i;

        //get min/max of array and init counter
        unsigned int offset = *array;
        unsigned int size_counter = *array;

        for (i = 1; i < size; ++i) {
            if(array[i] > size_counter) {
                size_counter = array[i];
            }
            if (array[i] < offset) {
                offset = array[i];
            }
        }
        size_counter = size_counter - offset + 1;
        int counter[size_counter];
        std::fill(counter, counter + size_counter, 0);

        for (i = 0; i < size; ++i) {
            ++counter[array[i] - offset];
        }
        int *ptr = array;
        for (i = 0; i < size_counter; ++i) {
            std::fill(ptr, ptr + counter[i], i + offset);
            ptr += counter[i];
        }
    }

    //-----------------------------------------------
    static unsigned int partition_radix_exchange(int *array, unsigned int left, unsigned int right, int mask) {

        // init running pointers
        int scan_left = left - 1, scan_right = right;
        bool left_lt_right;

        do {
            while ((array[++scan_left] & mask) == 0) { // find left candidate
                if (scan_left == right - 1) { break; }
            }
            while ((array[--scan_right] & mask) == mask) {// find right candidate
                if (scan_right == left) { break; }
            }

            left_lt_right = scan_left < scan_right;
            if(left_lt_right) {
                utility::swap(array, scan_left, scan_right);
            }
        } while (left_lt_right);
        return scan_right + 1;
    }

    static void sort_radix_exchange_aux(int *array, unsigned int left, unsigned int right, unsigned int mask) {
        if((right - left) < 2 || mask < 1) {
            return;
        }
        unsigned int split = partition_radix_exchange(array, left, right, mask);
        mask = mask >> 1;
        sort_radix_exchange_aux(array, left, split, mask);
        sort_radix_exchange_aux(array, split, right, mask);
    }

    /**
     * binary radix exchange sort analog to quicksort
     */
   void sort_radix_exchange(int *array, unsigned int size) {
        if(size < 2) {
            return;
        }

        int max = *array;
        for (int i = 1; i < size; ++i) {
            if(array[i] > max) {
                max = array[i];
            }
        }
        sort_radix_exchange_aux(array, 0, size, 1 << utility::get_position_MSB(max));
    }


    // for performance reasons always watch words
    constexpr int BINS = 16;
    constexpr int LSBS = 4;
    constexpr int MASK = 15;

    void sort_radix(int *array, unsigned int size) {

        // Falls Container leer ist
        if (size < 2) {
            return;
        }

        // init variables
        std::vector<int> partition[BINS];
        int i;
        int msb_pos = *array;
        for (i = 1; i < size; ++i) {
            if(array[i] > msb_pos) {
                msb_pos = array[i];
            }
        }
        msb_pos = utility::get_position_MSB(msb_pos);

        // radix steps
        for (int shift = 0; shift <= msb_pos; shift += LSBS) {

            // split into buckets
            for (i = 0; i < size; ++i) {
                partition[(array[i] >> shift) & MASK].push_back(array[i]);
            }

            // copy back to array
            int *ptr = array;
            for (i = 0; i < BINS; ++i) {
                std::copy(partition[i].begin(), partition[i].end(), ptr);
                ptr += partition[i].size();
                partition[i].clear();
            }
        }
    }

}
