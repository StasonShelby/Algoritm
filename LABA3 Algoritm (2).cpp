#include <iostream>
#include <cstdlib> // для malloc и free
#include <cmath>   // для abs()
#include <Windows.h> // для поддержки русского языка
#include <string>  // для std::string
#include <sstream> // для std::stringstream

using namespace std; // Используем пространство имен std

// Функция для безопасного ввода целого числа
int safeInputInt() {
    string input;
    int value;
    while (true) {
        getline(cin, input); // считываем всю строку
        stringstream ss(input);
        if (ss >> value && ss.eof()) { // проверяем, что ввод корректен
            break;
        }
        cerr << "Ошибка! Введите целое число: ";
    }
    return value;
}

// Функция для безопасного ввода размера массива
int safeInputSize() {
    string input;
    int value;
    while (true) {
        cout << "Введите размер массива N: ";
        getline(cin, input); // считываем всю строку
        stringstream ss(input);
        if (ss >> value && value >= 0 && ss.eof()) { // проверяем, что ввод корректен
            break;
        }
        cerr << "Ошибка: размер массива должен быть неотрицательным целым числом!" << endl;
    }
    return value;
}

// Структура для представления сегмента динамической памяти
struct PointerSegment {
    double* pointer; // указатель на динамический массив (теперь double)
    size_t size;     // размер массива
};

// Функция для выделения памяти под массив
PointerSegment* NewPointer(PointerSegment* p, size_t size) {
    if (p->pointer != nullptr) {
        cerr << "Предупреждение: утечка памяти! Старая память не была освобождена." << endl;
    }
    p->pointer = static_cast<double*>(malloc(size * sizeof(double))); // выделяем память
    if (p->pointer == nullptr && size > 0) {
        cerr << "Ошибка: не удалось выделить память!" << endl;
        return nullptr; // возвращаем nullptr в случае ошибки
    }
    p->size = size; // сохраняем размер массива
    return p;
}

// Функция для записи значения в массив по указателю
void WritePointer(PointerSegment* p, int offset, double value) {
    if (p->pointer == nullptr) {
        cerr << "Ошибка: попытка записи в nullptr!" << endl;
        return;
    }
    if (offset >= 0 && offset < p->size) {
        *(p->pointer + offset) = value; // записываем значение по указателю
    }
    else {
        cerr << "Ошибка: выход за пределы массива!" << endl;
    }
}

// Функция для чтения значения из массива по указателю
double ReadPointer(PointerSegment* p, int offset) {
    if (p->pointer == nullptr) {
        cerr << "Ошибка: попытка чтения из nullptr!" << endl;
        return -1; // возвращаем -1 в случае ошибки
    }
    if (offset >= 0 && offset < p->size) {
        return *(p->pointer + offset); // читаем значение по указателю
    }
    else {
        cerr << "Ошибка: выход за пределы массива!" << endl;
        return -1; // возвращаем -1 в случае ошибки
    }
}

// Функция для освобождения памяти
void FreePointer(PointerSegment* p) {
    if (p->pointer != nullptr) {
        free(p->pointer); // освобождаем память
        p->pointer = nullptr;
        p->size = 0;
    }
    else {
        cerr << "Ошибка: попытка освободить nullptr!" << endl;
    }
}

int main() {
    // Устанавливаем кодовую страницу для поддержки русского языка
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    // Ввод размера массива
    int N = safeInputSize(); // безопасный ввод размера массива

    // Создаем сегмент динамической памяти
    PointerSegment arr;
    arr.pointer = nullptr;
    arr.size = 0;

    // Демонстрация ошибки записи в nullptr
    cout << "Демонстрация ошибки записи в nullptr..." << endl;
    WritePointer(&arr, 0, 10.5); // попытка записи в nullptr

    // Демонстрация ошибки чтения из nullptr
    cout << "Демонстрация ошибки чтения из nullptr..." << endl;
    double value = ReadPointer(&arr, 0); // попытка чтения из nullptr
    cout << "Прочитанное значение: " << value << endl;

    // Выделяем память и заполняем массив
    if (!NewPointer(&arr, N)) {
        cerr << "Программа завершена из-за ошибки выделения памяти." << endl;
        return 1; // завершаем программу с кодом ошибки
    }

    // Заполняем массив
    if (N > 0) {
        cout << "Введите элементы массива через пробел: ";
        string input;
        getline(cin, input); // считываем всю строку
        stringstream ss(input);   // создаем поток для разбора строки

        double value;
        int count = 0;
        while (ss >> value) { // читаем числа из строки
            if (count < N) {
                WritePointer(&arr, count, value); // записываем значение в массив
                count++;
            }
            else {
                cerr << "Ошибка: введено больше элементов, чем размер массива!" << endl;
                break;
            }
        }

        if (count < N) {
            cerr << "Ошибка: введено меньше элементов, чем размер массива!" << endl;
            FreePointer(&arr); // освобождаем память
            return 1; // завершаем программу с кодом ошибки
        }
    }
    else {
        cout << "Массив пуст (размер массива равен 0)." << endl;
    }

    // Выводим содержимое массива
    if (N > 0) {
        cout << "Содержимое массива: ";
        for (int i = 0; i < N; ++i) {
            cout << ReadPointer(&arr, i) << " ";
        }
        cout << endl;
    }

    // Находим минимальный и максимальный элементы
    if (N > 0) {
        int minIndex = 0; // индекс минимального элемента
        int maxIndex = 0; // индекс максимального элемента
        for (int i = 1; i < N; ++i) {
            if (ReadPointer(&arr, i) < ReadPointer(&arr, minIndex)) {
                minIndex = i;
            }
            if (ReadPointer(&arr, i) > ReadPointer(&arr, maxIndex)) {
                maxIndex = i;
            }
        }

        // Определяем границы для вычисления произведения
        int start = (minIndex < maxIndex) ? minIndex : maxIndex;
        int end = (minIndex > maxIndex) ? minIndex : maxIndex;

        // Вычисляем произведение элементов между min и max
        double product = 1.0;
        bool hasElements = false;
        for (int i = start + 1; i < end; ++i) {
            product *= ReadPointer(&arr, i);
            hasElements = true;
        }

        // Выводим результат
        if (hasElements) {
            cout << "Произведение элементов между минимальным и максимальным элементами: " << product << endl;
        }
        else {
            cout << "Между минимальным и максимальным элементами нет других элементов." << endl;
        }
    }

    // Демонстрация утечки памяти
    cout << "Демонстрация утечки памяти..." << endl;
    cout << "Выделяем память повторно без освобождения старой." << endl;
    if (!NewPointer(&arr, N * 2)) { // выделяем память в два раза больше
        cerr << "Программа завершена из-за ошибки выделения памяти." << endl;
        return 1;
    }

    // Выводим содержимое массива после утечки
    cout << "Содержимое массива после утечки памяти: ";
    for (int i = 0; i < N * 2; ++i) {
        cout << ReadPointer(&arr, i) << " "; // значения могут быть случайными
    }
    cout << endl;

    // Освобождаем память
    FreePointer(&arr);

    // Демонстрация ошибки записи и чтения после освобождения памяти
    cout << "Демонстрация ошибки записи и чтения после освобождения памяти..." << endl;
    WritePointer(&arr, 0, 10.5); // попытка записи в освобожденную память
    value = ReadPointer(&arr, 0); // попытка чтения из освобожденной памяти
    cout << "Прочитанное значение: " << value << endl;

    return 0;
}