help:
        just --list

build:
        CFLAGS="-O0 -g -Wno-cpp" python setup.py build_ext

leak-check:
        just build
        valgrind --tool=memcheck \
        --leak-check=full \
        --show-leak-kinds=definite \
        --track-origins=yes \
        python test.py
