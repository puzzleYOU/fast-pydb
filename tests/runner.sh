#!/bin/sh

python3 setup.py build_ext --inplace

# unittests with valgrind
valgrind --tool=memcheck \
    --leak-check=full \
    --show-leak-kinds=definite \
    --track-origins=yes \
python3 tests/doctests.py

# benchmarks without (slow) memory tracer. Note this is a debug build!
python3 tests/benchmark.py
