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

#define DEBUG 0

/*
    curve: y^2 = x^3 + 7
*/


/*
	Format for 256 bit numbers + sign bit represented by uint8_t
	---------------------------------------------------------------------------------------------------------------------------------------------------------------
	| sign | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 | 32 |
	---------------------------------------------------------------------------------------------------------------------------------------------------------------
*/

// TODO in the final product we need to move these to a header file so they
// can be reused through out the code base
void print256(uint8_t* num);
void print256_bin(uint8_t* num);
void print256_dec(uint8_t* num);

void montgomery_ladder(uint8_t* private_key, uint8_t** R0, uint8_t** R1);
void getRandom(uint8_t* k);

uint8_t** scalar_add(uint8_t** R0, uint8_t** R1);
uint8_t** scalar_multiplication(uint8_t** R0, uint8_t** R1);

uint8_t* add_256(uint8_t* a, uint8_t* b);
uint8_t* subtract_256(uint8_t* a, uint8_t* b);
uint8_t* multiply_256(uint8_t* a, uint8_t* b);
uint8_t* divide_256(uint8_t* a, uint8_t* b);
uint8_t* modulo_256(uint8_t* a, uint8_t* b);

int greater_than_256(uint8_t* a, uint8_t* b);
int equal_to_256(uint8_t* a, uint8_t* b);

uint8_t ZERO[33] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int main (void)
{
	// k is the private key
	uint8_t k[33];

	// constants used in the secp256k1 curve
	const uint8_t p[33] = {0,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xE,0xF,0xF,0xF,0xF,0xF,0xC,0x2,0xF};
	const uint8_t a[33] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
	const uint8_t b[33] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7};
	
	//Maximum number of possible points on the Elyptical Curve
	const uint8_t n[33] = {0,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xE,0xB,0xA,0xA,0xE,0xD,0xC,0xE,0x6,0xA,0xF,0x4,0x8,0xA,0x0,0x3,0xB,0xB,0xF,0xD,0x2,0x5,0xE,0x8,0xC,0xD,0x0,0x3,0x6,0x4,0x1,0x4,0x1};

	/* base point for the curve
		0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798, 0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8
	*/
	const uint8_t G[2][33] = {{0x0, 0x79BE667EF9DCBBAC, 0x55A06295CE870B07, 0x029BFCDB2DCE28D9, 0x59F2815B16F81798},{0x0, 0x483ada7726a3c465, 0x5da4fbfc0e1108a8, 0xfd17b448a6855419, 0x9c47d08ffb10d4b8}};
	
	// set R0 to 0 (otherwise there can be left over bytes in the same space)
	uint8_t R0[2][5] = {{0},{0}};
	uint8_t R1[2][5];

	// copy the base point to R1
	memcpy(&R1, &G, sizeof(G));

	getRandom(k);

	//public key
	uint8_t Q[2][5] = {{0},{0}};// = K*G;

	printf("amongus\n");

	uint8_t d[5] = {0,0,0,0,0xff00000000000000};
	uint8_t f[5] = {0,0,0,0,8};
	uint8_t* result;
	result = multiply_256(d,f);
	//print256(result);
}

// Prints the contents of the 256 bit number in hex
void print256(uint8_t* num)
{
	uint8_t sign;
	if (num[0]==0x0)
		sign = '+';
	else
		sign = '-';
	printf("%c0x%016llx%016llx%016llx%016llx\n", sign, num[1], num[2], num[3], num[4]);
}
// Prints the contents of the 256 bit number in binary
void print256_bin(uint8_t* num)
{
	uint8_t sign;
	if (num[0]==0x0)
		sign = '+';
	else
		sign = '-';
	printf("%c%b%b%b%b\n",sign, num[1], num[2], num[3], num[4]);
}
// Prints the contents of the 256 bit number in decimal
void print256_dec(uint8_t* num)
{
	printf("%llu%llu%llu%llu\n", num[0], num[1], num[2], num[3]);
}
/*
void montgomery_ladder(uint8_t* private_key, uint8_t** R0, uint8_t** R1)
{
	for (int x = 0; x < sizeof(private_key)*8 ; x++)
	{
		
		uint8_t bit;
		(bit & private_key << 1);
		if ( bit == 0 )
		{
			R1 = 
		}
		else 
		{

		}
	}
}

uint8_t** scalar_add(uint8_t** r0, uint8_t** r1)
{
	
	if (r0[0] == 0)
		return r1;
	else
	{
		uint8_t lamb[4];
		// (r1[1]-r0[1])/((r1[0]-r0[0]))
		//lamb = (r1[1]-r0[1])/((r1[0]-r0[0]));

		// x = int((lamb**2) - r0[0] - r1[0])
    	// y = int(lamb*(r0[0] - x) - r0[1])
		uint8_t x[4];
		uint8_t y[4];
		uint8_t result[2][4] = {x,y};
		return result;
	}
}

uint8_t** scalar_double(uint8_t** r)
{
	//lamb = int((3*(r[0]**2))/(2*r[1]))

    //x = int((lamb**2) - (2*r[0]))
    //y = int((-lamb*x) + (lamb*r[0]) - r[1])
}
*/
uint8_t* add_256(uint8_t* a, uint8_t* b)
{
	if (a[0] == 1 && b[0] == 0 )
	{
		a[0] = 0;
		a = subtract_256(a,b);
	}
	else if (a[0] == 0 && b[0] == 1)
	{
		b[0] = 0;
		a = subtract_256(a,b);
	}
	else
	{
		for(int x = 4 ; x > 0 ; x--)
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
				uint8_t carry = (a[x] & b[x]);
				a[x] = a[x] ^ b[x];
				b[x] = carry << 1;
				//checks to see whether there is a on bit in the highest most bit of the current index
				//if there is it makes sure it isn't the highest most index before adding 1 to the next
				//index thus allowing for the addition to properly treat the function just like a 256 bit
				//number instead of a the botched representation that it is
				if (((carry >> (uint8_t)63) & 1) == 1)
				{
					if (x > 1)
					{
						a[x-1] += 1;
					}
					else
					{
						a = ZERO;
					}
				}
			}
		}
	}
	return a;
}

