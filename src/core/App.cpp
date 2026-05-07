//
// Created by jakub on 15.03.2026.
//

#include "App.h"

#include <iostream>
#include <utility>

#include "FileManager.h"
#include "../algorithms/BestSolver.h"
#include "../algorithms/BFSolver.h"
#include "../algorithms/BreadthSolver.h"
#include "../algorithms/DepthSolver.h"
#include "../utils/Timer.h"


App::App() {
}

void App::run() {
    while (true) {
        // Arguments:
        std::cout<<"\nDane wejsciowe algorytmu: <algorytm> <rozmiar> <iteracje> <czas_maks> <wyswietl_graf> <rozw_poczatkowe> [sciezka_grafu]\n"
                   "\t<algorytm>: 0 - DFS, 1 - BREADTH, 2 - BEST, 3 - ALL, 4 - BF, 5 - ALL BEZ BF, 6 - DFS i BEST\n"
                   "\t<rozmiar>: Rozmiar problemu N lub \"-\" jezeli podana sciezka grafu\n"
                   "\t<iteracje>: Liczba iteracji >0\n"
                   "\t<czas_maks>: [ms]\n"
                   "\t<wyswietl_graf>: 1 - Tak, 0 - Nie\n"
                   "\t<rozw_poczatkowe>: 0 - NN, 1 - RNN\n"
                   "\t[sciezka_grafu]: Relatywna do lokalizacji pliku wykonywalnego\n"
                   "\nWejscie: ";
        std::string input;
        std::getline(std::cin, input);

        // Program arguments
        bool isFileInput = false;
        Algorithm algorithm;
        int size, iterations, maxTime;
        bool printGraph;
        InitialSolution initialSolution;
        std::string inputPath="";

        // Parse arguments to string
        std::string args[7] = {};
        std::stringstream ss(input);
        int i = 0;
        while (ss >> args[i]) {
            i++;
        }
        if (i > 7 || i < 6) {
            std::cout<<"Niepoprawna liczba argumentow\n";
            continue;
        }

        if (i==7)   // argument [sciezka_grafu] was given
            isFileInput = true;

        // Check arguments
        bool check = true;
        check *= checkAlgorithm(args[0], algorithm);
        check *= checkSize(args[1], size, isFileInput);
        check *= checkInt(args[2], iterations);
        check *= checkInt(args[3], maxTime);
        check *= checkBool(args[4], printGraph);
        check *= checkInitialSolution(args[5], initialSolution);
        if (isFileInput)
            inputPath = args[6];
        if (!check)
            continue;

        // Get graph
        FileManager fileManager(inputPath, "wyniki_jf.csv");
        Graph* graph = nullptr;
        if (isFileInput) {
            graph = fileManager.loadGraph();
        } else {
            graph = new Graph(size);
        }

        for (int i = 0; i < iterations; i++) {
            // Setup graph
            if (!isFileInput)
                graph->generate();
            std::cout<<"====================================================\n"
                       "Iteracja "<<i+1<<" z "<<iterations<<"\n"
                       "====================================================\n";
            if (printGraph)
                graph->print();

            // Run algorithms
            if (algorithm == Algorithm::DEPTH || algorithm == Algorithm::ALL || algorithm == Algorithm::ALL_WITHOUT_BF || algorithm == Algorithm::DFS_AND_BEST)
                runAlgorithm(Algorithm::DEPTH, graph, fileManager, isFileInput, i, maxTime, initialSolution);
            if (algorithm == Algorithm::BREADTH || algorithm == Algorithm::ALL || algorithm == Algorithm::ALL_WITHOUT_BF)
                runAlgorithm(Algorithm::BREADTH, graph, fileManager, isFileInput, i, maxTime, initialSolution);
            if (algorithm == Algorithm::BEST || algorithm == Algorithm::ALL || algorithm == Algorithm::ALL_WITHOUT_BF || algorithm == Algorithm::DFS_AND_BEST)
                runAlgorithm(Algorithm::BEST, graph, fileManager, isFileInput, i, maxTime, initialSolution);
            if (algorithm == Algorithm::BF || algorithm == Algorithm::ALL)
                runAlgorithm(Algorithm::BF, graph, fileManager, isFileInput, i, maxTime, initialSolution);
        }

        delete graph;
    }
}

void App::runAlgorithm(Algorithm algorithm, Graph* graph, FileManager& fileManager, bool isFileInput, int iteration, int maxTime, InitialSolution initialSolution) {
    ISolver *solver = nullptr;
    switch (algorithm) {
        case Algorithm::BF: solver = new BFSolver(); break;
        case Algorithm::DEPTH: solver = new DepthSolver(maxTime, initialSolution); break;
        case Algorithm::BREADTH: solver = new BreadthSolver(maxTime, initialSolution); break;
        case Algorithm::BEST: solver = new BestSolver(maxTime, initialSolution); break;
        default: {
            std::cout<<"Niepoprawny wybor algorytmu - ustawiono Brute Force.\n";
            solver = new BFSolver();
        }
    }

    solver->solve(graph);
    solver->print();
    // Sava data to file
    fileManager.saveData(algorithm, graph->getSize(), solver->getCompleted(), solver->getTimeMs(), isFileInput, solver->getCost(), iteration, initialSolution);

    delete solver;
}

bool App::checkAlgorithm(const std::string& arg, Algorithm& algorithm) {
    // 0 - DFS, 1 - BREADTH, 2 - BEST, 3 - ALL, 4 - BF, 5 ALL WITHOUT BF
    bool correct = true;
    if (arg == "0")
        algorithm = Algorithm::DEPTH;
    else if (arg == "1")
        algorithm = Algorithm::BREADTH;
    else if (arg == "2")
        algorithm = Algorithm::BEST;
    else if (arg == "3")
        algorithm = Algorithm::ALL;
    else if (arg == "4")
        algorithm = Algorithm::BF;
    else if (arg == "5")
        algorithm = Algorithm::ALL_WITHOUT_BF;
    else if (arg == "6")
        algorithm = Algorithm::DFS_AND_BEST;
    else {
        correct = false;
        std::cout<<"Niepoprawny algorytm\n";
    }
    return correct;
}

bool App::checkSize(std::string arg, int& value, bool isFileInput) {
    if (isFileInput)
        return true;
    value = std::stoi(arg);
    if (value<2)
        std::cout<<"Niepoprawny rozmiar - musi byc wieksza od 1\n";
    return value>1;
}

bool App::checkInt(std::string arg, int& value) {
    value = std::stoi(arg);
    if (value<1)
        std::cout<<"Niepoprawna liczba - musi byc wieksza od 0\n";
    return value>0;
}

bool App::checkBool(std::string arg, bool& value) {
    int intValue = std::stoi(arg);
    if (intValue == 0 || intValue == 1) {
        value = intValue;
        return true;
    }

    std::cout<<"Niepoprawna wartosc - musi byc 0 lub 1\n";
    return false;
}

bool App::checkInitialSolution(std::string arg, InitialSolution& initialSolution) {
    if (arg == "0")
        initialSolution = InitialSolution::NN;
    else if (arg == "1")
        initialSolution = InitialSolution::RNN;
    else {
        std::cout<<"Niepoprawne rozwiazanie poczatkowe\n";
        return false;
    }
    return true;
}
