#include <iostream>
#include <bitset>
#include <cstdlib>
#include <vector>

using namespace std; 
#define byte char


void initiationAlpha()__attribute__((constructor));


unsigned lfsr[10];
unsigned fsmR1, fsmR2;
unsigned keySize;
unsigned ivSize;


// Odkodowanie tablicy bajtów do wartoœci 32-bitowej (little-endian)
unsigned decode32le(byte buf[], unsigned off){
return (buf[off] & 0xFF)
       | ((buf[off + 1] & 0xFF) << 8)
       | ((buf[off + 2] & 0xFF) << 16)
       | ((buf[off + 3] & 0xFF) << 24);
}


// Zakodowanie 32-bitowej wartoœci w tablicy bajtów
void encode32le(unsigned val, byte buf[], unsigned off){
    buf[off] = (byte)val;
    buf[off + 1] = (byte)(val >> 8);
    buf[off + 2] = (byte)(val >> 16);
    buf[off + 3] = (byte)(val >> 24);
}

// Rotacja w lewo n-bitów a 32-bitowym s³owie
unsigned rotateLeft(unsigned val, unsigned n){
        return (val << n) | (val >> (32 - n));
}


unsigned serpent24SubKeys[100];

// Inicjacja wartoœci klucza
// Realizacja Serpent24, w celu wygenerowania 100 32-bitowych podkluczy

