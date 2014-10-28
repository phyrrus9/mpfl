#include "mpfl.h"
#include <stdlib.h>
#include <stdio.h>

#define SYS_BITS_PER_BYTE 8
#define MPFL_INIT_CALLED_FLAG 0x76

#define mpfl_init() 0
#define mpfl_init2()\
	unsigned long i_init;\
	data = (mpfl_byte *)malloc(sizeof(mpfl_byte) * nbytes);\
	for (i_init = 0; i_init < nbytes; i_init++)\
		data[i_init].carry = 0;\
	init_called = MPFL_INIT_CALLED_FLAG;

mpfl::mpfl()
{
	nbytes = mpfl_nbits / SYS_BITS_PER_BYTE;
	mpfl_init();
	unsigned long i_init;
	data = (mpfl_byte *)malloc(sizeof(mpfl_byte) * nbytes);
	for (i_init = 0; i_init < nbytes; i_init++)
		data[i_init].carry = 0;
	init_called = MPFL_INIT_CALLED_FLAG;
}

mpfl::mpfl(const mpfl &old)
{
	unsigned long i;
	nbytes = mpfl_nbits / SYS_BITS_PER_BYTE;
	mpfl_init();
	unsigned long i_init;
	data = (mpfl_byte *)malloc(sizeof(mpfl_byte) * nbytes);
	for (i_init = 0; i_init < nbytes; i_init++)
		data[i_init].carry = 0;
	init_called = MPFL_INIT_CALLED_FLAG;
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
	nbytes = mpfl_nbits / SYS_BITS_PER_BYTE;
	mpfl_init();
	unsigned long i_init;
	data = (mpfl_byte *)malloc(sizeof(mpfl_byte) * nbytes);
	for (i_init = 0; i_init < nbytes; i_init++)
		data[i_init].carry = 0;
	init_called = MPFL_INIT_CALLED_FLAG;
	if (mpfl_nbits < 64) return;
	for (j = 0; j < nbytes; j++)
		data[j].data = 0;
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
	if (init_called != MPFL_INIT_CALLED_FLAG)
	{
		nbytes = mpfl_nbits / SYS_BITS_PER_BYTE;
		mpfl_init();
		unsigned long i_init;
		data = (mpfl_byte *)malloc(sizeof(mpfl_byte) * nbytes);
		for (i_init = 0; i_init < nbytes; i_init++)
			data[i_init].carry = 0;
		init_called = MPFL_INIT_CALLED_FLAG;
	}
	for (i = 0; i < nbytes; i++)
	{
		data[i].data = rhs.data[i].data;
		data[i].carry = rhs.data[i].carry;
	}
}

void increment(mpfl *trhs, unsigned long i)
{
	if (trhs->data[i].data == 255)
		increment(trhs, i + 1);
	trhs->data[i].data++;
}

mpfl operator+(const mpfl &lhs, const mpfl &rhs)
{
	unsigned long i;
	mpfl ret(0);
	mpfl trhs(rhs);
	for (i = lhs.nbytes; i >= 0; i--)
	{
		if ((unsigned short)lhs.data[i].data + (unsigned short)trhs.data[i].data > (unsigned short)255)
		{
			if (i > 0)
			{
				ret.data[i].carry = 1;
				ret.data[0].carry = 0;
			}
			else
			{
				ret.data[0].carry = 1;
			}
		}
		else
			ret.data[i].carry = 0;
		ret.data[i].data = lhs.data[i].data + trhs.data[i].data;
		if (i < lhs.nbytes)
		{
			if (ret.data[i].data == 255 && ret.data[i + 1].carry == 1)
				increment(&trhs, i + 1);
			ret.data[i].data += ret.data[i + 1].carry;
		}
		if (i == 0) break;
	}
	return ret;
}

mpfl operator-(const mpfl &lhs, const mpfl &rhs)
{
	unsigned long i, j;
	unsigned short tmpl, tmpr;
	mpfl ret(0);
	mpfl tlhs(lhs);
	for (i = lhs.nbytes; i >= 0; i--)
	{
		if (rhs.data[i].data > tlhs.data[i].data)
		{
			tmpl = 0;
			ret.data[i].data += 10;
			for (j = i; j >= 0; j--)
			{
				if (j > 0)
				{
					tlhs.data[j - 1].data--;
					ret.data[0].carry = 0;
				}
				else
					ret.data[0].carry = 1;
			}
		}
		ret.data[i].data = lhs.data[i].data - rhs.data[i].data;
	}
	return ret;
}

mpfl operator*(const mpfl &lhs, const mpfl &rhs)
{
	mpfl i;
	mpfl ret(lhs);
	for (i = 0; i < rhs; i = i + 1)
		ret = lhs + rhs;
	return ret;
}

mpfl operator/(const mpfl &lhs, const mpfl &rhs)
{
	mpfl num = lhs;
	mpfl ret = 0;
	while (num >= rhs)
	{
		num = num - rhs;
		ret = ret + 1;
	}
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
		sprintf(&buf[i * 2], "%02x", data[i].data);
	return (const char *)buf;
}
