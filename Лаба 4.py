import random
import time
import copy


def print_table(headers, data):
    # Определяем ширину колонок
    col_widths = [
        max(len(str(row[i])) for row in [headers] + data)
    for i in range(len(headers))
    ]

    # Шапка таблицы
    header_row = " | ".join(
        f"{headers[i]:<{col_widths[i]}}" for i in range(len(headers))
    )
    separator = "-+-".join("-" * width for width in col_widths)

    print(header_row)
    print(separator)

    # Данные
    for row in data:
        print(" | ".join(
            f"{str(row[i]):<{col_widths[i]}}" for i in range(len(row))
        ))

# --- Ввод размера массива с проверкой ---
def get_array_size():
    while True:
        try:
            size = int(input("Введите размер массива (целое положительное число): "))
            if size <= 0:
                print("Ошибка: размер должен быть положительным!")
                continue
            return size
        except ValueError:
            print("Ошибка: введите целое число!")


# --- Методы сортировки ---

# 1. Сортировка прямым включением (Insertion Sort)
def insertion_sort(arr):
    comparisons = 0
    swaps = 0
    iterations = 0
    for i in range(1, len(arr)):
        key = arr[i]
        j = i - 1
        iterations += 1
        while j >= 0 and arr[j] > key:
            comparisons += 1
            arr[j + 1] = arr[j]
            swaps += 1
            j -= 1
        comparisons += 1 if j >= 0 else 0  # Учитываем последнее сравнение при выходе
        arr[j + 1] = key
    return iterations, comparisons, swaps


# 2. Сортировка прямым выбором (Selection Sort)
def selection_sort(arr):
    comparisons = 0
    swaps = 0
    iterations = 0
    for i in range(len(arr)):
        min_idx = i
        iterations += 1
        for j in range(i + 1, len(arr)):
            comparisons += 1
            if arr[j] < arr[min_idx]:
                min_idx = j
        if min_idx != i:
            arr[i], arr[min_idx] = arr[min_idx], arr[i]
            swaps += 1
    return iterations, comparisons, swaps


# 3. Сортировка прямым обменом (Bubble Sort)
def bubble_sort(arr):
    comparisons = 0
    swaps = 0
    iterations = 0
    n = len(arr)
    for i in range(n - 1):
        iterations += 1
        for j in range(n - i - 1):
            comparisons += 1
            if arr[j] > arr[j + 1]:
                arr[j], arr[j + 1] = arr[j + 1], arr[j]
                swaps += 1
    return iterations, comparisons, swaps


# 4. Быстрая сортировка (Quick Sort) + подсчёт операций
def quick_sort(arr, low=0, high=None, stats=None):
    if stats is None:
        stats = {'iterations': 0, 'comparisons': 0, 'swaps': 0}
    if high is None:
        high = len(arr) - 1

    # Используем стек вместо рекурсии
    stack = []
    stack.append((low, high))

    while stack:
        low, high = stack.pop()
        if low < high:
            stats['iterations'] += 1
            pi = partition(arr, low, high, stats)

            # Сначала добавляем меньший подмассив в стек
            if pi - low < high - pi:
                stack.append((low, pi - 1))
                stack.append((pi + 1, high))
            else:
                stack.append((pi + 1, high))
                stack.append((low, pi - 1))

    return stats['iterations'], stats['comparisons'], stats['swaps']


def partition(arr, low, high, stats):
    pivot = arr[high]
    i = low - 1
    for j in range(low, high):
        stats['comparisons'] += 1
        if arr[j] <= pivot:
            i += 1
            if i != j:
                arr[i], arr[j] = arr[j], arr[i]
                stats['swaps'] += 1
    if i + 1 != high:
        arr[i + 1], arr[high] = arr[high], arr[i + 1]
        stats['swaps'] += 1
    return i + 1

def print_results(results):
    print("\nРезультаты сортировки:")
    print("+---------------------+------------+-------------+---------------+----------------+")
    print("| Метод               | Итерации   | Сравнения   | Перестановки  | Время          |")
    print("+=====================+============+=============+===============+================+")
    for row in results:
        print(f"| {row[0]:<19} | {row[1]:<10} | {row[2]:<11} | {row[3]:<13} | {row[4]:<14} |")
    print("+---------------------+------------+-------------+---------------+----------------+")

def prepare_partially_sorted(arr, percent):
    """Сортирует первые `percent`% массива по возрастанию."""
    if percent <= 0 or percent > 100:
        return arr
    split_idx = len(arr) * percent // 100
    arr[:split_idx] = sorted(arr[:split_idx])
    return arr

