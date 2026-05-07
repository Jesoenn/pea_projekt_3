//
// Created by jakub on 13.03.2026.
//

#ifndef FILEMANAGER_H
#define FILEMANAGER_H
#include <string>

#include "Graph.h"
#include "../utils/parameters.h"


class FileManager {
public:
    FileManager(std::string inputFileName, std::string outputFileName);

    Graph* loadGraph();
    Graph* loadGraphOld();
    void saveData(int size, bool completed, int cost, int iteration, double tempStart, double tempEnd, double coolingRate,
        int epochIterations, int timeMs, InitSolution initialSolution, CoolingType cooling);

private:
    std::string inputFileName, outputFileName;
    static std::string initialSolutionToString(InitSolution initialSolution);
    static std::string coolingTypeToString(CoolingType cooling);
};



#endif //FILEMANAGER_H
