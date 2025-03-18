#include <iostream>
#include <limits>
#include <string>
#include <cmath>
using namespace std;


int CustomInput(int length) {
    string input;
    int value;
    int max = (static_cast<int>(pow(2, length))) - 1;

    while (true) {
        cout << "Введите число в диапазоне от 0 до " << max << ": ";
        getline(cin, input);
        bool valid = true;
        // Проверка, что каждый символ строки является цифрой
        for (char c : input) {
            if (c < '0' || c > '9') {
                valid = false;
                break;
            }
        }
        // Если введены только цифры
        if (valid) {
            value = stoi(input);  // Преобразуем строку в число
            if (value >= 0 && value <= max) {
                break;  // Если число в пределах диапазона, выходим из цикла
            }
            else {
                cout << "Число выходит за пределы диапазона. Попробуйте снова." << endl;
            }
        }
        else {
            cout << "Введены недопустимые символы. Пожалуйста, введите только цифры." << endl;
        }
    }

    return value;
}

int DataToNumber(int d, int a, int b, int number) {
    return number | d << 7 | a << 4 | b << 1;
}

void NumberToData(int* d, int* a, int* b, int number) {
    *d = (number >> 7) & 0x1; // Сдвигаем number вправо на 7 бит и применяем маску 0000 0000 0000 0001 (number остается без изменений)
    *a = (number >> 4) & 0x7; // Сдвигаем number вправо на 4 бита и применяем маску 0000 0000 0000 0111
    *b = (number >> 1) & 0x7; // Сдвигаем number вправо на 1 бит и применяем маску 0000 0000 0000 0111
}
