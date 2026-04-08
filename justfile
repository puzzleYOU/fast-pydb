help:
        just --list

build:
        python setup.py build_ext --inplace

rebuild:
        rm -rf build
        rm fast_pydb.*.so
        just build

test:
        nix run .#tests
