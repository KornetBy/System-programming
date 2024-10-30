#include <stdio.h>
#include<Windows.h>

#define a 10
#define b 3

void Addition(int , int );
void Subtraction(int, int);
void PrintResult();


int main() {

    Addition(a, b);
    Subtraction(a, b);
    PrintResult();
    
    int c = 0;
    scanf("%d", c);
    return 0;
}
