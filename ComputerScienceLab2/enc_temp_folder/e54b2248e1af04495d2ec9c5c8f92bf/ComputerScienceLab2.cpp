#include <iostream>
using namespace std;


bool isInsideFigure(double x, double y) {
    return (y >= 0 && abs(x) <= 1 && y <= 1 || x * x + y * y <= 1);
}

void test(double x, double y) {
    cout << "x = " << x << endl;
    cout << "y = " << y << endl;
    cout << "result = " << isInsideFigure(x, y) << endl << endl;
}

int main() {
    cout << "Программа Крылова Ильи Кирилловича (М411):" << endl;
    cout << "Тесты для Анастасии Алексеевны:" << endl << endl;
    test(0, 0);
    test(0.5, 0.5);
    test(1, 1);
    test(-0.5, 0.5);
    test(-1, 1);
    test(0.5, -0.5);
    test(1, -1);
    test(-0.5, -0.5);
    test(-1, -1);
    test(0.7071067, -0.7071067);
    test(0.707108, -0.7071067);
    test(0.707108, -0.707108);
    test(-0.7071067, -0.7071067);
    test(-0.707108, -0.7071067);
    test(-0.7071067, -0.707108);
    cout << "Далее ручной ввод:" << endl;
    // Ввод координат точки
    while (true) {
        double x, y;
        cout << "Введите координату x: ";
        cin >> x;
        cout << "Введите координату y: ";
        cin >> y;
        if (isInsideFigure(x, y)) {
            cout << "Точка (" << x << ", " << y << ") попадает в область определения функции." << endl;
        }
        else {
            cout << "Точка (" << x << ", " << y << ") не попадает в область определения функции." << endl;
        }
    }
    return 0;
}
