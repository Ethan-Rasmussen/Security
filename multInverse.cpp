#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "DiffieHellman.cpp"

using namespace std;

int calcMultInv(int, int, int);

int RSA(int message, int option)  //eord = 0 to return decrypt, or any # != 0 for encrypt
{
    // int p, q, d;
    int p = 53;
    int q = 79;
    int d = 0;
    int n = 0;
    int totn = 0;
    int e = 5;

    // cout << "Enter a prime number for p: ";   //get p, q, and e values
    // cin >> p;
    // cout << "Enter a prime number for q: ";
    // cin >> q;
    // cout << "Enter a value for e (less than Totient(n) & relatively prime to p and q): ";
    // cin >> e;

    n = p*q;                    //calculate n and totn
    totn = (p-1)*(q-1);

    d = calcMultInv(n, totn, e);
    if (d == 0){}
        //cout << "Error: Invalid e value!" << endl;
    else{}
        //cout << "Multiplicative Inverse: " << d << endl;

if(option == 1)
{
    long long encrypt = fast_mod_expo(message, e, n);
    return encrypt;
}
else
{
    long long decrypt = fast_mod_expo(message, d, n);
    return decrypt;
}
}

int calcMultInv(int n, int totn, int e)
{
    if (e < n && e < totn){
        for (int d = 1; d < totn; d++)
                if (((e % totn) * (d % totn)) % totn == 1)
                    return d;
    }
    return 0;
}
