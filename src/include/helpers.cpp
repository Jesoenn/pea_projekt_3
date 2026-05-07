//
// Created by jakub on 13.03.2026.
//

#include "helpers.h"

void swap(int* arr, int i, int j) {
    int tmp = arr[i];
    arr[i] = arr[j];
    arr[j] = tmp;
}

void moveVertexToLast(int* arr, int v, int size) {
    for (int i = 0; i<size; i++) {
        if (arr[i] == v) {
            swap(arr, i, size-1);
            return;
        }
    }
}

long long factorial(int n) {
    long long res = 1;
    for (int i = 2; i <= n; i++) {
        res *= i;
    }
    return res;
}

void copy(int* origArr, int* destArr, int size) {
    for (int i = 0; i<size; i++) {
        destArr[i] = origArr[i];
    }
}

void copyMatrix(int** origMat, int** destMat, int size) {
    for (int i = 0; i<size; i++) {
        for (int j = 0; j<size; j++) {
            destMat[i][j] = origMat[i][j];
        }
    }
}
