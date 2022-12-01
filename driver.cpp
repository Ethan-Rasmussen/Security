#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <math.h>
#include <stdio.h>
#include "certificates.cpp"

using namespace std;


int main() {
  cout << "Hello User, Welcome to Project 4.\n";
      string choice;

  do {
    cout << "\nType 1 to Create and Sign a Cert.\n" <<
            "Type 2 to Parse the Tree.\n" <<
            "Type 3 to communicate between two nodes.\n" <<
            "Type anything else to exit.\n\n" <<
            "Input: ";

    cin >> choice;
    cout << endl;

    if (choice == "1") { // Create and Sign Certificate*************************
      CreateValidCert();
      SignCert();


    } else if (choice == "2") { // Parse the Tree*******************************
      string decision;
      cout << "User (All Lowercase): ";
      cin >> decision;
      parseChain(decision);


    } else if (choice == "3") { // Communicate between the Nodes****************
      string user1;
      string user2;
      cout << "Which nodes would like to communicate?\nUser 1: ";
      cin >> user1;
      cout << "User 2: ";
      cin >> user2;
      cout << endl;

      bool Vuser1 = parseChain(user1);
      bool Vuser2 = parseChain(user2);
      cout << endl;

      if (Vuser1 == true && Vuser2 == true) // Continue if Users are Valid******
      {
        cout << "Able to communicate;\n";
      }
      else { // Terminate if Invalid********************************************
        cout << "At least one node is invalid...";
      }

    } else { cout << "Thank You....Goodbye...\n\n"; } //Exit********************


  } while (choice == "1" || choice == "2" || choice == "3");
  return 0;
}
