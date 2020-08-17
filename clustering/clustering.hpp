//
// @author Thilo Kamradt
//
#pragma once
#include <vector>
#include <random>

struct point {
    int x;
    int y;
};

struct centroid {
    double x;
    double y;
};

class Point {
public:
    double x, y;
    int ptsCnt, cluster;
    double getDis(const Point & ot) {
        return sqrt((x-ot.x)*(x-ot.x)+(y-ot.y)*(y-ot.y));
    }
};

static inline void fill(std::vector<point> &data, int start, int end, int offsetX, int offsetY){
    std::random_device rd;
    std::normal_distribution<> distX(100 + offsetX, 50);
    std::normal_distribution<> distY(100 + offsetY, 50);
    for (int i = start; i < end; ++i) {
        data[i].x = std::round(distX(rd));
        data[i].y = std::round(distY(rd));
    }
}

static std::vector<point> generate(int size, int cluster) {
    int size_per_cluster = size/cluster;
    std::vector<point> data(size_per_cluster * cluster);

    int offsetX[] = {0, 200, 0, 200, 300, 100};
    int offsetY[] = {0, 0, 200, 200, 100, 300};
    int cluster_start = 0;
    int cluster_end = size_per_cluster;
    for (int clusterIdx = 0; clusterIdx < cluster; ++clusterIdx) {
        fill(data, cluster_start, cluster_end, offsetX[clusterIdx], offsetY[clusterIdx]);
        cluster_start += size_per_cluster;
        cluster_end += size_per_cluster;
    }
    return data;
}

static inline void fill_Point(std::vector<Point> &data, int start, int end, int offsetX, int offsetY){
    std::random_device rd;
    std::normal_distribution<> distX(100 + offsetX, 50);
    std::normal_distribution<> distY(100 + offsetY, 50);
    for (int i = start; i < end; ++i) {
        data[i].x = std::round(distX(rd));
        data[i].y = std::round(distY(rd));
        data[i].cluster = -1;
    }
}

static std::vector<Point> generate_Points(int size, int cluster) {
    int size_per_cluster = size/cluster;
    std::vector<Point> data(size_per_cluster * cluster);

    int offsetX[] = {0, 200, 0, 200, 300, 100};
    int offsetY[] = {0, 0, 200, 200, 100, 300};
    int cluster_start = 0;
    int cluster_end = size_per_cluster;
    for (int clusterIdx = 0; clusterIdx < cluster; ++clusterIdx) {
        fill_Point(data, cluster_start, cluster_end, offsetX[clusterIdx], offsetY[clusterIdx]);
        cluster_start += size_per_cluster;
        cluster_end += size_per_cluster;
    }
    return data;
}
