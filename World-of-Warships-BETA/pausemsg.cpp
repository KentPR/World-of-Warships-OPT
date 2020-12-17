#include <stdio.h>
#include <conio.h>
#include "pausemsg.h"

void pause(void)
{
	printf("Press any key to continue.");
	_getch();
}