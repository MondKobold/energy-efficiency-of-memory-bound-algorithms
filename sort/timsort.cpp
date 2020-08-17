//
// @author: Thilo Kamradt
//
// Reimplementation of Pythons Timsort (https://github.com/python/cpython/blob/master/Objects/listobject.c)
#include "../utility.hpp"
#include <omp.h>

namespace timsort {

    //is enough for array of sizes up to 32 * phi ** MAX_MERGE_PENDING
    const unsigned int MAX_MERGE_PENDING = 85;

    // threshold after which galloping is effective
    const short MIN_GALLOP = 7;

    struct run {
        int *pointer;
        unsigned int len;
    };

    typedef struct state {
        // indicates when to start gallop mode
        int min_gallop = MIN_GALLOP;

        // A stack of pending runs yet to be merged.
        int top = 0;
        struct run stack[MAX_MERGE_PENDING];
    } MergeState;

/**
 * Using expotential search to find the first element which is greater than the given key.
 * From the POV of start.
 * @param key comparison value for which the next smaller value should be found
 * @param array pointer to the fist element of Array start
 * @param size total length of the array
 * @param start startpoint of search
 * @return k in 0..size such that array[k-1] <= key < array[k]
 */
    static unsigned int gallop_small2big(int key, int *array, unsigned int size, unsigned int start) {
        int offset = 1;
        int last_offset = 0;

        array += start;
        if (key < *array) {
            return start;
        }
        const unsigned int max_offset = size - start;
        while (offset < max_offset) {
            if (key < array[offset]) { break; } // found it
            last_offset = offset;
            offset = (offset << 1) + 1;
            if (offset <= 0) { offset = max_offset; } // check for overflow
        }
        if (offset > max_offset) { offset = max_offset; }

        // Translate to offsets relative to &array[0].
        last_offset += start;
        offset += start;
        array -= start;

        // binary search in (last_offset, offset].
        ++last_offset;
        while (last_offset < offset) {
            unsigned int m = last_offset + ((offset - last_offset) >> 1);

            if (key < array[m]) {
                offset = m;
            } else {
                last_offset = m + 1;
            }
        }

        // check for equal values to the left
        while (offset > 0 && array[offset] == array[offset - 1]) {
            offset--;
        }

        return offset;
    }

/**
 * Using expotential search to find the first element which is greater smaller than the given key.
 * From the point start.
 * @param key comparison value for which the next smaller value should be found
 * @param array pointer to the fist element of Array start
 * @param size total length of the array
 * @param start startpoint of search
 * @return k in 0..size such that a[k-1] < key <= a[k]
 */
    static unsigned int gallop_big2small(int key, int *array, unsigned int size) {
        int offset = 1;
        int last_offset = 0;
        unsigned int start = size - 1;

        array += start;
        if (*array < key) {
            return size;
        }
        // key < array[start] --> gallop left, until array[start - offset] < key <= array[start - last_offset]

        const unsigned max_offset = start;
        while (offset < max_offset) {
            if (*(array - offset) < key) { break; }

            last_offset = offset;
            offset = (offset << 1) + 1;
            if (offset <= 0) { // check for overflow
                offset = max_offset;
            }
        }
        if (offset > max_offset) { offset = max_offset; }

        // Translate offsets relative to array[0]
        unsigned int k = last_offset;
        last_offset = size - offset;
        offset = size - k;
        array -= start;

        // binary search in (last_offset+1, offset] for the fist element >= key
        ++last_offset;
        while (last_offset < offset) {

            unsigned int m = last_offset + ((offset - last_offset) >> 1);

            if (array[m] < key) {
                last_offset = m + 1;
            } else {
                offset = m;
            }
        }

        // check for equal values to the left
        while (offset > 0 && key <= array[offset - 1]) {
            offset--;
        }
        return offset;
    }

/**
 * Copies the left partition [array, array+size_left) onto stack
 * and merges with the second partition [array + size-left, array + size-left + size_right).
 */
    static void merge(MergeState *ms, int *array_left, unsigned int size_left, unsigned int size_right) {
        int k;
        int *array_right = array_left + size_left;
        int *current = array_left;
        int min_gallop = ms->min_gallop;

        // move values from the array to a temp helper array
        array_left = new int[size_left];
        std::move(current, current + size_left, array_left);
        int *array_left_start = array_left;

        *current++ = *array_right++;
        --size_right;

        if (size_right == 0) {
            // move the elements form external array back
            std::move(array_left, array_left + size_left, current);
            delete[] array_left_start;
            return; // everything merged
        }
        if (size_left == 0) {
            delete[] array_left_start;
            return; // end of merging
        }

        for (;;) {
            int count_left = 0;          // # of times left won in a row
            int count_right = 0;          // # of times right won in a row

            // merge by feet until the have a candidate for galloping
            do {

                if (*array_right < *array_left) { // take from right side
                    // copy value
                    *current++ = *array_right++;
                    --size_right;
                    // increase counter
                    ++count_right;
                    count_left = 0;

                    if (size_right == 0) {
                        // move the elements form external array back
                        std::move(array_left, array_left + size_left, current);
                        delete[] array_left_start;
                        return; // everything merged
                    }
                } else { // take from left side
                    // copy value
                    *current++ = *array_left++;
                    --size_left;
                    // move counter
                    ++count_left;
                    count_right = 0;

                    if (size_left == 0) {
                        delete[] array_left_start;
                        return; // end of merging
                    }
                }
            } while (count_right < min_gallop && count_left < min_gallop);

            // start galloping until it is useless
            ++min_gallop;
            do {
                min_gallop -= min_gallop > 1;
                ms->min_gallop = min_gallop;

                // gallop on left partition
                k = gallop_small2big(*array_right, array_left, size_left, 0);
                count_left = k;
                if (k) {
                    std::move(array_left, array_left + k, current);
                    current += k;
                    array_left += k;
                    size_left -= k;
                    if (size_left == 0) {
                        delete[] array_left_start;
                        return; // end of merging
                    }
                }

                // gallop on right partition
                k = gallop_small2big(*array_left, array_right, size_right, 0);
                count_right = k;

                // no check needed since k is at least 1
                std::move(array_right, array_right + k, current);
                current += k;
                array_right += k;
                size_right -= k;
                if (size_right == 0) {
                    // move the elements form external array back
                    std::move(array_left, array_left + size_left, current);
                    delete[] array_left_start;
                    return; // everything merged
                }
            } while (count_left >= MIN_GALLOP || count_right >= MIN_GALLOP);
            ++min_gallop;           // increase it for leaving galloping mode
            ms->min_gallop = min_gallop;
        }
    }

/**
 * Copies the right partition [array + size_left, array + size-left + size_right) onto stack
 * and merges with the left partition [array, array+size_left).
 */
    static void merge_reverse(MergeState *ms, int *array_left, unsigned int size_left, unsigned int size_right) {
        int k;
        int *array_left_start = array_left, *array_right_start;
        int *array_right = array_left + size_left;
        int *current = array_right + size_right - 1;
        int min_gallop = ms->min_gallop;

        // copy array and move pointer
        array_right_start = new int[size_right];
        std::move(array_right, array_right + size_right, array_right_start);
        array_right = array_right_start + size_right - 1;
        array_left += size_left - 1;

        //move first element
        *current-- = *array_left--;
        --size_left;

        if (size_left == 0) {
            // move the elements form external array back
            std::move(array_right_start, array_right_start + size_right, current - size_right + 1);
            delete[] array_right_start;
            return; // end of merging
        }
        if (size_right == 0) {
            delete[] array_right_start;
            return; // end of merging
        }

        for (;;) {
            int count_left = 0;          // # of times left won in a row
            int count_right = 0;          // # of times right won in a row

            // merge by feet until we have a candidate for galloping
            do {
                if (*array_right < *array_left) { // left won
                    // move value
                    *current-- = *array_left--;
                    --size_left;
                    // increase counter
                    ++count_left;
                    count_right = 0;

                    if (size_left == 0) {
                        // move the elements form external array back
                        std::move(array_right_start, array_right_start + size_right, current - size_right + 1);
                        delete[] array_right_start;
                        return; // end of merging
                    }
                } else { // right won
                    *current-- = *array_right--;
                    ++count_right;
                    count_left = 0;
                    --size_right;
                    if (size_right == 0) {
                        //if (quit_on_last_extern_reverse(array_right, size_right, current, array_left, size_left)) {
                        delete[] array_right_start;
                        return; // end of merging
                    }
                }
            } while (count_left < min_gallop && count_right < min_gallop);

            // start galloping until it is useless
            ++min_gallop;
            do {
                // gallop on left partition
                min_gallop -= min_gallop > 1;
                ms->min_gallop = min_gallop;
                k = gallop_big2small(*array_right, array_left_start, size_left);
                k = size_left - k;
                count_left = k;
                if (k) {
                    current -= k;
                    array_left -= k;
                    std::move(array_left + 1, array_left + k + 1, current + 1);
                    size_left -= k;
                    if (size_left == 0) {
                        // move the elements form external array back
                        std::move(array_right_start, array_right_start + size_right, current - size_right + 1);
                        delete[] array_right_start;
                        return; // end of merging
                    }
                }

                // gallop on right partition
                k = gallop_big2small(*array_left, array_right_start, size_right);
                k = size_right - k;
                count_right = k;
                // no check needed since k is at least 1
                current -= k;
                array_right -= k;
                std::move(array_right + 1, array_right + 1 + k, current + 1);
                size_right -= k;
                if (size_right == 0) {
                    //if (quit_on_last_extern_reverse(array_right, size_right, current, array_left, size_left)) {
                    delete[] array_right_start;
                    return; // end of merging
                }
            } while (count_left >= MIN_GALLOP || count_right >= MIN_GALLOP);
            ++min_gallop;           /* penalize it for leaving galloping mode */
            ms->min_gallop = min_gallop;
        }
    }