void setKey(byte key[])
    {
		   
		
		if (keySize < 1 || keySize > 32)
            cout<<"bad key length: " + keySize;
        byte lkey[32];
        
        if (keySize == 32) {
        	for(int i = 0; i < 32; i++)
            	lkey[i] = key[i];
        } else {
        	for(int i = 0; i < keySize; i++)
        		lkey[i] = key[i];
            
            lkey[keySize] = 0x01;
            for (int i = keySize + 1; i < sizeof(lkey)/sizeof(lkey[0]); i ++)
                lkey[i] = 0x00;
        }

        unsigned w0, w1, w2, w3, w4, w5, w6, w7;
        unsigned r0, r1, r2, r3, r4, tt;
        unsigned i = 0;
        
        

        w0 = decode32le(lkey, 0);
        w1 = decode32le(lkey, 4);
        w2 = decode32le(lkey, 8);
        w3 = decode32le(lkey, 12);
        w4 = decode32le(lkey, 16);
        w5 = decode32le(lkey, 20);
        w6 = decode32le(lkey, 24);
        w7 = decode32le(lkey, 28);
        tt = w0 ^ w3 ^ w5 ^ w7 ^ ((unsigned)0x9E3779B9 ^ (0));
        w0 = rotateLeft(tt, 11);
        tt = w1 ^ w4 ^ w6 ^ w0 ^ ((unsigned)0x9E3779B9 ^ (0 + 1));
        w1 = rotateLeft(tt, 11);
        tt = w2 ^ w5 ^ w7 ^ w1 ^ ((unsigned)0x9E3779B9 ^ (0 + 2));
        w2 = rotateLeft(tt, 11);
        tt = w3 ^ w6 ^ w0 ^ w2 ^ ((unsigned)0x9E3779B9 ^ (0 + 3));
        w3 = rotateLeft(tt, 11);
        r0 = w0;
        r1 = w1;
        r2 = w2;
        r3 = w3;
        r4 = r0;
        r0 |= r3;
        r3 ^= r1;
        r1 &= r4;
        r4 ^= r2;
        r2 ^= r3;
        r3 &= r0;
        r4 |= r1;
        r3 ^= r4;
        r0 ^= r1;
        r4 &= r0;
        r1 ^= r3;
        r4 ^= r2;
        r1 |= r0;
        r1 ^= r2;
        r0 ^= r3;
        r2 = r1;
        r1 |= r3;
        r1 ^= r0;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r2;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r4;
        tt = w4 ^ w7 ^ w1 ^ w3 ^ ((unsigned)0x9E3779B9 ^ (4));
        w4 = rotateLeft(tt, 11);
		tt = w5 ^ w0 ^ w2 ^ w4 ^ ((unsigned)0x9E3779B9 ^ (4 + 1));
        w5 = rotateLeft(tt, 11);
        tt = w6 ^ w1 ^ w3 ^ w5 ^ ((unsigned)0x9E3779B9 ^ (4 + 2));
        w6 = rotateLeft(tt, 11);
        tt = w7 ^ w2 ^ w4 ^ w6 ^ ((unsigned)0x9E3779B9 ^ (4 + 3));
        w7 = rotateLeft(tt, 11);
        r0 = w4;
        r1 = w5;
        r2 = w6;
        r3 = w7;
        r4 = r0;
        r0 &= r2;
        r0 ^= r3;
        r2 ^= r1;
        r2 ^= r0;
        r3 |= r4;
        r3 ^= r1;
        r4 ^= r2;
        r1 = r3;
        r3 |= r4;
        r3 ^= r0;
        r0 &= r1;
        r4 ^= r0;
        r1 ^= r3;
        r1 ^= r4;
        r4 = ~r4;
        serpent24SubKeys[i ++] = r2;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r4;
        tt = w0 ^ w3 ^ w5 ^ w7 ^ ((unsigned)0x9E3779B9 ^ (8));
        w0 = rotateLeft(tt, 11);
        tt = w1 ^ w4 ^ w6 ^ w0 ^ ((unsigned)0x9E3779B9 ^ (8 + 1));
        w1 = rotateLeft(tt, 11);
        tt = w2 ^ w5 ^ w7 ^ w1 ^ ((unsigned)0x9E3779B9 ^ (8 + 2));
        w2 = rotateLeft(tt, 11);
        tt = w3 ^ w6 ^ w0 ^ w2 ^ ((unsigned)0x9E3779B9 ^ (8 + 3));
        w3 = rotateLeft(tt, 11);
        r0 = w0;
        r1 = w1;
        r2 = w2;
        r3 = w3;
        r0 = ~r0;
        r2 = ~r2;
        r4 = r0;
        r0 &= r1;
        r2 ^= r0;
        r0 |= r3;
        r3 ^= r2;
        r1 ^= r0;
        r0 ^= r4;
        r4 |= r1;
        r1 ^= r3;
        r2 |= r0;
        r2 &= r4;
        r0 ^= r1;
        r1 &= r2;
        r1 ^= r0;
        r0 &= r2;
        r0 ^= r4;
        serpent24SubKeys[i ++] = r2;
        serpent24SubKeys[i ++] = r0;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r1;
        tt = w4 ^ w7 ^ w1 ^ w3 ^ ((unsigned)0x9E3779B9 ^ (12));
        w4 = rotateLeft(tt, 11);
        tt = w5 ^ w0 ^ w2 ^ w4 ^ ((unsigned)0x9E3779B9 ^ (12 + 1));
        w5 = rotateLeft(tt, 11);
        tt = w6 ^ w1 ^ w3 ^ w5 ^ ((unsigned)0x9E3779B9 ^ (12 + 2));
        w6 = rotateLeft(tt, 11);
        tt = w7 ^ w2 ^ w4 ^ w6 ^ ((unsigned)0x9E3779B9 ^ (12 + 3));
        w7 = rotateLeft(tt, 11);
        r0 = w4;
        r1 = w5;
        r2 = w6;
        r3 = w7;
        r3 ^= r0;
        r4 = r1;
        r1 &= r3;
        r4 ^= r2;
        r1 ^= r0;
        r0 |= r3;
        r0 ^= r4;
        r4 ^= r3;
        r3 ^= r2;
        r2 |= r1;
        r2 ^= r4;
        r4 = ~r4;
        r4 |= r1;
        r1 ^= r3;
        r1 ^= r4;
        r3 |= r0;
        r1 ^= r3;
        r4 ^= r3;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r4;
        serpent24SubKeys[i ++] = r2;
        serpent24SubKeys[i ++] = r0;
        tt = w0 ^ w3 ^ w5 ^ w7 ^ ((unsigned)0x9E3779B9 ^ (16));
        w0 = rotateLeft(tt, 11);
        tt = w1 ^ w4 ^ w6 ^ w0 ^ ((unsigned)0x9E3779B9 ^ (16 + 1));
        w1 = rotateLeft(tt, 11);
        tt = w2 ^ w5 ^ w7 ^ w1 ^ ((unsigned)0x9E3779B9 ^ (16 + 2));
        w2 = rotateLeft(tt, 11);
        tt = w3 ^ w6 ^ w0 ^ w2 ^ ((unsigned)0x9E3779B9 ^ (16 + 3));
        w3 = rotateLeft(tt, 11);
        r0 = w0;
        r1 = w1;
        r2 = w2;
        r3 = w3;
        r4 = r1;
        r1 |= r2;
        r1 ^= r3;
        r4 ^= r2;
        r2 ^= r1;
        r3 |= r4;
        r3 &= r0;
        r4 ^= r2;
        r3 ^= r1;
        r1 |= r4;
        r1 ^= r0;
        r0 |= r4;
        r0 ^= r2;
        r1 ^= r4;
        r2 ^= r1;
        r1 &= r0;
        r1 ^= r4;
        r2 = ~r2;
        r2 |= r0;
        r4 ^= r2;
        serpent24SubKeys[i ++] = r4;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r0;
        tt = w4 ^ w7 ^ w1 ^ w3 ^ ((unsigned)0x9E3779B9 ^ (20));
        w4 = rotateLeft(tt, 11);
        tt = w5 ^ w0 ^ w2 ^ w4 ^ ((unsigned)0x9E3779B9 ^ (20 + 1));
        w5 = rotateLeft(tt, 11);
        tt = w6 ^ w1 ^ w3 ^ w5 ^ ((unsigned)0x9E3779B9 ^ (20 + 2));
        w6 = rotateLeft(tt, 11);
        tt = w7 ^ w2 ^ w4 ^ w6 ^ ((unsigned)0x9E3779B9 ^ (20 + 3));
        w7 = rotateLeft(tt, 11);
        r0 = w4;
        r1 = w5;
        r2 = w6;
        r3 = w7;
        r2 = ~r2;
        r4 = r3;
        r3 &= r0;
        r0 ^= r4;
        r3 ^= r2;
        r2 |= r4;
        r1 ^= r3;
        r2 ^= r0;
        r0 |= r1;
        r2 ^= r1;
        r4 ^= r0;
        r0 |= r3;
        r0 ^= r2;
        r4 ^= r3;
        r4 ^= r0;
        r3 = ~r3;
        r2 &= r4;
        r2 ^= r3;
        serpent24SubKeys[i ++] = r0;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r4;
        serpent24SubKeys[i ++] = r2;
        tt = w0 ^ w3 ^ w5 ^ w7 ^ ((unsigned)0x9E3779B9 ^ (24));
        w0 = rotateLeft(tt, 11);
        tt = w1 ^ w4 ^ w6 ^ w0 ^ ((unsigned)0x9E3779B9 ^ (24 + 1));
        w1 = rotateLeft(tt, 11);
        tt = w2 ^ w5 ^ w7 ^ w1 ^ ((unsigned)0x9E3779B9 ^ (24 + 2));
        w2 = rotateLeft(tt, 11);
        tt = w3 ^ w6 ^ w0 ^ w2 ^ ((unsigned)0x9E3779B9 ^ (24 + 3));
        w3 = rotateLeft(tt, 11);
        r0 = w0;
        r1 = w1;
        r2 = w2;
        r3 = w3;
        r0 ^= r1;
        r1 ^= r3;
        r3 = ~r3;
        r4 = r1;
        r1 &= r0;
        r2 ^= r3;
        r1 ^= r2;
        r2 |= r4;
        r4 ^= r3;
        r3 &= r1;
        r3 ^= r0;
        r4 ^= r1;
        r4 ^= r2;
        r2 ^= r0;
        r0 &= r3;
        r2 = ~r2;
        r0 ^= r4;
        r4 |= r3;
        r2 ^= r4;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r0;
        serpent24SubKeys[i ++] = r2;
        tt = w4 ^ w7 ^ w1 ^ w3 ^ ((unsigned)0x9E3779B9 ^ (28));
        w4 = rotateLeft(tt, 11);
        tt = w5 ^ w0 ^ w2 ^ w4 ^ ((unsigned)0x9E3779B9 ^ (28 + 1));
        w5 = rotateLeft(tt, 11);
        tt = w6 ^ w1 ^ w3 ^ w5 ^ ((unsigned)0x9E3779B9 ^ (28 + 2));
        w6 = rotateLeft(tt, 11);
        tt = w7 ^ w2 ^ w4 ^ w6 ^ ((unsigned)0x9E3779B9 ^ (28 + 3));
        w7 = rotateLeft(tt, 11);
        r0 = w4;
        r1 = w5;
        r2 = w6;
        r3 = w7;
        r1 ^= r3;
        r3 = ~r3;
        r2 ^= r3;
        r3 ^= r0;
        r4 = r1;
        r1 &= r3;
        r1 ^= r2;
        r4 ^= r3;
        r0 ^= r4;
        r2 &= r4;
        r2 ^= r0;
        r0 &= r1;
        r3 ^= r0;
        r4 |= r1;
        r4 ^= r0;
        r0 |= r3;
        r0 ^= r2;
        r2 &= r3;
        r0 = ~r0;
        r4 ^= r2;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r4;
        serpent24SubKeys[i ++] = r0;
        serpent24SubKeys[i ++] = r3;
        tt = w0 ^ w3 ^ w5 ^ w7 ^ ((unsigned)0x9E3779B9 ^ (32));
        w0 = rotateLeft(tt, 11);
        tt = w1 ^ w4 ^ w6 ^ w0 ^ ((unsigned)0x9E3779B9 ^ (32 + 1));
        w1 = rotateLeft(tt, 11);
        tt = w2 ^ w5 ^ w7 ^ w1 ^ ((unsigned)0x9E3779B9 ^ (32 + 2));
        w2 = rotateLeft(tt, 11);
        tt = w3 ^ w6 ^ w0 ^ w2 ^ ((unsigned)0x9E3779B9 ^ (32 + 3));
        w3 = rotateLeft(tt, 11);
        r0 = w0;
        r1 = w1;
        r2 = w2;
        r3 = w3;
        r4 = r0;
        r0 |= r3;
        r3 ^= r1;
        r1 &= r4;
        r4 ^= r2;
        r2 ^= r3;
        r3 &= r0;
        r4 |= r1;
        r3 ^= r4;
        r0 ^= r1;
        r4 &= r0;
        r1 ^= r3;
        r4 ^= r2;
        r1 |= r0;
        r1 ^= r2;
        r0 ^= r3;
        r2 = r1;
        r1 |= r3;
        r1 ^= r0;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r2;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r4;
        tt = w4 ^ w7 ^ w1 ^ w3 ^ ((unsigned)0x9E3779B9 ^ (36));
        w4 = rotateLeft(tt, 11);
        tt = w5 ^ w0 ^ w2 ^ w4 ^ ((unsigned)0x9E3779B9 ^ (36 + 1));
        w5 = rotateLeft(tt, 11);
        tt = w6 ^ w1 ^ w3 ^ w5 ^ ((unsigned)0x9E3779B9 ^ (36 + 2));
        w6 = rotateLeft(tt, 11);
        tt = w7 ^ w2 ^ w4 ^ w6 ^ ((unsigned)0x9E3779B9 ^ (36 + 3));
        w7 = rotateLeft(tt, 11);
        r0 = w4;
        r1 = w5;
        r2 = w6;
        r3 = w7;
        r4 = r0;
        r0 &= r2;
        r0 ^= r3;
        r2 ^= r1;
        r2 ^= r0;
        r3 |= r4;
        r3 ^= r1;
        r4 ^= r2;
        r1 = r3;
        r3 |= r4;
        r3 ^= r0;
        r0 &= r1;
        r4 ^= r0;
        r1 ^= r3;
        r1 ^= r4;
        r4 = ~r4;
        serpent24SubKeys[i ++] = r2;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r4;
        tt = w0 ^ w3 ^ w5 ^ w7 ^ ((unsigned)0x9E3779B9 ^ (40));
        w0 = rotateLeft(tt, 11);
        tt = w1 ^ w4 ^ w6 ^ w0 ^ ((unsigned)0x9E3779B9 ^ (40 + 1));
        w1 = rotateLeft(tt, 11);
        tt = w2 ^ w5 ^ w7 ^ w1 ^ ((unsigned)0x9E3779B9 ^ (40 + 2));
        w2 = rotateLeft(tt, 11);
        tt = w3 ^ w6 ^ w0 ^ w2 ^ ((unsigned)0x9E3779B9 ^ (40 + 3));
        w3 = rotateLeft(tt, 11);
        r0 = w0;
        r1 = w1;
        r2 = w2;
        r3 = w3;
        r0 = ~r0;
        r2 = ~r2;
        r4 = r0;
        r0 &= r1;
        r2 ^= r0;
        r0 |= r3;
        r3 ^= r2;
        r1 ^= r0;
        r0 ^= r4;
        r4 |= r1;
        r1 ^= r3;
        r2 |= r0;
        r2 &= r4;
        r0 ^= r1;
        r1 &= r2;
        r1 ^= r0;
        r0 &= r2;
        r0 ^= r4;
        serpent24SubKeys[i ++] = r2;
        serpent24SubKeys[i ++] = r0;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r1;
        tt = w4 ^ w7 ^ w1 ^ w3 ^ ((unsigned)0x9E3779B9 ^ (44));
        w4 = rotateLeft(tt, 11);
        tt = w5 ^ w0 ^ w2 ^ w4 ^ ((unsigned)0x9E3779B9 ^ (44 + 1));
        w5 = rotateLeft(tt, 11);
        tt = w6 ^ w1 ^ w3 ^ w5 ^ ((unsigned)0x9E3779B9 ^ (44 + 2));
        w6 = rotateLeft(tt, 11);
        tt = w7 ^ w2 ^ w4 ^ w6 ^ ((unsigned)0x9E3779B9 ^ (44 + 3));
        w7 = rotateLeft(tt, 11);
        r0 = w4;
        r1 = w5;
        r2 = w6;
        r3 = w7;
        r3 ^= r0;
        r4 = r1;
        r1 &= r3;
        r4 ^= r2;
        r1 ^= r0;
        r0 |= r3;
        r0 ^= r4;
        r4 ^= r3;
        r3 ^= r2;
        r2 |= r1;
        r2 ^= r4;
        r4 = ~r4;
        r4 |= r1;
        r1 ^= r3;
        r1 ^= r4;
        r3 |= r0;
        r1 ^= r3;
        r4 ^= r3;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r4;
        serpent24SubKeys[i ++] = r2;
        serpent24SubKeys[i ++] = r0;
        tt = w0 ^ w3 ^ w5 ^ w7 ^ ((unsigned)0x9E3779B9 ^ (48));
        w0 = rotateLeft(tt, 11);
        tt = w1 ^ w4 ^ w6 ^ w0 ^ ((unsigned)0x9E3779B9 ^ (48 + 1));
        w1 = rotateLeft(tt, 11);
        tt = w2 ^ w5 ^ w7 ^ w1 ^ ((unsigned)0x9E3779B9 ^ (48 + 2));
        w2 = rotateLeft(tt, 11);
        tt = w3 ^ w6 ^ w0 ^ w2 ^ ((unsigned)0x9E3779B9 ^ (48 + 3));
        w3 = rotateLeft(tt, 11);
        r0 = w0;
        r1 = w1;
        r2 = w2;
        r3 = w3;
        r4 = r1;
        r1 |= r2;
        r1 ^= r3;
        r4 ^= r2;
        r2 ^= r1;
        r3 |= r4;
        r3 &= r0;
        r4 ^= r2;
        r3 ^= r1;
        r1 |= r4;
        r1 ^= r0;
        r0 |= r4;
        r0 ^= r2;
        r1 ^= r4;
        r2 ^= r1;
        r1 &= r0;
        r1 ^= r4;
        r2 = ~r2;
        r2 |= r0;
        r4 ^= r2;
        serpent24SubKeys[i ++] = r4;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r0;
        tt = w4 ^ w7 ^ w1 ^ w3 ^ ((unsigned)0x9E3779B9 ^ (52));
        w4 = rotateLeft(tt, 11);
        tt = w5 ^ w0 ^ w2 ^ w4 ^ ((unsigned)0x9E3779B9 ^ (52 + 1));
        w5 = rotateLeft(tt, 11);
        tt = w6 ^ w1 ^ w3 ^ w5 ^ ((unsigned)0x9E3779B9 ^ (52 + 2));
        w6 = rotateLeft(tt, 11);
        tt = w7 ^ w2 ^ w4 ^ w6 ^ ((unsigned)0x9E3779B9 ^ (52 + 3));
        w7 = rotateLeft(tt, 11);
        r0 = w4;
        r1 = w5;
        r2 = w6;
        r3 = w7;
        r2 = ~r2;
        r4 = r3;
        r3 &= r0;
        r0 ^= r4;
        r3 ^= r2;
        r2 |= r4;
        r1 ^= r3;
        r2 ^= r0;
        r0 |= r1;
        r2 ^= r1;
        r4 ^= r0;
        r0 |= r3;
        r0 ^= r2;
        r4 ^= r3;
        r4 ^= r0;
        r3 = ~r3;
        r2 &= r4;
        r2 ^= r3;
        serpent24SubKeys[i ++] = r0;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r4;
        serpent24SubKeys[i ++] = r2;
        tt = w0 ^ w3 ^ w5 ^ w7 ^ ((unsigned)0x9E3779B9 ^ (56));
        w0 = rotateLeft(tt, 11);
        tt = w1 ^ w4 ^ w6 ^ w0 ^ ((unsigned)0x9E3779B9 ^ (56 + 1));
        w1 = rotateLeft(tt, 11);
        tt = w2 ^ w5 ^ w7 ^ w1 ^ ((unsigned)0x9E3779B9 ^ (56 + 2));
        w2 = rotateLeft(tt, 11);
        tt = w3 ^ w6 ^ w0 ^ w2 ^ ((unsigned)0x9E3779B9 ^ (56 + 3));
        w3 = rotateLeft(tt, 11);
        r0 = w0;
        r1 = w1;
        r2 = w2;
        r3 = w3;
        r0 ^= r1;
        r1 ^= r3;
        r3 = ~r3;
        r4 = r1;
        r1 &= r0;
        r2 ^= r3;
        r1 ^= r2;
        r2 |= r4;
        r4 ^= r3;
        r3 &= r1;
        r3 ^= r0;
        r4 ^= r1;
        r4 ^= r2;
        r2 ^= r0;
        r0 &= r3;
        r2 = ~r2;
        r0 ^= r4;
        r4 |= r3;
        r2 ^= r4;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r0;
        serpent24SubKeys[i ++] = r2;
        tt = w4 ^ w7 ^ w1 ^ w3 ^ ((unsigned)0x9E3779B9 ^ (60));
        w4 = rotateLeft(tt, 11);
        tt = w5 ^ w0 ^ w2 ^ w4 ^ ((unsigned)0x9E3779B9 ^ (60 + 1));
        w5 = rotateLeft(tt, 11);
        tt = w6 ^ w1 ^ w3 ^ w5 ^ ((unsigned)0x9E3779B9 ^ (60 + 2));
        w6 = rotateLeft(tt, 11);
        tt = w7 ^ w2 ^ w4 ^ w6 ^ ((unsigned)0x9E3779B9 ^ (60 + 3));
        w7 = rotateLeft(tt, 11);
        r0 = w4;
        r1 = w5;
        r2 = w6;
        r3 = w7;
        r1 ^= r3;
        r3 = ~r3;
        r2 ^= r3;
        r3 ^= r0;
        r4 = r1;
        r1 &= r3;
        r1 ^= r2;
        r4 ^= r3;
        r0 ^= r4;
        r2 &= r4;
        r2 ^= r0;
        r0 &= r1;
        r3 ^= r0;
        r4 |= r1;
        r4 ^= r0;
        r0 |= r3;
        r0 ^= r2;
        r2 &= r3;
        r0 = ~r0;
        r4 ^= r2;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r4;
        serpent24SubKeys[i ++] = r0;
        serpent24SubKeys[i ++] = r3;
        tt = w0 ^ w3 ^ w5 ^ w7 ^ ((unsigned)0x9E3779B9 ^ (64));
        w0 = rotateLeft(tt, 11);
        tt = w1 ^ w4 ^ w6 ^ w0 ^ ((unsigned)0x9E3779B9 ^ (64 + 1));
        w1 = rotateLeft(tt, 11);
        tt = w2 ^ w5 ^ w7 ^ w1 ^ ((unsigned)0x9E3779B9 ^ (64 + 2));
        w2 = rotateLeft(tt, 11);
        tt = w3 ^ w6 ^ w0 ^ w2 ^ ((unsigned)0x9E3779B9 ^ (64 + 3));
        w3 = rotateLeft(tt, 11);
        r0 = w0;
        r1 = w1;
        r2 = w2;
        r3 = w3;
        r4 = r0;
        r0 |= r3;
        r3 ^= r1;
        r1 &= r4;
        r4 ^= r2;
        r2 ^= r3;
        r3 &= r0;
        r4 |= r1;
        r3 ^= r4;
        r0 ^= r1;
        r4 &= r0;
        r1 ^= r3;
        r4 ^= r2;
        r1 |= r0;
        r1 ^= r2;
        r0 ^= r3;
        r2 = r1;
        r1 |= r3;
        r1 ^= r0;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r2;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r4;
        tt = w4 ^ w7 ^ w1 ^ w3 ^ ((unsigned)0x9E3779B9 ^ (68));
        w4 = rotateLeft(tt, 11);
        tt = w5 ^ w0 ^ w2 ^ w4 ^ ((unsigned)0x9E3779B9 ^ (68 + 1));
        w5 = rotateLeft(tt, 11);
        tt = w6 ^ w1 ^ w3 ^ w5 ^ ((unsigned)0x9E3779B9 ^ (68 + 2));
        w6 = rotateLeft(tt, 11);
        tt = w7 ^ w2 ^ w4 ^ w6 ^ ((unsigned)0x9E3779B9 ^ (68 + 3));
        w7 = rotateLeft(tt, 11);
        r0 = w4;
        r1 = w5;
        r2 = w6;
        r3 = w7;
        r4 = r0;
        r0 &= r2;
        r0 ^= r3;
        r2 ^= r1;
        r2 ^= r0;
        r3 |= r4;
        r3 ^= r1;
        r4 ^= r2;
        r1 = r3;
        r3 |= r4;
        r3 ^= r0;
        r0 &= r1;
        r4 ^= r0;
        r1 ^= r3;
        r1 ^= r4;
        r4 = ~r4;
        serpent24SubKeys[i ++] = r2;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r4;
        tt = w0 ^ w3 ^ w5 ^ w7 ^ ((unsigned)0x9E3779B9 ^ (72));
        w0 = rotateLeft(tt, 11);
        tt = w1 ^ w4 ^ w6 ^ w0 ^ ((unsigned)0x9E3779B9 ^ (72 + 1));
        w1 = rotateLeft(tt, 11);
        tt = w2 ^ w5 ^ w7 ^ w1 ^ ((unsigned)0x9E3779B9 ^ (72 + 2));
        w2 = rotateLeft(tt, 11);
        tt = w3 ^ w6 ^ w0 ^ w2 ^ ((unsigned)0x9E3779B9 ^ (72 + 3));
        w3 = rotateLeft(tt, 11);
        r0 = w0;
        r1 = w1;
        r2 = w2;
        r3 = w3;
        r0 = ~r0;
        r2 = ~r2;
        r4 = r0;
        r0 &= r1;
        r2 ^= r0;
        r0 |= r3;
        r3 ^= r2;
        r1 ^= r0;
        r0 ^= r4;
        r4 |= r1;
        r1 ^= r3;
        r2 |= r0;
        r2 &= r4;
        r0 ^= r1;
        r1 &= r2;
        r1 ^= r0;
        r0 &= r2;
        r0 ^= r4;
        serpent24SubKeys[i ++] = r2;
        serpent24SubKeys[i ++] = r0;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r1;
        tt = w4 ^ w7 ^ w1 ^ w3 ^ ((unsigned)0x9E3779B9 ^ (76));
        w4 = rotateLeft(tt, 11);
        tt = w5 ^ w0 ^ w2 ^ w4 ^ ((unsigned)0x9E3779B9 ^ (76 + 1));
        w5 = rotateLeft(tt, 11);
        tt = w6 ^ w1 ^ w3 ^ w5 ^ ((unsigned)0x9E3779B9 ^ (76 + 2));
        w6 = rotateLeft(tt, 11);
        tt = w7 ^ w2 ^ w4 ^ w6 ^ ((unsigned)0x9E3779B9 ^ (76 + 3));
        w7 = rotateLeft(tt, 11);
        r0 = w4;
        r1 = w5;
        r2 = w6;
        r3 = w7;
        r3 ^= r0;
        r4 = r1;
        r1 &= r3;
        r4 ^= r2;
        r1 ^= r0;
        r0 |= r3;
        r0 ^= r4;
        r4 ^= r3;
        r3 ^= r2;
        r2 |= r1;
        r2 ^= r4;
        r4 = ~r4;
        r4 |= r1;
        r1 ^= r3;
        r1 ^= r4;
        r3 |= r0;
        r1 ^= r3;
        r4 ^= r3;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r4;
        serpent24SubKeys[i ++] = r2;
        serpent24SubKeys[i ++] = r0;
        tt = w0 ^ w3 ^ w5 ^ w7 ^ ((unsigned)0x9E3779B9 ^ (80));
        w0 = rotateLeft(tt, 11);
        tt = w1 ^ w4 ^ w6 ^ w0 ^ ((unsigned)0x9E3779B9 ^ (80 + 1));
        w1 = rotateLeft(tt, 11);
        tt = w2 ^ w5 ^ w7 ^ w1 ^ ((unsigned)0x9E3779B9 ^ (80 + 2));
        w2 = rotateLeft(tt, 11);
        tt = w3 ^ w6 ^ w0 ^ w2 ^ ((unsigned)0x9E3779B9 ^ (80 + 3));
        w3 = rotateLeft(tt, 11);
        r0 = w0;
        r1 = w1;
        r2 = w2;
        r3 = w3;
        r4 = r1;
        r1 |= r2;
        r1 ^= r3;
        r4 ^= r2;
        r2 ^= r1;
        r3 |= r4;
        r3 &= r0;
        r4 ^= r2;
        r3 ^= r1;
        r1 |= r4;
        r1 ^= r0;
        r0 |= r4;
        r0 ^= r2;
        r1 ^= r4;
        r2 ^= r1;
        r1 &= r0;
        r1 ^= r4;
        r2 = ~r2;
        r2 |= r0;
        r4 ^= r2;
        serpent24SubKeys[i ++] = r4;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r0;
        tt = w4 ^ w7 ^ w1 ^ w3 ^ ((unsigned)0x9E3779B9 ^ (84));
        w4 = rotateLeft(tt, 11);
        tt = w5 ^ w0 ^ w2 ^ w4 ^ ((unsigned)0x9E3779B9 ^ (84 + 1));
        w5 = rotateLeft(tt, 11);
        tt = w6 ^ w1 ^ w3 ^ w5 ^ ((unsigned)0x9E3779B9 ^ (84 + 2));
        w6 = rotateLeft(tt, 11);
        tt = w7 ^ w2 ^ w4 ^ w6 ^ ((unsigned)0x9E3779B9 ^ (84 + 3));
        w7 = rotateLeft(tt, 11);
        r0 = w4;
        r1 = w5;
        r2 = w6;
        r3 = w7;
        r2 = ~r2;
        r4 = r3;
        r3 &= r0;
        r0 ^= r4;
        r3 ^= r2;
        r2 |= r4;
        r1 ^= r3;
        r2 ^= r0;
        r0 |= r1;
        r2 ^= r1;
        r4 ^= r0;
        r0 |= r3;
        r0 ^= r2;
        r4 ^= r3;
        r4 ^= r0;
        r3 = ~r3;
        r2 &= r4;
        r2 ^= r3;
        serpent24SubKeys[i ++] = r0;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r4;
        serpent24SubKeys[i ++] = r2;
        tt = w0 ^ w3 ^ w5 ^ w7 ^ ((unsigned)0x9E3779B9 ^ (88));
        w0 = rotateLeft(tt, 11);
        tt = w1 ^ w4 ^ w6 ^ w0 ^ ((unsigned)0x9E3779B9 ^ (88 + 1));
        w1 = rotateLeft(tt, 11);
        tt = w2 ^ w5 ^ w7 ^ w1 ^ ((unsigned)0x9E3779B9 ^ (88 + 2));
        w2 = rotateLeft(tt, 11);
        tt = w3 ^ w6 ^ w0 ^ w2 ^ ((unsigned)0x9E3779B9 ^ (88 + 3));
        w3 = rotateLeft(tt, 11);
        r0 = w0;
        r1 = w1;
        r2 = w2;
        r3 = w3;
        r0 ^= r1;
        r1 ^= r3;
        r3 = ~r3;
        r4 = r1;
        r1 &= r0;
        r2 ^= r3;
        r1 ^= r2;
        r2 |= r4;
        r4 ^= r3;
        r3 &= r1;
        r3 ^= r0;
        r4 ^= r1;
        r4 ^= r2;
        r2 ^= r0;
        r0 &= r3;
        r2 = ~r2;
        r0 ^= r4;
        r4 |= r3;
        r2 ^= r4;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r0;
        serpent24SubKeys[i ++] = r2;
        tt = w4 ^ w7 ^ w1 ^ w3 ^ ((unsigned)0x9E3779B9 ^ (92));
        w4 = rotateLeft(tt, 11);
        tt = w5 ^ w0 ^ w2 ^ w4 ^ ((unsigned)0x9E3779B9 ^ (92 + 1));
        w5 = rotateLeft(tt, 11);
        tt = w6 ^ w1 ^ w3 ^ w5 ^ ((unsigned)0x9E3779B9 ^ (92 + 2));
        w6 = rotateLeft(tt, 11);
        tt = w7 ^ w2 ^ w4 ^ w6 ^ ((unsigned)0x9E3779B9 ^ (92 + 3));
        w7 = rotateLeft(tt, 11);
        r0 = w4;
        r1 = w5;
        r2 = w6;
        r3 = w7;
        r1 ^= r3;
        r3 = ~r3;
        r2 ^= r3;
        r3 ^= r0;
        r4 = r1;
        r1 &= r3;
        r1 ^= r2;
        r4 ^= r3;
        r0 ^= r4;
        r2 &= r4;
        r2 ^= r0;
        r0 &= r1;
        r3 ^= r0;
        r4 |= r1;
        r4 ^= r0;
        r0 |= r3;
        r0 ^= r2;
        r2 &= r3;
        r0 = ~r0;
        r4 ^= r2;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r4;
        serpent24SubKeys[i ++] = r0;
        serpent24SubKeys[i ++] = r3;
        tt = w0 ^ w3 ^ w5 ^ w7 ^ ((unsigned)0x9E3779B9 ^ (96));
        w0 = rotateLeft(tt, 11);
        tt = w1 ^ w4 ^ w6 ^ w0 ^ ((unsigned)0x9E3779B9 ^ (96 + 1));
        w1 = rotateLeft(tt, 11);
        tt = w2 ^ w5 ^ w7 ^ w1 ^ ((unsigned)0x9E3779B9 ^ (96 + 2));
        w2 = rotateLeft(tt, 11);
        tt = w3 ^ w6 ^ w0 ^ w2 ^ ((unsigned)0x9E3779B9 ^ (96 + 3));
        w3 = rotateLeft(tt, 11);
        r0 = w0;
        r1 = w1;
        r2 = w2;
        r3 = w3;
        r4 = r0;
        r0 |= r3;
        r3 ^= r1;
        r1 &= r4;
        r4 ^= r2;
        r2 ^= r3;
        r3 &= r0;
        r4 |= r1;
        r3 ^= r4;
        r0 ^= r1;
        r4 &= r0;
        r1 ^= r3;
        r4 ^= r2;
        r1 |= r0;
        r1 ^= r2;
        r0 ^= r3;
        r2 = r1;
        r1 |= r3;
        r1 ^= r0;
        serpent24SubKeys[i ++] = r1;
        serpent24SubKeys[i ++] = r2;
        serpent24SubKeys[i ++] = r3;
        serpent24SubKeys[i ++] = r4;
    }



