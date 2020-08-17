//
// @author: Thilo Kamradt
//
#include <iostream>
#include <chrono>
#include "../../utility.hpp"
#include "sort.h"

int main(int argc, char **argv) {

    if(argc == 2) {
        // run benchmark
        int selected_dataset = std::strtol(argv[1], NULL, 10);
		if (selected_dataset <= 0 || selected_dataset > utility::DATASETS.size()) {
			// print explaination
			std::cout << "No set defined! Please add an integer between 1 and " << utility::DATASETS.size() << " to select the select a dataset." << std::endl;
			return 1;
		}
        auto start = std::chrono::steady_clock::now();
        auto end = start;
        auto sum = end - start;
        for (int i = 0; i < 10; i++) {
            start = std::chrono::steady_clock::now();
            generate_x_elements(utility::DATASETS[selected_dataset - 1]);
            end = std::chrono::steady_clock::now();
            auto delta = end - start;
            sum += delta;
        }

        // use average
        sum = sum/10;

        // print result
        auto duration_ns =  std::chrono::duration_cast<std::chrono::nanoseconds>(sum).count();
        auto duration_ms =  std::chrono::duration_cast<std::chrono::milliseconds>(sum).count();
        auto duration_s =  std::chrono::duration_cast<std::chrono::seconds>(sum).count();

        std::cout << std::endl << "Time needed: " << duration_ns << " ns/ " << duration_ms << " ms/ " << duration_s << " s" << std::endl;
    } else {
        // print explaination
        std::cout << "No set defined! Please add an integer between 1 and " << utility::DATASETS.size() << " to select the select a dataset." << std::endl;
    }
	return 0;
}