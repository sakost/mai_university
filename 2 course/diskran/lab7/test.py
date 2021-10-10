from pathlib import Path
import subprocess

import tqdm

MAX_N, MAX_M = 1000, 100

path_to_exec = Path('cmake-build-debug/lab7')
assert path_to_exec.exists() and path_to_exec.is_file()


def solve(n: int, m: int) -> int:
    ans = 0
    for i in range(1, n):
        if i % m == 0 and str(n) > str(i):
            ans += 1
    return ans


def run_tests():
    with tqdm.tqdm(total=MAX_M*MAX_N) as pbar:
        for n in range(1, MAX_N):
            for m in range(1, MAX_M):
                with subprocess.Popen([path_to_exec], stdin=subprocess.PIPE, stdout=subprocess.PIPE, shell=True) as proc:
                    out, _ = proc.communicate(f'{n} {m}\n'.encode())
                    # try:
                    if int(out.decode()) != (true_ans := solve(n, m)):
                        error_text = f'{n=}, {m=}, {true_ans=}, {out=}'
                        pbar.display(error_text)
                        raise ValueError(error_text)
                    # except ValueError:
                    #     pbar.display(f'{out=}, {err=}')
                    pbar.update(1)


if __name__ == '__main__':
    run_tests()