// Inicjacja wartoœci pocz¹tkowych za pomoc¹ podanego wektora pocz¹tkowego

void setIV(byte iv[])
    {
        if (ivSize > 16)
            cout<<"bad IV length: " + ivSize;
        byte piv[16];
        if (ivSize == 16) {
            for(int i = 0; i < 16; i++)
            	piv[i] = iv[i];
        } else {
        	for(int i = 0; i < ivSize; i++)
        		piv[i] = iv[i];
            for (int i = ivSize; i < sizeof(piv)/sizeof(piv[0]); i ++)
                piv[i] = 0x00;
        }

        unsigned r0, r1, r2, r3, r4;

        r0 = decode32le(piv, 0);
        r1 = decode32le(piv, 4);
        r2 = decode32le(piv, 8);
        r3 = decode32le(piv, 12);

        r0 ^= serpent24SubKeys[0];
        r1 ^= serpent24SubKeys[0 + 1];
        r2 ^= serpent24SubKeys[0 + 2];
        r3 ^= serpent24SubKeys[0 + 3];
        r3 ^= r0;
        r4 = r1;
        r1 &= r3;
        r4 ^= r2;
        r1 ^= r0;
        r0 |= r3;
        r0 ^= r4;
        r4 ^= r3;
        r3 ^= r2;
        r2 |= r1;
        r2 ^= r4;
        r4 = ~r4;
        r4 |= r1;
        r1 ^= r3;
        r1 ^= r4;
        r3 |= r0;
        r1 ^= r3;
        r4 ^= r3;
        r1 = rotateLeft(r1, 13);
        r2 = rotateLeft(r2, 3);
        r4 = r4 ^ r1 ^ r2;
        r0 = r0 ^ r2 ^ (r1 << 3);
        r4 = rotateLeft(r4, 1);
        r0 = rotateLeft(r0, 7);
        r1 = r1 ^ r4 ^ r0;
        r2 = r2 ^ r0 ^ (r4 << 7);
        r1 = rotateLeft(r1, 5);
        r2 = rotateLeft(r2, 22);
        r1 ^= serpent24SubKeys[4];
        r4 ^= serpent24SubKeys[4 + 1];
        r2 ^= serpent24SubKeys[4 + 2];
        r0 ^= serpent24SubKeys[4 + 3];
        r1 = ~r1;
        r2 = ~r2;
        r3 = r1;
        r1 &= r4;
        r2 ^= r1;
        r1 |= r0;
        r0 ^= r2;
        r4 ^= r1;
        r1 ^= r3;
        r3 |= r4;
        r4 ^= r0;
        r2 |= r1;
        r2 &= r3;
        r1 ^= r4;
        r4 &= r2;
        r4 ^= r1;
        r1 &= r2;
        r1 ^= r3;
        r2 = rotateLeft(r2, 13);
        r0 = rotateLeft(r0, 3);
        r1 = r1 ^ r2 ^ r0;
        r4 = r4 ^ r0 ^ (r2 << 3);
        r1 = rotateLeft(r1, 1);
        r4 = rotateLeft(r4, 7);
        r2 = r2 ^ r1 ^ r4;
        r0 = r0 ^ r4 ^ (r1 << 7);
        r2 = rotateLeft(r2, 5);
        r0 = rotateLeft(r0, 22);
        r2 ^= serpent24SubKeys[8];
        r1 ^= serpent24SubKeys[8 + 1];
        r0 ^= serpent24SubKeys[8 + 2];
        r4 ^= serpent24SubKeys[8 + 3];
        r3 = r2;
        r2 &= r0;
        r2 ^= r4;
        r0 ^= r1;
        r0 ^= r2;
        r4 |= r3;
        r4 ^= r1;
        r3 ^= r0;
        r1 = r4;
        r4 |= r3;
        r4 ^= r2;
        r2 &= r1;
        r3 ^= r2;
        r1 ^= r4;
        r1 ^= r3;
        r3 = ~r3;
        r0 = rotateLeft(r0, 13);
        r1 = rotateLeft(r1, 3);
        r4 = r4 ^ r0 ^ r1;
        r3 = r3 ^ r1 ^ (r0 << 3);
        r4 = rotateLeft(r4, 1);
        r3 = rotateLeft(r3, 7);
        r0 = r0 ^ r4 ^ r3;
        r1 = r1 ^ r3 ^ (r4 << 7);
        r0 = rotateLeft(r0, 5);
        r1 = rotateLeft(r1, 22);
        r0 ^= serpent24SubKeys[12];
        r4 ^= serpent24SubKeys[12 + 1];
        r1 ^= serpent24SubKeys[12 + 2];
        r3 ^= serpent24SubKeys[12 + 3];
        r2 = r0;
        r0 |= r3;
        r3 ^= r4;
        r4 &= r2;
        r2 ^= r1;
        r1 ^= r3;
        r3 &= r0;
        r2 |= r4;
        r3 ^= r2;
        r0 ^= r4;
        r2 &= r0;
        r4 ^= r3;
        r2 ^= r1;
        r4 |= r0;
        r4 ^= r1;
        r0 ^= r3;
        r1 = r4;
        r4 |= r3;
        r4 ^= r0;
        r4 = rotateLeft(r4, 13);
        r3 = rotateLeft(r3, 3);
        r1 = r1 ^ r4 ^ r3;
        r2 = r2 ^ r3 ^ (r4 << 3);
        r1 = rotateLeft(r1, 1);
        r2 = rotateLeft(r2, 7);
        r4 = r4 ^ r1 ^ r2;
        r3 = r3 ^ r2 ^ (r1 << 7);
        r4 = rotateLeft(r4, 5);
        r3 = rotateLeft(r3, 22);
        r4 ^= serpent24SubKeys[16];
        r1 ^= serpent24SubKeys[16 + 1];
        r3 ^= serpent24SubKeys[16 + 2];
        r2 ^= serpent24SubKeys[16 + 3];
        r1 ^= r2;
        r2 = ~r2;
        r3 ^= r2;
        r2 ^= r4;
        r0 = r1;
        r1 &= r2;
        r1 ^= r3;
        r0 ^= r2;
        r4 ^= r0;
        r3 &= r0;
        r3 ^= r4;
        r4 &= r1;
        r2 ^= r4;
        r0 |= r1;
        r0 ^= r4;
        r4 |= r2;
        r4 ^= r3;
        r3 &= r2;
        r4 = ~r4;
        r0 ^= r3;
        r1 = rotateLeft(r1, 13);
        r4 = rotateLeft(r4, 3);
        r0 = r0 ^ r1 ^ r4;
        r2 = r2 ^ r4 ^ (r1 << 3);
        r0 = rotateLeft(r0, 1);
        r2 = rotateLeft(r2, 7);
        r1 = r1 ^ r0 ^ r2;
        r4 = r4 ^ r2 ^ (r0 << 7);
        r1 = rotateLeft(r1, 5);
        r4 = rotateLeft(r4, 22);
        r1 ^= serpent24SubKeys[20];
        r0 ^= serpent24SubKeys[20 + 1];
        r4 ^= serpent24SubKeys[20 + 2];
        r2 ^= serpent24SubKeys[20 + 3];
        r1 ^= r0;
        r0 ^= r2;
        r2 = ~r2;
        r3 = r0;
        r0 &= r1;
        r4 ^= r2;
        r0 ^= r4;
        r4 |= r3;
        r3 ^= r2;
        r2 &= r0;
        r2 ^= r1;
        r3 ^= r0;
        r3 ^= r4;
        r4 ^= r1;
        r1 &= r2;
        r4 = ~r4;
        r1 ^= r3;
        r3 |= r2;
        r4 ^= r3;
        r0 = rotateLeft(r0, 13);
        r1 = rotateLeft(r1, 3);
        r2 = r2 ^ r0 ^ r1;
        r4 = r4 ^ r1 ^ (r0 << 3);
        r2 = rotateLeft(r2, 1);
        r4 = rotateLeft(r4, 7);
        r0 = r0 ^ r2 ^ r4;
        r1 = r1 ^ r4 ^ (r2 << 7);
        r0 = rotateLeft(r0, 5);
        r1 = rotateLeft(r1, 22);
        r0 ^= serpent24SubKeys[24];
        r2 ^= serpent24SubKeys[24 + 1];
        r1 ^= serpent24SubKeys[24 + 2];
        r4 ^= serpent24SubKeys[24 + 3];
        r1 = ~r1;
        r3 = r4;
        r4 &= r0;
        r0 ^= r3;
        r4 ^= r1;
        r1 |= r3;
        r2 ^= r4;
        r1 ^= r0;
        r0 |= r2;
        r1 ^= r2;
        r3 ^= r0;
        r0 |= r4;
        r0 ^= r1;
        r3 ^= r4;
        r3 ^= r0;
        r4 = ~r4;
        r1 &= r3;
        r1 ^= r4;
        r0 = rotateLeft(r0, 13);
        r3 = rotateLeft(r3, 3);
        r2 = r2 ^ r0 ^ r3;
        r1 = r1 ^ r3 ^ (r0 << 3);
        r2 = rotateLeft(r2, 1);
        r1 = rotateLeft(r1, 7);
        r0 = r0 ^ r2 ^ r1;
        r3 = r3 ^ r1 ^ (r2 << 7);
        r0 = rotateLeft(r0, 5);
        r3 = rotateLeft(r3, 22);
        r0 ^= serpent24SubKeys[28];
        r2 ^= serpent24SubKeys[28 + 1];
        r3 ^= serpent24SubKeys[28 + 2];
        r1 ^= serpent24SubKeys[28 + 3];
        r4 = r2;
        r2 |= r3;
        r2 ^= r1;
        r4 ^= r3;
        r3 ^= r2;
        r1 |= r4;
        r1 &= r0;
        r4 ^= r3;
        r1 ^= r2;
        r2 |= r4;
        r2 ^= r0;
        r0 |= r4;
        r0 ^= r3;
        r2 ^= r4;
        r3 ^= r2;
        r2 &= r0;
        r2 ^= r4;
        r3 = ~r3;
        r3 |= r0;
        r4 ^= r3;
        r4 = rotateLeft(r4, 13);
        r2 = rotateLeft(r2, 3);
        r1 = r1 ^ r4 ^ r2;
        r0 = r0 ^ r2 ^ (r4 << 3);
        r1 = rotateLeft(r1, 1);
        r0 = rotateLeft(r0, 7);
        r4 = r4 ^ r1 ^ r0;
        r2 = r2 ^ r0 ^ (r1 << 7);
        r4 = rotateLeft(r4, 5);
        r2 = rotateLeft(r2, 22);
        r4 ^= serpent24SubKeys[32];
        r1 ^= serpent24SubKeys[32 + 1];
        r2 ^= serpent24SubKeys[32 + 2];
        r0 ^= serpent24SubKeys[32 + 3];
        r0 ^= r4;
        r3 = r1;
        r1 &= r0;
        r3 ^= r2;
        r1 ^= r4;
        r4 |= r0;
        r4 ^= r3;
        r3 ^= r0;
        r0 ^= r2;
        r2 |= r1;
        r2 ^= r3;
        r3 = ~r3;
        r3 |= r1;
        r1 ^= r0;
        r1 ^= r3;
        r0 |= r4;
        r1 ^= r0;
        r3 ^= r0;
        r1 = rotateLeft(r1, 13);
        r2 = rotateLeft(r2, 3);
        r3 = r3 ^ r1 ^ r2;
        r4 = r4 ^ r2 ^ (r1 << 3);
        r3 = rotateLeft(r3, 1);
        r4 = rotateLeft(r4, 7);
        r1 = r1 ^ r3 ^ r4;
        r2 = r2 ^ r4 ^ (r3 << 7);
        r1 = rotateLeft(r1, 5);
        r2 = rotateLeft(r2, 22);
        r1 ^= serpent24SubKeys[36];
        r3 ^= serpent24SubKeys[36 + 1];
        r2 ^= serpent24SubKeys[36 + 2];
        r4 ^= serpent24SubKeys[36 + 3];
        r1 = ~r1;
        r2 = ~r2;
        r0 = r1;
        r1 &= r3;
        r2 ^= r1;
        r1 |= r4;
        r4 ^= r2;
        r3 ^= r1;
        r1 ^= r0;
        r0 |= r3;
        r3 ^= r4;
        r2 |= r1;
        r2 &= r0;
        r1 ^= r3;
        r3 &= r2;
        r3 ^= r1;
        r1 &= r2;
        r1 ^= r0;
        r2 = rotateLeft(r2, 13);
        r4 = rotateLeft(r4, 3);
        r1 = r1 ^ r2 ^ r4;
        r3 = r3 ^ r4 ^ (r2 << 3);
        r1 = rotateLeft(r1, 1);
        r3 = rotateLeft(r3, 7);
        r2 = r2 ^ r1 ^ r3;
        r4 = r4 ^ r3 ^ (r1 << 7);
        r2 = rotateLeft(r2, 5);
        r4 = rotateLeft(r4, 22);
        r2 ^= serpent24SubKeys[40];
        r1 ^= serpent24SubKeys[40 + 1];
        r4 ^= serpent24SubKeys[40 + 2];
        r3 ^= serpent24SubKeys[40 + 3];
        r0 = r2;
        r2 &= r4;
        r2 ^= r3;
        r4 ^= r1;
        r4 ^= r2;
        r3 |= r0;
        r3 ^= r1;
        r0 ^= r4;
        r1 = r3;
        r3 |= r0;
        r3 ^= r2;
        r2 &= r1;
        r0 ^= r2;
        r1 ^= r3;
        r1 ^= r0;
        r0 = ~r0;
        r4 = rotateLeft(r4, 13);
        r1 = rotateLeft(r1, 3);
        r3 = r3 ^ r4 ^ r1;
        r0 = r0 ^ r1 ^ (r4 << 3);
        r3 = rotateLeft(r3, 1);
        r0 = rotateLeft(r0, 7);
        r4 = r4 ^ r3 ^ r0;
        r1 = r1 ^ r0 ^ (r3 << 7);
        r4 = rotateLeft(r4, 5);
        r1 = rotateLeft(r1, 22);
        r4 ^= serpent24SubKeys[44];
        r3 ^= serpent24SubKeys[44 + 1];
        r1 ^= serpent24SubKeys[44 + 2];
        r0 ^= serpent24SubKeys[44 + 3];
        r2 = r4;
        r4 |= r0;
        r0 ^= r3;
        r3 &= r2;
        r2 ^= r1;
        r1 ^= r0;
        r0 &= r4;
        r2 |= r3;
        r0 ^= r2;
        r4 ^= r3;
        r2 &= r4;
        r3 ^= r0;
        r2 ^= r1;
        r3 |= r4;
        r3 ^= r1;
        r4 ^= r0;
        r1 = r3;
        r3 |= r0;
        r3 ^= r4;
        r3 = rotateLeft(r3, 13);
        r0 = rotateLeft(r0, 3);
        r1 = r1 ^ r3 ^ r0;
        r2 = r2 ^ r0 ^ (r3 << 3);
        r1 = rotateLeft(r1, 1);
        r2 = rotateLeft(r2, 7);
        r3 = r3 ^ r1 ^ r2;
        r0 = r0 ^ r2 ^ (r1 << 7);
        r3 = rotateLeft(r3, 5);
        r0 = rotateLeft(r0, 22);
        lfsr[9] = r3;
        lfsr[8] = r1;
        lfsr[7] = r0;
        lfsr[6] = r2;
        r3 ^= serpent24SubKeys[48];
        r1 ^= serpent24SubKeys[48 + 1];
        r0 ^= serpent24SubKeys[48 + 2];
        r2 ^= serpent24SubKeys[48 + 3];
        r1 ^= r2;
        r2 = ~r2;
        r0 ^= r2;
        r2 ^= r3;
        r4 = r1;
        r1 &= r2;
        r1 ^= r0;
        r4 ^= r2;
        r3 ^= r4;
        r0 &= r4;
        r0 ^= r3;
        r3 &= r1;
        r2 ^= r3;
        r4 |= r1;
        r4 ^= r3;
        r3 |= r2;
        r3 ^= r0;
        r0 &= r2;
        r3 = ~r3;
        r4 ^= r0;
        r1 = rotateLeft(r1, 13);
        r3 = rotateLeft(r3, 3);
        r4 = r4 ^ r1 ^ r3;
        r2 = r2 ^ r3 ^ (r1 << 3);
        r4 = rotateLeft(r4, 1);
        r2 = rotateLeft(r2, 7);
        r1 = r1 ^ r4 ^ r2;
        r3 = r3 ^ r2 ^ (r4 << 7);
        r1 = rotateLeft(r1, 5);
        r3 = rotateLeft(r3, 22);
        r1 ^= serpent24SubKeys[52];
        r4 ^= serpent24SubKeys[52 + 1];
        r3 ^= serpent24SubKeys[52 + 2];
        r2 ^= serpent24SubKeys[52 + 3];
        r1 ^= r4;
        r4 ^= r2;
        r2 = ~r2;
        r0 = r4;
        r4 &= r1;
        r3 ^= r2;
        r4 ^= r3;
        r3 |= r0;
        r0 ^= r2;
        r2 &= r4;
        r2 ^= r1;
        r0 ^= r4;
        r0 ^= r3;
        r3 ^= r1;
        r1 &= r2;
        r3 = ~r3;
        r1 ^= r0;
        r0 |= r2;
        r3 ^= r0;
        r4 = rotateLeft(r4, 13);
        r1 = rotateLeft(r1, 3);
        r2 = r2 ^ r4 ^ r1;
        r3 = r3 ^ r1 ^ (r4 << 3);
        r2 = rotateLeft(r2, 1);
        r3 = rotateLeft(r3, 7);
        r4 = r4 ^ r2 ^ r3;
        r1 = r1 ^ r3 ^ (r2 << 7);
        r4 = rotateLeft(r4, 5);
        r1 = rotateLeft(r1, 22);
        r4 ^= serpent24SubKeys[56];
        r2 ^= serpent24SubKeys[56 + 1];
        r1 ^= serpent24SubKeys[56 + 2];
        r3 ^= serpent24SubKeys[56 + 3];
        r1 = ~r1;
        r0 = r3;
        r3 &= r4;
        r4 ^= r0;
        r3 ^= r1;
        r1 |= r0;
        r2 ^= r3;
        r1 ^= r4;
        r4 |= r2;
        r1 ^= r2;
        r0 ^= r4;
        r4 |= r3;
        r4 ^= r1;
        r0 ^= r3;
        r0 ^= r4;
        r3 = ~r3;
        r1 &= r0;
        r1 ^= r3;
        r4 = rotateLeft(r4, 13);
        r0 = rotateLeft(r0, 3);
        r2 = r2 ^ r4 ^ r0;
        r1 = r1 ^ r0 ^ (r4 << 3);
        r2 = rotateLeft(r2, 1);
        r1 = rotateLeft(r1, 7);
        r4 = r4 ^ r2 ^ r1;
        r0 = r0 ^ r1 ^ (r2 << 7);
        r4 = rotateLeft(r4, 5);
        r0 = rotateLeft(r0, 22);
        r4 ^= serpent24SubKeys[60];
        r2 ^= serpent24SubKeys[60 + 1];
        r0 ^= serpent24SubKeys[60 + 2];
        r1 ^= serpent24SubKeys[60 + 3];
        r3 = r2;
        r2 |= r0;
        r2 ^= r1;
        r3 ^= r0;
        r0 ^= r2;
        r1 |= r3;
        r1 &= r4;
        r3 ^= r0;
        r1 ^= r2;
        r2 |= r3;
        r2 ^= r4;
        r4 |= r3;
        r4 ^= r0;
        r2 ^= r3;
        r0 ^= r2;
        r2 &= r4;
        r2 ^= r3;
        r0 = ~r0;
        r0 |= r4;
        r3 ^= r0;
        r3 = rotateLeft(r3, 13);
        r2 = rotateLeft(r2, 3);
        r1 = r1 ^ r3 ^ r2;
        r4 = r4 ^ r2 ^ (r3 << 3);
        r1 = rotateLeft(r1, 1);
        r4 = rotateLeft(r4, 7);
        r3 = r3 ^ r1 ^ r4;
        r2 = r2 ^ r4 ^ (r1 << 7);
        r3 = rotateLeft(r3, 5);
        r2 = rotateLeft(r2, 22);
        r3 ^= serpent24SubKeys[64];
        r1 ^= serpent24SubKeys[64 + 1];
        r2 ^= serpent24SubKeys[64 + 2];
        r4 ^= serpent24SubKeys[64 + 3];
        r4 ^= r3;
        r0 = r1;
        r1 &= r4;
        r0 ^= r2;
        r1 ^= r3;
        r3 |= r4;
        r3 ^= r0;
        r0 ^= r4;
        r4 ^= r2;
        r2 |= r1;
        r2 ^= r0;
        r0 = ~r0;
        r0 |= r1;
        r1 ^= r4;
        r1 ^= r0;
        r4 |= r3;
        r1 ^= r4;
        r0 ^= r4;
        r1 = rotateLeft(r1, 13);
        r2 = rotateLeft(r2, 3);
        r0 = r0 ^ r1 ^ r2;
        r3 = r3 ^ r2 ^ (r1 << 3);
        r0 = rotateLeft(r0, 1);
        r3 = rotateLeft(r3, 7);
        r1 = r1 ^ r0 ^ r3;
        r2 = r2 ^ r3 ^ (r0 << 7);
        r1 = rotateLeft(r1, 5);
        r2 = rotateLeft(r2, 22);
        r1 ^= serpent24SubKeys[68];
        r0 ^= serpent24SubKeys[68 + 1];
        r2 ^= serpent24SubKeys[68 + 2];
        r3 ^= serpent24SubKeys[68 + 3];
        r1 = ~r1;
        r2 = ~r2;
        r4 = r1;
        r1 &= r0;
        r2 ^= r1;
        r1 |= r3;
        r3 ^= r2;
        r0 ^= r1;
        r1 ^= r4;
        r4 |= r0;
        r0 ^= r3;
        r2 |= r1;
        r2 &= r4;
        r1 ^= r0;
        r0 &= r2;
        r0 ^= r1;
        r1 &= r2;
        r1 ^= r4;
        r2 = rotateLeft(r2, 13);
        r3 = rotateLeft(r3, 3);
        r1 = r1 ^ r2 ^ r3;
        r0 = r0 ^ r3 ^ (r2 << 3);
        r1 = rotateLeft(r1, 1);
        r0 = rotateLeft(r0, 7);
        r2 = r2 ^ r1 ^ r0;
        r3 = r3 ^ r0 ^ (r1 << 7);
        r2 = rotateLeft(r2, 5);
        r3 = rotateLeft(r3, 22);
        fsmR1 = r2;
        lfsr[4] = r1;
        fsmR2 = r3;
        lfsr[5] = r0;
        r2 ^= serpent24SubKeys[72];
        r1 ^= serpent24SubKeys[72 + 1];
        r3 ^= serpent24SubKeys[72 + 2];
        r0 ^= serpent24SubKeys[72 + 3];
        r4 = r2;
        r2 &= r3;
        r2 ^= r0;
        r3 ^= r1;
        r3 ^= r2;
        r0 |= r4;
        r0 ^= r1;
        r4 ^= r3;
        r1 = r0;
        r0 |= r4;
        r0 ^= r2;
        r2 &= r1;
        r4 ^= r2;
        r1 ^= r0;
        r1 ^= r4;
        r4 = ~r4;
        r3 = rotateLeft(r3, 13);
        r1 = rotateLeft(r1, 3);
        r0 = r0 ^ r3 ^ r1;
        r4 = r4 ^ r1 ^ (r3 << 3);
        r0 = rotateLeft(r0, 1);
        r4 = rotateLeft(r4, 7);
        r3 = r3 ^ r0 ^ r4;
        r1 = r1 ^ r4 ^ (r0 << 7);
        r3 = rotateLeft(r3, 5);
        r1 = rotateLeft(r1, 22);
        r3 ^= serpent24SubKeys[76];
        r0 ^= serpent24SubKeys[76 + 1];
        r1 ^= serpent24SubKeys[76 + 2];
        r4 ^= serpent24SubKeys[76 + 3];
        r2 = r3;
        r3 |= r4;
        r4 ^= r0;
        r0 &= r2;
        r2 ^= r1;
        r1 ^= r4;
        r4 &= r3;
        r2 |= r0;
        r4 ^= r2;
        r3 ^= r0;
        r2 &= r3;
        r0 ^= r4;
        r2 ^= r1;
        r0 |= r3;
        r0 ^= r1;
        r3 ^= r4;
        r1 = r0;
        r0 |= r4;
        r0 ^= r3;
        r0 = rotateLeft(r0, 13);
        r4 = rotateLeft(r4, 3);
        r1 = r1 ^ r0 ^ r4;
        r2 = r2 ^ r4 ^ (r0 << 3);
        r1 = rotateLeft(r1, 1);
        r2 = rotateLeft(r2, 7);
        r0 = r0 ^ r1 ^ r2;
        r4 = r4 ^ r2 ^ (r1 << 7);
        r0 = rotateLeft(r0, 5);
        r4 = rotateLeft(r4, 22);
        r0 ^= serpent24SubKeys[80];
        r1 ^= serpent24SubKeys[80 + 1];
        r4 ^= serpent24SubKeys[80 + 2];
        r2 ^= serpent24SubKeys[80 + 3];
        r1 ^= r2;
        r2 = ~r2;
        r4 ^= r2;
        r2 ^= r0;
        r3 = r1;
        r1 &= r2;
        r1 ^= r4;
        r3 ^= r2;
        r0 ^= r3;
        r4 &= r3;
        r4 ^= r0;
        r0 &= r1;
        r2 ^= r0;
        r3 |= r1;
        r3 ^= r0;
        r0 |= r2;
        r0 ^= r4;
        r4 &= r2;
        r0 = ~r0;
        r3 ^= r4;
        r1 = rotateLeft(r1, 13);
        r0 = rotateLeft(r0, 3);
        r3 = r3 ^ r1 ^ r0;
        r2 = r2 ^ r0 ^ (r1 << 3);
        r3 = rotateLeft(r3, 1);
        r2 = rotateLeft(r2, 7);
        r1 = r1 ^ r3 ^ r2;
        r0 = r0 ^ r2 ^ (r3 << 7);
        r1 = rotateLeft(r1, 5);
        r0 = rotateLeft(r0, 22);
        r1 ^= serpent24SubKeys[84];
        r3 ^= serpent24SubKeys[84 + 1];
        r0 ^= serpent24SubKeys[84 + 2];
        r2 ^= serpent24SubKeys[84 + 3];
        r1 ^= r3;
        r3 ^= r2;
        r2 = ~r2;
        r4 = r3;
        r3 &= r1;
        r0 ^= r2;
        r3 ^= r0;
        r0 |= r4;
        r4 ^= r2;
        r2 &= r3;
        r2 ^= r1;
        r4 ^= r3;
        r4 ^= r0;
        r0 ^= r1;
        r1 &= r2;
        r0 = ~r0;
        r1 ^= r4;
        r4 |= r2;
        r0 ^= r4;
        r3 = rotateLeft(r3, 13);
        r1 = rotateLeft(r1, 3);
        r2 = r2 ^ r3 ^ r1;
        r0 = r0 ^ r1 ^ (r3 << 3);
        r2 = rotateLeft(r2, 1);
        r0 = rotateLeft(r0, 7);
        r3 = r3 ^ r2 ^ r0;
        r1 = r1 ^ r0 ^ (r2 << 7);
        r3 = rotateLeft(r3, 5);
        r1 = rotateLeft(r1, 22);
        r3 ^= serpent24SubKeys[88];
        r2 ^= serpent24SubKeys[88 + 1];
        r1 ^= serpent24SubKeys[88 + 2];
        r0 ^= serpent24SubKeys[88 + 3];
        r1 = ~r1;
        r4 = r0;
        r0 &= r3;
        r3 ^= r4;
        r0 ^= r1;
        r1 |= r4;
        r2 ^= r0;
        r1 ^= r3;
        r3 |= r2;
        r1 ^= r2;
        r4 ^= r3;
        r3 |= r0;
        r3 ^= r1;
        r4 ^= r0;
        r4 ^= r3;
        r0 = ~r0;
        r1 &= r4;
        r1 ^= r0;
        r3 = rotateLeft(r3, 13);
        r4 = rotateLeft(r4, 3);
        r2 = r2 ^ r3 ^ r4;
        r1 = r1 ^ r4 ^ (r3 << 3);
        r2 = rotateLeft(r2, 1);
        r1 = rotateLeft(r1, 7);
        r3 = r3 ^ r2 ^ r1;
        r4 = r4 ^ r1 ^ (r2 << 7);
        r3 = rotateLeft(r3, 5);
        r4 = rotateLeft(r4, 22);
        r3 ^= serpent24SubKeys[92];
        r2 ^= serpent24SubKeys[92 + 1];
        r4 ^= serpent24SubKeys[92 + 2];
        r1 ^= serpent24SubKeys[92 + 3];
        r0 = r2;
        r2 |= r4;
        r2 ^= r1;
        r0 ^= r4;
        r4 ^= r2;
        r1 |= r0;
        r1 &= r3;
        r0 ^= r4;
        r1 ^= r2;
        r2 |= r0;
        r2 ^= r3;
        r3 |= r0;
        r3 ^= r4;
        r2 ^= r0;
        r4 ^= r2;
        r2 &= r3;
        r2 ^= r0;
        r4 = ~r4;
        r4 |= r3;
        r0 ^= r4;
        r0 = rotateLeft(r0, 13);
        r2 = rotateLeft(r2, 3);
        r1 = r1 ^ r0 ^ r2;
        r3 = r3 ^ r2 ^ (r0 << 3);
        r1 = rotateLeft(r1, 1);
        r3 = rotateLeft(r3, 7);
        r0 = r0 ^ r1 ^ r3;
        r2 = r2 ^ r3 ^ (r1 << 7);
        r0 = rotateLeft(r0, 5);
        r2 = rotateLeft(r2, 22);
        r0 ^= serpent24SubKeys[96];
        r1 ^= serpent24SubKeys[96 + 1];
        r2 ^= serpent24SubKeys[96 + 2];
        r3 ^= serpent24SubKeys[96 + 3];
        lfsr[3] = r0;
        lfsr[2] = r1;
        lfsr[1] = r2;
        lfsr[0] = r3;
    }




