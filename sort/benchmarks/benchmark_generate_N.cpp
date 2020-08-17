//
// @author: Thilo Kamradt
//
#include <iostream>
#include "../../utility.hpp"
#include "sort.h"

int main(int argc, char **argv) {

    if(argc == 2) {
        int selected_dataset = std::strtol(argv[1], NULL, 10);
        if(selected_dataset <= 0 || selected_dataset > utility::DATASETS.size()){
			std::cout << "No set defined! Please add an integer between 1 and " << utility::DATASETS.size() << " to select the select a dataset." << std::endl;
			return 1;
		}
		selected_dataset--;
		
		// number of repeats
		int iterations = 1;
		if (selected_dataset == 0) {
			iterations = 100;
		} else if (selected_dataset < 5) { 
			iterations = 10;
		}
	
        for (int i = 0; i < iterations; ++i) {
            generate_x_elements(utility::DATASETS[selected_dataset]);
        }
    } else {
        std::cout << "No set defined! Please add an integer between 1 and " << utility::DATASETS.size() << " to select the select a dataset." << std::endl;
		return 1;
	}
	
	return 0;
}