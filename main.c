/*
	Programmed with blood, sweat, and tears by Diego Noria

	Adapted from Cooper Pflaum's gen_secrets.py
	Math by Sarah Ogden and Tony Ivanov

	Example and template code for the C implementation of the
	secp256k1 curve used on the Tiva TM4C23GXL Embedded device
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DEBUG 1

/*
    curve: y^2 = x^3 + 7
*/

void getRandom(uint64_t* k);
void print256(uint64_t* num);
void print256_bin(uint64_t* num);
void print256_dec(uint64_t* num);
void montgomery_ladder(uint64_t* private_key, uint64_t** R0, uint64_t** R1);

uint64_t** scalar_add(uint64_t** R0, uint64_t** R1);
uint64_t** scalar_multiplication(uint64_t** R0, uint64_t** R1);

uint64_t* subtract_256();
uint64_t* add_256();
uint64_t* multiply_256(uint64_t* a, uint64_t* b);
uint64_t* divide_256(uint64_t* a, uint64_t* b);

int main (void)
{
	// k is the private key
	uint64_t k[4];

	// constants used in the secp256k1 curve
	const uint64_t p[4] = {0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFEFFFFFC2F};
	const uint64_t a[4] = {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001};
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

	//public key
	uint64_t Q[2][4] = {{0},{0}};// = K*G;

	printf("The difference between b and a:\n");
	print256(subtract_256(b,a));
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
		/*
		uint8_t bit;
		(bit & private_key << 1);
		if ( bit == 0 )
		{
			R1 = 
		}
		else 
		{

		}*/
	}
}

uint64_t** scalar_add(uint64_t** r0, uint64_t** r1)
{
	
	if (r0[0] == 0)
		return r1;
	else
	{
		uint64_t lamb[4];
		// (r1[1]-r0[1])/((r1[0]-r0[0]))
		//lamb = (r1[1]-r0[1])/((r1[0]-r0[0]));

		// x = int((lamb**2) - r0[0] - r1[0])
    	// y = int(lamb*(r0[0] - x) - r0[1])
		uint64_t x[4];
		uint64_t y[4];
		uint64_t result[2][4] = {x,y};
		return result;
	}
}

uint64_t** scalar_double(uint64_t** r)
{
	//lamb = int((3*(r[0]**2))/(2*r[1]))

    //x = int((lamb**2) - (2*r[0]))
    //y = int((-lamb*x) + (lamb*r[0]) - r[1])
}

uint64_t* subtract_256(uint64_t* a, uint64_t* b)
{
	for (int x = 3 ; x > -1 ; x--)
	{
		if (DEBUG)
		{
			printf("index: %i\n",x);
			printf("contents:\na[%i]:%016llx\nb[%i]:%016llx\n",x,a[x],x,b[x]);
		}

		while (b[x] != 0)
		{
			uint64_t borrow = ((~a[x]) & b[x]);
			a[x] = a[x] ^ b[x];
			b[x] = borrow << 1;

			if (((borrow >> (uint64_t)63) & 1) == 1)
			{
				if (x != 0)
				{
					printf("borrowing from the next index\n");
					a[x-1] -= 1;
				}
				else
				{
					// need to return an overflow essentially
				}
			}
		}
	}
	return a;
}

uint64_t* add_256(uint64_t* a, uint64_t* b)
{
	for(int x = 3 ; x > -1 ; x--)
	{
		if (DEBUG)
		{
			printf("index: %i\n",x);
			printf("contents:\na[%i]:%016llx\nb[%i]:%016llx\n",x,a[x],x,b[x]);
		}

		//addition loop
		while (b[x] != 0)
		{
			// basic implementation of bitwise addition
			uint64_t carry = (a[x] & b[x]);
			a[x] = a[x] ^ b[x];
			b[x] = carry << 1;
			//checks to see whether there is a on bit in the highest most bit of the current index
			//if there is it makes sure it isn't the highest most index before adding 1 to the next
			//index thus allowing for the addition to properly treat the function just like a 256 bit
			//number instead of a the botched representation that it is
			if (((carry >> (uint64_t)63) & 1) == 1)
			{
				if (x != 0)
				{
					a[x-1] += 1;
				}
				else
				{
					// need to return an overflow essentially
				}
			}
		}
	}
	return a;
}
/*
uint64_t* multiply_256()
{
	
}

uint64_t* divide_256()
{
	
}

int greater_than_256()
{

}

int less_than_256()
{

}
*/
void getRandom(uint64_t* private_key)
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
		private_key[i] = result;
	}
}