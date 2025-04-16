import random
import time
import copy


def get_array_size():
    """Функция для ввода размера массива с проверкой"""
    while True:
        try:
            size = int(input("Введите размер массива (целое положительное число): "))
            if size <= 0:
                print("Ошибка: размер должен быть положительным!")
                continue
            return size
        except ValueError:
            print("Ошибка: введите целое число!")


def print_table(headers, data):
    """Функция для вывода таблицы"""
    col_widths = [
        max(len(str(row[i])) for row in [headers] + data)
        for i in range(len(headers))
    ]

    header_row = " | ".join(f"{headers[i]:<{col_widths[i]}}" for i in range(len(headers)))
    separator = "-+-".join("-" * width for width in col_widths)

    print(header_row)
    print(separator)

    for row in data:
        print(" | ".join(f"{str(row[i]):<{col_widths[i]}}" for i in range(len(row))))


def get_search_target():
    """Функция для ввода числа с проверкой"""
    while True:
        try:
            target = int(input("Введите число для поиска (от 0 до 10000): "))
            if target < 0 or target > 10000:
                print("Ошибка: число должно быть от 0 до 10000!")
                continue
            return target
        except ValueError:
            print("Ошибка: введите целое число!")


def linear_search(arr, target):
    """Линейный поиск с подсчётом сравнений"""
    comparisons = 0
    for i in range(len(arr)):
        comparisons += 1
        if arr[i] == target:
            return i, comparisons
    return -1, comparisons


def run_linear_search_part():
    """Основная функция для выполнения линейного поиска"""
    sizes = [20, 500, 1000, 3000, 5000, 10000]
    target = get_search_target()
    results = []

    for size in sizes:
        original_array = [random.randint(0, 10000) for _ in range(size)]
        arr_copy = copy.deepcopy(original_array)

        start_time = time.time()
        index, comparisons = linear_search(arr_copy, target)
        end_time = time.time()

        results.append([
            size,
            comparisons,
            f"{(end_time - start_time):.6f} сек",
            "Найден (индекс: " + str(index) + ")" if index != -1 else "Не найден"
        ])

    headers = ["Размер", "Сравнения", "Время", "Результат"]
    print("\n" + "=" * 60 + "\nЛинейный поиск числа " + str(target) + "\n" + "=" * 60)
    print_table(headers, results)


def prepare_partially_sorted(arr, percent):
    """Сортирует первые percent% элементов массива"""
    if 0 < percent <= 100:
        split_idx = len(arr) * percent // 100
        arr[:split_idx] = sorted(arr[:split_idx])
    return arr


def hybrid_search(arr, target):
    """Гибридный поиск: бинарный в отсортированной части + линейный в остатке"""
    # Находим границу отсортированной части
    sorted_len = 0
    for i in range(1, len(arr)):
        if arr[i] < arr[i - 1]:
            break
        sorted_len = i + 1

    comparisons = 0

    # Бинарный поиск в отсортированной части
    if sorted_len > 0:
        left, right = 0, sorted_len - 1
        while left <= right:
            mid = (left + right) // 2
            comparisons += 1
            if arr[mid] == target:
                return mid, comparisons
            elif arr[mid] < target:
                left = mid + 1
            else:
                right = mid - 1

    # Линейный поиск в остатке
    for i in range(sorted_len, len(arr)):
        comparisons += 1
        if arr[i] == target:
            return i, comparisons

    return -1, comparisons


def run_search_comparison():
    size = get_array_size()
    original_array = [random.randint(0, 10000) for _ in range(size)]
    target = get_search_target()

    # Создаем список для копий (теперь только 5 копий)
    copies = [
        ["Полная сортировка", copy.deepcopy(original_array)],
        ["Обратная сортировка", copy.deepcopy(original_array)],
        ["25% отсортировано", copy.deepcopy(original_array)],
        ["50% отсортировано", copy.deepcopy(original_array)],
        ["75% отсортировано", copy.deepcopy(original_array)]
    ]

    # Подготовка копий
    copies[0][1].sort()  # Полная сортировка
    copies[1][1].sort(reverse=True)  # Обратная сортировка
    copies[2][1] = prepare_partially_sorted(copies[2][1], 25)
    copies[3][1] = prepare_partially_sorted(copies[3][1], 50)
    copies[4][1] = prepare_partially_sorted(copies[4][1], 75)

    results = []
    for name, arr in copies:
        # Линейный поиск
        start = time.time()
        lin_idx, lin_comp = linear_search(arr, target)
        lin_time = time.time() - start

        # Гибридный поиск
        start = time.time()
        hyb_idx, hyb_comp = hybrid_search(arr, target)
        hyb_time = time.time() - start

        results.append([
            name,
            f"{lin_time:.6f}",
            lin_comp,
            f"{hyb_time:.6f}",
            hyb_comp,
            "✓" if lin_idx == hyb_idx else "✗"
        ])

    headers = [
        "Тип массива", "Линейное время", "Лин. сравнения",
        "Гибридное время", "Гибр. сравнения", "Совпадение"
    ]
    print("\n" + "=" * 90)
    print("Сравнение линейного и гибридного поиска".center(90))
    print("=" * 90)
    print_table(headers, results)


def linear_search_with_barrier(arr, target):
    """Линейный поиск с барьером (сторожевым элементом)"""
    comparisons = 0
    n = len(arr)
    arr.append(target)  # Добавляем барьер

    i = 0
    while True:
        comparisons += 1
        if arr[i] == target:
            break
        i += 1

    arr.pop()  # Убираем барьер (чтобы не менять исходный массив)
    return (i, comparisons) if i < n else (-1, comparisons)


def run_barrier_search_comparison():
    """Сравнение обычного и барьерного линейного поиска"""
    size = get_array_size()
    target = get_search_target()

    original_array = [random.randint(0, 10000) for _ in range(size)]

    # Создаем две копии
    arr_normal = copy.deepcopy(original_array)
    arr_barrier = copy.deepcopy(original_array)

    # Обычный линейный поиск
    start_time = time.time()
    normal_idx, normal_comparisons = linear_search(arr_normal, target)
    normal_time = time.time() - start_time

    # Линейный поиск с барьером
    start_time = time.time()
    barrier_idx, barrier_comparisons = linear_search_with_barrier(arr_barrier, target)
    barrier_time = time.time() - start_time

    # Формируем таблицу
    headers = ["Метод", "Время", "Сравнения", "Результат"]
    data = [
        ["Обычный", f"{normal_time:.6f} сек", normal_comparisons,
         f"Найден (индекс: {normal_idx})" if normal_idx != -1 else "Не найден"],
        ["С барьером", f"{barrier_time:.6f} сек", barrier_comparisons,
         f"Найден (индекс: {barrier_idx})" if barrier_idx != -1 else "Не найден"],
    ]

    print("\n" + "=" * 70)
    print("Сравнение обычного и барьерного линейного поиска".center(70))
    print("=" * 70)
    print_table(headers, data)

def main():
    # Пункт 1 (не изменяем)
    print("\n" + "=" * 50 + "\nПервый пункт: Линейный поиск\n" + "=" * 50)
    run_linear_search_part()

    # Пункт 2 (новый)
    print("\n" + "=" * 50 + "\nВторой пункт: Сравнение методов поиска\n" + "=" * 50)
    run_search_comparison()

# Пункт 3 (новый)
    print("\n" + "=" * 50 + "\nТретий пункт: Поиск с барьером\n" + "=" * 50)
    run_barrier_search_comparison()

if __name__ == "__main__":
    main()