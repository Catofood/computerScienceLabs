#include <iostream>
#include <sstream>
#include <iomanip>
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

// Безопасный ввод целого числа
int getValidatedInt(const std::string& prompt) {
    int value;
    std::string line;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, line)) {
            std::cerr << "Ошибка ввода." << std::endl;
            exit(EXIT_FAILURE);
        }
        std::stringstream ss(line);
        if (ss >> value && ss.eof()) {
            return value;
        }
        std::cout << "Неверный ввод. Введите целое число.\n";
    }
}

// Безопасный ввод вещественного числа
double getValidatedDouble(const std::string& prompt) {
    double value;
    std::string line;
    while (true) {
        std::cout << prompt;
        if (!std::getline(std::cin, line)) {
            std::cerr << "Ошибка ввода." << std::endl;
            exit(EXIT_FAILURE);
        }
        std::stringstream ss(line);
        if (ss >> value && ss.eof()) {
            return value;
        }
        std::cout << "Неверный ввод. Введите число (можно с дробной частью).\n";
    }
}

// Ввод матрицы
void readMatrix(double** matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            matrix[i][j] = getValidatedDouble("Введите элемент [" + std::to_string(i) + "][" + std::to_string(j) + "]: ");
}

// Проверка на наличие положительных элементов
bool hasPositive(double** matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (matrix[i][j] > 0)
                return true;
    return false;
}

// Индекс первой строки с положительным элементом
int firstPositiveRow(double** matrix, int rows, int cols) {
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            if (matrix[i][j] > 0)
                return i;
    return -1;
}

// Удаление нулевых строк и столбцов
void removeZeroRowsCols(double**& matrix, int& rows, int& cols) {
    bool* zeroRow = new bool[rows];
    bool* zeroCol = new bool[cols];
    int newRows = 0, newCols = 0;

    for (int i = 0; i < rows; ++i) {
        zeroRow[i] = true;
        for (int j = 0; j < cols; ++j)
            if (matrix[i][j] != 0.0) {
                zeroRow[i] = false;
                break;
            }
        if (!zeroRow[i]) newRows++;
    }

    for (int j = 0; j < cols; ++j) {
        zeroCol[j] = true;
        for (int i = 0; i < rows; ++i)
            if (matrix[i][j] != 0.0) {
                zeroCol[j] = false;
                break;
            }
        if (!zeroCol[j]) newCols++;
    }

    if (newRows == 0 || newCols == 0) {
        newRows = newCols = 0;
    }

    int* rowIndex = new int[newRows];
    int* colIndex = new int[newCols];

    for (int i = 0, k = 0; i < rows; ++i)
        if (!zeroRow[i]) rowIndex[k++] = i;

    for (int j = 0, k = 0; j < cols; ++j)
        if (!zeroCol[j]) colIndex[k++] = j;

    double** newMatrix = nullptr;
    if (newRows > 0 && newCols > 0) {
        newMatrix = new double* [newRows];
        for (int i = 0; i < newRows; ++i)
            newMatrix[i] = new double[newCols];

        for (int i = 0; i < newRows; ++i)
            for (int j = 0; j < newCols; ++j)
                newMatrix[i][j] = matrix[rowIndex[i]][colIndex[j]];
    }

    for (int i = 0; i < rows; ++i)
        delete[] matrix[i];
    delete[] matrix;

    matrix = newMatrix;
    rows = newRows;
    cols = newCols;

    delete[] zeroRow;
    delete[] zeroCol;
    delete[] rowIndex;
    delete[] colIndex;
}

int main() {
#ifdef _DEBUG
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    int rows, cols;
    double** matrix = nullptr;

    while (true) {
        rows = getValidatedInt("Введите количество строк (целое число): ");
        cols = getValidatedInt("Введите количество столбцов (целое число): ");
        if (rows <= 0 || cols <= 0) {
            std::cout << "Размеры должны быть положительными.\n";
            continue;
        }

        matrix = new double* [rows];
        for (int i = 0; i < rows; ++i)
            matrix[i] = new double[cols];

        readMatrix(matrix, rows, cols);

        if (!hasPositive(matrix, rows, cols)) {
            std::cout << "Матрица не содержит положительных значений.\n";
            for (int i = 0; i < rows; ++i)
                delete[] matrix[i];
            delete[] matrix;
            matrix = nullptr;
            continue;
        }
        break;
    }

    std::cout << "\nВведённая матрица:\n";
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j)
            std::cout << std::setw(8) << std::fixed << std::setprecision(2) << matrix[i][j];
        std::cout << '\n';
    }

    removeZeroRowsCols(matrix, rows, cols);

    std::cout << "\nРезультирующая матрица:\n";
    if (rows == 0 || cols == 0)
        std::cout << "(пустая матрица)\n";
    else
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j)
                std::cout << std::setw(8) << std::fixed << std::setprecision(2) << matrix[i][j];
            std::cout << '\n';
        }

    int firstRow = firstPositiveRow(matrix, rows, cols);
    if (firstRow >= 0)
        std::cout << "Первая строка с положительным элементом: " << firstRow << '\n';
    else
        std::cout << "Положительных элементов не осталось.\n";

    for (int i = 0; i < rows; ++i)
        delete[] matrix[i];
    delete[] matrix;

    return 0;
}
