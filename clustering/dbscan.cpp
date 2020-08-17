//
// @author Thilo Kamradt
//
#include "clustering.hpp"

using namespace std;

class DBSCAN {
#define NOISE 0
#define NOT_CLASSIFIED -1

public:
    int minPts;
    double eps;
    vector<point> points;
    int size;
    vector<vector<int>> adjPoints;
    vector<vector<int>> cluster;
    vector<int> clustering;
    vector<bool> isCore;
    int clusterIdx;

    DBSCAN( double eps, int minPts, vector<point> points) {
        this->eps = eps;
        this->minPts = minPts;
        this->points = points;
        this->size = (int)points.size();
        adjPoints.resize(size);
        this->clusterIdx=0;
    }

    vector<vector<int>> run () {
        vector<int> zuordnung(size, NOT_CLASSIFIED);
        vector<bool> core(size, false);
        this->isCore = core;
        this->clustering = zuordnung;

        // determine point classification
        determine_Neighborhood();

        // iterrate over all points and add them to a cluster or noise
        for(int i=0; i < size; i++) {
            if(clustering[i] != NOT_CLASSIFIED) {
                continue;
            }

            if(isCore[i]) {
                addPoints2cluster(i, ++clusterIdx);
            } else {
                clustering[i] = NOISE; // this point could be a borderpoint but this needs to be proven
            }
        }

        // generate output clustering list
        cluster.resize(clusterIdx+1);
        for(int i=0;i<size;i++) {
            if(clustering[i] != NOISE) {
                cluster[clustering[i]].push_back(i);
            }
        }

        return cluster;
    }

    /**
     * Adds the Point to the given cluster and adds connected Points via DFS.
     */
    void addPoints2cluster (int current, int cluster) {
        clustering[current] = cluster;
        if(!isCore[current]) {
            return;
        }

        for(auto&next:adjPoints[current]) {
            if(clustering[next] != NOT_CLASSIFIED) {
                continue;
            }
            addPoints2cluster(next, cluster);
        }
    }

    /**
     * Determines the epsilon-neigborhood for each point
     */
    void determine_Neighborhood() {
        for(int i=0;i<size;i++) {
            for(int j=0;j<size;j++) {
                if(i==j) {
                    continue;// the point itself
                }
                if(getDis(points[i], points[j]) <= eps) {
                    adjPoints[i].push_back(j);
                }
            }
        }
    }

    vector<vector<int>> getCluster() {
        return cluster;
    }

    double getDis(point a, point b) {
        return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
    }
};