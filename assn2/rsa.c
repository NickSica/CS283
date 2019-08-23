#include "miniRSA.h"

int main(int argc, char** argv)
{
    if(argc < 3)
    {
	printf("Incorrect Usage.\n");
	return 1;
    }

    if(strcmp(argv[1], "genkey") == 0)
    {
	int a = atoi(argv[2]);
	int b = atoi(argv[3]);
	long m_prime = primeAt(a);
	long n_prime = primeAt(b);
	long m, c, pub_key, priv_key;
	computeKeys(m_prime, n_prime, &c, &m, &pub_key, &priv_key);
	printf("Prime %d = %ld, Prime %d = %ld, c = %ld, m = %ld, e = %ld, d = %ld, Public Key = (%ld, %ld), Private Key = (%ld, %ld)", a, m_prime, b, n_prime, c, m, pub_key, priv_key, pub_key, c, priv_key, c);
    }
    else if(strcmp(argv[1], "encrypt") == 0)
    {
	printf("Please enter a sentence to encrypt\n");
	char buf[256];
	scanf("%255s", buf);
	for(int i = 0; i < (int)strlen(buf); i++)
	    printf("%ld\n", encrypt(buf[i], atol(argv[2]), atol(argv[3])));
    }
    else if(strcmp(argv[1], "keycrack") == 0)
    {
	long pub_key = atol(argv[2]);
	long c = atol(argv[3]);
	long eul_tot = totient(c);
	
	long count = 0;
	long a = primeAt(count);
	while(((c % a) != 0) || (isPrime(c / a) == 0))
	{
	    count++;
	    a = primeAt(count);
	    if(a > ceil(sqrt(c)))
	    {
		printf("There isn't a prime pair that equals that number.\n");
		return 1;
	    }
	}
	
	long b = c / a;
	long m = (a - 1) * (b - 1);
	long priv_key = mod_inverse(pub_key, m);
	printf("a was %ld b was %ld\n", a, b);
	printf("The totient was %ld\n", eul_tot);
	printf("d was found to be %ld\n", priv_key);
	
	char buf[256];
	printf("Enter a letter to decrypt, or quit to exit\n");
	scanf("%255s", buf);
	while(strcmp(buf, "quit") != 0)
	{
	    uint8_t decrypted = decrypt(atol(buf), priv_key, c);
	    printf("This char decrypted to %u\n", decrypted);
	    printf("The lettter is %c\n", decrypted);
	    scanf("%255s", buf);
	}
    }
    return 0;
}
