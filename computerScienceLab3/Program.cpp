// computerScienceLab3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <limits>
#include <string>
#include <cmath>
#include "Functions.h"
using namespace std;


int main()
{
    int number = 0xC600; // aka 1100011000000000 aka 11000110xxxxxxx0
    int d = CustomInput(1);
    int a = CustomInput(3);
    int b = CustomInput(3);
    cout << DataToNumber(d,a,b,number) << endl;
    int x = CustomInput(16);
    int dRef, aRef, bRef;
    NumberToData(&dRef, &aRef, &bRef, x);
    cout << "D,A,B = " << dRef << ", " << aRef << ", " << bRef << endl;
}
