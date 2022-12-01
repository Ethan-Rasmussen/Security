#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <math.h>
#include <stdio.h>
#include "hash.cpp"
#include "multInverse.cpp"

using namespace std;


void CreateValidCert();
void SignCert();
void AddValidCert(string choice);
bool parseChain(string decision);
string getIssuer(string filename);
bool checkCertList(string hash);
bool cert_validation(string filename);
bool checkTimeFrame(string filename);
string getSN(string filename);
string getStartDate(string filename);
string getEndDate(string filename);
string hashCert(string filename);
string signer();
string recipient();
int ASCIIfunc(string str);
void appendfile(string filename, string str);
bool checkCRL(string filename);
bool checkChain(string filename);

//****************************************************************************



//****************************************************************************

void CreateValidCert() {

  string version;
  string serialNumber;
  string encryptionType;
  string issuer;
  string startDate;
  string endDate;
  string subject;
  string publicKey;
  string trustLevel;

  cout << "Enter the version: ";
  cin >> version;
  cout << "Enter the serial number: ";
  cin >> serialNumber;
  cout << "Enter the type of Encryption: ";
  cin >> encryptionType;
  cout << "Enter the name of the issuer: ";
  cin >> issuer;
  cout << "Enter the start date: ";
  cin >> startDate;
  cout << "Enter the end date: ";
  cin >> endDate;
  cout << "Enter the name of the subject: ";
  cin >> subject;
  cout << "Enter the Public Key: ";
  cin >> publicKey;
  cout << "Enter the trust level digit: ";
  cin >> trustLevel;

  string filename = subject + ".txt";
  ofstream outfile (filename);
  outfile.close();

  appendfile(subject, version);
  appendfile(subject, serialNumber);
  appendfile(subject, encryptionType);
  appendfile(subject, issuer);
  appendfile(subject, startDate);
  appendfile(subject, endDate);
  appendfile(subject, subject);
  appendfile(subject, publicKey);
  appendfile(subject, trustLevel);

  AddValidCert(subject);

  return;
}

//****************************************************************************

void SignCert() {

  string signer1 = signer();
  string recipient1 = recipient();

  string hashed = hashCert(signer1);
  int hashInt = ASCIIfunc(hashed);
  int Signature = RSA(hashInt, 1);

  string sig = to_string(Signature);

  appendfile(recipient1, sig);

  return;
}

//****************************************************************************

void AddValidCert(string choice) {

  //string choice;
  //cout << "What file would you like to hash?\nInput: ";
  //cin >> choice;
  string hashed2 = hashCert(choice);
  int hashInt2 = ASCIIfunc(hashed2);
  int Signature = RSA(hashInt2, 1);

  string sig2 = to_string(Signature);

  appendfile("hash_list", sig2);

  return;
}

//****************************************************************************

bool parseChain(string decision) {
  string arr[6];
  int count = 1;
  //string decision;
  //cout << "User (All Lowercase): ";
  //cin >> decision;
  string file = decision;
  arr[0] = file;
  cout << endl;

  string issuer;
  string issuerFile;
  while (issuer != "ca")
  {
    issuer = getIssuer(file);
    file = issuer;
    arr[count] = issuer;
    count++;
  }
  count = count-1;

  for(count; count > -1; count--) {
    //cout << arr[count];

    bool validity = checkChain(arr[count]);
    //cout << validity;
    if (validity == 0) {
      cout << "Your certs are valid up to " << arr[count+1] << endl;
      return false;
    }

    validity = cert_validation(arr[count]);
    //cout << validity;
    if (validity == 0) {
      cout << "Your certs are valid up to " << arr[count+1] << endl;
      return false;
    }

  }
  cout << "Your Chain has been verified.\n";

  return true;;
}

//*****************************************************************************

string getIssuer(string filename) {
  filename = filename + ".txt";
  int count = 0;
  string validDate;
  ifstream readfile;
  string y;
  readfile.open(filename);
  if (!readfile) {
    cerr << "Cert Does Not Exist.\n";
    exit(1);
  }
  while (readfile >> y) {
    count++;
    if(count == 4)
      validDate = y;
  }
  readfile.close();

  return validDate;
}

//*****************************************************************************

bool cert_validation(string filename) {
  bool validity = true;

  validity = checkCRL(filename);
  if (validity == false){
    //cout << filename << " is invalid for CRL";
    return validity;
  }
  validity = checkTimeFrame(filename);
  if (validity == false) {
    //cout << filename << " is invalid for TimeFrame";
    return validity;
  }

  string hashed2 = hashCert(filename);
  int hashInt2 = ASCIIfunc(hashed2);
  int Signature = RSA(hashInt2, 1);

  string sig2 = to_string(Signature);
  validity = checkCertList(sig2);
  //cout << validity;

  return validity;
}

//****************************************************************************

bool checkCertList(string hash) {
  bool validity = false;
  string filename = "hash_list.txt";
  ifstream readfile;
  string y;
  readfile.open(filename);
  if (!readfile) {
    cerr << "Unable to open file datafile.txt";
    exit(1);
  }
  while (readfile >> y) {
    if(y == hash)
      validity = true;
  }
  readfile.close();

  //2cout << "CERT LIST VALIDATION: " << validity << endl;
  return validity;
}

