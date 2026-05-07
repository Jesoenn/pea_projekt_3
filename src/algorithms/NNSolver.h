//
// Created by jakub on 13.03.2026.
//

#ifndef NNSOLVER_H
#define NNSOLVER_H

#include "../core/Graph.h"

class NNSolver {
public:
    NNSolver();
    ~NNSolver();
    void solve(Graph& graph);
    void print();

    double getPermutations();
    int* getAns();
    int getCost();

private:
    int* ans;
    int size, cost;
};



#endif //NNSOLVER_H
