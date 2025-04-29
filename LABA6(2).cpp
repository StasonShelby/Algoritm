#include <iostream>
#include <limits>
#include <string>
#include <algorithm>
using namespace std;

struct TQueue {
    int* data;
    int head;
    int tail;
    int size;
    int capacity;
};

// Предварительное объявление функций
int Dequeue(TQueue& Q);
void Enqueue(TQueue& Q, int value);

// Функция для безопасного ввода числа
int InputNumber(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Ошибка: введите целое число!\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

void InitQueue(TQueue& Q, int capacity) {
    Q.data = new int[capacity];
    Q.head = Q.tail = Q.size = 0;
    Q.capacity = capacity;
}

// Теперь определение Enqueue может использовать Dequeue
void Enqueue(TQueue& Q, int value) {
    if (Q.size == Q.capacity) {
        Dequeue(Q); // Извлекаем самый старый элемент
    }
    Q.data[Q.tail] = value;
    Q.tail = (Q.tail + 1) % Q.capacity;
    Q.size++;
}

int Dequeue(TQueue& Q) {
    if (Q.size == 0) {
        cerr << "Очередь пуста!" << endl;
        return -1;
    }
    int value = Q.data[Q.head];
    Q.head = (Q.head + 1) % Q.capacity;
    Q.size--;
    return value;
}

void PrintQueue(const TQueue& Q) {
    int current = Q.head;
    for (int i = 0; i < Q.size; ++i) {
        cout << Q.data[current] << " ";
        current = (current + 1) % Q.capacity;
    }
    cout << endl;
}

void DeleteBetweenMinMax(TQueue& Q) {
    if (Q.size < 2) return;

    // Находим индексы минимального и максимального элементов
    int minIndex = Q.head;
    int maxIndex = Q.head;
    int current = Q.head;

    for (int i = 0; i < Q.size; ++i) {
        if (Q.data[current] < Q.data[minIndex]) {
            minIndex = current;
        }
        if (Q.data[current] > Q.data[maxIndex]) {
            maxIndex = current;
        }
        current = (current + 1) % Q.capacity;
    }

    // Определяем границы удаления
    int start = min(minIndex, maxIndex);
    int end = max(minIndex, maxIndex);

    // Если есть элементы между start и end
    if (end - start > 1) {
        // Создаем новую очередь только с нужными элементами
        TQueue temp;
        InitQueue(temp, Q.capacity);

        // Копируем элементы до start
        for (int i = 0; i <= start; ++i) {
            int pos = (Q.head + i) % Q.capacity;
            Enqueue(temp, Q.data[pos]);
        }

        // Копируем элементы после end
        for (int i = end; i < Q.size; ++i) {
            int pos = (Q.head + i) % Q.capacity;
            Enqueue(temp, Q.data[pos]);
        }

        // Заменяем исходную очередь
        delete[] Q.data;
        Q = temp;
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    TQueue Q;

    // Ввод размера очереди с проверкой
    int capacity = InputNumber("Введите размер очереди (целое число > 0): ");
    while (capacity <= 0) {
        cout << "Размер должен быть положительным!\n";
        capacity = InputNumber("Введите размер очереди (целое число > 0): ");
    }
    InitQueue(Q, capacity);

    // Ввод элементов
    cout << "Введите элементы очереди (целые числа). Для завершения введите 'q':\n";
    while (true) {
        cout << "> ";
        string input;
        cin >> input;

        if (input == "q" || input == "Q") {
            break;
        }

        try {
            int num = stoi(input);
            Enqueue(Q, num);
        }
        catch (...) {
            cout << "Ошибка: введите число или 'q' для выхода!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    if (Q.size == 0) {
        cout << "Очередь пуста. Завершение программы.\n";
        return 0;
    }

    cout << "\nИсходная очередь: ";
    PrintQueue(Q);

    // Задание 1: Извлечь 5 элементов или сколько есть
    int toDequeue = min(5, Q.size);
    cout << "Извлечённые элементы: ";
    for (int i = 0; i < toDequeue; ++i) {
        cout << Dequeue(Q) << " ";
    }
    cout << "\nОчередь после извлечения: ";
    PrintQueue(Q);

    // Задание 2: Удалить между min и max
    DeleteBetweenMinMax(Q);
    cout << "Очередь после удаления элементов между min и max: ";
    PrintQueue(Q);

    delete[] Q.data;
    system("pause"); // Добавим паузу перед завершением
    return 0;
}