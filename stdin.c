/* stdin.c - Copyright (C) 2015 Aaron Ryan Klingler

   Turn off echoing and line-buffering
   Useful for C game programming, and Java if you do it right (currently implemented in all versions of rogue112)
*/

#include <termios.h>

#define get_stdin(x) tcgetattr(1,x);
#define set_stdin(x) tcsetattr(1,0,x);

void stdin_set(void)
{
	struct termios t;
	get_stdin(&t);
	t.c_lflag &= ~(ECHO | ICANON);
	set_stdin(&t);
}

void stdin_reset(void)
{
	struct termios t;
	get_stdin(&t);
	t.c_lflag |= ECHO;
	t.c_lflag |= ICANON;
	set_stdin(&t);
}
