import string
import subprocess
import itertools
import pathlib

from tqdm import tqdm

ALPHABET = string.ascii_lowercase[:5]
PROG_NAME = pathlib.Path('cmake-build-debug/lab5')
STRING_LENGTH = 7


def generate_strings(length):
    chars = ALPHABET
    for item in itertools.product(chars, repeat=length):
        yield "".join(item)


def find_answer(s: str) -> str:
    ss = {s}
    for _ in range(len(s)):
        s = s[-1] + s[:-1]
        ss.add(s)
    return min(ss)


def run_test():
    with tqdm(total=len(ALPHABET)**STRING_LENGTH) as pb:
        for s in generate_strings(STRING_LENGTH):
            true_ans = find_answer(s)

            with subprocess.Popen([PROG_NAME], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True) as proc:
                out, err = proc.communicate(s.encode())
                out = out.decode().strip()
                try:
                    proc.wait(0)
                except subprocess.TimeoutExpired:
                    proc.kill()
                    raise RuntimeError(f'WARNING!: {err = !r}')
                pb.update()
                if out != true_ans:
                    raise RuntimeError(f'values are different: {s = !r}, {out = !r}, {true_ans = !r}')


if __name__ == '__main__':
    run_test()
