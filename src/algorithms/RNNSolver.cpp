//
// Created by jakub on 15.03.2026.
//

#include "RNNSolver.h"

#include <iostream>
#include "../include/helpers.h"

RNNSolver::RNNSolver(): ans(nullptr), size(0), cost(-1) {
}

RNNSolver::~RNNSolver() {
    delete[] ans;
}

void RNNSolver::solve(Graph& graph) {
    // Delete previous answer
    delete[] ans;
    size = graph.getSize();

    // Initialize variables
    int currCost = 0, bestCost = INT_MAX;
    int* currPath = new int[size];
    int* bestPath = new int[size];
    int* unvisitedArr = new int[size];

    // Pick each vertex as starting point to run NN
    for (int start = 0; start<size; start++) {
        currCost = 0;
        // Array of unvisited vertices from 0 to visited-1
        int visited = 0;
        for (int i = 0; i<size; i++) {
            unvisitedArr[i] = i;
        }
        // Set starting vertex (unvisitedArr elements are sorted) and swap with last unvisited
        currPath[0] = start;
        swap(unvisitedArr, start, size-1);
        visited++;

        // Solve with NN algorithm
        recursiveSolve(graph, visited, currCost, currPath, unvisitedArr, bestCost, bestPath);
    }

    delete[] unvisitedArr;
    delete[] currPath;
    ans = bestPath;
    cost = bestCost;
}

// Recursive function to find all paths when cost is the same
void RNNSolver::recursiveSolve(Graph& graph, int visited, int currCost, int* currPath, int* unvisitedArr,
    int& bestCost, int* bestPath) {
    // If every vertex was visited
    if (visited == size) {
        currCost += graph.get(currPath[size-1], currPath[0]); // from last element to first
        if (currCost < bestCost) {
            bestCost = currCost;
            // Copy path
            for (int i = 0; i<size; i++) {
                bestPath[i] = currPath[i];
            }
            return;
        }
    }

    // Find edge with the least cost from prev vertex
    int minEdgeCost = INT_MAX;
    for (int j = 0; j<size-visited; j++) {
        int edgeCost = graph.get(currPath[visited-1], unvisitedArr[j]);
        if (edgeCost < minEdgeCost)
            minEdgeCost = edgeCost;
    }

    currCost += minEdgeCost;
    // Iterate though lowest cost edges
    for (int j = 0; j<size-visited; j++) {
        int edgeCost = graph.get(currPath[visited-1], unvisitedArr[j]);
        if (edgeCost == minEdgeCost) {
            currPath[visited] = unvisitedArr[j];
            // Swap visited with last unvisited
            swap(unvisitedArr, j, size-1-visited);
            // Solve next vertex (visited+1)
            recursiveSolve(graph, visited + 1, currCost, currPath, unvisitedArr, bestCost, bestPath);
            // Swap back for next iteration. Makes unvisitedArr unchanged
            swap(unvisitedArr, j, size-1-visited);
        }
    }
}

void RNNSolver::print() {
    if (!ans) {
        std::cout<<"No solution found."<<std::endl;
        return;
    }

    std::cout<<"Best REPETITIVE NEAREST NEIGHBOUR ALGORITHM path: ";
    for (int i = 0; i<size; i++) {
        std::cout << ans[i] << " ";
    }

    std::cout << "\n\tCost = "<<cost<<std::endl;
}

double RNNSolver::getPermutations() {
    return -1.;
}

int* RNNSolver::getAns() {
    return ans;
}

int RNNSolver::getCost() {
    return cost;
}