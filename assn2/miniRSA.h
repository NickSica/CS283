#ifndef __MINIRSA_H__
#define __MINIRSA_H__

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

long primeAt(long loc);
long randPrime();
int isPrime(long x);
long coprime(long x);
int isCoprime(long x, long y);
long encrypt(uint8_t msg_char, long pub_key, long c);
uint8_t decrypt(long encrypted_val, long priv_key, long c);
void computeKeys(long a, long b, long *c, long *m, long *pub_key, long *priv_key);
long gcd(long a, long b);
long mod_inverse(long base, long m);
long modulo(long a, long b, long c);
long totient(long n);

#endif
