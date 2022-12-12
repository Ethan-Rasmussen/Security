
#include<stdio.h>      // used printf/scanf for demo (puts/getchar would be leaner)
#include<string.h>
#include<fstream>
#include<sstream>
#include<sys/socket.h>
#include<arpa/inet.h>  // for inet_addr and sockaddr_in structs
#include<iostream>
#include<bitset>
//#include "sdes_enc.cpp"
//#include "rsa.cpp"

using namespace std;

int int_conversion(char charArr[]);

int exchangeSecret()
{
	int socket_desc;    // file descripter returned by socket command
	int read_size;
	struct sockaddr_in server;    // in arpa/inet.h
	char  server_reply[100], client_message[100];   // will need to be bigger
	int generator = 13;

	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);

	//printf("Trying to create socket\n");
	cout << "Trying to create socket\n";
	if (socket_desc == -1)
	{
		//printf("Unable to create socket\n");
		cout << "Unable to create socket\n";
	}

// *********** This is the line you need to edit ****************
	server.sin_addr.s_addr = inet_addr("169.254.209.235");  // doesn't like localhost?
	server.sin_family = AF_INET;
	server.sin_port = htons( 8421 );    // random "high"  port number

	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//printf(" connect error");
		cout << " connect error";
		return 1;
	}

	string genstr, modstr;
	ifstream inFile("GlobalKeys.txt");
	if (inFile.is_open())
	{
		string line;
		while(getline(inFile,line) )
		{
			stringstream ss(line);


			getline(ss,genstr,','); //cout << genstr << endl;
			getline(ss,modstr,','); //cout << modstr << endl;
		}
	}

	int gen = stoi(genstr);
	int rsa_gen = RSA(gen, 1);
	int mod = stoi(modstr);
	//int rsa_mod = RSA(mod, 1);
	int privateKey = 3;


	send(socket_desc, &rsa_gen, 4, 0); // sends the generator to the server.
	recv(socket_desc, server_reply , 100 , 0); // have to call recv() after every send()!!!
	send(socket_desc, &mod, 4, 0); // sends the modulus to the server

	recv(socket_desc, server_reply , 100 , 0);
	// gets the mod expo reply from the server.
	int replyExp = int_conversion(server_reply);
	replyExp = RSA(replyExp, 0);


	cout <<"The modExpo from the server is: "<< replyExp << "\n";
	// calculating shared key form the reply
	int sharedKey = fast_mod_expo(replyExp, privateKey, mod);
	int rsa_sharedKey = RSA(sharedKey, 1);
	cout << "The shared key is: " << rsa_sharedKey << "\n";

	//calculating modexpo to send to the server
	int modExpo = fast_mod_expo(gen, privateKey, mod);
	//int rsa_modExpo = RSA(modExpo, 1);


	send(socket_desc, &modExpo, 1,0);
	recv(socket_desc, server_reply, 100, 0);

	//Get data from keyboard and send  to server
	//printf("What do you want to send to the server. (b for bye)\n");
	cout << "What do you want to send to the server. (b for bye)\n";
	while(strncmp(client_message,"b",1))      // quit on "b" for "bye"
	{
		memset(client_message,'\0',100);
		cin >> client_message;

		char encrypted_message[strlen(client_message)];

		/*for(int i=0;i<strlen(client_message);i++){

			encrypted_message[i] = encrypt(client_message[i], sharedKey);
			//cout << encrypted_message[i];//Testing Print**********************************
		}*/

		if( send(socket_desc , &encrypted_message, strlen(client_message) , 0) < 0)
		{
			//printf("Send failed");
			cout << "Send failed";
			return 1;
		}

		//printf("\nSending Message: %.*s\n", (int) strlen(client_message),client_message);
		cout << "\nEncrypting message: " << client_message << "\n";
		cout << "\nSending ciphertext: " << encrypted_message << "\n";


		//Receive a reply from the server
		if( (read_size = recv(socket_desc, server_reply , 100 , 0)) < 0)
		{
			//printf("recv failed");
			cout << "recv failed";
		}

		//for loop to print the reply
		cout << "Server Replies: ";
		for(int i=0; i<read_size;i++){
			cout<<server_reply[i];
		}
		cout << endl << endl;
	}


	return 0;
}

//converts the char array sent over the socket to integers.
int int_conversion(char charArr[]){


		// converting to a binary string
	string bin = "";
	for(int i=0; i<4; i++){
		int tmp = charArr[i];
		string byte = bitset<8>(tmp).to_string();
		bin = byte+bin;
	}
	//cout << bin;
	//conversion back to an integer
	int number = std::bitset<32>(bin).to_ulong();

	return number;
}
