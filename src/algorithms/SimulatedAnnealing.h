//
// Created by jakub on 06.05.2026.
//

#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include <random>

#include "../core/Graph.h"

#include "../utils/parameters.h"
#include "../utils/Timer.h"

// 4.5 CoolingScheme
// 3.5 InitSolution

class SimulatedAnnealing {
public:
    SimulatedAnnealing();
    ~SimulatedAnnealing();

    int solve(Graph* graph);
    void print();

    // Setters
    void setInitialTemp(double temp);
    void setEndingTemp(double temp);
    void setCoolingRate(double rate);
    void setEpochIterations(int length);
    void setCoolingType(CoolingType type);
    void setInitSolutionType(InitSolution type);
    void setMaxTimeMs(int ms);

    // Getters
    double getInitialTemp() const;
    double getEndingTemp() const;
    double getCoolingRate() const;
    int getEpochIterations() const;
    CoolingType getCoolingType() const;
    InitSolution getInitSolutionType() const;
    int getTime() const;
    int* getAns() const;
    int getCost() const;
    int getTimeMs() const;
    bool getCompleted() const;

private:
    Timer timer;
    std::random_device rd;
    std::mt19937 gen;

    double initialTemp, endingTemp;
    double coolingRate;
    int epochIterations;
    int countEpochs;
    int maxTimeMs;
    CoolingType coolingScheme;
    InitSolution initType;

    int* bestPath;
    int bestCost;
    int size;
    int executionTimeMs;

    int* generateInitialSolution(int size, Graph* graph);
    double decreaseTemperature(double currentTemp);
    int calculateCost(const int* path, int size, Graph* graph);
};


#endif //SIMULATEDANNEALING_H
