#include <corecrt_math_defines.h>
#include <iostream>
#include <cmath>
#include <Windows.h>
using namespace std;

// Функция для вычислений z1 и z2
void calculate(double alpha_deg) {
    // Преобразование угла в градусах в радианы
    double alpha_rad = alpha_deg * M_PI / 180.0;

    // Вычисление z1
    double z1 = (sin(4 * alpha_rad) * cos(2 * alpha_rad)) / ((1 + cos(4 * alpha_rad)) * (1 + cos(2 * alpha_rad)));

    // Вычисление z2
    double z2 = 1 / tan((3 * M_PI / 2) - alpha_rad);
    
    // Вывод результатов
    cout << "z1 = " << z1 << endl;
    cout << "z2 = " << z2 << endl;
}

int main() {
    double alpha_deg;  // Переменная для угла α в градусах
    char str[100];

    // Вывод запроса пользователю
    CharToOem(L"Введите значение альфа в градусах: ", str);
    cout << str;

    // Ввод угла α в градусах
    cin >> alpha_deg;

    // Вызов функции для вычислений
    calculate(alpha_deg);

    return 0;
}