def analyze_sorted_versions(original_array):
    # Создаём 5 копий
    copy1 = copy.deepcopy(original_array)
    copy2 = copy.deepcopy(original_array)
    copy3 = copy.deepcopy(original_array)
    copy4 = copy.deepcopy(original_array)
    copy5 = copy.deepcopy(original_array)

    # Подготовка копий
    copy1.sort()  # Полная сортировка по возрастанию
    copy2.sort(reverse=True)  # По убыванию
    copy3 = prepare_partially_sorted(copy3, 25)  # 25% отсортировано
    copy4 = prepare_partially_sorted(copy4, 50)  # 50%
    copy5 = prepare_partially_sorted(copy5, 75)  # 75%

    # Список всех копий для анализа
    versions = [
        ("Полностью отсортирован (возрастание)", copy1),
        ("Отсортирован в обратном порядке (убывание)", copy2),
        ("25% отсортировано", copy3),
        ("50% отсортировано", copy4),
        ("75% отсортировано", copy5)
    ]

    # Анализ каждой версии
    for name, arr in versions:
        print(f"\nАнализ: {name}")
        results = []
        arr_copy = copy.deepcopy(arr)

        # Тестируем все 4 метода сортировки
        methods = [
            ("Прямое включение", insertion_sort),
            ("Прямой выбор", selection_sort),
            ("Прямой обмен", bubble_sort),
            ("Быстрая сортировка", quick_sort)
        ]

        for method_name, sort_func in methods:
            temp_arr = copy.deepcopy(arr_copy)
            start_time = time.time()
            iterations, comparisons, swaps = sort_func(temp_arr)
            end_time = time.time()
            results.append([
                method_name,
                iterations,
                comparisons,
                swaps,
                f"{(end_time - start_time):.6f} сек"
            ])

        # Вывод таблицы для текущей версии
        headers = ["Метод", "Итерации", "Сравнения", "Перестановки", "Время"]
        print_table(headers, results)

def digit_difference(num):
    return (num // 10) - (num % 10)  # Десятки - единицы

def sort_by_digit_diff(arr):
    return sorted(arr, key=digit_difference, reverse=True)

# --- Основная программа ---
def main():
    size = get_array_size()
    original_array = [random.randint(0, 1000) for _ in range(size)]  # Генерация случайного массива

    # Создаём 4 копии
    arr1 = copy.deepcopy(original_array)
    arr2 = copy.deepcopy(original_array)
    arr3 = copy.deepcopy(original_array)
    arr4 = copy.deepcopy(original_array)

    # Словарь для хранения результатов
    results = []

    # Сортировка 1: Прямое включение
    start_time = time.time()
    iterations, comparisons, swaps = insertion_sort(arr1)
    end_time = time.time()
    results.append([
        "Прямое включение",
        iterations,
        comparisons,
        swaps,
        f"{(end_time - start_time):.6f} сек"
    ])

    # Сортировка 2: Прямой выбор
    start_time = time.time()
    iterations, comparisons, swaps = selection_sort(arr2)
    end_time = time.time()
    results.append([
        "Прямой выбор",
        iterations,
        comparisons,
        swaps,
        f"{(end_time - start_time):.6f} сек"
    ])

    # Сортировка 3: Прямой обмен
    start_time = time.time()
    iterations, comparisons, swaps = bubble_sort(arr3)
    end_time = time.time()
    results.append([
        "Прямой обмен",
        iterations,
        comparisons,
        swaps,
        f"{(end_time - start_time):.6f} сек"
    ])

    # Сортировка 4: Быстрая сортировка
    start_time = time.time()
    iterations, comparisons, swaps = quick_sort(arr4)
    end_time = time.time()
    results.append([
        "Быстрая сортировка",
        iterations,
        comparisons,
        swaps,
        f"{(end_time - start_time):.6f} сек"
    ])

    print_results(results)

    analyze_sorted_versions(original_array)

    print("\n" + "="*50 + "\nТретий пункт: Сортировка двузначных чисел по разности цифр\n" + "="*50)
    size = get_array_size()
    two_digit_array = [random.randint(10, 99) for _ in range(size)]

    # Сортировка по разности цифр
    sorted_by_diff = sort_by_digit_diff(two_digit_array)
    print("\nМассив после сортировки по разности цифр (первые 10 элементов):")
    print(sorted_by_diff[:10])

    # Проверка разностей
    print("\nПроверка разностей:")
    for num in sorted_by_diff[:10]:
        print(f"{num} → {digit_difference(num)}")

    # Сортировка прямым выбором
    arr_for_selection = copy.deepcopy(sorted_by_diff)
    iterations, comparisons, swaps = selection_sort(arr_for_selection)
    print("\nРезультат сортировки прямым выбором:")
    print(f"Итерации: {iterations}, Сравнения: {comparisons}, Перестановки: {swaps}")
    print("Первые 10 элементов:", arr_for_selection[:10])

if __name__ == "__main__":
    main()
