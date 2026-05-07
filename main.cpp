#include <iostream>

// #include "src/core/App.h"
#include "src/algorithms/SimulatedAnnealing.h"
#include "src/core/FileManager.h"
#include "src/utils/Timer.h"

int main() {
    Timer timer;
    timer.start();
    FileManager fileManager("../data/ATSP/ftv170.atsp", "results.csv");
    Graph* graph = fileManager.loadGraph();
    // FileManager fileManager("../data/tsp_6_1.txt", "results.csv");
    // Graph* graph = fileManager.loadGraphOld();
    // graph->print();

    SimulatedAnnealing sa;
    // sa.setCoolingType(CoolingType::SLOW);
    sa.solve(graph);
    sa.print();

    return 0;
    // App app;
    // app.run();
    //
    // return 0;
}
