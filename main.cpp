#include "mpfl.h"
#include <stdio.h>
#include <limits.h>

int main()
{
	mpfl_set_precision(64);
	mpfl i(4);
	mpfl j(ULONG_MAX);
	mpfl k(i + j);
	mpfl l(j - i);

	mpfl m(0x8102);
	mpfl n(0x7eff);
	mpfl o(m - n);

	mpfl p(2);
	mpfl q(2);
	mpfl r(p * q);
	mpfl s(r / p);

	mpfl t(sqrt(144));

	printf("i: %s\n", i.genstr());
	printf("j: %s\n", j.genstr());
	printf("k: %s\n", k.genstr());
	printf("l: %s\n", l.genstr());

	printf("m: %s\n", m.genstr());
	printf("n: %s\n", n.genstr());
	printf("o: %s\n", o.genstr());

	printf("p: %s\n", p.genstr());
	printf("q: %s\n", q.genstr());
	printf("r: %s\n", r.genstr());
	printf("s: %s\n", s.genstr());

	printf("t: %s\n", t.genstr());
	return 0;
}
