//
// Created by jakub on 15.03.2026.
//

#ifndef RNNSOLVER_H
#define RNNSOLVER_H
#include "../core/Graph.h"

class RNNSolver {
public:
    RNNSolver();
    ~RNNSolver();
    void solve(Graph& graph);
    void recursiveSolve(Graph& graph, int visited, int currCost,
                        int* currPath, int* unvisitedArr, int& bestCost,
                        int* bestPath);
    void print();

    double getPermutations();
    int* getAns();
    int getCost();

private:
    int* ans;
    int size, cost;
};



#endif //RNNSOLVER_H
