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

To make Cubically an (almost) usable language, there is one other piece of memory: the "notepad", which is a single-value register. This can store one value at a time. Performing a command on the nonexistent sixth face index will perform the command on memory.

Input (temporarily) is performed using the nonexistent seventh-indexed face. When input is read it is stored there and can be accessed by all commands that access face indexes. It is not changed until input is read again. It is initialized to zero.

# Limitations

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

General commands:

|Command|Description|Arguments|
|-|-|-|
|`&`|exit if specified face sum/value is truthy|face index (0-7)|
|`+`|add all values of specified face together, add to notepad value, write result on notepad|^|
|`-`|add all values of specified face together, subtract from notepad value, write result on notepad|^|
|`*`|add all values of specified face together, multiply by notepad value, write result on notepad|^|
|`/`|add all values of specified face together, divide notepad value by, write result on notepad|^|
|`^`|set the notepad value to `pow(notepad, faceval)`|^|
|`=`|compare notepad and specified face for equality, write result on notepad|^|
|`<`|set the notepad to (notepad < faceval)|^|
|`>`|set the notepad to (notepad > faceval)|^|
|`:`|set notepad to specified face|^|

I/O commands:

|Command|Description|Arguments|
|-|-|-|
|`@`|add all values of specified face together, print as ASCII|face index (0-7)|
|`%`|add all values of specified face together, print as decimal|^|
|`$`|input integer, store in input buffer (index 7)|anything|
|`~`|input character, store in input buffer (index 7)|anything|

Loops:

|Command|Description|Arguments|
|-|-|-|
|`(`|create jump point with arguments. if none, create a point that can be jumped to freely. with face indexes, create a point that can be jumped to if any provided face has a truthy sum.|any number of face indexes (0-7)|
|`)`|jump back to previous jump point with arguments. if none, jump back if previous jump point can be jumped to. with face indexes, jump to previous point if both it can be jumped to and if every provided face has a truthy sum.|^|

A better explanation of loops is that `(...)` will be an infinite loop. `(1...)` loops as long as the first face is truthy. `(12...)34` is a loop as long as both `1 || 2` and `3 || 4` is truthy.

Loop idea thanks to [TehPers](//github.com/TehPers), who implemented it first in the [Lua interpreter](//github.com/Cubically/cubically-lua).

# Faces

|Number|Face|Visualizer color|
|-|-|-|
|0|top|red|
|1|left|blue|
|2|front|purple|
|3|right|green|
|4|back|blue|
|5|down|yellow|
