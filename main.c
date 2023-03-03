#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/*
#define getuint256_t(d7, d6, d5, d4, d3, d2, d1) \
{{ \
	(d0) | (((uint64_t)(d1)) << 32),\
	(d2) | (((uint64_t)(d3)) << 32),\
	(d4) | (((uint64_t)(d5)) << 32),\
	(d6) | (((uint64_t)(d7)) << 32)\
}}/
*/

/*
    curve: y^2 = x^3 + 7
*/

void getRandom(uint64_t* k);

int main (void)
{
	uint64_t k[4];

	const uint64_t p[4] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFEFFFFFC2F};
	const uint64_t n[4] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFE, 0xBAAEDCE6AF48A03B, 0xBFD25E8CD0364141};
	const uint64_t a[4] = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};
	const uint64_t b[4] = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000007};

	const uint64_t G[2][4] = {{0x79BE667EF9DCBBAC, 0x55A06295CE870B07, 0x029BFCDB2DCE28D9, 0x59F2815B16F81798},{0x483ada7726a3c465, 0x5da4fbfc0e1108a8, 0xfd17b448a6855419, 0x9c47d08ffb10d4b8}};

	uint64_t rand[4];
	
	getRandom(rand);

	printf("%llu\n", rand[0]);
	printf("%llu\n", rand[1]);
	printf("%llu\n", rand[2]);
	printf("%llu\n", rand[3]);
}


void getRandom(uint64_t* k)
{
    FILE* random;

    random = fopen("/dev/random", "r");
    for(int i = 0; i < 4 ; i++)
	{
		uint64_t result;
		uint8_t temp[8];
		for (int x = 0 ; x < sizeof(temp) ; x++)
    	{
        	fgets(temp,sizeof(temp),random);
        	//printf("%s\n",temp);
		}
		for (int y = 0; y < 8; ++y)
    			result = (result << 8) | temp[y];
		k[i] = result;
	}
}