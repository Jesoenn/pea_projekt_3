cmake_minimum_required(VERSION 3.26.4)
project(pea_projekt_3)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")

add_executable(pea_projekt_3 main.cpp
        src/utils/Timer.h
        src/utils/Timer.cpp
        src/core/Graph.h
        src/core/Graph.cpp
        src/core/FileManager.cpp
        src/core/FileManager.h
        src/algorithms/SimulatedAnnealing.cpp
        src/algorithms/SimulatedAnnealing.h
        src/utils/parameters.h
        #        src/include/helpers.cpp
#        src/include/helpers.h
#        src/include/algorithms.h
#        src/core/App.cpp
#        src/core/App.h
#        src/include/answers.h
#        src/include/inputs.h
#        src/utils/Node.h
#        src/include/inital_solutions.h
#        src/algorithms/NNSolver.cpp
#        src/algorithms/NNSolver.h
#        src/algorithms/RNNSolver.cpp
#        src/algorithms/RNNSolver.h
)

#target_compile_options(pea_projekt_3 PRIVATE -O3)