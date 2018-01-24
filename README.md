# Cubically [![Build Status](https://travis-ci.org/aaronryank/Cubically.svg?branch=master)](https://travis-ci.org/aaronryank/Cubically) [![Code Coverage](https://codecov.io/github/aaronryank/Cubically/coverage.svg?branch=master)](https://codecov.io/github/aaronryank/Cubically?branch=master) [![Read the Docs](https://readthedocs.org/projects/cubically/badge/?version=latest)](https://cubically.github.io)

Cubically is a [work-in-progress](//github.com/cubically) esoteric programming language built to be complex and painful, and to encourage users to extensively [study](//github.com/cubically/algorithms) Rubik's Cubes.

The most recent [version](//github.com/aaronryank/cubically/releases) of Cubically is [3.3](//github.com/aaronryank/cubically/releases/tag/v3.3). &nbsp;&nbsp;<sub>[![GitHub commits](https://img.shields.io/github/commits-since/aaronryank/Cubically/v3.3.svg)](https://github.com/aaronryank/Cubically/releases)</sub>

# Interpreter

To build the interpreter, run `make`. This generates the executable `cubically`:

    Usage: cubically <flags> <file | string> <size>
    Flags: f | second argument is a file
           s | second argument is a string
           u | read file/string as UTF-8
           c | read file/string as Cubically SBCS
    <size> specifies the size of the memory cube. If blank, a 3x3x3 will be assumed.

As a 3x3x3 memory cube is standard, it will be used throughout the tutorial and examples in the [docs](//github.com/aaronryank/Cubically/wiki).

To clean up after building, run `make clean`.

# How to use

To learn how to use Cubically, check out [the documentation!](//cubically.github.io/) It contains up-to-date information about every part of the language. Sometimes.

If you need help on a specific point, or something is unclear, feel free to [open an issue](//github.com/aaronryank/Cubically/issues).