//*****************************************************************************

bool checkTimeFrame(string filename) {
  bool validity = true;
  int date = 4;

  string start = getStartDate(filename);
  string end = getEndDate(filename);

  int startDate = ASCIIfunc(start);
  startDate = startDate - 48;
  //cout << startDate << endl;

  int endDate = ASCIIfunc(end);
  endDate = endDate - 48;
  //cout << endDate << endl;;

  if (date >= startDate && date <= endDate)
    validity = true;
  else
      validity = false;

  return validity;
}

//*****************************************************************************

string getSN(string filename) {
  filename = filename + ".txt";
  int count = 0;
  string validDate;
  ifstream readfile;
  string y;
  readfile.open(filename);
  if (!readfile) {
    cerr << "Unable to open file datafile.txt";
    exit(1);
  }
  while (readfile >> y) {
    count++;
    if(count == 2)
      validDate = y;
  }
  readfile.close();

  return validDate;
}

//****************************************************************************

string getStartDate(string filename) {
  filename = filename + ".txt";
  int count = 0;
  string validDate;
  ifstream readfile;
  string y;
  readfile.open(filename);
  if (!readfile) {
    cerr << "Unable to open file datafile.txt";
    exit(1);
  }
  while (readfile >> y) {
    count++;
    if(count == 5)
      validDate = y;
  }
  readfile.close();

  return validDate;
}

//****************************************************************************

string getEndDate(string filename) {
  filename = filename + ".txt";
  int count = 0;
  string validDate;
  ifstream readfile;
  string y;
  readfile.open(filename);
  if (!readfile) {
    cerr << "Unable to open file datafile.txt";
    exit(1);
  }
  while (readfile >> y) {
    count++;
    if(count == 6)
      validDate = y;
  }
  readfile.close();

  return validDate;
}

//****************************************************************************

string hashCert(string filename) {
  string str_to_hash = "";
  string x;
  int count = 0;
  string hashValue;
  ifstream newfile;

  filename = filename + ".txt";
  newfile.open(filename);
  if (!newfile) {
    cerr << "Unable to open file datafile.txt";
    exit(1);
  }
  while (newfile >> x) {
    if (count <= 8)
      str_to_hash = str_to_hash + x;
    count++;
  }
  newfile.close();
  //cout << str_to_hash << endl;
  hashValue = hashit(str_to_hash);

  return hashValue;
}

//***************************************************************************

string signer() {
  string user;
  cout << "who is doing the signing?\nInput: ";
  cin >> user;
  return user;
}

//***************************************************************************

string recipient() {
  string user;
  cout << "who is being signed?\nInput: ";
  cin >> user;
  return user;
}

//**************************************************************************

int ASCIIfunc(string str) {

  int l = str.length();
  int convert;
  for (int i=0; i<l; i++)
    convert = str[i];

  return convert;
}

//************************************************************************

void appendfile(string filename, string str) {
  filename = filename + ".txt";
  ofstream outfile;
  outfile.open(filename, ios_base::app);
  outfile << str << "\n";
}

//*************************************************************************

bool checkCRL(string filename) {
    ifstream newfile;
    ifstream newfile1;
    string x, y;
    string serial = "";
    int cnt = 0;
    bool found = 0;
    bool valid = true;

    filename = filename + ".txt";

    string filename1 = "crl.txt";
      newfile1.open(filename1);
      if (!newfile1) {
        cerr << "Unable to open file datafile.txt";
        exit(1);
      }

    while(newfile1 >> x)
    {
      newfile.open(filename);
      if (!newfile) {
        cerr << "Unable to open file datafile.txt";
        exit(1);
      }

//    cout << x << endl;
      y = "";
      cnt = 0;
      while (newfile >> y) {
        serial = "";
        if(cnt == 1)
          serial = y;
//      cout << serial << endl;
        cnt++;
        if(x == serial)
          found = 1;
      }
        x = "";
      newfile.close();
    }

    if(found == 1) {
      //cout << filename << " was found on the CRL." << endl;
      valid = false;
    }else{
      //cout << filename << " was not found on the CRL." << endl;
    }

    newfile1.close();
    return valid;
  }

//*****************************************************************************

bool checkChain(string filename) {
    ifstream newfile;
    ifstream newfile1;
    string x, y;
    string serial = "";
    int cnt = 0;
    bool found = 0;
    bool valid = true;

    filename = filename + ".txt";

    string filename1 = "chain.txt";
      newfile1.open(filename1);
      if (!newfile1) {
        cerr << "Unable to open file chain.txt";
        exit(1);
      }

    while(newfile1 >> x)
    {
      newfile.open(filename);
      if (!newfile) {
        cerr << "Unable to open file cert.txt";
        exit(1);
      }

//    cout << x << endl;
      y = "";
      cnt = 0;
      while (newfile >> y) {
        serial = "";
        if(cnt == 6)
          serial = y;
//      cout << serial << endl;
        cnt++;
        if(x == serial)
          found = 1;
      }
        x = "";
      newfile.close();
    }

    if(found == 1) {
      //cout << filename << " was found on the CRL." << endl;

    }else{
      //cout << filename << " was not found on the CRL." << endl;
      valid = false;
    }

    newfile1.close();
    return valid;
  }

//*****************************************************************************
