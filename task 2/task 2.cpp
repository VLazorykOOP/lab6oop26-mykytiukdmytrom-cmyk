#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>
#include <random>
#include <string>
#include <iomanip>
#include <fcntl.h>  // Потрібно для _setmode
#include <io.h>     // Потрібно для _setmode

const double PI = 3.14159265358979323846;

// Базовий абстрактний клас
class Shape {
public:
    virtual ~Shape() = default;
    virtual double getArea() const = 0;
    virtual double getPerimeter() const = 0;
    virtual void print() const = 0;
};

// Похідний клас: Коло
class Circle : public Shape {
private:
    double radius;
public:
    Circle(double r) : radius(r > 0 ? r : 1.0) {}

    double getArea() const override { return PI * radius * radius; }
    double getPerimeter() const override { return 2 * PI * radius; }

    void print() const override {
        std::wcout << L"[Коло] Радіус: " << radius
            << L" | Площа: " << std::fixed << std::setprecision(2) << getArea()
            << L" | Периметр: " << getPerimeter() << L"\n";
    }
};

// Похідний клас: Прямокутник
class Rectangle : public Shape {
private:
    double width, height;
public:
    Rectangle(double w, double h) : width(w > 0 ? w : 1.0), height(h > 0 ? h : 1.0) {}

    double getArea() const override { return width * height; }
    double getPerimeter() const override { return 2 * (width + height); }

    void print() const override {
        std::wcout << L"[Прямокутник] Ширина: " << width << L", Висота: " << height
            << L" | Площа: " << getArea()
            << L" | Периметр: " << getPerimeter() << L"\n";
    }
};

// Похідний клас: Трапеція
class Trapezoid : public Shape {
private:
    double base1, base2, height;
public:
    Trapezoid(double b1, double b2, double h)
        : base1(b1 > 0 ? b1 : 1.0), base2(b2 > 0 ? b2 : 1.0), height(h > 0 ? h : 1.0) {
    }

    double getArea() const override { return 0.5 * (base1 + base2) * height; }

    double getPerimeter() const override {
        double lateralSide = std::sqrt(std::pow(std::abs(base1 - base2) / 2.0, 2) + std::pow(height, 2));
        return base1 + base2 + (2 * lateralSide);
    }

    void print() const override {
        std::wcout << L"[Трапеція] Основи: " << base1 << L" і " << base2 << L", Висота: " << height
            << L" | Площа: " << getArea()
            << L" | Периметр: " << getPerimeter() << L"\n";
    }
};

// Введення з клавіатури
void inputFromKeyboard(std::vector<std::unique_ptr<Shape>>& shapes) {
    int choice;
    std::wcout << L"\nОберіть фігуру (1 - Коло, 2 - Прямокутник, 3 - Трапеція): ";
    std::wcin >> choice;

    if (choice == 1) {
        double r;
        std::wcout << L"Введіть радіус кола: ";
        std::wcin >> r;
        shapes.push_back(std::make_unique<Circle>(r));
    }
    else if (choice == 2) {
        double w, h;
        std::wcout << L"Введіть ширину та висоту прямокутника: ";
        std::wcin >> w >> h;
        shapes.push_back(std::make_unique<Rectangle>(w, h));
    }
    else if (choice == 3) {
        double b1, b2, h;
        std::wcout << L"Введіть першу основу, другу основу та висоту трапеції: ";
        std::wcin >> b1 >> b2 >> h;
        shapes.push_back(std::make_unique<Trapezoid>(b1, b2, h));
    }
    else {
        std::wcout << L"Невірний вибір.\n";
    }
}

// Введення з файлу
void inputFromFile(std::vector<std::unique_ptr<Shape>>& shapes) {
    std::string filename = "shapes.txt";
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::wcout << L"Помилка відкриття файлу " << filename.c_str() << L"!\n";
        return;
    }

    std::string type;
    while (file >> type) {
        if (type == "circle") {
            double r; file >> r;
            shapes.push_back(std::make_unique<Circle>(r));
        }
        else if (type == "rectangle") {
            double w, h; file >> w >> h;
            shapes.push_back(std::make_unique<Rectangle>(w, h));
        }
        else if (type == "trapezoid") {
            double b1, b2, h; file >> b1 >> b2 >> h;
            shapes.push_back(std::make_unique<Trapezoid>(b1, b2, h));
        }
    }
    std::wcout << L"Дані з файлу успішно зчитано.\n";
    file.close();
}

// Випадкові числа
void inputRandom(std::vector<std::unique_ptr<Shape>>& shapes) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> disType(1, 3);
    std::uniform_real_distribution<> disValue(1.0, 20.0);

    int count;
    std::wcout << L"Скільки випадкових фігур згенерувати? ";
    std::wcin >> count;

    for (int i = 0; i < count; ++i) {
        int type = disType(gen);
        if (type == 1) {
            shapes.push_back(std::make_unique<Circle>(disValue(gen)));
        }
        else if (type == 2) {
            shapes.push_back(std::make_unique<Rectangle>(disValue(gen), disValue(gen)));
        }
        else if (type == 3) {
            shapes.push_back(std::make_unique<Trapezoid>(disValue(gen), disValue(gen), disValue(gen)));
        }
    }
    std::wcout << L"Випадкові фігури успішно згенеровано.\n";
}

int main() {
    // Вмикаємо режим Юнікоду (UTF-16) для консолі введення/виведення
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    std::vector<std::unique_ptr<Shape>> shapes;
    int mode = 0;

    while (true) {
        std::wcout << L"\n=== МЕНЮ УПРАВЛІННЯ ФІГУРАМИ ===\n";
        std::wcout << L"1. Додати фігуру з клавіатури\n";
        std::wcout << L"2. Завантажити фігури з файлу (shapes.txt)\n";
        std::wcout << L"3. Згенерувати випадкові фігури\n";
        std::wcout << L"4. Вивести всі фігури на екран\n";
        std::wcout << L"5. Очистити список фігур\n";
        std::wcout << L"0. Вихід\n";
        std::wcout << L"Ваш вибір: ";
        std::wcin >> mode;

        if (mode == 0) break;

        switch (mode) {
        case 1:
            inputFromKeyboard(shapes);
            break;
        case 2:
            inputFromFile(shapes);
            break;
        case 3:
            inputRandom(shapes);
            break;
        case 4:
            if (shapes.empty()) {
                std::wcout << L"Список фігур порожній.\n";
            }
            else {
                std::wcout << L"\n--- Список створених фігур ---\n";
                for (const auto& shape : shapes) {
                    shape->print();
                }
            }
            break;
        case 5:
            shapes.clear();
            std::wcout << L"Список очищено.\n";
            break;
        default:
            std::wcout << L"Невірний пункт меню.\n";
        }
    }

    return 0;
}