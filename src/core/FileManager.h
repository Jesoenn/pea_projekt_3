//
// Created by jakub on 13.03.2026.
//

#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <string>

#include "Graph.h"
#include "../include/algorithms.h"
#include "../include/inital_solutions.h"
#include "../include/inputs.h"


class FileManager {
public:
    FileManager(std::string inputFileName, std::string outputFileName);

    Graph* loadGraph();
    Graph* loadGraphOld();
    void saveData(Algorithm algorithm, int size, bool completed, int time, bool isFileInput, int cost, int iteration, InitialSolution initialSolution);

private:
    std::string inputFileName, outputFileName;

    static std::string inputToString(bool isFileInput);
    static std::string algorithmToString(Algorithm algorithm);
    static std::string initialSolutionToString(InitialSolution initialSolution);
};



#endif //FILEMANAGER_H
