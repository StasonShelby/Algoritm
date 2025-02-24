import os

def Chislo():
    while True:
        try:
            K = int(input("Введите целое число K (> 0): "))
            if K > 0:
                return K
            else:
                print("Число должно быть больше 0. Попробуйте снова.")
        except ValueError:
            print("Введено не целое число. Попробуйте снова.")

def checkfile(filename):
    if not os.path.exists(filename):
        print(f"Файл '{filename}' не существует.")
        return False
    if os.path.getsize(filename) == 0:
        print(f"Файл '{filename}' пуст.")
        return False
    return True

def process(filename, K):
    with open(filename, 'r', encoding='utf-8') as file:
        lines = file.readlines()

    with open('File1.txt', 'w', encoding='utf-8') as file1, \
         open('File2.txt', 'w', encoding='utf-8') as file2:
        for line in lines:
            part1 = line[:K]
            part2 = line[K:] if len(line) > K else ' '
            file1.write(part1 + '\n')
            file2.write(part2)

def main():
    K = Chislo()

    filename = input("Введите название текстового файла: ")
    if not checkfile(filename):
        return

    process(filename, K)
    print("Файлы успешно созданы: 'File1.txt' и 'File2.txt'.")

if __name__ == "__main__":
    main()