//
// Created by jakub on 15.03.2026.
//

#include "App.h"

#include <iostream>
#include "FileManager.h"
#include "../algorithms/SimulatedAnnealing.h"
#include "../utils/Timer.h"


App::App() {
}

void App::run() {
    while (true) {
//        3 10000 0.0001 0.99 0 15000 1000 0 0 ../data/ATSP/ftv47.atsp
        // Arguments:
        std::cout<<"\nDane wejsciowe algorytmu: <iteracje> <temp_pocz> <temp_konc> <ochladzanie> <typ_ochladzania> <czas_maks> <iteracje_epoki> <rozw_poczatkowe> <wyswietl_graf> <sciezka_grafu>\n"
                   "\t<iteracje>: Liczba iteracji >0\n"
                   "\t<temp_pocz>: Temperatura poczatkowa [double]\n"
                   "\t<temp_konc>: Temperatura koncowa [double]\n"
                   "\t<ochladzanie>: Wspolczynnik ochladzania (0,1)\n"
                   "\t<typ_ochladzania>: 0 - Exponential, 1 - Linear\n"
                   "\t<czas_maks>: [ms]\n"
                   "\t<iteracje_epoki>: Liczba iteracji w jednej epoce >0\n"
                   "\t<rozw_poczatkowe>: 0 - RANDOM, 1 - NN, 2 - RNN\n"
                   "\t<wyswietl_graf>: 1 - Tak, 0 - Nie\n"
                   "\t<sciezka_grafu>: Relatywna do lokalizacji pliku wykonywalnego\n"
                   "\nWejscie: ";

        std::string input;
        std::getline(std::cin, input);

        // Program arguments
        double tempStart, tempEnd, coolingRate;
        int iterations, maxTime, epochIterations;
        bool printGraph;
        InitSolution initialSolution;
        CoolingType coolingType;
        std::string inputPath="";

        // Parse arguments to string
        std::string args[10] = {};
        std::stringstream ss(input);
        int i = 0;
        while (ss >> args[i]) {
            i++;
        }
        if (i != 10) {
            std::cout << "Niepoprawna liczba argumentow\n";
            continue;
        }

        // Check arguments
        bool check = true;
        // <iteracje> <temp_pocz> <temp_konc> <ochladzanie> <typ_ochladzania> <czas_maks> <iteracje_epoki> <rozw_poczatkowe> <wyswietl_graf> <sciezka_grafu>
        // 3 10000 0.0001 0.99 0 15000 1000 0 0 ../data/ATSP/ftv47.atsp

        check *= checkInt(args[0], iterations);
        check *= checkDouble(args[1], tempStart);
        check *= checkDouble(args[2], tempEnd);
        check *= checkDouble(args[3], coolingRate);
        check *= checkCoolingScheme(args[4], coolingType);
        check *= checkInt(args[5], maxTime);
        check *= checkInt(args[6], epochIterations);
        check *= checkInitialSolution(args[7], initialSolution);
        check *= checkBool(args[8], printGraph);
        inputPath = args[9];
        if (!check)
            continue;

        // Get graph
        FileManager fileManager(inputPath, "wyniki_jf.csv");
        Graph* graph = fileManager.loadGraph();
        int graphSize = graph->getSize();

        for (int i = 0; i < iterations; i++) {
            std::cout<<"====================================================\n"
                       "Iteracja "<<i+1<<" z "<<iterations<<"\n"
                       "====================================================\n";
            if (printGraph)
                graph->print();

            SimulatedAnnealing* sa = new SimulatedAnnealing();
            sa->setInitialTemp(tempStart);
            sa->setEndingTemp(tempEnd);
            sa->setCoolingRate(coolingRate);
            sa->setCoolingType(coolingType);
            sa->setEpochIterations(epochIterations);
            sa->setInitSolutionType(initialSolution);
            sa->setMaxTimeMs(maxTime);

            sa->solve(graph);
            sa->print();
            fileManager.saveData(graphSize, sa->getCompleted(), sa->getCost(), i+1, sa->getInitialTemp(), sa->getEndingTemp(), sa->getCoolingRate(),
                sa->getEpochIterations(), sa->getTimeMs(), sa->getInitSolutionType(), sa->getCoolingType());
            delete sa;
        }
        delete graph;
    }
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

bool App::checkInitialSolution(std::string arg, InitSolution& initialSolution) {
    if (arg == "0")
        initialSolution = InitSolution::RANDOM;
    else if (arg == "1")
        initialSolution = InitSolution::NN;
    else if (arg == "2")
        initialSolution = InitSolution::RNN;
    else {
        std::cout<<"Niepoprawne rozwiazanie poczatkowe\n";
        return false;
    }
    return true;
}

bool App::checkCoolingScheme(std::string arg, CoolingType& coolingScheme) {
    if (arg == "0")
        coolingScheme = CoolingType::EXPONENTIAL;
    else if (arg == "1")
        coolingScheme = CoolingType::LINEAR;
    else {
        std::cout<<"Niepoprawny typ ochladzania\n";
        return false;
    }
    return true;
}

bool App::checkDouble(const std::string& arg, double& value) {
    try {
        value = std::stod(arg);
        return true;
    } catch (...) {
        std::cout << "Niepoprawna liczba zmiennoprzecinkowa\n";
        return false;
    }
}
