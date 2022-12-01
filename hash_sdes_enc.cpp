#include <stdio.h>
#include <string.h>
#include <math.h>
using namespace std;

int s_matrix0[4][4] = { {1,0,3,2},
						{3,2,1,0},
						{0,2,1,3},
						{3,1,3,2} };

int s_matrix1[4][4] = { {0,1,2,3},
						{2,0,1,3},
						{3,0,1,0},
						{2,1,0,3} };

int s0=0, s1=0;
int row=0, col=0;
int s0_binary[2], s1_binary[2];
int result[2];

int ipow(int base, int exp)
{
	int result = 1;
	for(;;)
	{
		if(exp & 1)
				result *= base;
			exp >>= 1;
			if(!exp)
				break;
			base *= base;
	}
	return result;
}

int to_digit(int a, int b)
{
    int output;
    if(a==1 && b==1)
        output = 3;

    if(a==0 && b==1)
        output = 1;

    if(a==1 && b==0)
        output = 2;

    if(a==0 && b==0)
        output = 0;

    return output;
}

void to_binary(int num)
{
    int x;

    if(num == 3)
    {
        for(x=0; x<2; x++)
            result[x] = 1;
    }
    else if(num == 1)
    {
        result[0] = 0;
        result[1] = 1;
    }
    else if(num == 2)
    {
        result[0] = 1;
        result[1] = 0;
    }
    else if(num == 0)
    {
        for(x=0; x<2; x++)
            result[x] = 0;
    }
}