unsigned mulAlpha[256];
unsigned divAlpha[256];

// Utworzenie tablic potrzebnych do operacji mno¿enia i dzielenia przez alphe
// mulAlpha[x] jest równe x * alpha^4
// divAlpha[x] jest równe x / alpha

void initiationAlpha(){
	unsigned expb[256];
	for(int i = 0, x = 0x01; i < 0xFF; i++){
		expb[i] = x;
		x <<= 1;
		if(x > 0xFF)
			x ^= 0x1A9;
	}
	expb[0xFF] = 0x00;
	unsigned logb[256];
	for(int i = 0; i < 0x100; i++)
        logb[expb[i]] = i;
        
        
    mulAlpha[0x00] = 0x00000000;
    divAlpha[0x00] = 0x00000000;
    for (int x = 1; x < 0x100; x++) {
        unsigned ex = logb[x];
        mulAlpha[x] = (expb[(ex + 23) % 255] << 24)
                | (expb[(ex + 245) % 255] << 16)
                | (expb[(ex + 48) % 255] << 8)
                | expb[(ex + 239) % 255];
        divAlpha[x] = (expb[(ex + 16) % 255] << 24)
                | (expb[(ex + 39) % 255] << 16)
                | (expb[(ex + 6) % 255] << 8)
                | expb[(ex + 64) % 255];
    }
	
}


