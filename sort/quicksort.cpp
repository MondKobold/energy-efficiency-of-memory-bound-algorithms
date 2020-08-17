//
// @author: Thilo Kamradt
//
#include <random>
#include "../utility.hpp"

namespace quicksort {

// ----------------------------- Median of Three -----------------------------
/**
 * Takes the fist 3 Elements of the array and sets the median as pivot element.
 * The other two are set as Sentinals;
 */
    static int config_MedianOfThree(int *array, int left, int right) {
        int mot[3] = {std::move(array[left]), std::move(array[left + 1]), std::move(array[left + 2])};
        utility::insertionSort(mot, mot + 3);
        array[left] = mot[1];
        array[left + 1] = std::move(mot[0]);
        array[left + 2] = std::move(array[right - 1]);
        array[right - 1] = std::move(mot[2]);
        return mot[1];
    }

    static int partition_MedianOfThree(int *array, int left, int right) {
        int pivot = config_MedianOfThree(array, left, right);
        int scan_left = left + 1, scan_right = right;
        while (true) {
            while (array[++scan_left] < pivot) if (scan_left == right) break;
            while (pivot < array[--scan_right]) if (scan_right == left) break;
            if (scan_left >= scan_right) break;
            utility::swap(array, scan_left, scan_right);
        }
        utility::swap(array, left, scan_right);
        return scan_right;
    }

    void auxiliary_MedianOfThree(int *array, int left, int right) {
        if (right <= (left + 3)) { // 3 elements left
            utility::insertionSort(array + left, array + right);
            return;
        }
        int index = partition_MedianOfThree(array, left, right);
        auxiliary_MedianOfThree(array, left, index);
        auxiliary_MedianOfThree(array, index + 1, right);
    }

    static void sort_MedianOfThree(int *array, unsigned int size) {
        auxiliary_MedianOfThree(array, 0, size);
    }

// ----------------------------- quicksort basics -----------------------------
/**
 * Partition into a[left..i-1], a[i], a[i+1..right]. Left <= Pivot <= Right
 * Partitions the values in [left, right)
 */
    static int partition(int *array, int left, int right) {
        int scan_left = left, scan_right = right; // left and right scan indices
        int pivot = array[left];
        //int left_limit = left + 1;
        while (true) { // Scan right, scan left, check for scan complete, and exchange.
            while (array[++scan_left] <= pivot) { // find left exchange partner
                if (scan_left == right) { break; }
            }
            while (pivot <= array[--scan_right]) {// find right exchange partner
                if (scan_right == left) { break; }
            }
            if (scan_left >= scan_right) { break; } // end of partitioning
            utility::swap(array, scan_left, scan_right);
        }
        if(scan_right != left) {
            utility::swap(array, left, scan_right); // Put pivot = a[scan_right] into position
        }
        return scan_right; // with a[left..scan_right-1] <= a[scan_right] <= a[scan_right+1..right].
    }

// ----------------------------- Base Implementation -----------------------------
    void auxiliary_BaseImpl(int *array, int left, int right) {
        if ((right - left) < 2) {
            // end of recursion
            return;
        }
        int pivot_pointer = partition(array, left, right);
        auxiliary_BaseImpl(array, left, pivot_pointer);
        auxiliary_BaseImpl(array, pivot_pointer + 1, right);
    }

    static void sort_BaseImpl(int *array, unsigned int size) {
        auxiliary_BaseImpl(array, 0, size);
    }

// ----------------------------- Base PreCheck -----------------------------
    void auxiliary_Precheck(int *array, int left, int right) {
        if (right <= (left + 3)) { // 3 elements left
            utility::insertionSort(array + left, array + right);
            return;
        }
        if (std::is_sorted(array + left, array + right)) {
            // end of recursion
            return;
        }
        int pivot_pointer = partition_MedianOfThree(array, left, right);
        auxiliary_Precheck(array, left, pivot_pointer);
        auxiliary_Precheck(array, pivot_pointer + 1, right);
    }

    static void sort_Precheck(int *array, unsigned int size) {
        auxiliary_Precheck(array, 0, size);
    }

// ----------------------------- Cut Off -----------------------------
    void auxiliary_CutOff(int *array, int left, int right) {
        int dist = right - left;
        if (dist <= utility::CUT_OFF_THRESHOLD) {
            utility::insertionSort(array + left, array + right);
            return;
        }
        int pivot_pointer = partition_MedianOfThree(array, left, right);
        auxiliary_CutOff(array, left, pivot_pointer);
        auxiliary_CutOff(array, pivot_pointer + 1, right);
    }

