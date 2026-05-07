//
// Created by jakub on 13.03.2026.
//

#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <utility>

FileManager::FileManager(std::string inputFileName, std::string outputFileName):
    inputFileName(std::move(inputFileName)), outputFileName(std::move(outputFileName)) {
}

Graph* FileManager::loadGraph() {
    std::ifstream file(inputFileName);
    if (!file.is_open())
        throw std::invalid_argument("File could not be opened");

    std::string line;
    int size = 0;
    bool sectionFound = false;

    while (std::getline(file, line)) {
        if (line.find("DIMENSION") != std::string::npos) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                size = std::stoi(line.substr(pos + 1));
            }
        }
        if (line.find("EDGE_WEIGHT_SECTION") != std::string::npos) {
            sectionFound = true;
            break;
        }
    }

    if (size <= 0 || !sectionFound) {
        throw std::runtime_error("Wrong ATSP format.");
    }

    Graph* graph = new Graph(size);
    for (int row = 0; row < size; row++) {
        for (int col = 0; col < size; col++) {
            std::string weight;
            if (!(file >> weight)) {
                delete graph;
                throw std::runtime_error("Wrong file format.");
            }
            graph->at(row, col) = std::stoi(weight);
        }
    }

    file.close();
    return graph;
}

Graph* FileManager::loadGraphOld() {
    std::ifstream file(inputFileName);
    if (!file.is_open())
        throw std::invalid_argument("File could not be opened");

    std::string in;

    file >> in; // Read graph size;
    int size = std::stoi(in);
    Graph* graph = new Graph(size);

    int row = 0, col = 0;
    // Fill graph
    while (file >> in) {
        if (col >= size) {
            row++;
            col = 0;
        }

        graph->at(row, col) = std::stoi(in);
        col++;
    }
    file.close();
    return graph;
}

void FileManager::saveData(int size, bool completed, int cost, int iteration, double tempStart, double tempEnd, double coolingRate,
        int epochIterations, int timeMs, InitSolution initialSolution, CoolingType cooling) {
    std::ofstream file(outputFileName, std::ios_base::app);
    if (!file.is_open())
        throw std::invalid_argument("File could not be opened");

    // Delete all characters before last /
    std::string fileName = inputFileName;
    size_t pos;
    while ((pos = fileName.find('/')) != std::string::npos) {
        fileName.erase(0, pos + 1);
    }

    // GraphName; Size; Iteration; Completed; Cost; Time; TempStart; TempEnd; CoolingRate; EpochIterations; InitialSolution; CoolingType
    file << fileName << ";" << size << ";" << iteration << ";" << completed << ";" << cost
        << ";" << timeMs << ";" << tempStart << ";" << tempEnd << ";" << coolingRate
        << ";" << epochIterations << ";" << initialSolutionToString(initialSolution)<<";"<<coolingTypeToString(cooling) << "\n";

    file.close();
}

std::string FileManager::initialSolutionToString(InitSolution initialSolution) {
    switch (initialSolution) {
        case InitSolution::RANDOM: return "RANDOM";
        case InitSolution::NN: return "NN";
        case InitSolution::RNN: return "RNN";
        default: return "ERROR";
    }
}

std::string FileManager::coolingTypeToString(CoolingType cooling) {
    switch (cooling) {
        case CoolingType::EXPONENTIAL: return "EXPONENTIAL";
        case CoolingType::LINEAR: return "LINEAR";
        default: return "ERROR";
    }
}