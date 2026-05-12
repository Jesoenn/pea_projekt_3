//
// Created by jakub on 06.05.2026.
//

#include "SimulatedAnnealing.h"
#include "../include/helpers.cpp"
#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include "RNNSolver.h"
#include "NNSolver.h"

SimulatedAnnealing::SimulatedAnnealing() {
    // Default parameters
    initialTemp = 10000.0;
    coolingRate = 0.99;
    epochIterations = 1000;
    maxTimeMs = 900000;
    coolingScheme = CoolingType::EXPONENTIAL;
    initType = InitSolution::RANDOM;
    bestCost = INT_MAX;
    executionTimeMs = 0;
    bestPath = nullptr;
    endingTemp = 0.0001;
    countEpochs = 0;
    size = -1;
    gen = std::mt19937(rd());
}

SimulatedAnnealing::~SimulatedAnnealing() {
    delete[] bestPath;
}

void SimulatedAnnealing::setInitialTemp(double temp) {
    initialTemp = temp;
}
void SimulatedAnnealing::setEndingTemp(double temp) {
    endingTemp = temp;
}
void SimulatedAnnealing::setCoolingRate(double rate) {
    coolingRate = rate;
}
void SimulatedAnnealing::setEpochIterations(int length) {
    epochIterations = length;
}
void SimulatedAnnealing::setCoolingType(CoolingType type) {
    coolingScheme = type;
}
void SimulatedAnnealing::setInitSolutionType(InitSolution type) {
    initType = type;
}
void SimulatedAnnealing::setMaxTimeMs(int ms) {
    maxTimeMs = ms;
}

double SimulatedAnnealing::getInitialTemp() const {
    return initialTemp;
}

double SimulatedAnnealing::getEndingTemp() const {
    return endingTemp;
}

double SimulatedAnnealing::getCoolingRate() const {
    return coolingRate;
}

int SimulatedAnnealing::getEpochIterations() const {
    return epochIterations;
}

CoolingType SimulatedAnnealing::getCoolingType() const {
    return coolingScheme;
}

InitSolution SimulatedAnnealing::getInitSolutionType() const {
    return initType;
}

int SimulatedAnnealing::getTime() const {
    return executionTimeMs;
}

int* SimulatedAnnealing::getAns() const {
    return bestPath;
}

int SimulatedAnnealing::getCost() const {
    return bestCost;
}

int SimulatedAnnealing::getTimeMs() const {
    return timer.result();
}

bool SimulatedAnnealing::getCompleted() const {
    return timer.result() < maxTimeMs;
}

void SimulatedAnnealing::moveVertex(int* path, int from, int to) {
    int temp = path[from];
    if (from < to) {
        for (int i = from; i < to; i++) {
            path[i] = path[i + 1];
        }
    } else {
        for (int i = from; i > to; i--) {
            path[i] = path[i - 1];
        }
    }
    path[to] = temp;
}

// Generate initial solution based on selected type
int* SimulatedAnnealing::generateInitialSolution(int size, Graph* graph) {
    int* path = new int[size];
    if (initType == InitSolution::RANDOM) {
        for (int i = 0; i < size; i++) {
            path[i] = i;
        }
        std::shuffle(path + 1, path + size, gen);
    }
    else if (initType == InitSolution::NN) {
        NNSolver nnSolver;
        nnSolver.solve(*graph);
        copy(nnSolver.getAns(), path, size);
    }
    else if (initType == InitSolution::RNN) {
        RNNSolver rnnSolver;
        rnnSolver.solve(*graph);
        copy(rnnSolver.getAns(), path, size);
    }
    return path;
}

// Calculate cost of given path
int SimulatedAnnealing::calculateCost(const int* path, int size, Graph* graph) {
    int cost = 0;
    for (int i = 0; i < size - 1; i++) {
        cost += graph->get(path[i], path[i + 1]);
    }
    cost += graph->get(path[size - 1], path[0]);
    return cost;
}

// https://algorithmafternoon.com/books/simulated_annealing/chapter02/
double SimulatedAnnealing::decreaseTemperature(double currentTemp) {
    if (coolingScheme == CoolingType::EXPONENTIAL)
        return currentTemp * coolingRate;
    if (coolingScheme == CoolingType::LINEAR)
        return currentTemp - coolingRate;
    if (coolingScheme == CoolingType::LOGARITHMIC)
        return initialTemp / std::log(1 + countEpochs);
    return currentTemp * 0.99;
}


int SimulatedAnnealing::solve(Graph* graph) {
    // Delete previous answer
    delete[] bestPath;
    size = graph->getSize();
    countEpochs = 0;

    std::uniform_int_distribution<int> dist(1, size - 1);
    std::uniform_real_distribution<> distOne(0.0, 1.0);

    // Start timer
    timer.start();
    // Generate initial solution and copy to best found path
    int* currentPath = generateInitialSolution(size, graph);
    int currentCost = calculateCost(currentPath, size, graph);

    bestPath = new int[size];
    copy(currentPath, bestPath, size);
    bestCost = currentCost;
    initialCost = currentCost;

    // Current parameters
    double temp = initialTemp;

    // While current temperature is above the ending temperature and time limit is not achieved
    while (temp > endingTemp) {
        // Check every epoch if time limit achieved
        if (timer.result() > maxTimeMs) {
            std::cout << "Time limit achieved" << std::endl;
            break;
        }

        // Epochs
        countEpochs++;
        for (int i = 0; i < epochIterations; i++) {
            // Generate random swap
            int v1 = dist(gen);
            int v2 = dist(gen);
            while (v1 == v2) {
                v2 = dist(gen);
            }

            // Insert vertex v1 in place of v2
            moveVertex(currentPath, v1, v2);

            int newCost = calculateCost(currentPath, size, graph);
            int deltaCost = newCost - currentCost;

            // If cost is lower (delta <= 0) or probability tells to swap worse solution, accept it
            if (deltaCost <= 0 || std::exp(-deltaCost / temp) > distOne(gen)) {
                currentCost = newCost;

                // If the best solution yet found, copy it to bestPath
                if (currentCost < bestCost) {
                    copy(currentPath, bestPath, size);
                    bestCost = currentCost;
                }
            } else {
                // Reverse insert if not solution is not accepted
                moveVertex(currentPath, v2, v1);
            }
        }

        temp = decreaseTemperature(temp);
    }

    timer.stop();
    executionTimeMs = timer.result();

    delete[] currentPath;
    return bestCost;
}

void SimulatedAnnealing::print() {
    if (!bestPath) {
        std::cout<<"No solution found."<<std::endl;
        return;
    }

    std::cout<<"Best path: ";
    for (int i = 0; i<size; i++) {
        std::cout << bestPath[i] << " ";
    }

    std::cout<<"\n\tInitial solution cost: "<<initialCost<<std::endl;
    std::cout << "\tCost = "<<bestCost<<std::endl;
    std::cout<<"\tEpochs = "<<countEpochs<<std::endl;
    std::cout<<"\tTime = "<<executionTimeMs<<" ms"<<std::endl;
}
