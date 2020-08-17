//
// @author Thilo Kamradt
//
#define QS_3_BMI
#include "runtime_performance.cpp"

int main(int argc, char **argv) {
    int size, precision, single_mode = 0;
    std::string input;

    // load parameters
    if (argc == 2) {
        size = std::strtol(argv[1], NULL, 10);
        input = "ns";
    } else if (argc == 3) {
        size = std::strtol(argv[1], NULL, 10);
        input = argv[2];
    }  else if (argc == 4) {
        size = std::strtol(argv[1], NULL, 10);
        input = argv[2];
        single_mode = 1;
    } else {
        std::cout << "How many Dataset should be used? [1 - " << utility::DATASETS.size() << "]" << std::endl;
        std::cin >> size;

        std::cout << "Choose your precision[ns|ms|s]:" << std::endl;
        std::cin >> input;
    }

    precision = cast_input_2_precision(input);

    printSingleRuntime(size, precision);

    return 0;
}