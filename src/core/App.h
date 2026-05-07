//
// Created by jakub on 15.03.2026.
//

#ifndef APP_H
#define APP_H
#include <string>

#include "FileManager.h"
#include "Graph.h"
#include "../include/answers.h"
#include "../utils/parameters.h"


class App {
public:
    App();
    void run();

private:
    bool checkSize(std::string arg, int& value, bool isFileInput);
    bool checkInt(std::string arg, int& value);
    bool checkBool(std::string arg, bool& value);
    bool checkInitialSolution(std::string arg, InitSolution& initialSolution);
    bool checkCoolingScheme(std::string arg, CoolingType& coolingScheme);
    bool checkDouble(const std::string& arg, double& value);
};



#endif //APP_H