unsigned char encrypt(unsigned char plaintext)
{

    int plain[8] = {0,1,1,0,1,1,0,1};

	int key[10] = {1,0,1,0,0,0,0,0,0,0};
/*  string skey = bitset<10>(ikey).to_string();

    //conversion of chars to int
    for(int i=0;i<10;i++){
        key[i] = skey[i] - 48;
				//cout << key[i] << endl;//Testing Print********************************
    }
*/

    //cout<< "\n";




    for(int i=0;i<8;i++){

		if(plaintext >= ipow(2,7-i)){
			plain[i] = 1;
			plaintext = plaintext - ipow(2,7-i);
		}
		else{
			plain[i] = 0;
		}
	}


    int i,j,index;

    int k1[8], k2[8];
    int afterp10[11], ls1[10], ls2[10], afterip[8], afterep[8], afterp4[4];
    int aftersboxesone[4], aftersboxestwo[4];
    int leftafterip[4], rightafterip[4];
    int leftafterep[4], rightafterep[4];
    int leftafterone[4], rightafterone[4];
    int afteripinverse[8];
    int afterone[8], aftertwo[8];

    int p10[10] = {3,5,2,7,4,10,1,9,8,6};
    int p8[8] = {6,3,7,4,8,5,10,9};

    int ip[8] = {2,6,3,1,4,8,5,7};
    int ep[8] = {4,1,2,3,2,3,4,1};
    int p4[4] = {2,4,3,1};
    int ipinverse[8] = {4,1,3,5,7,2,8,6};

    for(i=0; i<=9; i++)
    {
        index = p10[i];
        afterp10[i] = key[index - 1];
    }
    afterp10[i] = '\0';

    for(i=0; i<5; i++)
    {
        if(i == 4)
            ls1[i]=afterp10[0];
        else
            ls1[i]=afterp10[i+1];
    }

    for(i=5; i<10; i++)
    {
        if(i == 9)
            ls1[i]=afterp10[5];
        else
            ls1[i]=afterp10[i+1];
    }


    index=0;
    for(i=0; i<=9; i++)
    {
        index = p8[i];
        k1[i] = ls1[index - 1];
    }


    for(i=0; i<3; i++)
    {
        ls2[i]=ls1[i+2];
    }
    ls2[3]=ls1[0];
    ls2[4]=ls1[1];

    for(i=5; i<8; i++)
    {
        ls2[i]=ls1[i+2];
    }
    ls2[8]=ls1[5];
    ls2[9]=ls1[6];


    index=0;
    for(i=0; i<=9; i++)
    {
        index = p8[i];
        k2[i] = ls2[index - 1];
    }


    for(i=0; i<8; i++)
    {
        index = ip[i];
        afterip[i] = plain[index - 1];
    }
    afterip[i] = '\0';


    for(j=0; j<4; j++)
        leftafterip[j] = afterip[j];

    for(j=0; j<4; j++)
        rightafterip[j] = afterip[j+4];

    for(i=0; i<4; i++)
    {
        index = ep[i];
        afterep[i] = rightafterip[index - 1];
    }

    for(i=4; i<8; i++)
    {
        index = ep[i];
        afterep[i] = rightafterip[index - 1];
    }
    afterep[i] = '\0';


    for(i=0; i<8; i++)
        k1[i] = k1[i] ^ afterep[i];


    row = to_digit(k1[0],k1[3]);
    col = to_digit(k1[1],k1[2]);
    s0 = s_matrix0[row][col];

    to_binary(s0);
    for(j=0; j<2; j++)
        s0_binary[j] = result[j];

    row = to_digit(k1[4],k1[7]);
    col = to_digit(k1[5],k1[6]);

    s1 = s_matrix1[row][col];

    to_binary(s1);
    for(j=0; j<2; j++)
        s1_binary[j] = result[j];

    for(j=0; j<2; j++)
        aftersboxesone[j] = s0_binary[j];

    for(i=0,j=2; i<2,j<4; i++,j++)
        aftersboxesone[j] = s1_binary[i];


    for(i=0; i<4; i++)
    {
        index = p4[i];
        afterp4[i] = aftersboxesone[index - 1];
    }
    afterp4[i] = '\0';


    for(i=0; i<4; i++)
        afterp4[i] = afterp4[i] ^ leftafterip[i];


    for(i=0; i<4; i++)
        afterone[i] = rightafterip[i];

    for(i=0,j=4; i<4,j<8; i++,j++)
        afterone[j] = afterp4[i];

    afterone[j] = '\0';


    for(j=0; j<4; j++)
        leftafterone[j] = afterone[j];

    for(j=0; j<4; j++)
        rightafterone[j] = afterone[j+4];


    for(i=0; i<4; i++)
    {
        index = ep[i];
        afterep[i] = rightafterone[index - 1];
    }

    for(i=4; i<8; i++)
    {
        index = ep[i];
        afterep[i] = rightafterone[index - 1];
    }
    afterep[i] = '\0';


    for(i=0; i<8; i++)
        k2[i] = k2[i] ^ afterep[i];


    row = to_digit(k2[0],k2[3]);
    col = to_digit(k2[1],k2[2]);

    s0 = s_matrix0[row][col];
    to_binary(s0);
    for(j=0; j<2; j++)
        s0_binary[j] = result[j];

    row = to_digit(k2[4],k2[7]);
    col = to_digit(k2[5],k2[6]);

    s1 = s_matrix1[row][col];
    to_binary(s1);
    for(j=0; j<2; j++)
        s1_binary[j] = result[j];

    for(j=0; j<2; j++)
        aftersboxestwo[j] = s0_binary[j];

    for(i=0,j=2; i<2,j<4; i++,j++)
        aftersboxestwo[j] = s1_binary[i];


    for(i=0; i<4; i++)
    {
        index = p4[i];
        afterp4[i] = aftersboxestwo[index - 1];
    }
    afterp4[i] = '\0';


    for(i=0; i<4; i++)
        afterp4[i] = afterp4[i] ^ leftafterone[i];


    for(i=0; i<4; i++)
        aftertwo[i] = afterp4[i];

    for(i=0,j=4; i<4,j<8; i++,j++)
        aftertwo[j] = rightafterone[i];

    aftertwo[j] = '\0';

    for(i=0; i<8; i++)
    {
        index = ipinverse[i];
        afteripinverse[i] = aftertwo[index - 1];
    }
    afteripinverse[j] = '\0';

    int int_val = 0;

    // converts binary array to integer
    for(int i=0;i<8;i++){
		if(afteripinverse[i] == 1){
			int_val = int_val + ipow(2,7-i);
		}
	}
    // converts integer to char
    unsigned char char_val = int_val;

    return char_val;


}
