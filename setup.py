#!/usr/bin/python3

from glob import glob

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext

extensions_config = {
    "fast_pydb": {
        "sources": glob("fast_pydb/**/*.c", recursive=True),
        "extra_compile_args": {
            "msvc": [
                # treat warnings as errors
                "/WX",
            ],
            "unix": [
                # treat warnings as errors
                "-Wno-cpp",
                "-O0",
                "-Werror",
                "-Wno-missing-braces",
            ],
        },
        "extra_link_args": {
            "msvc": [],
            "unix": [],
        },
    }
}


class ExtraCompileArgsExtension(build_ext):
    def build_extension(self, ext: Extension):
        ctype = self.compiler.compiler_type
        config = extensions_config[ext.name]
        ext.extra_compile_args = config["extra_compile_args"][ctype]
        ext.extra_link_args = config["extra_link_args"][ctype]
        super().build_extension(ext)


setup(
    name="fast_pydb",
    version="0.1.0",
    license="MIT",

    description="fast implementations of various utility functions",
    author="puzzleYOU GmbH",

    packages=[
        "fast_pydb",
    ],
    ext_modules=[
        Extension(name, **config)
        for name, config in extensions_config.items()
    ],
    cmdclass={'build_ext': ExtraCompileArgsExtension},
)
