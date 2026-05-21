/*
 * =====================================================================
 *
 *  Схема успадкування (7 класів, перехресне):
 *
 *        A         B       <- базові класи (рівень 0)
 *        ↑  \   /  ↑
 *        |   × ×   |      <- перехресні стрілки
 *        C         D       <- рівень 1  (C : B,A  і  D : A,B)
 *         ↖      ↗
 *             E            <- рівень 2  (E : C, D) — діамант
 *             ↑
 *             F            <- рівень 3
 *             ↑
 *             G            <- похідний клас (рівень 4)
 *
 *  Без вірт. успадкування: E містить 2 копії A і 2 копії B.
 *  З    вірт. успадкування: E містить тільки по 1 копії.
 *
 *  Введення: 1 — клавіатура, 2 — файл, 3 — генератор випадкових чисел
 * =====================================================================
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iomanip>
#include <windows.h>

using namespace std;

// ─────────────────────────────────────────────────────────────────────
//  Допоміжна функція: роздільник
// ─────────────────────────────────────────────────────────────────────
static void separator(const string& title = "")
{
    cout << "\n" << string(60, '=') << "\n";
    if (!title.empty())
        cout << "  " << title << "\n" << string(60, '=') << "\n";
}

// =====================================================================
//  ІЄРАРХІЯ 1 — БЕЗ ВІРТУАЛЬНОГО УСПАДКУВАННЯ
//  (діамантова проблема: E отримує A і B двічі)
// =====================================================================
namespace NV   // NonVirtual
{
    // ── Рівень 0 ──────────────────────────────────────────────────────
    struct A {
        int dataA;
        A() : dataA(0) {}
        explicit A(int a) : dataA(a) {}
        virtual ~A() = default;
        virtual void print() const {
            cout << "  [A] dataA = " << dataA << "\n";
        }
    };

    struct B {
        double dataB;
        B() : dataB(0.0) {}
        explicit B(double b) : dataB(b) {}
        virtual ~B() = default;
        virtual void print() const {
            cout << "  [B] dataB = " << fixed << setprecision(2) << dataB << "\n";
        }
    };

    // ── Рівень 1 (перехресне успадкування) ───────────────────────────
    //  C успадковує спочатку B, потім A  (стрілки перехрещуються)
    struct C : public B, public A {
        float dataC;
        C() : dataC(0.0f) {}
        C(int a, double b, float c) : A(a), B(b), dataC(c) {}
        virtual void print() const {
            A::print();
            B::print();
            cout << "  [C] dataC = " << fixed << setprecision(2) << dataC << "\n";
        }
    };

    //  D успадковує спочатку A, потім B  (стрілки перехрещуються)
    struct D : public A, public B {
        char dataD;
        D() : dataD('?') {}
        D(int a, double b, char d) : A(a), B(b), dataD(d) {}
        virtual void print() const {
            A::print();
            B::print();
            cout << "  [D] dataD = '" << dataD << "'\n";
        }
    };

    // ── Рівень 2 (діамант: E отримує A і B ДВІЧІ) ────────────────────
    struct E : public C, public D {
        int dataE;
        E() : dataE(0) {}
        E(int aC, double bC, float c, int aD, double bD, char d, int e)
            : C(aC, bC, c), D(aD, bD, d), dataE(e) {
        }
        virtual void print() const {
            cout << "  -- гілка C --\n";  C::print();
            cout << "  -- гілка D --\n";  D::print();
            cout << "  [E] dataE = " << dataE << "\n";
        }
    };

    // ── Рівень 3 ──────────────────────────────────────────────────────
    struct F : public E {
        string dataF;
        F() : dataF("") {}
        F(int aC, double bC, float c, int aD, double bD, char d, int e,
            const string& f)
            : E(aC, bC, c, aD, bD, d, e), dataF(f) {
        }
        virtual void print() const {
            E::print();
            cout << "  [F] dataF = \"" << dataF << "\"\n";
        }
    };

    // ── Рівень 4 ──────────────────────────────────────────────────────
    struct G : public F {
        bool dataG;
        G() : dataG(false) {}
        G(int aC, double bC, float c, int aD, double bD, char d, int e,
            const string& f, bool g)
            : F(aC, bC, c, aD, bD, d, e, f), dataG(g) {
        }
        void print() const override {
            F::print();
            cout << "  [G] dataG = " << boolalpha << dataG << "\n";
        }
    };

} // namespace NV


// =====================================================================
//  ІЄРАРХІЯ 2 — З ВІРТУАЛЬНИМ УСПАДКУВАННЯМ
//  (A і B включаються в E тільки по одному разу)
// =====================================================================
namespace VT   // Virtual
{
    // ── Рівень 0 ──────────────────────────────────────────────────────
    struct A {
        int dataA;
        A() : dataA(0) {}
        explicit A(int a) : dataA(a) {}
        virtual ~A() = default;
        virtual void print() const {
            cout << "  [A] dataA = " << dataA << "\n";
        }
    };

    struct B {
        double dataB;
        B() : dataB(0.0) {}
        explicit B(double b) : dataB(b) {}
        virtual ~B() = default;
        virtual void print() const {
            cout << "  [B] dataB = " << fixed << setprecision(2) << dataB << "\n";
        }
    };

    // ── Рівень 1 (перехресне + VIRTUAL) ──────────────────────────────
    struct C : public virtual B, public virtual A {
        float dataC;
        C() : dataC(0.0f) {}
        C(int a, double b, float c) : A(a), B(b), dataC(c) {}
        virtual void print() const {
            A::print();
            B::print();
            cout << "  [C] dataC = " << fixed << setprecision(2) << dataC << "\n";
        }
    };

    struct D : public virtual A, public virtual B {
        char dataD;
        D() : dataD('?') {}
        D(int a, double b, char d) : A(a), B(b), dataD(d) {}
        virtual void print() const {
            A::print();
            B::print();
            cout << "  [D] dataD = '" << dataD << "'\n";
        }
    };

    // ── Рівень 2 (E отримує A і B тільки ОДИН раз) ───────────────────
    struct E : public C, public D {
        int dataE;
        E() : dataE(0) {}
        //  При вірт. успадкуванні конструктор найбільш похідного класу
        //  ініціалізує віртуальні бази напряму:
        E(int a, double b, float c, char d, int e)
            : A(a), B(b), C(a, b, c), D(a, b, d), dataE(e) {
        }
        virtual void print() const {
            cout << "  -- спільна база A --\n";  A::print();
            cout << "  -- спільна база B --\n";  B::print();
            cout << "  [C] dataC = " << fixed << setprecision(2) << C::dataC << "\n";
            cout << "  [D] dataD = '" << D::dataD << "'\n";
            cout << "  [E] dataE = " << dataE << "\n";
        }
    };

    // ── Рівень 3 ──────────────────────────────────────────────────────
    struct F : public E {
        string dataF;
        F() : dataF("") {}
        F(int a, double b, float c, char d, int e, const string& f)
            : A(a), B(b), E(a, b, c, d, e), dataF(f) {
        }
        virtual void print() const {
            E::print();
            cout << "  [F] dataF = \"" << dataF << "\"\n";
        }
    };

    // ── Рівень 4 ──────────────────────────────────────────────────────
    struct G : public F {
        bool dataG;
        G() : dataG(false) {}
        G(int a, double b, float c, char d, int e, const string& f, bool g)
            : A(a), B(b), F(a, b, c, d, e, f), dataG(g) {
        }
        void print() const override {
            F::print();
            cout << "  [G] dataG = " << boolalpha << dataG << "\n";
        }
    };

} // namespace VT


// =====================================================================
//  ФУНКЦІЇ ВВЕДЕННЯ ДАНИХ
// =====================================================================

// ─── 1. Клавіатура ────────────────────────────────────────────────────
struct InputData {
    int    a;
    double b;
    float  c;
    char   d;
    int    e;
    string f;
    bool   g;
};

InputData readFromKeyboard()
{
    InputData d;
    cout << "\n  Введіть дані:\n";
    cout << "  dataA (int)    : "; cin >> d.a;
    cout << "  dataB (double) : "; cin >> d.b;
    cout << "  dataC (float)  : "; cin >> d.c;
    cout << "  dataD (char)   : "; cin >> d.d;
    cout << "  dataE (int)    : "; cin >> d.e;
    cin.ignore();
    cout << "  dataF (string) : "; getline(cin, d.f);
    cout << "  dataG (0/1)    : "; cin >> d.g;
    return d;
}

// ─── 2. Файл ──────────────────────────────────────────────────────────
InputData readFromFile(const string& filename)
{
    InputData d;
    ifstream in(filename);
    if (!in.is_open()) {
        cerr << "  [!] Не вдалося відкрити файл \"" << filename << "\"\n";
        return d;
    }
    in >> d.a >> d.b >> d.c >> d.d >> d.e;
    in.ignore();
    getline(in, d.f);
    in >> d.g;
    in.close();
    cout << "  Дані зчитано з файлу \"" << filename << "\"\n";
    return d;
}

// ─── 3. Генератор випадкових чисел ────────────────────────────────────
InputData readFromRandom()
{
    InputData d;
    d.a = rand() % 200 - 100;               // [-100 .. 99]
    d.b = (rand() % 10000) / 100.0 - 50.0; // [-50 .. 99.99]
    d.c = static_cast<float>(rand() % 1000) / 10.0f;
    d.d = static_cast<char>('A' + rand() % 26);
    d.e = rand() % 500;
    d.f = "random_" + to_string(rand() % 9999);
    d.g = (rand() % 2) != 0;
    cout << "  Дані згенеровано випадково.\n";
    return d;
}

// ─── Створити тестовий файл ────────────────────────────────────────────
void createSampleFile(const string& filename)
{
    ofstream out(filename);
    if (!out.is_open()) { cerr << "  Не вдалося створити файл.\n"; return; }
    out << "42\n"
        << "3.14\n"
        << "2.71\n"
        << "X\n"
        << "100\n"
        << "Hello from file\n"
        << "1\n";
    out.close();
    cout << "  Тестовий файл \"" << filename << "\" створено.\n";
}


// =====================================================================
//  ДЕМОНСТРАЦІЯ РОЗМІРІВ КЛАСІВ
// =====================================================================
void showSizes()
{
    separator("РОЗМІРИ ОБ'ЄКТІВ (sizeof)");

    cout << "\n  --- Без віртуального успадкування ---\n";
    cout << "  sizeof(NV::A) = " << sizeof(NV::A) << " байт\n";
    cout << "  sizeof(NV::B) = " << sizeof(NV::B) << " байт\n";
    cout << "  sizeof(NV::C) = " << sizeof(NV::C) << " байт\n";
    cout << "  sizeof(NV::D) = " << sizeof(NV::D) << " байт\n";
    cout << "  sizeof(NV::E) = " << sizeof(NV::E) << " байт  <- A і B включені ДВІЧІ\n";
    cout << "  sizeof(NV::F) = " << sizeof(NV::F) << " байт\n";
    cout << "  sizeof(NV::G) = " << sizeof(NV::G) << " байт\n";

    cout << "\n  --- З віртуальним успадкуванням ---\n";
    cout << "  sizeof(VT::A) = " << sizeof(VT::A) << " байт\n";
    cout << "  sizeof(VT::B) = " << sizeof(VT::B) << " байт\n";
    cout << "  sizeof(VT::C) = " << sizeof(VT::C) << " байт  (+ vptr для вірт. бази)\n";
    cout << "  sizeof(VT::D) = " << sizeof(VT::D) << " байт  (+ vptr для вірт. бази)\n";
    cout << "  sizeof(VT::E) = " << sizeof(VT::E) << " байт  <- A і B включені ОДИН раз\n";
    cout << "  sizeof(VT::F) = " << sizeof(VT::F) << " байт\n";
    cout << "  sizeof(VT::G) = " << sizeof(VT::G) << " байт\n";

    cout << "\n  Пояснення різниці:\n";
    int diffE = static_cast<int>(sizeof(NV::E)) - static_cast<int>(sizeof(VT::E));
    cout << "  NV::E - VT::E = " << diffE << " байт"
        << " (зайві копії A та B у NV)\n";
}


// =====================================================================
//  ГОЛОВНА ФУНКЦІЯ
// =====================================================================
int main()
{
    // ── Кодування консолі Windows (кирилиця) ──────────────────────────
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    srand(static_cast<unsigned>(time(nullptr)));

    // ── Привітання ────────────────────────────────────────────────────
    separator("Завдання 1. Віртуальне успадкування (Рис. 11)");
    cout << "\n  Схема: A,B -> C,D (перехресно) -> E -> F -> G\n";

    // ── Вибір джерела даних ────────────────────────────────────────────
    int choice = 0;
    cout << "\n  Оберіть джерело введення даних:\n";
    cout << "    1 - Клавіатура\n";
    cout << "    2 - Файл\n";
    cout << "    3 - Генератор випадкових чисел\n";
    cout << "  Ваш вибір: ";
    cin >> choice;

    InputData data;
    const string FILE_NAME = "input_data.txt";

    switch (choice) {
    case 1:
        data = readFromKeyboard();
        break;
    case 2:
        createSampleFile(FILE_NAME);
        data = readFromFile(FILE_NAME);
        break;
    case 3:
    default:
        data = readFromRandom();
        break;
    }

    // ── Виведення зчитаних даних ──────────────────────────────────────
    separator("ЗЧИТАНІ ДАНІ");
    cout << "  a=" << data.a << "  b=" << data.b << "  c=" << data.c
        << "  d='" << data.d << "'  e=" << data.e
        << "  f=\"" << data.f << "\"  g=" << boolalpha << data.g << "\n";


    // =====================================================================
    //  ІЄРАРХІЯ БЕЗ ВІРТУАЛЬНОГО УСПАДКУВАННЯ
    // =====================================================================
    separator("ІЄРАРХІЯ 1 — БЕЗ VIRTUAL (NV)");

    // Окремі об'єкти кожного рівня
    NV::A objA(data.a);
    NV::B objB(data.b);
    NV::C objC(data.a, data.b, data.c);
    NV::D objD(data.a, data.b, data.d);

    // E потребує два незалежних набори для A і B (через відсутність virtual)
    NV::E objE(data.a, data.b, data.c,
        data.a + 10, data.b + 1.0, data.d, data.e);

    NV::F objF(data.a, data.b, data.c,
        data.a + 10, data.b + 1.0, data.d, data.e, data.f);

    NV::G objG(data.a, data.b, data.c,
        data.a + 10, data.b + 1.0, data.d, data.e, data.f, data.g);

    cout << "\n--- NV::A ---\n"; objA.print();
    cout << "\n--- NV::B ---\n"; objB.print();
    cout << "\n--- NV::C ---\n"; objC.print();
    cout << "\n--- NV::D ---\n"; objD.print();
    cout << "\n--- NV::E (дублювання A і B!) ---\n"; objE.print();
    cout << "\n--- NV::F ---\n"; objF.print();
    cout << "\n--- NV::G ---\n"; objG.print();


    // =====================================================================
    //  ІЄРАРХІЯ З ВІРТУАЛЬНИМ УСПАДКУВАННЯМ
    // =====================================================================
    separator("ІЄРАРХІЯ 2 — З VIRTUAL (VT)");

    VT::A objVA(data.a);
    VT::B objVB(data.b);
    VT::C objVC(data.a, data.b, data.c);
    VT::D objVD(data.a, data.b, data.d);
    VT::E objVE(data.a, data.b, data.c, data.d, data.e);
    VT::F objVF(data.a, data.b, data.c, data.d, data.e, data.f);
    VT::G objVG(data.a, data.b, data.c, data.d, data.e, data.f, data.g);

    cout << "\n--- VT::A ---\n"; objVA.print();
    cout << "\n--- VT::B ---\n"; objVB.print();
    cout << "\n--- VT::C ---\n"; objVC.print();
    cout << "\n--- VT::D ---\n"; objVD.print();
    cout << "\n--- VT::E (одна копія A і B) ---\n"; objVE.print();
    cout << "\n--- VT::F ---\n"; objVF.print();
    cout << "\n--- VT::G ---\n"; objVG.print();

    // ── Поліморфізм через базовий вказівник ───────────────────────────
    separator("ПОЛІМОРФІЗМ: вказівник VT::A* на об'єкт VT::G");
    VT::A* pBase = &objVG;
    pBase->print();   // викликає VT::G::print() через vtable

    // ── Порівняння розмірів ───────────────────────────────────────────
    showSizes();

    separator("ЗАВЕРШЕНО");
    cout << "\n  Натисніть Enter для виходу...";
    cin.ignore(); cin.get();
    return 0;
}