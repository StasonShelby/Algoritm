#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <cmath>

using namespace std;

// Функция для проверки ввода
int getValidatedInput(const string& prompt) {
    int value;
    bool isValid = false;

    while (!isValid) {
        cout << prompt;
        if (cin >> value && value > 0) {
            isValid = true;
        }
        else {
            cout << "Ошибка: введите положительное целое число!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return value;
}

// Функция для генерации массива случайных двузначных чисел
void generateRandomArray(int* arr, int size) {
    srand(time(0));
    for (int i = 0; i < size; i++) {
        arr[i] = 10 + rand() % 90; // Двузначные числа от 10 до 99
    }
}

// Функция для вычисления разности цифр числа
int digitDifference(int num) {
    return abs((num / 10) - (num % 10));
}

// Сортировка прямым выбором с подсчётом операций
void selectionSort(int* arr, int size, int& comparisons, int& swaps) {
    comparisons = 0;
    swaps = 0;

    for (int i = 0; i < size - 1; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < size; j++) {
            if (digitDifference(arr[j]) > digitDifference(arr[maxIndex])) {
                comparisons++; // Считаем только значимые сравнения
                maxIndex = j;
            }
        }
        if (maxIndex != i) {
            swap(arr[i], arr[maxIndex]);
            swaps++;
        }
    }
}

// Подготовка отсортированного массива (прямого или обратного)
void prepareSortedArray(int* original, int* arr, int size, bool reverse = false) {
    copy(original, original + size, arr);
    if (reverse) {
        sort(arr, arr + size, [](int a, int b) { return digitDifference(a) > digitDifference(b); });
    }
    else {
        sort(arr, arr + size, [](int a, int b) { return digitDifference(a) < digitDifference(b); });
    }
}

// Подготовка частично отсортированного массива
void preparePartiallySortedArray(int* original, int* arr, int size, double sortedPart) {
    copy(original, original + size, arr);
    int sortedSize = static_cast<int>(size * sortedPart);
    sort(arr, arr + sortedSize, [](int a, int b) { return digitDifference(a) < digitDifference(b); });
}

// Вывод первых 5 элементов массива
void printFirstFive(int* arr, int size, const string& label) {
    cout << "[Проверка] " << label << ": ";
    int printCount = min(5, size);
    for (int i = 0; i < printCount; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
}

// Тестирование сортировки и вывод результатов
void testSorting(int* original, int size, const string& testName) {
    int* arr = new int[size];

    // Вывод исходного массива для проверки
    printFirstFive(original, size, "Исходный массив перед " + testName);

    // Подготовка массива в зависимости от типа теста
    if (testName == "Случайный массив") {
        copy(original, original + size, arr);
    }
    else if (testName == "Отсортированный массив") {
        prepareSortedArray(original, arr, size);
    }
    else if (testName == "Обратно отсортированный массив") {
        prepareSortedArray(original, arr, size, true);
    }
    else if (testName == "25% отсортированный") {
        preparePartiallySortedArray(original, arr, size, 0.25);
    }
    else if (testName == "50% отсортированный") {
        preparePartiallySortedArray(original, arr, size, 0.5);
    }
    else if (testName == "75% отсортированный") {
        preparePartiallySortedArray(original, arr, size, 0.75);
    }

    // Вывод копии перед сортировкой
    printFirstFive(arr, size, "Копия перед сортировкой (" + testName + ")");

    int comparisons, swaps;
    auto start = chrono::high_resolution_clock::now();
    selectionSort(arr, size, comparisons, swaps);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    cout << "| " << setw(25) << left << testName
        << " | " << setw(10) << comparisons
        << " | " << setw(8) << swaps
        << " | " << setw(12) << duration.count() << " |\n";

    delete[] arr;
}

// Вывод шапки таблицы
void printTableHeader() {
    cout << "| Тип массива              | Сравнения  | Обмены   | Время (сек)  |\n";
}

int main() {
    setlocale(LC_ALL, "RU");
    int size = getValidatedInput("Введите размер массива: ");
    int* originalArray = new int[size];
    generateRandomArray(originalArray, size);

    // Вывод всего исходного массива для наглядности
    cout << "\nИсходный массив полностью :\n";
    int printCount = min(10000, size);
    for (int i = 0; i < printCount; i++) {
        cout << originalArray[i] << " ";
    }
    cout << "\n\n";

    printTableHeader();

    // Тестируем разные типы массивов
    testSorting(originalArray, size, "Случайный массив");
    testSorting(originalArray, size, "Отсортированный массив");
    testSorting(originalArray, size, "Обратно отсортированный массив");
    testSorting(originalArray, size, "25% отсортированный");
    testSorting(originalArray, size, "50% отсортированный");
    testSorting(originalArray, size, "75% отсортированный");

    delete[] originalArray;
    return 0;
}