help:
        just --list

build:
        python setup.py build_ext --inplace

test:
        nix build ".#testOciImage"
        nix eval --raw ".#testOciImage" | xargs docker load -i
        docker run -it --volume "$PWD:/code" \
                fast-pydb-python313-debug:latest /bin/bash tests/runner.sh
