//
// Created by jakub on 15.03.2026.
//

#ifndef APP_H
#define APP_H
#include <string>

#include "FileManager.h"
#include "Graph.h"
#include "../include/algorithms.h"
#include "../include/answers.h"
#include "../include/inputs.h"
#include "../include/ISolver.h"
#include "../include/inital_solutions.h"


class App {
public:
    App();
    void run();

private:
    bool checkAlgorithm(const std::string& arg, Algorithm& algorithm);
    bool checkSize(std::string arg, int& value, bool isFileInput);
    bool checkInt(std::string arg, int& value);
    bool checkBool(std::string arg, bool& value);
    bool checkInitialSolution(std::string arg, InitialSolution& initialSolution);

    void runAlgorithm(Algorithm algorithm, Graph* graph, FileManager& fileManager, bool isFileInput, int iteration, int maxTime, InitialSolution initialSolution);

};



#endif //APP_H
