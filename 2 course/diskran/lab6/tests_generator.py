#! /usr/bin/env python

import sys
import random
import os

# Максимально возможное значение числа.
MAX_POW = 20
MAX_VALUE = 100 ** MAX_POW
MIN_VALUE = 0

MAX_POW_FOR_POW = 2


def get_random_int(MM=None):
    if MM is None: MM = MAX_POW
    return random.randint(MIN_VALUE, 100 ** random.randint(1, MM))


def get_random_nums(MM=None):
    return get_random_int(MM), get_random_int(MM)


def get_answer(num1, num2, operation):
    if operation == "+":
        return str(num1 + num2)
    if operation == "-":
        return str(num1 - num2) if num1 >= num2 else "Error"
    if operation == "*":
        return str(num1 * num2)
    if operation == "<":
        # num1 < num2 вернёт True или False, чтобы
        # получить true или false, то сначала необходимо
        # привести его к int'у, а потом уже в строку.
        return str(num1 < num2).lower()
    if operation == "=":
        # Аналогично как для оператора <.
        return str(num1 == num2).lower()
    if operation == "^":
        if num1 == 0 and num2 == 0:
            return "Error"
        return str(num1 ** num2)
    if operation == "/":
        if num2 == 0:
            return "Error"
        return str(num1 // num2)
    return None


def main():
    # Ожидаем, что будет три аргумента: название программы,
    # путь до директории с тестами и количество тестов в каждом
    # файле.
    if len(sys.argv) != 3:
        print(f"Usage: {sys.argv[0]} <tests directory> <tests count>")
        sys.exit(1)

    # Считываем путь до папки с тестами.
    test_dir = sys.argv[1]
    # Считываем количество тестов для каждой операции.
    tests_count = int(sys.argv[2])

    # Создаем папку с тестами
    try:
        os.mkdir(f"{test_dir}")
    except FileExistsError as exc:
        raise FileExistsError(f"Папка {test_dir} уже существует. Переименуйте её или удалите") from exc

    # Пробегаемся по операциям, для которых мы хотим
    # сгенерировать тесты.
    for enum, operation in enumerate(["+", "-", "*", "<", "=", "^", "/"]):
        # Открываем файлы для записи самих тестов и ответов
        # к ним.
        filename_pattern = f'{test_dir}/{enum + 1:02}'
        with open(f'{filename_pattern}.t', 'w') as test_file, \
                open(f'{filename_pattern}.a', 'w') as answer_file:
            for _ in range(0, tests_count):
                MM = None
                if operation == "^":
                    MM = MAX_POW_FOR_POW
                # Генерируем рандомные большие числа.
                num1, num2 = get_random_nums(MM)
                # Записываем в файл получившийся тест.
                test_file.write(f"{num1}\n{num2}\n{operation}\n")
                # Получаем ответ в виде строки и записываем его
                # в файл с ответами.
                answer = get_answer(num1, num2, operation)
                answer_file.write(f"{answer}\n")


if __name__ == "__main__":
    main()
