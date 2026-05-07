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

void FileManager::saveData(Algorithm algorithm, int size, bool completed, int time, bool isFileInput, int cost, int iteration, InitialSolution initialSolution) {
    std::ofstream file(outputFileName, std::ios_base::app);
    if (!file.is_open())
        throw std::invalid_argument("File could not be opened");

    // Iteration; Algorithm; Size; Cost; Completed; Time; Input Type; Initial Solution
    file << iteration << ";" << algorithmToString(algorithm) << ";" << size << ";" << cost << ";" << completed
        << ";" << time << ";" << inputToString(isFileInput) << ";" << initialSolutionToString(initialSolution) << "\n";
    file.close();
}

std::string FileManager::inputToString(bool isFileInput) {
    if (isFileInput)
        return "FILE";
    return "RANDOM";
}

std::string FileManager::algorithmToString(Algorithm algorithm) {
    switch (algorithm) {
        case Algorithm::BF: return "BF";
        case Algorithm::DEPTH: return "DFS";
        case Algorithm::BREADTH: return "BREADTH";
        case Algorithm::BEST: return "BEST";
        default: return "ERROR";
    }
}

std::string FileManager::initialSolutionToString(InitialSolution initialSolution) {
    switch (initialSolution) {
        case InitialSolution::NN: return "NN";
        case InitialSolution::RNN: return "RNN";
        default: return "ERROR";
    }
}

