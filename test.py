import doctest
import re
from time import time
from typing import Callable

import fast_pydb

SAMPLES = 1_000_000


def python_old_clean_string(s):
    if isinstance(s, str):
        return re.sub(r"\s+", " ", s).strip()
    else:
        return None


pattern = re.compile(r"\s+")
def python_optimized_clean_string(s):
    if s is not None:
        return pattern.sub(" ", s).strip()



def bench(label: str, impl: Callable) -> float:
    print(f"==== timing {label}")
    t0 = time()
    for _ in range(SAMPLES):
        res = impl("   li  la  lächerlich   ")
        assert res == "li la lächerlich", res
    dt = time() - t0
    print(dt)
    return dt


if __name__ == '__main__':
    doctest.testmod(fast_pydb.string)

    c_impl = bench("C implementation", fast_pydb.string.clean_string)
    python_old = bench("Python, old (very slow)", python_old_clean_string)
    python_opt = bench("Python, optimized", python_optimized_clean_string)

    assert c_impl < python_opt < python_old