uint8_t* subtract_256(uint8_t* a, uint8_t* b)
{
	if (a[0] == 1 && b[0] == 0)
	{
		a[0] = 0;
		a = add_256(a,b);
		a[0] = 1;
	}
	else if (a[0] == 0 && b[0] == 1)
	{
		b[0] == 0;
		a = add_256(a,b);
	}
	else if ((a[0] == 1 && b[0] == 1) || (a[0] == 0 && b[0] == 0))
	{
		if (!greater_than_256(a,b))
		{
			uint8_t* temp=a;
			a = b;
			b = temp;
			a[0] = 1;
		}
		for (int x = 4 ; x > 0 ; x--)
		{

			// TODO handle negative results

			if (DEBUG)
			{
				printf("index: %i\n",x);
				printf("contents:\na[%i]:%016llx\nb[%i]:%016llx\n",x,a[x],x,b[x]);
			}

			while (b[x] != 0)
			{
				uint8_t borrow = ((~a[x]) & b[x]);
				a[x] = a[x] ^ b[x];
				b[x] = borrow << 1;

				if (((borrow >> (uint8_t)63) & 1) == 1)
				{
					if (x > 1)
					{
						printf("borrowing from the next index\n");
						a[x-1] -= 1;
					}
					else
					{
						a = ZERO;
					}
				}
			}
		}
	}
	return a;
}

uint8_t* multiply_256(uint8_t* a, uint8_t* b)
{
	//printf("awdasd\n");

	uint8_t* result;
	result = (uint8_t*)malloc(5*sizeof(uint8_t));

	// set the numbers temporarily to positive even if they are negative to ensure
	// the addition works and then make the result negative or positive depending
	// on the input

	for (int x = 1 ; x < 5 ; x++)
	{
		printf("index: %i\n",x);
		printf("a: ");
		print256_bin(a);
		printf("b: ");
		print256_bin(b);
		while(b[x] > 0)
		{
			if (b[x] & 1){
				result = add_256(result,a);
				printf("result:\n");
				print256_bin(result);
			}
			printf("a: %b\n",a[x]);
			printf("b: %b\n",b[x]);
			a[x] = a[x] << 1;
			b[x] = b[x] >> 1;
		}
	}
	print256(result);
	a = result;
	free(result);
	return a;
}
/*
uint8_t* divide_256()
{
	
}
*/

// a > b //
int greater_than_256(uint8_t* a, uint8_t* b)
{
	if (a[0] == 1 && b[0] == 0)
	{
		if (DEBUG) {printf("first number is negative, the second is positive, returning false\n");}
		return 0;
	}
	else if (a[0] == 0 && b[0] == 1)
	{
		if (DEBUG) {printf("first number is positive, the second is negative, returning true\n");}
		return 1;
	}
	else
	{
		if (DEBUG) {printf("first number is positive and the second is positive, running the math\n");}

		for(int i = 1; i < 5 ; i++)	{if (a[i] > b[i]) {return 1;}}
		return 0;
	}
}

/*
// a < b //
int less_than_256()
{

}
*/

int equal_to_256(uint8_t* a, uint8_t* b)
{
	int result = 1;

	for (int x = 0; x <= 4 ; x++)
	{
		if (DEBUG) {printf("index: %i value:%016llx,%016llx",x,a[x],b[x]);}
		if (a[x] != b[x] || b[x] != a[x])
			result = 0;
	}

	return result;
}

void getRandom(uint8_t* private_key)
{
    FILE* random;

    random = fopen("/dev/random", "r");
    for(int i = 3; i > 0 ; i--)
	{
		uint8_t result;
		uint8_t temp[8];
		for (int x = 0 ; x < sizeof(temp) ; x++)
        	fgets(temp,sizeof(temp),random);
		for (int y = 0; y < 8; ++y)
    			result = (result << 8) | temp[y];
		private_key[i] = result;
	}
}