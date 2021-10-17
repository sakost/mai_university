import random
from string import ascii_lowercase

STR_LEN = 1000000

def main():
    print(''.join(random.choices(ascii_lowercase, k=STR_LEN)))

if __name__ == '__main__':
    main()