// Aktualizacja FSM
void updateFSM(){
    unsigned oldR1 = fsmR1;
    fsmR1 = fsmR2 + (lfsr[1] ^ ((fsmR1 & 0x01) != 0 ? lfsr[8] : 0));
    fsmR2 = rotateLeft(oldR1 * 0x54655307, 7);
}


// Aktualizacja stanu LFSR, zwraca wartoœæ wysuwaj¹c¹ siê z rejestru     
unsigned updateLFSR(){
    unsigned v1 = lfsr[9];
    unsigned v2 = (lfsr[3] >> 8) ^ divAlpha[lfsr[3] & 0xFF];
    unsigned v3 = (lfsr[0] << 8) ^ mulAlpha[lfsr[0] >> 24];
    unsigned dropped = lfsr[0];
    for (int i = 0; i < 9; i ++)
        lfsr[i] = lfsr[i + 1];
    lfsr[9] = v1 ^ v2 ^ v3;
    return dropped;
}


// Funkcja obliczaj¹ca wartoœæ f_t
unsigned computeIntermediate(){
    return (lfsr[9] + fsmR1) ^ fsmR2;
}


// Produkuje strumieñ 16 bajtów i wpisuje do podanego bufora

void makeStreamBlock(byte buf[], unsigned off){
    updateFSM();
    unsigned f0 = computeIntermediate();
    unsigned s0 = updateLFSR();
    updateFSM();
    unsigned f1 = computeIntermediate();
    unsigned s1 = updateLFSR();
    updateFSM();
    unsigned f2 = computeIntermediate();
    unsigned s2 = updateLFSR();
    updateFSM();
    unsigned f3 = computeIntermediate();
    unsigned s3 = updateLFSR();

    unsigned f4 = f0;
    f0 &= f2;
    f0 ^= f3;
    f2 ^= f1;
    f2 ^= f0;
    f3 |= f4;
    f3 ^= f1;
    f4 ^= f2;
    f1 = f3;
    f3 |= f4;
    f3 ^= f0;
    f0 &= f1;
    f4 ^= f0;
    f1 ^= f3;
    f1 ^= f4;
    f4 = ~f4;

    encode32le(f2 ^ s0, buf, off);
    encode32le(f3 ^ s1, buf, off + 4);
    encode32le(f1 ^ s2, buf, off + 8);
    encode32le(f4 ^ s3, buf, off + 12);
}


