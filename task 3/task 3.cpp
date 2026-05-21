#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <windows.h> // Для коректного відображення кирилиці у консолі

using namespace std;

// ==========================================
// Базовий клас 1: Сигнал
// ==========================================
class Signal {
protected:
    vector<double> data;

public:
    // Конструктор за замовчуванням
    Signal() {}

    // Конструктор з параметрами
    Signal(const vector<double>& d) : data(d) {}

    // Конструктор копіювання
    Signal(const Signal& other) {
        data = other.data;
    }

    // Віртуальний деструктор
    virtual ~Signal() {}

    // Перевантаження оператора виводу
    friend ostream& operator<<(ostream& os, const Signal& s) {
        os << "Дані сигналу [" << s.data.size() << " елементів]: ";
        for (double val : s.data) {
            os << val << " ";
        }
        return os;
    }

    // Перевантаження оператора введення
    friend istream& operator>>(istream& is, Signal& s) {
        size_t size;
        cout << "Введіть кількість елементів сигналу: ";
        is >> size;
        s.data.resize(size);
        cout << "Введіть " << size << " значень: ";
        for (size_t i = 0; i < size; ++i) {
            is >> s.data[i];
        }
        return is;
    }

    // Метод для заповнення випадковими числами
    void generateRandom(size_t size) {
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(-10.0, 10.0); // Випадкові числа від -10 до 10

        data.resize(size);
        for (size_t i = 0; i < size; ++i) {
            data[i] = dis(gen);
        }
    }

    // Метод для зчитування з файлу
    void loadFromFile(const string& filename) {
        ifstream in(filename);
        if (in.is_open()) {
            double val;
            data.clear();
            while (in >> val) {
                data.push_back(val);
            }
            in.close();
            cout << "Дані успішно завантажено з файлу: " << filename << endl;
        }
        else {
            cerr << "Помилка відкриття файлу: " << filename << endl;
        }
    }
};

// ==========================================
// Базовий клас 2: Дані результату обробки
// ==========================================
class ProcessingResult {
protected:
    double processedValue; // Наприклад, середнє значення або коефіцієнт
    string status;         // Статус обробки

public:
    ProcessingResult() : processedValue(0.0), status("Невідомо") {}

    ProcessingResult(double val, const string& st) : processedValue(val), status(st) {}

    ProcessingResult(const ProcessingResult& other) {
        processedValue = other.processedValue;
        status = other.status;
    }

    virtual ~ProcessingResult() {}

    friend ostream& operator<<(ostream& os, const ProcessingResult& pr) {
        os << "Результат обробки (Значення: " << pr.processedValue << ", Статус: " << pr.status << ")";
        return os;
    }

    friend istream& operator>>(istream& is, ProcessingResult& pr) {
        cout << "Введіть значення результату обробки: ";
        is >> pr.processedValue;
        cout << "Введіть статус обробки (одне слово): ";
        is >> pr.status;
        return is;
    }
};

// ==========================================
// Похідний клас: Оброблені дані, що є сигналом
// (Множинне успадкування)
// ==========================================
class ProcessedSignal : public Signal, public ProcessingResult {
private:
    string filterName;

public:
    ProcessedSignal() : Signal(), ProcessingResult(), filterName("Без фільтра") {}

    ProcessedSignal(const vector<double>& d, double val, const string& st, const string& fName)
        : Signal(d), ProcessingResult(val, st), filterName(fName) {
    }

    ProcessedSignal(const ProcessedSignal& other)
        : Signal(other), ProcessingResult(other), filterName(other.filterName) {
    }

    virtual ~ProcessedSignal() {}

    friend ostream& operator<<(ostream& os, const ProcessedSignal& ps) {
        os << "--- Оброблений Сигнал ---" << endl;
        os << "Фільтр: " << ps.filterName << endl;
        os << static_cast<const Signal&>(ps) << endl;
        os << static_cast<const ProcessingResult&>(ps);
        return os;
    }

    friend istream& operator>>(istream& is, ProcessedSignal& ps) {
        cout << "--- Введення обробленого сигналу ---" << endl;
        is >> static_cast<Signal&>(ps);
        is >> static_cast<ProcessingResult&>(ps);
        cout << "Введіть назву застосованого фільтра (одне слово): ";
        is >> ps.filterName;
        return is;
    }
};

// ==========================================
// Головна функція
// ==========================================
int main() {
    // Налаштування кирилиці для консолі Windows (Visual Studio)
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "=== Демонстрація ієрархії класів ===" << endl << endl;

    // 1. Введення з клавіатури
    cout << "[1] Введення початкових даних з клавіатури:" << endl;
    ProcessedSignal keyboardSignal;
    cin >> keyboardSignal;
    cout << "\nРезультат з клавіатури:\n" << keyboardSignal << endl << endl;

    // 2. Генерація випадкових чисел
    cout << "[2] Використання датчика випадкових чисел:" << endl;
    ProcessedSignal randomSignal(vector<double>(), 5.5, "Успіх", "Шум_Фільтр");
    randomSignal.generateRandom(5); // Генеруємо 5 випадкових значень сигналу
    cout << "\nРезультат випадкової генерації:\n" << randomSignal << endl << endl;

    // 3. Введення з файлу
    cout << "[3] Введення початкових даних з файлу:" << endl;

    // Створюємо тестовий файл для зчитування
    ofstream testFile("test_signal.txt");
    if (testFile.is_open()) {
        testFile << "1.1 2.2 3.3 4.4 5.5";
        testFile.close();
    }

    ProcessedSignal fileSignal;
    fileSignal.loadFromFile("test_signal.txt");
    cout << "\nРезультат зчитаний з файлу:\n" << fileSignal << endl;

    // Використання конструктора копіювання
    cout << "\n[4] Демонстрація конструктора копіювання (Копіюємо випадковий сигнал):" << endl;
    ProcessedSignal copiedSignal = randomSignal;
    cout << copiedSignal << endl;

    return 0;
}