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

void Enqueue(TQueue& Q, int value) {
    if (Q.size == Q.capacity) {
        cerr << "Ошибка: очередь переполнена! Элемент " << value << " не добавлен.\n";
        return;
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
    for (int i = 0; i < Q.size; i++) {
        cout << Q.data[current] << " ";
        current = (current + 1) % Q.capacity;
    }
    cout << endl;
}

void DeleteBetweenMinMax(TQueue& Q) {
    if (Q.size < 2) return;

    // Находим индексы min и max относительно head
    int minPos = 0, maxPos = 0;
    for (int i = 1; i < Q.size; ++i) {
        int currentPos = (Q.head + i) % Q.capacity;
        if (Q.data[currentPos] < Q.data[(Q.head + minPos) % Q.capacity]) {
            minPos = i;
        }
        if (Q.data[currentPos] > Q.data[(Q.head + maxPos) % Q.capacity]) {
            maxPos = i;
        }
    }

    // Определяем границы (start и end)
    int start = min(minPos, maxPos);
    int end = max(minPos, maxPos);

    // Если между start и end есть элементы
    if (end - start > 1) {
        // Создаём новый массив без удалённых элементов
        int newSize = Q.size - (end - start - 1);
        int* temp = new int[Q.capacity]; // Сохраняем исходный capacity

        // Копируем элементы до start и после end
        int index = 0;
        for (int i = 0; i <= start; ++i) {
            temp[index++] = Q.data[(Q.head + i) % Q.capacity];
        }
        for (int i = end; i < Q.size; ++i) {
            temp[index++] = Q.data[(Q.head + i) % Q.capacity];
        }

        // Заменяем старый массив
        delete[] Q.data;
        Q.data = temp;
        Q.head = 0;
        Q.tail = index % Q.capacity;
        Q.size = index;
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    TQueue Q;

    
    int capacity = InputNumber("Введите размер очереди (целое число > 0): ");
    while (capacity <= 0) {
        cout << "Размер должен быть положительным!\n";
        capacity = InputNumber("Введите размер очереди (целое число > 0): ");
    }
    InitQueue(Q, capacity);

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
            if (Q.size < Q.capacity) {
                Enqueue(Q, num);
            }
            else {
                cout << "Очередь заполнена! Максимальный размер: " << Q.capacity << "\n";
                break; // Завершаем ввод при переполнении
            }
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

    // Задание 1
    int toDequeue = min(5, Q.size);
    cout << "Извлечённые элементы: ";
    for (int i = 0; i < toDequeue; ++i) {
        cout << Dequeue(Q) << " ";
    }
    cout << "\nОчередь после извлечения: ";
    PrintQueue(Q);

    // Задание 2
    DeleteBetweenMinMax(Q);
    cout << "Очередь после удаления элементов между min и max: ";
    PrintQueue(Q);

    delete[] Q.data;
    system("pause"); 
    return 0;
}