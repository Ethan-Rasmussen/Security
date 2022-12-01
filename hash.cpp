#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <math.h>
#include <stdio.h>
#include "hash_sdes_enc.cpp"

using namespace std;

string hashit(string str) {
  //string str;
  unsigned char IVarr = 'q';
  //cout << "Enter what you would like to hash: ";
  //cin >> str;
  //cout << "\n\n";

  int length = str.size();


  char str_arr [length];
  strcpy(str_arr, str.c_str());
  /*for (int i=0; i<length; i++)
    cout << str_arr[i];*/
  str_arr[0] = str_arr[0] ^ IVarr;
  str_arr[0] = encrypt(str_arr[0]);
  for (int i=1; i<length; i++)
  {
    str_arr[i] = str_arr[i] ^ str_arr[i-1];
    str_arr[i] = encrypt(str_arr[i]);
  }

  char final = str_arr[length-1];
  //cout << endl << endl;
  //cout << final;
  string ret;
  ret.append(1,final);
  //cout << endl << ret;
  return ret;
}
