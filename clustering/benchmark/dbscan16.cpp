//
// @author Thilo Kamradt
//
#include "../../utility.hpp"
#include "../clustering.hpp"
#include "../dbscan.cpp"
#include <chrono>
#include <iostream>


int main() {
    std::vector<point> data = generate(utility::KB16, 5);
    DBSCAN dbScan( 10, 6, data);

    auto begin = std::chrono::steady_clock::now();
    dbScan.run();
    auto end = std::chrono::steady_clock::now();
    std::cout << "Runtime DBSCAN 16 MiB: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin).count() << std::endl;

    return 0;
}