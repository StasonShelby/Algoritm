def Point(x1, y1, x2, y2):
    points = set()
    for x in range(min(x1, x2), max(x1, x2) + 1):
        for y in range(min(y1, y2), max(y1, y2) + 1):
            points.add((x, y))
    return points

def peresechenie(a, b):
    return not (a[2] < b[0] or a[0] > b[2] or
                a[3] < b[1] or a[1] > b[3])

def Сpoints(a, b):
    common = set()
    for x in range(max(a[0], b[0]), min(a[2], b[2]) + 1):
        for y in range(max(a[1], b[1]), min(a[3], b[3]) + 1):
            common.add((x, y))
    return common

def printset(setpoint):
    if setpoint:
        print("Общие точки:")
        for point in setpoint:
            print(point)
        print(f"Количество общих точек: {len(setpoint)}")
    else:
        print("Нет общих точек.")

def inputcoord(prompt):
    while True:
        try:
            coords = list(map(int, input(prompt).split()))
            if len(coords) != 4 or any(c < 1 or c > 10 for c in coords):
                raise ValueError
            return coords
        except ValueError:
            print("Неправильный ввод. Пожалуйста, введите четыре натуральных числа от 1 до 10 через пробел.")

x1_a, y1_a, x2_a, y2_a = inputcoord("Введите координаты первого прямоугольника (x1 y1 x2 y2): ")
x1_b, y1_b, x2_b, y2_b = inputcoord("Введите координаты второго прямоугольника (x1 y1 x2 y2): ")

a = Point(x1_a, y1_a, x2_a, y2_a)
b = Point(x1_b, y1_b, x2_b, y2_b)

if peresechenie((x1_a, y1_a, x2_a, y2_a), (x1_b, y1_b, x2_b, y2_b)):
    common = Сpoints((x1_a, y1_a, x2_a, y2_a), (x1_b, y1_b, x2_b, y2_b))
    printset(common)
else:
    print("Прямоугольники не пересекаются.")