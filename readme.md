# Cubically

Cubically is a work-in-progress, painfully complex, very esoteric programming language. 

This repository contains:

 - `rubiks.c` - cube operations
 - `lang.c` - the language
 - `visualizer.c` - a 2d cube visualizer/emulator

To build everything type `make`. This will generate two executables, `rubiks-lang` and `visualizer`. `rubiks-lang` either takes a file as a command-line argument or reads from standard input.
# Memory

Cubically has an odd memory structure. The memory is a Rubik's cube. Various commands perform operations on this cube, such as turning the right face clockwise by 90 degrees. Other commands perform operations on certain faces of the cube, like adding all values on the top face together and storing them on the notepad (see below).

The six faces of the cube are initialized with the numbers 0 through 5, like this:

       000
       000
       000
    111222333444
    111222333444
    111222333444
       555
       555
       555

After performing a clockwise 90° turn on the right face, the memory cube would look like this:

       002
       002
       002
    111225333044
    111225333044
    111225333044
       554
       554
       554

To make Cubically an (almost) usable language, there is one other piece of memory: the "notepad" (also called the scratchpad). This can store one value at a time. Performing a command on the nonexistent sixth face index will perform the command on memory.

# Limitations

 - There is no input yet.
 - Cubically is not remotely Turing-Complete.
 - A 3x3 is the only cube size right now; this may extend later.

# Syntax

A non-integer in the source code will set the command to that character. Each integer will execute the previous command, using that integer as an argument. There is no way to pass a value outside the range `0..9` to a command.

For example, `Y0` executes the command `Y` with argument `0`. `Y015` executes `Y` with `0`, then with `1`, then with `5`.

# Commands

Memory commands:

|Command|Description|Arguments|
|-|-|-|
|`R`|turn right cube face clockwise 90°|number of turns|
|`L`|turn left cube face clockwise 90°|number of turns|
|`U`|turn top cube face clockwise 90°|number of turns|
|`D`|turn bottom cube face clockwise 90°|number of turns|
|`F`|turn front cube face clockwise 90°|number of turns|
|`B`|turn back cube face clockwise 90°|number of turns|

Control flow:

|Command|Description|Arguments
|-|-|-|
|`(`|set a point to be jumped to by `]` and `)`|anything|
|`)`|unconditionally jump to the most recently accumulated `(`|anything|
|`]`|jump to the most recently accumulated `(` if cube isn't solved|anything|

General commands:

|Command|Description|Arguments|
|-|-|-|
|`E`|exit|anything|
|`@`|add all values of specified face together, print as ASCII|face value (0-6), 6 = notepad|
|`%`|add all values of specified face together, print as decimal|^|
|`+`|add all values of specified face together, add to notepad, write result on notepad|^|
|`-`|add all values of specified face together, subtract from notepad, write result on notepad|^|
|`*`|add all values of specified face together, multiply by notepad, write result on notepad|^|
|`/`|add all values of specified face together, divide notepad by, write result on notepad|^|
|`=`|compare notepad and specified face for equality, write result on notepad|face value (0-5)|
|`:`|set notepad to specified face|^|

# Faces

|Number|Face|Visualizer color|
|-|-|-|
|0|top|red|
|1|left|blue|
|2|front|purple|
|3|right|green|
|4|back|blue|
|5|down|yellow|
