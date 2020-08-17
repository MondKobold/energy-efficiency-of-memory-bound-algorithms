//
// @author Thilo Kamradt
//
#include "clustering.hpp"
#include <queue>

using namespace std;

class DBSCAN {
#define NOISE 0
#define NOT_CLASSIFIED -1

public:
    int minPts;
    double eps;
    vector<Point> points;
    int size;
    vector<vector<int> > adjPoints;
    vector<vector<int> > cluster;
    int clusterIdx;

    DBSCAN( double eps, int minPts, vector<Point> points) {
        this->eps = eps;
        this->minPts = minPts;
        this->points = points;
        this->size = (int)points.size();
        adjPoints.resize(size);
        this->clusterIdx=0;
    }

    void run () {

        // determine point classification
        determine_Neighborhood();

        // iterrate over all points and add them to a cluster or noise
        for(int i=0; i < size; i++) {
            if(points[i].cluster != NOT_CLASSIFIED) {
                continue;
            }

            if(isCorePoint(i)) {
                addPoints2cluster(i, ++clusterIdx);
            } else {
                points[i].cluster = NOISE; // this point could be a borderpoint but this needs to be proven
            }
        }

        // generate output clustering list
        cluster.resize(clusterIdx+1);
        for(int i=0;i<size;i++) {
            if(points[i].cluster != NOISE) {
                cluster[points[i].cluster].push_back(i);
            }
        }
    }

    /**
     * Adds the Point to the given cluster and adds connected Points via DFS.
     */
    void addPoints2cluster (int current, int cluster) {
        queue<Point*> q;
        q.push(&points[current]);
        int qSize;
        while (!q.empty()) {
            qSize = q.size();
#pragma omp parallel for
            for (int i = 0; i < qSize; i++) {
                Point *currNode;
#pragma omp critical
                {
                    currNode = q.front();
                    q.pop();
                }
                if(!isCorePoint(currNode)) {
                    continue;
                }
                currNode->cluster = cluster;
                for(auto &next: adjPoints[current]) {
                    if(points[next].cluster != NOT_CLASSIFIED) {
                        continue;
                    }
#pragma omp critical
                    q.push(&points[next]);
                }

            }
        }
    }

    /**
     * Determines the epsilon-neigborhood for each point
     */
    void determine_Neighborhood() {
#pragma omp parallel for num_threads(4)
        for(int i=0;i<size;i++) {
            for(int j=0;j<size;j++) {
                if(i==j) {
                    continue;// the point itself
                }
                if(points[i].getDis(points[j]) <= eps) {
                    points[i].ptsCnt++;
                    adjPoints[i].push_back(j);
                }
            }
        }
    }

    /**
     * Checks if the given Point is a Core Point
     */
    bool isCorePoint(Point *point) {
        return point->ptsCnt >= minPts;
    }

    /**
     * Checks if the given Point is a Core Point
     * @param idx of the Point
     */
    bool isCorePoint(int idx) {
        return points[idx].ptsCnt >= minPts;
    }

    vector<vector<int>> getCluster() {
        return cluster;
    }
};