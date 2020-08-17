//
// @author Thilo Kamradt
//


#include <vector>
#include <cmath>
#include <limits>
#include <random>
#include "clustering.hpp"

static double getDistance(point &v1, centroid &v2) {
    double squaredSum = 0;
    squaredSum = pow(v1.x - v2.x, 2);
    squaredSum += pow(v1.y - v2.y, 2);

    return sqrt(squaredSum);
}

static centroid add(centroid &v1, point &v2) {
    centroid result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    return result;
}

static centroid frac(centroid &v1, int quotient) {
    centroid result;
    result.x = v1.x / quotient;
    result.y = v1.y / quotient;
    return result;
}

static void apply_centroids(std::vector<point> &data, std::vector<centroid> &centroids, std::vector<int> &clustering) {
    int nearestCentroid;
    double minDistance;
    for (int dataIndex = 0; dataIndex < data.size(); ++dataIndex) {
        minDistance = std::numeric_limits<double>::max();
        for (int centroidIndex = 0; centroidIndex < centroids.size(); ++centroidIndex) {
            double distance = getDistance(data[dataIndex], centroids[centroidIndex]);
            if(minDistance > distance) {
                nearestCentroid = centroidIndex;
                minDistance = distance;
            }
        }
        clustering[dataIndex] = nearestCentroid;
    }
}

static void update_centroids(std::vector<point> &data, std::vector<centroid> &centroids, std::vector<int> &clustering) {
    for (int i = 0; i < centroids.size(); ++i) {
        centroids[i].x = 0;
        centroids[i].y = 0;
    }
    std::vector<int> count(centroids.size(),0);
    for (int dataIndex = 0; dataIndex < data.size(); ++dataIndex) {
        count[clustering[dataIndex]]++;
        centroids[clustering[dataIndex]] = add(centroids[clustering[dataIndex]], data[dataIndex]);
    }
    for (int i = 0; i < centroids.size(); ++i) {
        centroids[i] = frac(centroids[i], count[i]);
    }
}

static std::vector<centroid> getCodebook(std::vector<point> &data, int k) {
    std::vector<centroid> codebook(k);

    //get range of dataset
    point min = data[0];
    point max = data[0];

    for (int i = 1; i < data.size(); ++i) {
        if(data[i].x > max.x) {
            max.x = data[i].x;
        }
        if (data[i].x < min.x) {
            min.x = data[i].x;
        }
        if(data[i].y > max.y) {
            max.y = data[i].y;
        }
        if (data[i].y < min.y) {
            min.y = data[i].y;
        }
    }

    max.x = max.x - min.x;
    max.y = max.y - min.y;

    // fill with random values within range
    for (int j = 0; j < k; ++j) {
        codebook[j].x = min.x + (rand() % max.x);
        codebook[j].y = min.y + (rand() % max.y);
    }

    return codebook;
}

static std::vector<centroid> apply_kMeans(std::vector<point> &data, int k) {
    std::vector<int> clustering(data.size());
    std::vector<centroid> codebook = getCodebook(data, k);
    for (int i = 0; i < 10000; ++i) {
        apply_centroids(data, codebook, clustering);
        update_centroids(data, codebook, clustering);
    }
    return codebook;
}