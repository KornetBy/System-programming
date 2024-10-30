#include <Windows.h>
#include <iostream>
#define NAME_LIB L"Dll2.dll"
#define SUM "Sum"
#define SUB "Sub"
#define MULT "Mult"
#define DIV "Div"
#define PART "Part"

using namespace std;

int main()
{
    HMODULE hDLL = LoadLibrary(NAME_LIB);
    if (!hDLL)
        return 1;

    // Используйте явное приведение типа для указателей на функции
    int (*Sum)(int, int) = (int(*)(int, int))GetProcAddress(hDLL, SUM);
    int (*Sub)(int, int) = (int(*)(int, int))GetProcAddress(hDLL, SUB);
    int (*Mult)(int, int) = (int(*)(int, int))GetProcAddress(hDLL, MULT);
    int (*Div)(int, int) = (int(*)(int, int))GetProcAddress(hDLL, DIV);
    int (*Part)(int, int) = (int(*)(int, int))GetProcAddress(hDLL, PART);

    if (Sum && Sub && Mult && Div && Part)
    {
        int iValSum = Sum(1, 2);
        int iValSub = Sub(100, 63);
        int iValMult = Mult(2, 7);
        int iValDiv = Div(36, 4);
        int iValPart = Part(2018, 4);

        // Вывод результатов
        cout << "Sum: " << iValSum << endl;
        cout << "Sub: " << iValSub << endl;
        cout << "Mult: " << iValMult << endl;
        cout << "Div: " << iValDiv << endl;
        cout << "Part: " << iValPart << endl;
    }
    else
    {
        cout << "Failed to get function addresses." << endl;
    }

    FreeLibrary(hDLL);
    return 0;
}
