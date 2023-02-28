#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

struct Point
{
    uint64_t x;
    uint64_t y;
};

struct uint256_t
{
    uint64_t vec[4];
};

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

void getRandom();

int main (void)
{
    getRandom(&k);
    printf("%lu\n", k);
}

void getRandom(uint64_t* k)
{
    FILE* random;

    uint8_t result[4];

    random = fopen("/dev/random", "r");
    for (int i = 0 ; i < 4 ; i++)
    {
        uint8_t character;
        character = fgetc(random);
        printf("%c\n",character);
        result[i] = character;
    }

    memcpy(&result[0], &k, sizeof(uint8_t));
    memcpy(&result[1], &k+0xF, sizeof(uint8_t));
    memcpy(&result[2], &k+0xF+0xF, sizeof(uint8_t));
    memcpy(&result[3], &k+0xF+0xF+0xF, sizeof(uint8_t));
}