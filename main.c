#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct Point
{
    uint64_t x;
    uint64_t y;
};

struct uint256_t
{
    uint64_t d[4];
};

#define getuint256_t(d7, d6, d5, d4, d3, d2, d1) \
{{ \
	(d0) | (((uint64_t)(d1)) << 32),\
	(d2) | (((uint64_t)(d3)) << 32),\
	(d4) | (((uint64_t)(d5)) << 32),\
	(d6) | (((uint64_t)(d7)) << 32)\
}}/


struct Point G = {0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798, 0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8};
uint64_t k;

#define p = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F;
#define n = 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141;
#define a = 0x0000000000000000000000000000000000000000000000000000000000000000;
#define b = 0x0000000000000000000000000000000000000000000000000000000000000007;

/*
    curve: y^2 = x^3 + 7
*/

struct Point Q;

void getRandom(uint64_t* k);

int main (void)
{
	uint64_t rand[4];
    FILE* random;

    random = fopen("/dev/random", "r");
    for(int i = 0; i < 4 ; i++)
	{
		uint64_t result;
		uint8_t tempa[8];
		for (int x = 0 ; x < 8 ; x++)
    		{
        		uint8_t character;
        		character = fgetc(random);
        		printf("%i\n",character);
			tempa[x] = character;
		}
		for (int y = 0; y < 8; ++y)
    			temp = (temp << 8) | tempa[y];
		result[i] = temp;
	}

	printf("%llu\n", result[0]);
	printf("%llu\n", result[1]);
	printf("%llu\n", result[2]);
	printf("%llu\n", result[3]);
}

/*
void getRandom(uint64_t* k)
{
    FILE* random;

    //uint8_t result[4];

    random = fopen("/dev/random", "r");
    for (int i = 0 ; i < 4 ; i++)
    {
	uint8_t buffer[10];
        uint8_t character;
        character = fgetc(random);
	itoa(character, buffer, 10);
        printf("%s\n",buffer);
        //result[i] = character;
    }

    
    memcpy(&result[0], &k, sizeof(uint8_t));
    memcpy(&result[1], &k+0xF, sizeof(uint8_t));
    memcpy(&result[2], &k+0xF+0xF, sizeof(uint8_t));
    memcpy(&result[3], &k+0xF+0xF+0xF, sizeof(uint8_t));
    
}*/
