//
// @author Thilo Kamradt
//
#include "../../utility.hpp"
#include "../clustering.hpp"
#include "../kmeans_opt.cpp"
#include <chrono>
#include <iostream>


int main() {
    auto data = generate(utility::KB16, 5);

    auto begin = std::chrono::steady_clock::now();
    std::vector<centroid> codebook = apply_kMeans(data, 5);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Runtime k-Means OPT 16 MiB: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() << std::endl;

    return 0;
}