    static void sort_CutOff(int *array, unsigned int size) {
        auxiliary_CutOff(array, 0, size);
    }

// ----------------------------- Cut Off And Ignore -----------------------------
    void auxiliary_CutOffAndIgnore(int *array, int left, int right) {
        if ((right - left) <= utility::CUT_OFF_THRESHOLD) {
            return;
        }
        int pivot_pointer = partition_MedianOfThree(array, left, right);
        auxiliary_CutOffAndIgnore(array, left, pivot_pointer);
        auxiliary_CutOffAndIgnore(array, pivot_pointer + 1, right);
    }

    static void sort_CutOffAndIgnore(int *array, unsigned int size) {
        auxiliary_CutOffAndIgnore(array, 0, size);
        utility::insertionSort(array, array + size);
    }


// ----------------------------- Shuffle -----------------------------
    static void sort_Shuffle(int *array, unsigned int size) {
        // shuffle array values to prevent worst case runtime
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(array, array + size, g);
        // start quicksort
        sort_BaseImpl(array, size);
    }

    void auxiliary_random_pivot(int *array, int left, int right) {
        if ((right - left) < 2) {
            // end of recursion
            return;
        }
        utility::swap(array, left, left + (rand() % (right - left - 1)));
        int pivot_ptr = partition(array, left, right);
        auxiliary_random_pivot(array, left, pivot_ptr);
        auxiliary_random_pivot(array, pivot_ptr + 1, right);
    }

    static void sort_random_pivot(int *array, unsigned int size) {
        auxiliary_random_pivot(array, 0, size);
    }
// ----------------------------- Entropy Optimal Sorting -----------------------------
    /**
     *  3-Way partitioning with left < pivot-Partition < right.
     *  Implementation after Dijkstra.
     *  Scan from left to right and swap elements to the begin/end of the array that are not equal to the pivot element
     */
    static void partition_3_way_dijkstra(int *array, int low, int high) {
        if (high - low < 2) {
            return;
        }

        // partition
        int smaller_than_pivot = low, i = low + 1, greater_than_pivot = high - 1;
        int pivot = array[low];
        while (i <= greater_than_pivot) {
            int current = array[i];
            if (current < pivot) {
                utility::swap(array, smaller_than_pivot++, i++);
            } else if (current > pivot) {
                utility::swap(array, i, greater_than_pivot--);
            } else {
                i++;
            }
        } // [low..smaller_than_pivot-1] < [smaller_than_pivot..greater_than_pivot] < [greater_than_pivot+1..high].

        // sort rest
        partition_3_way_dijkstra(array, low, smaller_than_pivot);
        partition_3_way_dijkstra(array, greater_than_pivot + 1, high);
    }

    static void sort_3_way_dijkstra(int *array, unsigned int size) {
        partition_3_way_dijkstra(array, 0, size);
    }

    /**
    *  3-Way partitioning with left < pivot-Partition < right.
    *  Implementation after Bentley and McIlroy
    */
    static void partition_3_way_Bentley_McIlroy(int *array, int low, int high) {
        if (high - low < 2) {
            return;
        }
        // Bentley_McIlroy partitioning
        int i = low, j = high;
        int p = low, q = high;
        int pivot = array[low];
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

    static void sort_3_way_Bentley_McIlroy(int *array, unsigned int size) {
        partition_3_way_Bentley_McIlroy(array, 0, size);
    }
// ----------------------------- No Tail Call -----------------------------
    /**
    *  keine Enrekursion d.h. der rekursive aufruf wird durch eine iteration ersetzt
    */
    static void auxiliary_TailCallElimination(int *array, int low, int high) {
        int pivot_ptr;
        while (high - low > 1) {
            pivot_ptr = partition_MedianOfThree(array, low, high);
            auxiliary_TailCallElimination(array, low, pivot_ptr);
            low = pivot_ptr + 1;
        }
    }

    static void sort_TailCallElimination(int *array, int size) {
        auxiliary_TailCallElimination(array, 0, size);
    }

}
