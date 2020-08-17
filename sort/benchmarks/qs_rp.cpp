//
// @author Thilo Kamradt
//
#define QS_RP
#include "benchmark_sort_N.cpp"

int main(int argc, char **argv) {
    if(argc == 2) {
        int set_size = std::strtol(argv[1], NULL, 10);
        run(set_size);
    } else {
        std::cout << "No set defined! Please add an integer of the intervall [0, " << utility::DATASETS.size() << ") to select a dataset." << std::endl;
    }
    return 0;
}