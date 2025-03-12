#include <iostream>
#include <cstdlib> // для malloc и free
#include <cmath>   // для abs()
#include <Windows.h> // для поддержки русского языка
#include <string>  // для std::string
#include <sstream> // для std::stringstream
using namespace std;

// Функция для безопасного ввода размера массива
int safeInputSize() {
    string input;
    int value;
    while (true) {
        cout << "Введите размер массива N: ";
        getline(std::cin, input); // считываем всю строку
        stringstream ss(input);
        if (ss >> value && value >= 0 && ss.eof()) { // проверяем, что ввод корректен
            break;
        }
        cerr << "Ошибка: размер массива должен быть неотрицательным целым числом без пробелов " << endl;
    }
    return value;
}

// Структура для представления сегмента динамической памяти
struct PointerSegment {
    int* pointer; // указатель на динамический массив
    size_t size;  // размер массива
};

// Функция для выделения памяти под массив
PointerSegment* NewPointer(PointerSegment* p, size_t size) {
    if (p->pointer != nullptr) {
        cerr << "Предупреждение: утечка памяти! Старая память не была освобождена." << endl;
    }
    p->pointer = static_cast<int*>(malloc(size * sizeof(int))); // выделяем память
    if (p->pointer == nullptr && size > 0) {
        cerr << "Ошибка: не удалось выделить память!" << std::endl;
        return nullptr; // возвращаем nullptr в случае ошибки
    }
    p->size = size; // сохраняем размер массива
    return p;
}

// Функция для записи значения в массив по указателю
void WritePointer(PointerSegment* p, int offset, int value) {
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
int ReadPointer(PointerSegment* p, int offset) {
    if (p->pointer == nullptr) {
        std::cerr << "Ошибка: попытка чтения из nullptr!" << std::endl;
        return -1; // возвращаем -1 в случае ошибки
    }
    if (offset >= 0 && offset < p->size) {
        return *(p->pointer + offset); // читаем значение по указателю
    }
    else {
        std::cerr << "Ошибка: выход за пределы массива!" << std::endl;
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
    cout << "запись в nullptr..." << endl;
    WritePointer(&arr, 0, 10); // попытка записи в nullptr

    // Демонстрация ошибки чтения из nullptr
    cout << " чтение из nullptr..." << endl;
    int value = ReadPointer(&arr, 0); // попытка чтения из nullptr
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

        int value;
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

    // Выводим содержимое массива до утечки
    if (N > 0) {
        cout << "Содержимое массива до утечки памяти: ";
        for (int i = 0; i < N; ++i) {
            cout << ReadPointer(&arr, i) << " ";
        }
        cout << endl;
    }

    // Находим минимальный по модулю элемент
    if (N > 0) {
        int minAbsOffset = 0; // смещение минимального по модулю элемента
        int minAbsValue = abs(ReadPointer(&arr, 0)); // значение минимального по модулю элемента
        for (int i = 1; i < N; ++i) {
            int currentAbsValue = abs(ReadPointer(&arr, i));
            if (currentAbsValue < minAbsValue) {
                minAbsValue = currentAbsValue;
                minAbsOffset = i;
            }
        }

        // Вычисляем сумму модулей элементов после минимального по модулю элемента
        int sum = 0;
        int K = 0;
        for (int i = minAbsOffset + 1; i < N; ++i) {
            sum += abs(ReadPointer(&arr, i));
            ++K;
        }

        // Выводим результат
        cout << "Сумма модулей элементов после минимального по модулю элемента: " << sum << endl;
        cout << "Количество таких элементов: " << K << endl;
    }

    // Демонстрация утечки памяти
    cout << "Демонстрация утечки памяти..." << std::endl;
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
    std::cout << std::endl;

    // Освобождаем память
    FreePointer(&arr);

    // Демонстрация ошибки записи и чтения после освобождения памяти
    cout << "Демонстрация ошибки записи и чтения после освобождения памяти..." << endl;
    WritePointer(&arr, 0, 10); // попытка записи в освобожденную память
    value = ReadPointer(&arr, 0); // попытка чтения из освобожденной памяти
    cout << "Прочитанное значение: " << value << endl;

    return 0;
}