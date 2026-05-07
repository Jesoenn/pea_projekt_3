//
// Created by jakub on 21.03.2025.
//

#include "Timer.h"
#include <chrono>

Timer::Timer() {
    isRunning = false;
    duration = 0;
}

void Timer::start() {
    if ( !isRunning ) {
        duration = 0;
        startTime = std::chrono::high_resolution_clock::now();
        isRunning = true;
    }
}

void Timer::resume() {
    if (!isRunning) {
        startTime = std::chrono::high_resolution_clock::now();
        isRunning = true;
    }
}

int Timer::stop() {
    if (isRunning) {
        endTime = std::chrono::high_resolution_clock::now();
        duration += std::chrono::duration_cast<std::chrono::milliseconds>(endTime-startTime).count();
        isRunning = false;
        return 0;
    }
    return -1;
}

int Timer::result() const {
    if ( isRunning ) {
        const std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        return static_cast<int>(duration + std::chrono::duration_cast<std::chrono::milliseconds>(now-startTime).count());
    }
    return static_cast<int>(duration);
}

