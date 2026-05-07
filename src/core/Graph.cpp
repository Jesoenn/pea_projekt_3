//
// Created by jakub on 2026-03-13.
//

#include <iostream>
#include "Graph.h"
#include <random>
#include <filesystem>
#include <fstream>

const int MAX_WEIGHT = 100;

Graph::Graph(int size) {
    this->size = size;
    // Initialize empty adjacency matrix
    adjMat = new int*[size];
    for (int i = 0; i < size; ++i) {
        adjMat[i] = new int[size];
        for (int j = 0; j < size; ++j) {
            adjMat[i][j] = 0;
        }
    }
}

Graph::~Graph() {
    for (int i = 0; i < size; ++i) {
        delete[] adjMat[i];
    }
    delete[] adjMat;
}

int &Graph::at(int from, int to) {
    // checkErr(from, to); // Verification kills performance: BF N=12 from 11.5s to 9.5s
    return adjMat[from][to];
}

void Graph::set(int from, int to, int weight) {
    // checkErr(from, to);
    adjMat[from][to] = weight;
}

int Graph::get(int from, int to) {
    // checkErr(from, to); // Verification kills performance: BF N=12 from 11.5s to 9.5s
    return adjMat[from][to];
}

int Graph::getSize() {
    return size;
}

int** Graph::getAdjMat() {
    return adjMat;
}

void Graph::generate() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distWeight(10, MAX_WEIGHT);

    // Fill graph with random weights
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            if (i != j) {
                adjMat[i][j] = distWeight(gen);
            } else {
                adjMat[i][j] = -1;
            }
        }
    }
    save();
}

void Graph::print() {
    std::cout<<"==============================="<<std::endl;
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            std::cout << adjMat[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout<<"==============================="<<std::endl;
}

void Graph::checkErr(int from, int to) const {
    if(from < 0 || from >= size || to < 0 || to >= size) {
        throw std::out_of_range("Index out of bounds");
    }
}

void Graph::save() {
    if(!adjMat)
        return;

    std::filesystem::path folder = "graphs_jf";
    if (!std::filesystem::exists(folder)) {
        if (!std::filesystem::create_directory(folder)) {
            std::cout<<"Nie mozna utworzyc katalogu: " << folder << std::endl;
            return;
        }
    }

    int fileIndex = 0;
    std::string filename;
    // Look for first available name
    do {
        filename = "./graphs_jf/graph_gen_" + std::to_string(fileIndex) + ".txt";
        fileIndex++;
    } while (std::filesystem::exists(filename));

    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cout << "Nie mozna utworzyc pliku: " << filename << std::endl;
        return;
    }

    // Graph size
    outFile << size << "\n";

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            outFile << adjMat[i][j];
            if (j < size - 1)
                outFile << " ";
        }
        outFile << "\n";
    }

    outFile.close();
}
