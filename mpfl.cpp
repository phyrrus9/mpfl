#include "mpfl.h"
#include <stdlib.h>
#include <stdio.h>

#define SYS_BITS_PER_BYTE 8
#define MPFL_INIT_CALLED_FLAG 0x76

#define mpfl_init() \
	unsigned long i_init;\
	data = new mpfl_byte[nbytes];\
	init_called = MPFL_INIT_CALLED_FLAG;

void mpfl_set_precision(unsigned long prec)
{
	mpfl_nbits = prec > 64 ? prec : 64;
}

mpfl::mpfl()
{
	nbytes = mpfl_nbits / SYS_BITS_PER_BYTE;
	mpfl_init();
}

mpfl::mpfl(const mpfl &old)
{
	unsigned long i;
	nbytes = mpfl_nbits / SYS_BITS_PER_BYTE;
	mpfl_init();
	for (i = 0; i < nbytes; i++)
		data[i] = old.data[i];
}

mpfl::mpfl(unsigned long input)
{
	long i;
	unsigned long j;
	unsigned char *tmp = (unsigned char *)&input;
	nbytes = mpfl_nbits / SYS_BITS_PER_BYTE;
	mpfl_init();
	if (mpfl_nbits < 64) return;
	for (j = 0; j < nbytes; j++)
		data[j] = 0;
	for (i = nbytes - 1, j = 0; j < sizeof(unsigned long); i--, j++)
		data[i] = tmp[j];
}

mpfl::~mpfl()
{
}

mpfl mpfl::operator=(const mpfl &rhs)
{
	unsigned long i;
	if (init_called != MPFL_INIT_CALLED_FLAG)
	{
		nbytes = mpfl_nbits / SYS_BITS_PER_BYTE;
		mpfl_init();
	}
	for (i = 0; i < nbytes; i++)
		data[i] = rhs.data[i];
}

mpfl operator+(const mpfl &lhs, const mpfl &rhs)
{
	unsigned long i;
	mpfl ret(0);
	for (i = lhs.nbytes; i >= 0; i--)
	{
		ret.data[i] = lhs.data[i] + rhs.data[i] + ret.carry;
		if ((unsigned short)lhs.data[i] + (unsigned short)rhs.data[i] + ret.carry > (unsigned short)255)
			ret.carry = 1;
		else
			ret.carry = 0;
		if (i == 0) break;
	}
	return ret;
}

void doborrow(unsigned char *bflag, mpfl_byte *arr, unsigned long i)
{
	if (i == 0)
	{
		*bflag = 1;
		return;
	}
	if (arr[i - 1] == 0)
	{
		doborrow(bflag, arr, i - 1);
		arr[i - 1] = 0xff;
	}
	arr[i - 1]--;
}

mpfl operator-(const mpfl &lhs, const mpfl &rhs)
{
	unsigned long i, j;
	unsigned short tmpl, tmpr;
	unsigned char borrow = 0;
	mpfl ret(0);
	mpfl tlhs(lhs);
	ret.borrow = 0;
	for (i = lhs.nbytes; i >= 0; i--)
	{
		if (rhs.data[i] > tlhs.data[i])
		{
			borrow = 1;
			doborrow(&ret.borrow, tlhs.data, i);
		}
		else
			borrow = 0;
		ret.data[i] = (unsigned char)(
					(unsigned short)(tlhs.data[i]) + (255 * borrow)
					 - (unsigned short)rhs.data[i]);
		if (i == 0) break;
	}
	return ret;
}

mpfl operator*(const mpfl &lhs, const mpfl &rhs)
{
	mpfl i;
	mpfl ret = 0;
	for (i = 0; i < rhs; i++)
		ret += rhs;
	return ret;
}

mpfl operator/(const mpfl &lhs, const mpfl &rhs)
{
	mpfl num = lhs;
	mpfl ret = 0;
	while (num >= rhs)
	{
		num = num - rhs;
		ret++;
	}
	return ret;
}

mpfl operator%(const mpfl &lhs, const mpfl &rhs)
{
	mpfl num = lhs;
	mpfl ret = 0;
	while (num >= rhs)
	{
		num = num - rhs;
		ret++;
	}
	return num;
}

mpfl mpfl::operator+=(const mpfl &rhs)
{
	*this = *this + rhs;
	return *this;
}

mpfl mpfl::operator-=(const mpfl &rhs)
{
	*this = *this - rhs;
	return *this;
}

mpfl mpfl::operator*=(const mpfl &rhs)
{
	*this = *this * rhs;
	return *this;
}

mpfl mpfl::operator/=(const mpfl &rhs)
{
	*this = *this / rhs;
	return *this;
}

mpfl mpfl::operator%=(const mpfl &rhs)
{
	*this = *this % rhs;
	return *this;
}

bool operator<(const mpfl &lhs, const mpfl &rhs)
{
	mpfl tmp = lhs - rhs;
	return tmp.borrow;
}

bool operator>(const mpfl &lhs, const mpfl &rhs)
{
	mpfl tmp = rhs - lhs;
	return tmp.borrow;
}

bool operator==(const mpfl &lhs, const mpfl &rhs)
{
	unsigned long i;
	if (lhs.nbytes != rhs.nbytes) return false;
	for (i = 0; i < lhs.nbytes; i++)
		if (lhs.data[i] != rhs.data[i])
			return false;
	return true;
}

bool operator!=(const mpfl &lhs, const mpfl &rhs)
{
	return !(lhs == rhs);
}

bool operator<=(const mpfl &lhs, const mpfl &rhs)
{
	return (lhs < rhs) || (lhs == rhs);
}

bool operator>=(const mpfl &lhs, const mpfl &rhs)
{
	return (lhs > rhs) || (lhs == rhs);
}

mpfl mpfl::operator++()
{
	*this = *this + 1;
	return *this;
}

mpfl mpfl::operator--()
{
	*this = *this - 1;
	return *this;
}

mpfl mpfl::operator++(int)
{
	mpfl tmp = *this;
	*this = *this + 1;
	return tmp;
}

mpfl mpfl::operator--(int)
{
	mpfl tmp = *this;
	*this = *this - 1;
	return tmp;
}

const char * mpfl::genstr()
{
	unsigned long i;
	char *buf = (char *)malloc(nbytes * 2 + 1);
	for (i = 0; i < nbytes; i++)
		sprintf(&buf[i * 2], "%02x", data[i]);
	return (const char *)buf;
}

mpfl sqrt(mpfl num)
{
	mpfl n;
	mpfl n1;
	if (0 == num) return 0;
	n = (num / 2) + 1;
	n1 = (n + (num / n)) / 2;
	while (n1 < n)
	{
		n = n1;
		n1 = (n + (num / n)) / 2;
	}
	return n;
}
