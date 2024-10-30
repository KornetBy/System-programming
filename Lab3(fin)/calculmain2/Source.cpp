#include <iostream>

using namespace std;

extern "C" int Sum(int i, int j);
extern "C" int Sub(int i, int j);
extern "C" int Mult(int i, int j);
extern "C" int Div(int i, int j);
extern "C" int Part(int i, int j);

int main(){


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

    return 0;
}


