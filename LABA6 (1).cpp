#include <iostream>
#include <limits>
#include <string>
#include <sstream>
using namespace std;

struct Node {
    int data;
    Node* next;
};

struct TQueue {
    Node* Head;
    Node* Tail;
    int size;  // Добавим счетчик элементов
};

void InitQueue(TQueue& Q) {
    Q.Head = Q.Tail = nullptr;
    Q.size = 0;
}

void Enqueue(TQueue& Q, int value) {
    Node* newNode = new Node{ value, nullptr };
    if (Q.Tail) {
        Q.Tail->next = newNode;
    }
    else {
        Q.Head = newNode;
    }
    Q.Tail = newNode;
    Q.size++;
}

int Dequeue(TQueue& Q) {
    if (!Q.Head) {
        cerr << "Очередь пустая!" << endl;
        return -1;
    }
    int value = Q.Head->data;
    Node* temp = Q.Head;
    Q.Head = Q.Head->next;
    if (!Q.Head) {
        Q.Tail = nullptr;
    }
    delete temp;
    Q.size--;
    return value;
}

void PrintQueue(const TQueue& Q) {
    Node* current = Q.Head;
    while (current) {
        cout << current->data << " ";
        current = current->next;
    }
    cout << endl;
}

void ClearQueue(TQueue& Q) {
    while (Q.Head) {
        Dequeue(Q);
    }
}

void DeleteBetweenMinMax(TQueue& Q) {
    if (Q.size < 2) return;

    Node* minNode = Q.Head, * maxNode = Q.Head;
    Node* current = Q.Head;

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
        Q.size--;
    }
    start->next = end;

    // Обновляем Tail, если удалили последний элемент
    if (end == Q.Tail) {
        Q.Tail = start;
    }
}

int main() {
    setlocale(LC_ALL, "RU");
    TQueue Q;
    InitQueue(Q);

    cout << "Введите элементы очереди (целые числа). Для завершения введите 'q':\n";

    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);  // Используем getline вместо cin >>

        if (input == "q" || input == "Q") {
            break;
        }

        stringstream ss(input);
        int num;
        if (ss >> num) {
            // Проверяем, что в строке только число
            string remaining;
            if (ss >> remaining) {
                cout << "Ошибка: введите только одно число!\n";
            }
            else {
                Enqueue(Q, num);
            }
        }
        else {
            cout << "Ошибка: введите число или 'q' для выхода!\n";
        }
    }

    if (!Q.Head) {
        cout << "Очередь пуста. Завершение программы.\n";
        return 0;
    }

    cout << "Исходная очередь: ";
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

    ClearQueue(Q);
    return 0;
}