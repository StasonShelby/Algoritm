import os
import re

def checkfile(filename):
    if not os.path.exists(filename):
        print(f"Файл '{filename}' не существует.")
        return False
    if os.path.getsize(filename) == 0:
        print(f"Файл '{filename}' пуст.")
        return False
    return True

def Chisla(filename):
    with open(filename, 'r', encoding='utf-8') as file:
        lines = file.readlines()

    with open('g.txt', 'w', encoding='utf-8') as outfile:
        for linenumber, line in enumerate(lines, start=1):
            number = ''
            for char in line:
                if char.isdigit() or (char == '-' and number == ''):
                    number += char
                else:
                    if number:
                        outfile.write(f"{number} - {linenumber}\n")
                        number = ''
            if number:
                outfile.write(f"{number} - {linenumber}\n")

def Print(filename):
    print(f"\nСодержимое файла '{filename}':")
    with open(filename, 'r', encoding='utf-8') as file:
        print(file.read())

def main():
    filename = input("Введите название текстового файла: ")
    if not checkfile(filename):
        return

    Chisla(filename)
    print("Числа успешно извлечены и записаны в файл 'g.txt'.")

    # Вывод содержимого файла g.txt
    Print('g.txt')

if __name__ == "__main__":
    main()