    static int next_run(int *start, int *end, int *descending) {

        if (++start == end) { return 1; }

        end--;
        unsigned int length = 2;
        if (*start < *(start - 1)) {
            *descending = 1; // descending run
            while (start < end) {
                start++;
                if (*start > *(start - 1)) { break; }
                length++;
            }
        } else {
            *descending = 0; // ascending run
            while (start < end) {
                start++;
                if (*start < *(start - 1)) { break; }
                length++;
            }
        }

        return length;
    }

    // Reverse the run [from, to).
    static void reverse_run(int *array, unsigned int from, unsigned int to) {
        int rendevous = ((to - from) >> 1) - 1;
        to--;
        for (int i = 0; i <= rendevous; i++) {
            utility::swap(array, from + i, to - i);
        }
    }

    /** merges run i and i+1 on stack */
    static void merge_at(MergeState *ms, int i) {
        int *array_left = ms->stack[i].pointer;
        int *array_right = ms->stack[i + 1].pointer;
        unsigned int size_left = ms->stack[i].len;
        unsigned int size_right = ms->stack[i + 1].len;

        // set new values on stack and shift remaining run
        ms->stack[i].len = size_left + size_right;
        if (i == (ms->top - 3)) {
            ms->stack[i + 1] = ms->stack[i + 2];
        }
        --ms->top;

        // skip elements already in place on left side
        unsigned int k = gallop_small2big(*array_right, array_left, size_left, 0);
        array_left += k;
        size_left -= k;
        if (size_left == 0) { return; }

        // skip elements already in place on right side
        size_right = gallop_big2small(array_left[size_left - 1], array_right, size_right);
        if (size_right <= 0) { return; }


        // Merge what remains of the runs
        if (size_left <= size_right) {
            merge(ms, array_left, size_left, size_right);
        } else {
            merge_reverse(ms, array_left, size_left, size_right);
        }
    }

