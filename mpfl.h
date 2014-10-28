typedef unsigned char mpfl_byte;
static unsigned long mpfl_nbits = 128;

void mpfl_set_precision(unsigned long prec);

class mpfl
{
protected:
	mpfl_byte *data;
	unsigned long nbytes;
	unsigned char init_called;
	unsigned char overflow, carry, borrow;
public:
	     mpfl             ();
	     mpfl             (const mpfl &old);
	     mpfl             (unsigned long input);
	     ~mpfl            ();
	mpfl operator       = (const mpfl &rhs);
public: /*arithmatic operators*/
	friend mpfl operator+ (const mpfl &lhs, const mpfl &rhs);
	friend mpfl operator- (const mpfl &lhs, const mpfl &rhs);
	friend mpfl operator* (const mpfl &lhs, const mpfl &rhs);
	friend mpfl operator/ (const mpfl &lhs, const mpfl &rhs);
	friend mpfl operator% (const mpfl &lhs, const mpfl &rhs);
	//friend mpfl operator^ (const mpfl &lhs, const mpfl &rhs);
public: /*arithmatic-assignment operators*/
	mpfl operator+=(const mpfl &rhs);
	mpfl operator-=(const mpfl &rhs);
	mpfl operator*=(const mpfl &rhs);
	mpfl operator/=(const mpfl &rhs);
	mpfl operator%=(const mpfl &rhs);
	//friend mpfl operator^=(const mpfl &lhs, const mpfl &rhs);
public: /*exclusive comparison operators*/
	friend bool operator< (const mpfl &lhs, const mpfl &rhs);
	friend bool operator> (const mpfl &lhs, const mpfl &rhs);
public: /*inclusive comparison operators*/
	friend bool operator==(const mpfl &lhs, const mpfl &rhs);
	friend bool operator!=(const mpfl &lhs, const mpfl &rhs);
	friend bool operator>=(const mpfl &lhs, const mpfl &rhs);
	friend bool operator<=(const mpfl &lhs, const mpfl &rhs);
public: /*increment and decrement operators*/
	mpfl operator++();
	mpfl operator--();
	mpfl operator++(int);
	mpfl operator--(int);
public: /*debug options*/
	const char *genstr();
};

mpfl sqrt(mpfl num);
