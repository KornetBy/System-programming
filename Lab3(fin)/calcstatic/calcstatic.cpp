// calcstatic.cpp : Определяет функции для статической библиотеки.
//

#include "pch.h"
#include "framework.h"

using namespace std;

extern "C" int Sum(int i, int j) {
    return i + j;
}

extern "C" int Sub(int i, int j) {
    return i - j;
}

extern "C" int Mult(int i, int j) {
    return i * j;
}

extern "C" int Div(int i, int j) {
    if (!j)
        return 0; // Обработка деления на ноль
    return i / j;
}

extern "C" int Part(int i, int j) {
    if (!j)
        return 0; // Обработка деления на ноль
    return i % j;
}
