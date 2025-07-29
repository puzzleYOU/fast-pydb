help:
        just --list

build:
        CFLAGS="-O0 -g -Wno-cpp" python setup.py build_ext

test:
        just build
        # unittests with valgrind
        valgrind --tool=memcheck \
        --leak-check=full \
        --show-leak-kinds=definite \
        --track-origins=yes \
        python tests/doctests.py
        # benchmarks without (slow) memory tracer. Note this is a debug build!
        python tests/benchmark.py