#define BUFFERLEN 16
byte streamBuf[BUFFERLEN];
unsigned streamPtr = BUFFERLEN;

// Produkuje wymagan¹ liczbê bitów strumienia wyjœciowego

void makeStream(byte buf[], unsigned off, unsigned len){
    if (streamPtr < BUFFERLEN) {
        unsigned blen = BUFFERLEN - streamPtr;
        if (blen > len)
            blen = len;

		
        for(int i = 0; i < blen; i++)
        	buf[off + i] = streamBuf[streamPtr + i];
        
        streamPtr += blen;
        off += blen;
        len -= blen;
    }
    while (len > 0) {
        if (len >= BUFFERLEN) {
            makeStreamBlock(buf, off);
            off += BUFFERLEN;
            len -= BUFFERLEN;
        } else {
            makeStreamBlock(streamBuf, 0);
                        
        	for(int i = 0; i < len; i++)
        		buf[off + i] = streamBuf[i];
            streamPtr = len;
            len = 0;
        }
    }
}


char hexnum[] = {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };


int main(){ 

// UTWORZENIE KLUCZA
	byte key[] = {
            (byte)0xA7, (byte)0xC0, (byte)0x83, (byte)0xFE,
            (byte)0xB7
    };
	keySize = sizeof(key)/sizeof(key[0]);
	
// UTWORZENIE WEKTORA POCZ¥TKOWEGO
    byte iv[] = {
            (byte)0x00, (byte)0x11, (byte)0x22, (byte)0x33,
            (byte)0x44, (byte)0x55, (byte)0x66, (byte)0x77,
            (byte)0x88, (byte)0x99, (byte)0xAA, (byte)0xBB,
            (byte)0xCC, (byte)0xDD, (byte)0xEE, (byte)0xFF
    };
    ivSize = sizeof(iv)/sizeof(iv[0]);

// USTAWIENIE KLUCZA
    setKey(key);
    
// USTAWIENIE WEKTORA POCZ¥TKOWEGO    
    setIV(iv);
    
    
// UTWORZENIE DANYCH, KTÓRE CHCEMY ZASZYFROWAÆ

// Dane: 54 6f 20 6a 65 73 74 20 74 65 6b 73 74 20 6e 61 20 42 44 41 4e 2e
//    byte data[] = {
//            (byte)0x54, (byte)0x6f, (byte)0x20, (byte)0x6a,
//            (byte)0x65, (byte)0x73, (byte)0x74, (byte)0x20,
//            (byte)0x74, (byte)0x65, (byte)0x6b, (byte)0x73,
//            (byte)0x74, (byte)0x20, (byte)0x6e, (byte)0x61,
//            (byte)0x20, (byte)0x42, (byte)0x44, (byte)0x41,
//            (byte)0x4e, (byte)0x2e, (byte)0x00, (byte)0x00,
//            (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00,
//            (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00
//    };


// Dane: 54 65 6b 73 74 20 64 6f 20 64 72 75 67 69 65 67 6f 20 74 65 73 74 75 20 6e 61 20 42 44 41 4e 2e
    byte data[] = {
            (byte)0x54, (byte)0x65, (byte)0x6b, (byte)0x73,
            (byte)0x74, (byte)0x20, (byte)0x64, (byte)0x6f,
            (byte)0x20, (byte)0x64, (byte)0x72, (byte)0x75,
            (byte)0x67, (byte)0x69, (byte)0x65, (byte)0x67,
            (byte)0x6f, (byte)0x20, (byte)0x74, (byte)0x65,
            (byte)0x73, (byte)0x74, (byte)0x75, (byte)0x20,
            (byte)0x6e, (byte)0x61, (byte)0x20, (byte)0x42,
            (byte)0x44, (byte)0x41, (byte)0x4e, (byte)0x2e
    };
        
        
// WYGENEROWANIE STRUMIENIA WYJŒCIA
    byte tmp[160];  
    makeStream(tmp, 0, sizeof(tmp)/sizeof(tmp[0]));


    
// WYKONANIE OPERACJI XOR NA STRUMIENIU I DANYCH DO ZASZYFROWANIA
    for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++){
        data[i] = (byte)(data[i] ^ tmp[i]);
    }
    
    
// WYPISANIE SZYFROGRAMU HEXADECYMALNIE
    for (int i = 0; i < 2; i ++) {
        for (int j = 0; j < 16; j ++) {
 			//	if(i==1 &&j>5)break;//dla krótszych danych
            unsigned v = data[i * 16 + j] & 0xFF;
            unsigned v4 = v >> 4;
            unsigned vf = v & 0x0F;

            cout<<" " << hexnum[v4]
                    << hexnum[vf];
        }
        cout<<endl;
    }
	
	return 0; 
} 

