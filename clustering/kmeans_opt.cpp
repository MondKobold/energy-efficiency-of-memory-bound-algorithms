//
// @author Thilo Kamradt
//

#include <vector>
#include <cmath>
#include <limits>
#include <random>
#include "clustering.hpp"

static std::vector<int> get_zero_vector_int(int size) {
    std::vector<int> v(size, 0);
    return v;
}

static void increase_by_int(std::vector<int> &c, std::vector<int> &v) {
    for (int i = 0; i <c.size(); ++i) {
        c[i] += v[i];
    }
}

static std::vector<double> get_zero_vector_double(int size) {
    std::vector<double> v(size, 0);
    return v;
}

static void increase_by_double(std::vector<double> &c, std::vector<double> &v) {
    for (int i = 0; i <c.size(); ++i) {
        c[i] += v[i];
    }
}

//#pragma omp declare reduction(VecSum: Custom: omp_out += omp_in)
//#pragma omp declare reduction(CodebookSum: Custom: omp_out += omp_in)
#pragma omp declare reduction(vec_int_sum : std::vector<int> : increase_by_int(omp_out, omp_in)) \
                    initializer(omp_priv = get_zero_vector_int(omp_orig.size()))
#pragma omp declare reduction(vec_double_sum : std::vector<double> : increase_by_double(omp_out, omp_in)) \
                    initializer(omp_priv = get_zero_vector_double(omp_orig.size()))

static double getDistance(point &v1, centroid &v2) {
    double diffx = v1.x - v2.x;
    double diffy = v1.y - v2.y;
    return sqrt((diffx * diffx) + (diffy * diffy));
}

static centroid add(centroid &v1, point &v2) {
    centroid result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    return result;
}

static inline void increase_by(centroid &c, point &v) {
    c.x += v.x;
    c.y += v.y;
}

static centroid frac(centroid &v1, int quotient) {
    centroid result;
    result.x = v1.x / quotient;
    result.y = v1.y / quotient;
    return result;
}

static inline void split_by(centroid &c, int quotient) {
    c.x = c.x / quotient;
    c.y = c.y / quotient;
}

static std::vector<centroid> get_next_codebook(std::vector<point> &data, std::vector<centroid> &centroids, std::vector<int> &clustering) {

    // init next codebook and helperfield
    std::vector<int> count(centroids.size(),0);
    std::vector<double> bookX(centroids.size(),0);
    std::vector<double> bookY(centroids.size(),0);

    // update clustering and collect data for new codebook
    int nearestCentroid;
    double minDistance;
    point current;
#pragma omp parallel for reduction(vec_int_sum:count) reduction(vec_double_sum:bookX,bookY) num_threads(4)
    for (int dataIndex = 0; dataIndex < data.size(); ++dataIndex) {
        minDistance = std::numeric_limits<double>::max();
        current = data[dataIndex];
        for (int centroidIndex = 0; centroidIndex < centroids.size(); ++centroidIndex) {
            double distance = getDistance(current, centroids[centroidIndex]);
            if(minDistance > distance) {
                nearestCentroid = centroidIndex;
                minDistance = distance;
            }
        }
        clustering[dataIndex] = nearestCentroid;
        count[nearestCentroid]++;
        bookX[nearestCentroid] += current.x;
        bookY[nearestCentroid] += current.y;
    }

    // replaces summed codebook by correct values
    std::vector<centroid> next_codebook(centroids.size());
    for (int i = 0; i < next_codebook.size(); ++i) {
        next_codebook[i].x = bookX[i] / count[i];
        next_codebook[i].y = bookY[i] / count[i];
    }

    return next_codebook;
}

static std::vector<centroid> getCodebook(std::vector<point> &data, int k) {
    std::vector<centroid> codebook(k);

    //get range of dataset
    point current = data[0];
    int minx = current.x;
    int miny = current.y;
    int maxx = current.x;
    int maxy = current.y;

#pragma omp parallel for reduction(min:minx,miny) reduction(max:maxy,maxx) num_threads(4)
    for (int i = 1; i < data.size(); ++i) {
        current = data[i];
        if(current.x > maxx) {
            maxx = current.x;
        }
        if (current.x < minx) {
            minx = current.x;
        }
        if(current.y > maxy) {
            maxy = current.y;
        }
        if (current.y < miny) {
            miny = current.y;
        }
    }

    maxx = maxx - minx;
    maxy = maxy - miny;

    // fill with random values within range
    for (int j = 0; j < k; ++j) {
        codebook[j].x = minx + (rand() % maxx);
        codebook[j].y = miny + (rand() % maxy);
    }

    return codebook;
}

static std::vector<centroid> apply_kMeans(std::vector<point> &data, int k) {
    std::vector<int> clustering(data.size());
    std::vector<centroid> codebook = getCodebook(data, k);
    for (int i = 0; i < 10000; ++i) {
        codebook = get_next_codebook(data, codebook, clustering);
    }
    return codebook;
}