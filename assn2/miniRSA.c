#include "miniRSA.h"

long primeAt(long loc)
{
    long num = 1;
    long count = -1;
    while(count != loc)
    {
	num += 1;
	if(isPrime(num))
	    count++;
    }
    return num;
}

long randPrime()
{
    srand(time(0));
    long x;
    while(1)
    {
	x = rand();
	if(isPrime(x))
	    break;
    }
    return x;
}

int isPrime(long x)
{
    if(x < 2)
	return 0;
    if(x % 2 == 0)
	return x == 2;
    if(x % 3 == 0)
	return x == 3;
    
    long upper_limit = sqrt(x);
    for(long i = 2; i <= upper_limit; i++)
	if(x % i == 0)
	    return 0;
    return 1;
}

long coprime(long x)
{
    srand(time(0));
    long y;
    while(1)
    {
	y = rand();
	if(isCoprime(x, y))
	    break;
    }
    return y;
}

int isCoprime(long x, long y)
{
    if(gcd(x, y) == 1)
	return 1;
    else
	return 0;
	/*
    int min = x;
    if(y < min)
	min = y;
    int i = min;
    for(; i > 1; i--)
    {
	if(x == y)
	{
	    return 0;
	}
	else if(min != x)
	{
	    if(x % i == 0)
		return 0;
	}
	else if(min != y)
	{
	    if(y % i == 0)
		return 0;
	}

    }
    return 1;
	*/
}

uint8_t decrypt(long encrypted_val, long priv_key, long c)
{
    return (uint8_t)modulo(encrypted_val, priv_key, c);
}

long encrypt(uint8_t msg_char, long pub_key, long c)
{
    return modulo(msg_char, pub_key, c);
}

void computeKeys(long a, long b, long *c, long *m, long *pub_key, long *priv_key)
{
    *m = (a - 1) * (b - 1);
    *c = a * b;
    *pub_key = (*m) * 2;
    while(isCoprime(*pub_key, *m) == 0 || isCoprime(*pub_key, *c) == 0 || *pub_key % *m <= 1)
    {
	*pub_key += 1;
	while(isPrime(*pub_key) == 1)
	    *pub_key += 1;
    }

    *priv_key = mod_inverse(*pub_key, *m);
}
    
long gcd(long a, long b)
{
    while(a != 0 || b != 0)
    {
	if(a == b)
	    return a;

	if(a > b)
	    a = a - b;
	else
	    b = b - a;
    }

    if(a == 0)
	return b;

    if(b == 0)
	return a;
    return -1;
}

long mod_inverse(long base, long m)
{
    for(long i = 0; i < m; i++)
	if((base * i) % m == 1)
	    return i;
    return -1;
}

long modulo(long a, long b, long c)
{
    if(b <= 1)
	return a % c;
    
    long mod = (a * a) % c;
    for(long i = 3; i <= b; i++)
	mod = (mod * a) % c;
    
    return mod;
}

long totient(long n)
{
    long numCoprime = 0;
    for(int i = 2; i < n; i++)
	if(isCoprime(i, n))
	    numCoprime++;
    return numCoprime;
}





