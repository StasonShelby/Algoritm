def Simvol(A):
    simvols = {'5', '6', '7', '8', '9', '+', '-', '*', '/'}
    result = set()

    for char in A:
        if char in simvols:
            result.add(char)
    return result


def printset(setsim):
    if setsim:
        print("Элементы множества:")
        for i in setsim:
            print(i)
        print(f"Количество уникальных элементов: {len(setsim)}")
    else:
        print("Нет символов, удовлетворяющих условиям задания. Пожалуйста, попробуйте снова.")


while True:
    intA = input("Введите последовательность символов: ")
    sim = Simvol(intA)

    if sim:
        printset(sim)
        break
    else:
        print("Нет символов, удовлетворяющих условиям задания. Пожалуйста, попробуйте снова.")