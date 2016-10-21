// Given an integer k and an array of integers S, find out 3 numbers in the array, which sum of them is clostest to k.
// return the clostest sum.

// Inputs:
//  S: an integer array
//  len: length of the number array
//  k: the target integer
#include <stdio.h>
#include <stdlib.h>

#ifndef THREE_SUM_H_
#define THREE_SUM_H_

int cmpfunc(const void * a, const void * b)
{
	return (*(int*)a - *(int*)b);
}

int threeSum(int* S, int len, int k)
{
    ////////////////////////////////////////////////////////
    // TODO: Finish your algorithm here                   //
    // Hint: pre-sorting the number array may be helpful  //
    ////////////////////////////////////////////////////////

	// sort the array.
	qsort(S, len, sizeof(int), cmpfunc);
	int closest = 0;

	int mid = len / 2, low = 0, high = len-1;
	int s_head = 0, s_tail = len-1;
	while (low <= high)
	{
		int temp_k = k - S[mid];
		while (s_head < s_tail)
		{
			int sum_two = S[s_head] + S[s_tail];
			if (sum_two == temp_k)
			{
				return k;
			}
			else if (sum_two < temp_k)
			{
				s_head++;
				if (s_head == k)
				{
					s_head++;
				}
			}
			else
			{
				s_tail--;
				if (s_tail == k)
				{
					s_tail--;
				}
			}
		}

	}

    return closest;
}

#endif
