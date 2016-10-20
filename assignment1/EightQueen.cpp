
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int count = 0;

char* BitToPos(unsigned char bit, char* Sol){
	// get the Position of current bit
	short int Pos = 49;
	while (bit != 128){
		bit = bit << 1;
		Pos++;
	}
	Sol[0] = Pos;
	return Sol;
}
int countBits(int n){
	// count the 1-bits in a number
	n = (n & 0x55555555) + ((n >> 1) & 0x55555555);
	n = (n & 0x33333333) + ((n >> 2) & 0x33333333);
	n = (n & 0x0f0f0f0f) + ((n >> 4) & 0x0f0f0f0f);
	n = (n & 0x00ff00ff) + ((n >> 8) & 0x00ff00ff);
	n = (n & 0x0000ffff) + ((n >> 16) & 0x0000ffff);
	return n;
}
void try_Queen(unsigned char A, unsigned char B, unsigned char C, char* Sol = NULL){
	if (B == 255)
	{
		count++;
		printf("Solution %d: %s\n", count, Sol);
		Sol = "";
		return;
	}
	unsigned char D = ~(A | B | C); // without attacking in current row
	if (Sol == NULL)
	{
		char tempSol[9] = ""; // Solution-string
		Sol = tempSol;
	}
	while (D)
	{
		unsigned char bit = D&(~D + 1); // the rightmost 1-bit
		BitToPos(bit, Sol+countBits(B)); // add current pos to Solution-string
		D -= bit; // try placing the Queen
		try_Queen((A | bit) << 1, B | bit, (C | bit) >> 1, Sol);
	}
}

int main(){
	unsigned char left_flag = 0, column_flag = 0, right_flag = 0;
	try_Queen(left_flag, column_flag, right_flag);
	printf("The count is %d \n", count);
	return 0;
}