import doctest

import fast_pydb


if __name__ == "__main__":
    print("running doctests.")
    result = doctest.testmod(fast_pydb.string)
    if result.failed == 0:
        print("OK.")