    /** Check stack for runs to merge.
     *  Two runs will be merged if they meet the following conditions:
     *
     *  1. len[-3] > len[-2] + len[-1]
     *  2. len[-2] > len[-1]
     */
    static void merge_collapse(MergeState *ms) {
        struct run *runs = ms->stack;
        int n;

        while (ms->top > 1) {
            n = ms->top - 2;
            if (n > 0 && runs[n - 1].len <= runs[n].len + runs[n + 1].len) {
                if (runs[n - 1].len < runs[n + 1].len) { --n; }
                merge_at(ms, n);
            } else if (runs[n].len <= runs[n + 1].len) {
                merge_at(ms, n);
            } else { break; }
        }
    }

    /** merges all runs on ms->stack */
    static void collapse_stack(MergeState *ms) {
        struct run *runs = ms->stack;
        while (ms->top > 1) {
            int n = ms->top - 2;
            if (n > 0 && runs[n - 1].len < runs[n + 1].len) { --n; }
            merge_at(ms, n);
        }
    }

    /**
     * Reimplementation of Pythons Timsort algorithm
     * @param array the array of ints to sort
     * @param size size of the array to sort
     */
    static void sort(int *array, unsigned int size) {
        MergeState ms;
        int run_length;

        /* Scan the array left to right to find natural runs and
         * extending short runs to minrun elements.
         */
        int *first = array, *last = array + size - 1;
        int min_run = utility::minRunLength(size);
        unsigned int remaining_size = size;
        do {
            int descending;

            // Identify next run.
            run_length = next_run(first, last, &descending);
            if (descending) {
                reverse_run(first, 0, run_length);
            }

            // If run ist too short, extend to min(minrun, nremaining).
            if (run_length < min_run) {
                run_length = remaining_size < min_run ? remaining_size : min_run;
                utility::insertionSort(first, first + run_length);
            }

            // Push run onto pending-runs stack
            ms.stack[ms.top].pointer = first;
            ms.stack[ms.top].len = run_length;
            ++ms.top;

            //check for possible merges
            merge_collapse(&ms);

            // increase counter
            first += run_length;
            remaining_size -= run_length;

        } while (remaining_size);

        // merge remaining runs
        collapse_stack(&ms);
    }

