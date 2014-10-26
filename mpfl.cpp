#include "mpfl.h"
#include <stdlib.h>

#define SYS_BITS_PER_BYTE 8

#define mpfl_init(arg, nbits)\
{\
	unsigned long i;\
	arg->nbytes = mpfl_nbits / SYS_BITS_PER_BYTE;\
	arg->data = (mpfl_byte *)malloc(sizeof(mpfl_byte) * arg->nbytes);\
	for (i = 0; i < arg->nbytes; i++)\
		arg->data[i].carry = 0;\
}

mpfl::mpfl()
{
	mpfl_init(this, mpfl_nbits);
}

mpfl::mpfl(const mpfl &old)
{
	unsigned long i;
	mpfl_init(this, old.nbytes * SYS_BITS_PER_BYTE);
	for (i = 0; i < nbytes; i++)
	{
		data[i].data = old.data[i].data;
		data[i].carry = old.data[i].carry;
	}
}

mpfl::mpfl(unsigned long input)
{
	long i;
	unsigned long j;
	unsigned char *tmp = (unsigned char *)&input;
	mpfl_init(this, mpfl_nbits);
	if (mpfl_nbits < 64) return;
	for (j = 0; j < nbytes; j++)
		data[i].data = 0;
	for (i = nbytes - 1, j = 0; j < sizeof(unsigned long); i--, j++)
		data[i].data = tmp[j];
}

mpfl::~mpfl()
{
	free(data);
}

mpfl mpfl::operator=(const mpfl &rhs)
{
	unsigned long i;
	mpfl_init(this, rhs.nbytes * SYS_BITS_PER_BYTE);
	for (i = 0; i < nbytes; i++)
	{
		data[i].data = rhs.data[i].data;
		data[i].carry = rhs.data[i].carry;
	}
}

mpfl operator+(const mpfl &lhs, const mpfl &rhs)
{
	mpfl ret(lhs);
	return ret;
}

mpfl operator-(const mpfl &lhs, const mpfl &rhs)
{
	mpfl ret(lhs);
	return ret;
}

mpfl operator*(const mpfl &lhs, const mpfl &rhs)
{
	mpfl ret(lhs);
	return ret;
}

mpfl operator/(const mpfl &lhs, const mpfl &rhs)
{
	mpfl ret(lhs);
	return ret;
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

bool operator<(const mpfl &lhs, const mpfl &rhs)
{
	unsigned long hdigitl, hdigitr, i;
	hdigitl = hdigitr = lhs.nbytes;
	if (lhs.nbytes < rhs.nbytes) return true;
	if (lhs.nbytes > rhs.nbytes) return false;
	for (i = 0; i < lhs.nbytes || (hdigitl < lhs.nbytes && hdigitr < lhs.nbytes); i++)
	{
		if (lhs.data[i].data > 0 && hdigitl < i)
			hdigitl = i;
		else if (rhs.data[i].data > 0 && hdigitr < i)
			hdigitr = i;
	}
	if (hdigitl > hdigitr) return true;
	else if (hdigitl < hdigitr) return false;
	else //they are equal
	{
		for (; hdigitl >= 0; hdigitl--)
		{
			if (lhs.data[hdigitl].data < rhs.data[hdigitl].data)
				return true;
		}
	}
	return false;
}

bool operator>(const mpfl &lhs, const mpfl &rhs)
{
	unsigned long hdigitl, hdigitr, i;
	hdigitl = hdigitr = lhs.nbytes;
	if (lhs.nbytes > rhs.nbytes) return true;
	if (lhs.nbytes < rhs.nbytes) return false;
	for (i = 0; i < lhs.nbytes || (hdigitl < lhs.nbytes && hdigitr < lhs.nbytes); i++)
	{
		if (lhs.data[i].data > 0 && hdigitl < i)
			hdigitl = i;
		else if (rhs.data[i].data > 0 && hdigitr < i)
			hdigitr = i;
	}
	if (hdigitl < hdigitr) return true;
	else if (hdigitl > hdigitr) return false;
	else //they are equal
	{
		for (; hdigitl >= 0; hdigitl--)
		{
			if (lhs.data[hdigitl].data > rhs.data[hdigitl].data)
				return true;
		}
	}
	return false;
}

bool operator==(const mpfl &lhs, const mpfl &rhs)
{
	unsigned long i;
	if (lhs.nbytes != rhs.nbytes) return false;
	for (i = 0; i < lhs.nbytes; i++)
		if (lhs.data[i].data != rhs.data[i].data)
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
