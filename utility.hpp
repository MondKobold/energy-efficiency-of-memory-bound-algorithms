//
// @author Thilo Kamradt
//
#pragma once
#include <array>
#include <algorithm>
#include <vector>
#include <iostream>

namespace utility {

    //size in Byte of Testdata based on 32 bit / 4 byte Integer
    static const unsigned int KB1 = 256;
    static const unsigned int KB16 = 4'096;
    static const unsigned int KB32 = 8'192;
    static const unsigned int KB64 = 16'382;
    static const unsigned int KB128 = 32'764;
    static const unsigned int KB256 = 65'528;
    static const unsigned int KB512 = 131'056;
    static const unsigned int MB1 = 262'112;
    static const unsigned int MB2 = 524'224;
    static const unsigned int MB4 = 1'048'448;
    static const unsigned int MB8 = 2'096'896;
    static const unsigned int MB16 = 4'193'738;
    static const unsigned int MB32 = 8'387'476;
    static const unsigned int GB1 = 268'402'688;
    static const unsigned int GB2 = 536'805'376;

    static const std::array<unsigned int, 15> DATASETS{KB1, KB16, KB32, KB64, KB128, KB256, KB512, MB1, MB2, MB4, MB8,
                                                       MB16, MB32, GB1, GB2};
    static const int DATASETS_SMALL_LIMIT = 10;

    static const int CUT_OFF_THRESHOLD = 32;
    static const int SEVEN_BIT = 1 << 6;

    void print_array(int *array, int length) {
        std::cout << array[0];
        for (int i = 1; i < length; i++) {
            std::cout << ", " << array[i];
        }
        std::cout << std::endl;
    }

    inline void swap(int *array, int one, int two) {
        int temp = std::move(array[one]);
        array[one] = std::move(array[two]);
        array[two] = std::move(temp);
    }

    /**
     * InsertionSort for the Interval [left_pointer, right_pointer) of the given array
     */
    void insertionSort(int *start, int *end) {
        for (int *i = start + 1; i < end; i++) {

            int current_element = std::move(*i);
            int *compare_pointer = i - 1;

            // move all elements to the end until array[compare_pointer] <= current_element
            int *destination = std::upper_bound(start, compare_pointer, current_element);
            for (; compare_pointer > destination; compare_pointer--) {
                *(compare_pointer + 1) = std::move(*compare_pointer);
            }
            *destination = std::move(current_element);
        }
    }

    /**
     * InsertionSort for the Interval [left_pointer, right_pointer) of the given array
     */
    void insertionSort(std::vector<int>::iterator start, std::vector<int>::iterator end) {
        std::vector<int>::iterator iter = start;
        while (iter < end) {
        //for (int *i = start + 1; i < end; i++) {

            int current_element = std::move(*iter);
            std::vector<int>::iterator compare_pointer = iter - 1;

            // move all elements to the end until array[compare_pointer] <= current_element
            std::vector<int>::iterator destination = std::upper_bound(start, compare_pointer, current_element);
            for (; compare_pointer > destination; compare_pointer--) {
                *(compare_pointer + 1) = std::move(*compare_pointer);
            }
            *destination = std::move(current_element);
        }
    }

    static int minRunLength(int n) {
        int r = 0;
        while (n >= SEVEN_BIT) {
            r |= (n & 1);
            n >>= 1;
        }
        return n + r;
    }

    static int get_position_MSB(int n) {
        int msb = 0;
        while (n > 0) {
            n = n >> 1;
            msb++;
        }
        return msb;
    }

#ifdef _WIN32
#include <windows.h>

    static void sleep_for(int milliseconds)
    {
        Sleep(milliseconds * 1'000);
    }
#else

#include <unistd.h>

    static void sleep_for(int seconds) {
        usleep(seconds * 1'000'000); // takes microseconds
    }

#endif

}