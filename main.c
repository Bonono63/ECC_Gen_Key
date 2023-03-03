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
void print256(uint64_t* num);
void print256_bin(uint64_t* num);
void print256_dec(uint64_t* num);
void montgomery_ladder(uint64_t* private_key, uint64_t** R0, uint64_t** R1);

int main (void)
{
	// k is the private key
	uint64_t k[4];

	// constants used in the secp256k1 curve
	const uint64_t p[4] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFEFFFFFC2F};
	const uint64_t a[4] = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000};
	const uint64_t b[4] = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000007};
	
	//Maximum number of possible points on the Elyptical Curve
	const uint64_t n[4] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFE, 0xBAAEDCE6AF48A03B, 0xBFD25E8CD0364141};

	/* base point for the curve
		0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798, 0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8
	*/
	const uint64_t G[2][4] = {{0x79BE667EF9DCBBAC, 0x55A06295CE870B07, 0x029BFCDB2DCE28D9, 0x59F2815B16F81798},{0x483ada7726a3c465, 0x5da4fbfc0e1108a8, 0xfd17b448a6855419, 0x9c47d08ffb10d4b8}};
	
	// set R0 to 0 (otherwise there can be left over bytes in the same space)
	uint64_t R0[2][4] = {{0},{0}};
	uint64_t R1[2][4];

	// copy the base point to R1
	memcpy(&R1, &G, sizeof(G));

	getRandom(k);

	printf("k:\n");
	print256(k);
	printf("R0:\n");
	print256(R0[0]);
	print256(R0[1]);
	printf("R1:\n");
	print256(R1[0]);
	print256(R1[1]);

	printf("\nNumber of bits in Private key: %i\n", sizeof(k)*8);

	//public key
	//uint64_t Q[2][4] = K*G;
}

// Prints the contents of the 256 bit number in hex
void print256(uint64_t* num)
{
	printf("0x%016llx%016llx%016llx%016llx\n", num[0], num[1], num[2], num[3]);
}
// Prints the contents of the 256 bit number in binary
void print256_bin(uint64_t* num)
{
	printf("%b%b%b%b\n", num[0], num[1], num[2], num[3]);
}
// Prints the contents of the 256 bit number in decimal
void print256_dec(uint64_t* num)
{
	printf("%llu%llu%llu%llu\n", num[0], num[1], num[2], num[3]);
}

void montgomery_ladder(uint64_t* private_key, uint64_t** R0, uint64_t** R1)
{
	for (int x = 0; x < sizeof(private_key)*8 ; x++)
	{
		
	}
}

void scalar_add()
{
	
}

void scalar_multiplication()
{

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
        	//printf("%b\n",*temp);
		}
		for (int y = 0; y < 8; ++y)
    			result = (result << 8) | temp[y];
		k[i] = result;
	}
}