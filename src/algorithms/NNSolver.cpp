//
// Created by jakub on 13.03.2026.
//

#include "NNSolver.h"
#include <iostream>
#include "../include/helpers.h"

NNSolver::NNSolver(): ans(nullptr), size(0), cost(-1) {

}

NNSolver::~NNSolver() {
    delete[] ans;
}

void NNSolver::solve(Graph& graph) {
    // Delete previous answer
    delete[] ans;
    size = graph.getSize();

    // Array of unvisited vertices from 0 to visited-1
    int visited = 0;
    int* unvisitedArr = new int[size];
    for (int i = 0; i<size; i++) {
        unvisitedArr[i] = i;
    }

    cost = 0;
    int* currPath = new int[size];

    // Choose starting vertex
    int swapIndex = 0;
    currPath[0] = 0;
    // Swap with last unvisited
    swap(unvisitedArr, swapIndex, size-1);
    visited++;

    // NN Algorithm
    for (int i = 1; i<size; i++) {
        int minEdgeCost = INT_MAX;
        // Find edge with the least cost from prev vertex
        for (int j = 0; j<size-visited; j++) {
            int edgeCost = graph.get(currPath[i-1], unvisitedArr[j]);
            if (edgeCost < minEdgeCost) {
                minEdgeCost = edgeCost;
                swapIndex = j;
            }
        }
        // Assign the least costly neighbour
        currPath[i] = unvisitedArr[swapIndex];
        cost += minEdgeCost;
        // Swap visited with last unvisited
        swap(unvisitedArr, swapIndex, size-1-visited);
        visited++;
    }
    cost += graph.get(currPath[size-1], currPath[0]); // From last element to first

    delete[] unvisitedArr;
    ans = currPath;
}

void NNSolver::print() {
    if (!ans) {
        std::cout<<"No solution found."<<std::endl;
        return;
    }

    std::cout<<"Best NEAREST NEIGHBOUR ALGORITHM path: ";
    for (int i = 0; i<size; i++) {
        std::cout << ans[i] << " ";
    }

    std::cout << "\n\tCost = "<<cost<<std::endl;
}

double NNSolver::getPermutations() {
    return -1.;
}

int* NNSolver::getAns() {
    return ans;
}

int NNSolver::getCost() {
    return cost;
}
