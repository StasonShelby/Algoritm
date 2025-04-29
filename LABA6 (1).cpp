#include <iostream>
#include<limits>
#include<sstream>
using namespace std;

struct Node {
    int data;
    Node* next;
};

struct TQueue {
    Node* Head;
    Node* Tail;
};

// Инициализация очереди
void InitQueue(TQueue& Q) {
    Q.Head = Q.Tail = nullptr;
}

// Добавление элемента в очередь
void Enqueue(TQueue& Q, int value) {
    Node* newNode = new Node{ value, nullptr };
    if (Q.Tail) {
        Q.Tail->next = newNode;
    }
    else {
        Q.Head = newNode;
    }
    Q.Tail = newNode;
}

// Извлечение элемента из очереди (задание 1)
int Dequeue(TQueue& Q) {
    if (!Q.Head) {
        cerr << "Очередь пуста!" << endl;
        return -1; // Ошибка
    }
    int value = Q.Head->data;
    Node* temp = Q.Head;
    Q.Head = Q.Head->next;
    if (!Q.Head) {
        Q.Tail = nullptr;
    }
    delete temp;
    return value;
}

// Вывод очереди
void PrintQueue(const TQueue& Q) {
    Node* current = Q.Head;
    while (current) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}

int InputNumber(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) { // Если ввод некорректен
            cin.clear(); // Сбрасываем флаг ошибки
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очищаем буфер
            cout << "Ошибка: введите число!\n";
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Очищаем лишние символы
            return value;
        }
    }
}

// Удаление элементов между min и max (задание 2)
void DeleteMinMax(Node*& head) {
    if (!head || !head->next) return;

    Node* minNode = head, * maxNode = head;
    Node* current = head;

    // Находим min и max
    while (current) {
        if (current->data < minNode->data) minNode = current;
        if (current->data > maxNode->data) maxNode = current;
        current = current->next;
    }

    // Определяем границы
    Node* start = (minNode->data < maxNode->data) ? minNode : maxNode;
    Node* end = (start == minNode) ? maxNode : minNode;

    // Удаляем элементы между start и end
    current = start->next;
    while (current && current != end) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
    start->next = end;
}

int main() {
    setlocale(LC_ALL, "RU");

    TQueue Q;
    InitQueue(Q);

    cout << "Введите элементы очереди (целые числа). Для завершения введите 'q':\n";

    while (true) {
        cout << "> ";
        string input;
        cin >> input;

        if (input == "q" || input == "Q") {
            break;
        }

        stringstream ss(input);
        int num;
        if (ss >> num) {
            Enqueue(Q, num);
        }
        else {
            cout << "Ошибка: введите число или 'q' для выхода!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Проверка на пустую очередь
    if (!Q.Head) {
        cout << "Очередь пуста. Завершение программы.\n";
        return 0;
    }

    cout << "Исходная очередь: ";
    PrintQueue(Q);

    // Задание 1: Извлечь 5 элементов
    cout << "Извлечённые элементы: ";
    for (int i = 0; i < 5; ++i) {
        cout << Dequeue(Q) << " ";
    }
    cout << "\nОчередь после извлечения: ";
    PrintQueue(Q);

    // Задание 2: Удалить между min и max
    DeleteMinMax(Q.Head);
    cout << "Очередь после удаления элементов между min и max: ";
    PrintQueue(Q);

    return 0;
}