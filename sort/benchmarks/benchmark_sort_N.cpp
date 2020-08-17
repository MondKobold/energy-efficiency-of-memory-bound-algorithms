//
// @author: Thilo Kamradt
//
#include <iostream>
#include "../../utility.hpp"
#include "sort.h"

void run(int set_size) {

    //check size
    if (set_size < 0 || set_size >= utility::DATASETS.size()) {
        std::cout << "No set defined! Please add an integer of the intervall [0, " << utility::DATASETS.size()
                  << ") to select a dataset." << std::endl;
    }

    // set number of iterations for correct measurements
    int iterations = 1;
    if (set_size < 6) {
        iterations = 10;
    }

    // run sorting
    for (int i = 0; i < iterations; ++i) {
        sort_x_elements(utility::DATASETS[set_size]);
    }
}