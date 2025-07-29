help:
        just --list

build:
        python setup.py build_ext --inplace

rebuild:
        rm -rf build
        rm fast_pydb.*.so
        just build

test:
        nix build ".#testOciImage"
        nix eval --raw ".#testOciImage" | xargs docker load -i
        docker run -it --volume "$PWD:/code" \
                fast-pydb-python313-debug:latest /bin/bash tests/runner.sh
