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
    while True:
        try:
            num1, num2, op = int(input()), int(input()), input().strip()
            print(get_answer(num1, num2, op), flush=False)
        except EOFError:
            print(flush=True)
            break


if __name__ == '__main__':
    main()
