#include <corecrt_math_defines.h>
#include <iostream>
#include <cmath>
#include <Windows.h>
using namespace std;

bool areEqual(double a, double b, double epsilon = 1e-6) {
    // fabs находит абсолютное значение для чисел с плавающей точкой
    return fabs(a - b) < epsilon;
}

// Функция для вычислений z1 и z2
void calculate(double alpha_deg) {
    // Преобразование угла в градусах в радианы
    double alpha_rad = alpha_deg * M_PI / 180.0;

    // Вычисление z1
    double z1 = (sin(4 * alpha_rad) * cos(2 * alpha_rad)) / ((1 + cos(4 * alpha_rad)) * (1 + cos(2 * alpha_rad)));

    // Вычисление z2
    double z2 = 1 / tan((3 * M_PI / 2) - alpha_rad);
    char str[100];
    if (areEqual(z1, z2))
    {
        CharToOem(L"Числа равны.", str);
    }
    else {
        CharToOem(L"Числа не равны.", str);
    }
    // Вывод результатов
	// endl - символ перехода на новую строку
    cout << "z1 = " << z1 << endl;
    cout << "z2 = " << z2 << endl;
    cout << str << endl;
}

int main() {
    double alpha_deg;  // Переменная для угла α в градусах
    char str[100];

    // Вывод запроса пользователю
    // L перед строкой указывает на то, что строка является широкой строкой(wide string), 
    // то есть строкой в формате UTF - 16 или Unicode, а не обычной строкой ASCII.
    CharToOem(L"Введите значение альфа в градусах: ", str);
    cout << str; // << - вывод данных из потока

    // Ввод угла α в градусах
	cin >> alpha_deg; // >> - ввод данных в поток
	// cin, cout - консольные потоки ввода и вывода

    // Вызов функции для вычислений
    calculate(alpha_deg);

    return 0;
}
