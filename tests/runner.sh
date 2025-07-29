#!/bin/sh

python setup.py build_ext --inplace

# unittests with valgrind
valgrind --tool=memcheck \
    --leak-check=full \
    --show-leak-kinds=definite \
    --track-origins=yes \
python tests/doctests.py

# benchmarks without (slow) memory tracer. Note this is a debug build!
python tests/benchmark.py