    static void merge_independent(MergeState *ms, int *array, unsigned int size_left, unsigned int size_right) {

        // skip elements already in place on left side
        unsigned int k = gallop_small2big(array[size_left], array, size_left, 0);
        array += k;
        size_left -= k;
        if (size_left == 0) { return; }

        // skip elements already in place on right side
        size_right = gallop_big2small(array[size_left - 1], array + size_left, size_right);
        if (size_right <= 0) { return; }


        // Merge what remains of the runs
        if (size_left <= size_right) {
            merge(ms, array, size_left, size_right);
        } else {
            merge_reverse(ms, array, size_left, size_right);
        }
    }

    static void sort_2_parallel(int *array, unsigned int size) {
        int iam, start, split = size >> 1;

        //use specific number of threads
        omp_set_dynamic(0);

#pragma omp parallel default(none)  private(iam, start) shared(array, size, split) num_threads(2)
        {
            iam = omp_get_thread_num();
            start = iam * split;
            if (iam == 0) {
                sort(array, split);
            } else {
                sort(array + split, size - split);
            }

        }

        // merge splitted data
        MergeState ms;
        merge_independent(&ms, array, split, size - split);
    }


    static void sort_n_parallel(int *array, unsigned int size) {
        int iam, start, partition_size, max_threads = omp_get_max_threads();

        // use max number of threads
        omp_set_dynamic(0);
        omp_set_num_threads(max_threads);
        partition_size = size / max_threads;

        //sort in parallel
#pragma omp parallel default(none)  private(iam, start) shared(array, size, partition_size, max_threads)
        {
            iam = omp_get_thread_num();
            start = iam * partition_size;

            if (iam == (max_threads - 1)) {
                sort(array + start, (size - start));
            } else {
                sort(array + start, partition_size);
            }

        }

        //merge partitions per level (bottom up)
        MergeState ms;
        while (partition_size < size) {

            unsigned int step = partition_size + partition_size;
            unsigned int start = 0, mid = partition_size, end = step;
            while (mid < size) {

                merge_independent(&ms, array + start, partition_size, end - mid);

                // increase pointer
                start += step;
                mid += step;
                end = std::min(end + step, size);
            }
            partition_size += partition_size;
        }
    }

} // namespace timsort