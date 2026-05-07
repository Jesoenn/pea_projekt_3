//
// Created by jakub on 2026-03-13.
//

#ifndef PEA_PROJEKT1_GRAPH_H
#define PEA_PROJEKT1_GRAPH_H


class Graph {
public:
    Graph(int size);
    ~Graph();

    void print();
    void generate();

    int &at(int from, int to);  // Getter/Setter

    // Setters
    void set(int from, int to, int weight);

    // Getters
    int get(int from, int to);
    int getSize();
    int** getAdjMat();

private:
    int size;
    int** adjMat;

    void checkErr(int from, int to) const;
    void save();
};


#endif //PEA_PROJEKT1_GRAPH_H
