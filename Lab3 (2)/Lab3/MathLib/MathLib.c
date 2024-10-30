// MathLib.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"
#include <stdio.h>

// TODO: Это пример библиотечной функции.
void fnMathLib()
{
}

int addition = 0;

__declspec(dllexport) void Addition(int a, int b) {
    addition = a + b;
}

int subtraction = 0;

__declspec(dllexport) void Subtraction(int a, int b) {
    subtraction = a - b;
}

__declspec(dllexport) void PrintResult() {
    if (0 != addition) {
        printf("addition = %d \n", addition);
    }
    if (0 != subtraction) {
        printf("subtraction = %d \n", subtraction);
    }
}

