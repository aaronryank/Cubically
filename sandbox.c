#include <locale.h>
#include <wchar.h>
#include "rubiks.h"

int main(void)
{
    CUBESIZE = 4;
    initcube();

    turncube(RIGHT,2,3);
    turncube(LEFT,2,3);

    printcube();
}
