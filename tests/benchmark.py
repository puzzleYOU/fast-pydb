import re
from time import time
from typing import Callable

import fast_pydb

SAMPLES = 1_000_000


###############################################################################
# slow Python implementation
###############################################################################

def python_old_clean_string(s):
    if isinstance(s, str):
        return re.sub(r"[ ]+", " ", s).strip()
    else:
        return None


###############################################################################
# faster Python implementation
###############################################################################

pattern = re.compile(r"\s+")


def python_optimized_clean_string(s):
    if s is not None:
        return pattern.sub(" ", s).strip()


###############################################################################
# microbenchmark implementation
###############################################################################


def bench(label: str, impl: Callable) -> float:
    print(f"==== timing {label}")
    t0 = time()
    for _ in range(SAMPLES):
        result = impl(
            "   i    am     a string with   MORE and    less  spaces  "
        )
        if result != "i am a string with MORE and less spaces":
            raise AssertionError(f"got {result}")
    dt = time() - t0
    print(f"label: {dt}")
    return dt


if __name__ == "__main__":
    c_impl = bench("C implementation", fast_pydb.string.clean_string)
    python_old = bench("Python, old (very slow)", python_old_clean_string)
    python_opt = bench("Python, optimized", python_optimized_clean_string)

    if not (c_impl < python_opt < python_old):
        raise AssertionError()
