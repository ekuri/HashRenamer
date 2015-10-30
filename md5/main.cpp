#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string>

using namespace std;

// shift amounts for particular round
unsigned int roundForShift[64] = {
            7,12,17,22,7,12,17,22,7,12,17,22,7,
            12,17,22,5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
            4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,6,10,
            15,21,6,10,15,21,6,10,15,21,6,10,15,21
};
const char hexSymbol[]="0123456789ABCDEF";
unsigned int constrants[64];

inline string getMD5Sum(string target);
inline void compute(unsigned int numbers[16], unsigned int &ra, unsigned int &rb, unsigned int &rc, unsigned int &rd);
inline void standardizeInput(string target, unsigned int result[], const unsigned int standardLength);
string getMD5ResultFromABCD(unsigned int a, unsigned int b, unsigned int c, unsigned int d);
inline unsigned int shift(unsigned int x, unsigned int n);

inline unsigned int FOperation(unsigned int x, unsigned int y, unsigned int z) {
    return (x & y) | ((~x) & z);
}
inline unsigned int GOperation(unsigned int x, unsigned int y, unsigned int z) {
    return (x & z) | (y & (~z));
}
inline unsigned int HOperation(unsigned int x, unsigned int y, unsigned int z) {
    return x ^ y ^ z;
}
inline unsigned int IOperation(unsigned int x, unsigned int y, unsigned int z) {
    return y ^ (x | (~z));
}

int main()
{
    for (int count = 0; count < 64; count++) {
        constrants[count] = (unsigned int )(floor(fabs(sin(count + 1)) * pow(2, 32)));
    }
    string target;

    target = "abc";
    cout << "MD5 of " << target << " is: " << getMD5Sum(target) << endl;
    target = "qwe";
    cout << "MD5 of " << target << " is: " << getMD5Sum(target) << endl;
    target = "123asd";
    cout << "MD5 of " << target << " is: " << getMD5Sum(target) << endl;

    cout << endl << "Input something to test: ";
    cin >> target;
    cout << "The MD5 sum is: " << getMD5Sum(target) << endl;
    return 0;
}

string getMD5Sum(string target) {
    //Initialize variables:
    unsigned int a = 0x67452301;
    unsigned int b = 0xEFCDAB89;
    unsigned int c = 0x98BADCFE;
    unsigned int d = 0x10325476;

    unsigned int standardLength = (((target.length() + 8) / 64) + 1) * 16;
    unsigned int standardizeInputData[standardLength];
    standardizeInput(target, standardizeInputData, standardLength);
    for (unsigned int count = 0; count < standardLength / 16; count += 16) {
        unsigned int numbers[16];
        for (unsigned int offset = 0; offset < 16; offset++) {
            numbers[offset] = standardizeInputData[count * 16 + offset];
        }
        compute(numbers, a, b, c, d);
    }
    string result = getMD5ResultFromABCD(a, b, c, d);

    return result;
}

inline void compute(unsigned int numbers[16], unsigned int &ra, unsigned int &rb, unsigned int &rc, unsigned int &rd) {
    unsigned int f, g;
    unsigned int a = ra;
    unsigned int b = rb;
    unsigned int c = rc;
    unsigned int d = rd;
    for (unsigned int i = 0; i < 64; i++)
    {
        if(i < 16) {
            f = FOperation(b, c, d);
            g = i;
        } else if (i < 32) {
            f = GOperation(b, c, d);
            g = (5 * i + 1) % 16;
        } else if (i < 48) {
            f = HOperation(b, c, d);
            g = (3 * i + 5) % 16;
        } else {
            f = IOperation(b, c, d);
            g = (7 * i) % 16;
        }
        unsigned int tmp = d;
        d = c;
        c = b;
        b = b + shift((a + f + constrants[i] + numbers[g]), roundForShift[i]);
        a = tmp;
        //cout << "constrants: " << constrants[i] << " numbers: " << numbers[g] << " roundForShift: " << roundForShift[i] << endl;
        //cout << "count: " << i << " a: " << a << " b: " << b << " c: " << a << " d: " << d << endl;
    }
    ra = a + ra;
    rb= b + rb;
    rc = c + rc;
    rd = d + rd;
}

inline void standardizeInput(string target, unsigned int result[], const unsigned int standardLength) {
    for (unsigned int count = 0; count < standardLength; count++) {
        result[count] = 0;
    }
    for (unsigned int count = 0; count < target.length(); count++) {
        result[count >> 2] |= (target[count]) << ((count % 4) * 8);
    }
    result[target.length() >> 2] |= 0x80 << ((target.length() % 4) * 8);
    result[standardLength - 2] = target.length() * 8;
}

inline string convertToHex(unsigned int a) {
    int b;
    string str1;
    string str="";
    for(int i=0;i<4;i++)
    {
        str1="";
        b=((a>>i*8)%(1<<8))&0xff;   //逆序处理每个字节
        for (int j = 0; j < 2; j++)
        {
            str1.insert(0,1,hexSymbol[b%16]);
            b=b/16;
        }
        str+=str1;
    }
    return str;
}

string getMD5ResultFromABCD(unsigned int a, unsigned int b, unsigned int c, unsigned int d)
{
    return convertToHex(a) + convertToHex(b) + convertToHex(c) + convertToHex(d);
}

inline unsigned int shift(unsigned int x, unsigned int n) {
    return (x << n) | (x >> (32 - n));
}
