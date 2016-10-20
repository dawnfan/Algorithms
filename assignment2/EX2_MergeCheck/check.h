// Given three character sequence A, B and C, check if A and B could be merged into C
// Return 1 if they could be merged, else return 0
// Inputs:
//  A: a character sequence with length m
//  B: a character sequence with length n
//  C: a character sequence with length m+n
// NOTE:
//  1. The character order in C should also be same with A and B
//     For example, A = "ab", B = "cd" and C = "badc", the algorithm should return 0 since the character order is different
//  2. Here we use 1 to represent boolean value true, and 0 to represent false. Do not change the return type !!!

#ifndef CHECK_H_
#define CHECK_H_

int check(char *A, char *B, char *C)
{
    /////////////////////////////////////////////////
    // TODO: Finish your checking algorithm        //
    // Hint: using '\0' to check end of the string //
    /////////////////////////////////////////////////

	int isPass = 1;
	int p_A = 0, p_B = 0, p_C = 0;

	while (isPass == 1 && C[p_C] != '\0'){
		if (A[p_A] != '\0' && C[p_C] == A[p_A])
		{
			p_A++;
		}
		else if (B[p_B] != '\0' && C[p_C] == B[p_B])
		{
			p_B++;
		}
		else
		{
			isPass = 0;
		}
		p_C++;
	}

    return isPass;
}

#endif
