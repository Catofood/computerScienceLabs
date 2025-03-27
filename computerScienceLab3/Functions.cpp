#include <iostream>
#include <cmath>
#include <limits>
#include <string>

using namespace std;

// Функция для проверки корректного ввода числа
template <typename T>
void inputCheck(T& value) {
    string input;
    while (true) {
        cin >> input;
        size_t pos;
        try {
            if constexpr (is_same_v<T, int>) {
                value = stoi(input, &pos);
            }
            else {
                value = stod(input, &pos);
            }
            if (pos != input.length()) {
                throw invalid_argument("Некорректный ввод");
            }
            break;
        }
        catch (...) {
            cout << "Ошибка! Введите корректное число: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Функция для поиска индекса максимального по модулю элемента
int findMaxAbsIndex(const double* arr, int n) {
    int maxIndex = 0;
    for (int i = 1; i < n; i++) {
        if (fabs(arr[i]) > fabs(arr[maxIndex])) {
            maxIndex = i;
        }
    }
    return maxIndex;
}

// Функция для вычисления суммы модулей элементов после первого положительного
double sumAfterFirstPositive(const double* arr, int n) {
    bool foundPositive = false;
    double sum = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] > 0) {
            foundPositive = true;
            continue;
        }
        if (foundPositive) {
            sum += fabs(arr[i]);
        }
    }
    return sum;
}

// Функция для перестановки элементов по критерию
void rearrangeArray(double* arr, int n, double a, double b) {
    int left = 0, right = 0;
    double* temp = new double[n];

    // Заполняем временный массив элементами, целая часть которых в [a; b]
    for (int i = 0; i < n; i++) {
        if (floor(arr[i]) >= a && floor(arr[i]) <= b) {
            temp[left++] = arr[i];
        }
    }

    // Добавляем остальные элементы
    for (int i = 0; i < n; i++) {
        if (!(floor(arr[i]) >= a && floor(arr[i]) <= b)) {
            temp[left + right++] = arr[i];
        }
    }

    // Копируем обратно в оригинальный массив
    for (int i = 0; i < n; i++) {
        arr[i] = temp[i];
    }

    delete[] temp; // Освобождение памяти
}

int main() {
    int n;
    cout << "Введите размер массива: ";
    inputCheck(n);

    double* arr = new double[n];

    cout << "Введите элементы массива: ";
    for (int i = 0; i < n; i++) {
        inputCheck(arr[i]);
    }

    int maxAbsIndex = findMaxAbsIndex(arr, n);
    cout << "Индекс максимального по модулю элемента: " << maxAbsIndex << endl;

    double sumMod = sumAfterFirstPositive(arr, n);
    cout << "Сумма модулей элементов после первого положительного: " << sumMod << endl;

    double a, b;
    cout << "Введите a и b для перестановки: ";
    inputCheck(a);
    inputCheck(b);

    rearrangeArray(arr, n, a, b);

    cout << "Преобразованный массив: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;

    delete[] arr; // Освобождение памяти
    return 0;
}
