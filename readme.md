# Cubically

Cubically is a [work-in-progress](//github.com/cubically), painfully complex, very esoteric programming language.

The most recent [version](//github.com/aaronryank/cubically/releases) of Cubically is [2.0-beta](//github.com/aaronryank/cubically/releases/tag/v2.0-beta).

# Interpreter

 - To build the interpreter, type `make`. This will generate the executable `rubiks-lang`.
 - To build the TryItOnline interpreter, which prints debugging information without color, type `make tio`. This will generate the executable `rubiks-tio`.
 - To build the visualizer, type `make visualizer`. This will generate the executable `visualizer`.
 - To clean up the object files after building, type `make clean`.

The interpreter is used like so:

    Usage: rubiks-lang <flags> <file | string> <size>
    Flags: f | second argument is a file
           s | second argument is a string
           u | read file/string as UTF-8
           c | read file/string as Cubically SBCS
           i | interactive mode, print cube before each input
    <size> specifies the size of the memory cube. If blank, a 3x3x3 will be assumed.

As a 3x3x3 memory cube is standard, it will be used throughout the tutorial and examples here.

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

Input is performed using the nonexistent seventh-indexed face. When input is read it is stored there and can be accessed by all commands that access face indexes. It is not changed until input is read again. It is initialized to zero.

# Syntax

A non-integer in the source code will set the command to that character. Each integer will execute the previous command, using that integer as an argument. There is no way to pass a value outside the range `0..9` to a command.

For example, `Y0` executes the command `Y` with argument `0`. `Y015` executes `Y` with `0`, then with `1`, then with `5`.

Certain commands can be executed with no arguments, and are called when another command is read, such as `$` (read integer as input). See commands below for more info.

# Commands

### Memory

|Command|Description|Arguments|
|-|-|-|
|`R`|turn right cube face clockwise 90°|number of turns|
|`L`|turn left cube face clockwise 90°|number of turns|
|`U`|turn top cube face clockwise 90°|number of turns|
|`D`|turn bottom cube face clockwise 90°|number of turns|
|`F`|turn front cube face clockwise 90°|number of turns|
|`B`|turn back cube face clockwise 90°|number of turns|

### General

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

### I/O

|Command|Description|Arguments|
|-|-|-|
|`@`|add all values of specified face together, print as ASCII|face index (0-7)|
|`%`|add all values of specified face together, print as decimal|^|
|`$`|input integer, store in input buffer (index 7)|anything or nothing|
|`~`|input character, store in input buffer (index 7)|anything or nothing|
|`#`|interpret input character (0-7) as argument to previous command|nothing|

### Loops

|Command|Description|Arguments|
|-|-|-|
|`(`|create jump point with arguments. if none, create a point that can be jumped to freely. with face indexes, create a point that can be jumped to if any provided face has a truthy sum.|any number of face indexes (0-7)|
|`)`|jump back to previous jump point with arguments. if none, jump back if previous jump point can be jumped to. with face indexes, jump to previous point if both it can be jumped to and if every provided face has a truthy sum.|^|

A better explanation of loops:

 - `(...)` will be an infinite loop.
 - `(0...)` loops as long as the top face has a nonzero sum.
 - `(12...)34` is a loop as long as both `1 || 2` and `3 || 4` is truthy. (numbers are face indexes)

### Conditionals

|Command|Description|Arguments|
|-|-|-|
|`?`|execute next command or code block if specified face is truthy|face index (0-7)|
|`!`|execute next command or code block if specified face is falsy. also can be used as if-else statement as specified below|face index (0-7) or nothing|
|`{`|open a code block|nothing|
|`}`|close a code block|nothing|

`!` can also be used as if-else as shown below. This code is equivalent to `if (face 6 is truthy) { ... } else { ... }`

    ?6{...}!{...}

# Experimental commands

These commands are not guaranteed to stay in Cubically; they will be removed if more important commands need to be added.

|Command|Description|Arguments|
|-|-|-|
|`` ` ``|set notepad to (notepad XOR faceval) where faceval is the sum of all values on the specified face|face index (0-7)|
|`_`|set notepad to (notepad modulo faceval)|^|
|`"`|set notepad to (notepad bitwise-and faceval)|^|
|`\|`|set notepad to (notepad bitwise-or faceval)|^|

# Faces

|Number|Face|Visualizer color|
|-|-|-|
|0|top|red|
|1|left|blue|
|2|front|purple|
|3|right|green|
|4|back|blue|
|5|down|yellow|
