#include "mpfl.h"
#include <stdio.h>
#include <limits.h>

int main()
{
	mpfl i(4);
	mpfl j(0xffffffff);
	mpfl k(i + j);
//	mpfl l(k * 16);
	printf("i: %s\n", i.genstr());
	printf("j: %s\n", j.genstr());
	printf("k: %s\n", k.genstr());
//	printf("l: %s\n", l.genstr());
	return 0;
}
