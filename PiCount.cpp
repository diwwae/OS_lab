#include <windows.h>
#include <iostream>
#include <iomanip>
#include "cmath"
//
// Created by meteo on 16.11.2023.
//
extern "C" {

// Ряд Лейбница
__declspec(dllexport) float countPi_1realize(int k){
    float result_1 = 0.f;
    int tmp = 0;
    for (int iter = 1; iter <= k; iter += 2){
        result_1 += pow(-1, tmp) * (1.f / (float) iter);
        tmp++;
    }
    return result_1 * 4.f;
}

// Формула Валлиса
__declspec(dllexport) float countPi_2realize(int k){
    float result_2 = 1.f;
    for (int iter = 2; iter <= k; iter+=2){
        result_2 *= (float) pow(iter,2) / (float) ((iter--) * (iter++));
    }
    return result_2 * 2.f;
}

}