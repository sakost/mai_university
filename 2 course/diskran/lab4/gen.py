#!usr/bin/python

from random import *
from string import ascii_letters

def get_random_pattern_key():
    return ' '.join(''.join(choice('123456789') for i in range(randint(1,3))) for j in range(randint(1, 10)))

def get_random_text_key():
    return ''.join(choice('123456789') for i in range(randint(1,5)))

if __name__ == "__main__":
    pattern = ''
    text = ''
    keys = []
    answer = ''
    with open( "input.txt", 'w' ) as output_file:
        key = get_random_pattern_key()
        pattern = pattern + ' ' + key
        output_file.write("{0}\n".format( pattern.lstrip() ))

        for i in range(5000): # kolvo strok v file
            if i % 1000 == 0:
                print('Generate ', i)
            text = ''
            for x in range(1000): # kolvo chisel v str
                check = randint(1, 100000)
                if check % 2 == 0:
                    text = text + ' ' + key
                else:
                    other_key = get_random_text_key()
                    text = text + ' ' + other_key
            output_file.write("{0}\n".format( text.lstrip() ))
        with open('answer.t', 'w') as file_ans:
            file_ans.write(answer)
