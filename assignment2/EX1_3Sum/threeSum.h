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

void swapEle(int* in_a, int* in_b){
	int temp = *in_a;
	*in_a = *in_b;
	*in_b = temp;
}

void quickSort(int* S, int low, int high){
	if (low >= high)
	{
		return;
	}
	int pivot = S[low];
	int low_p = low;
	int high_p = high + 1;
	while (1)
	{
		while (S[++low_p] < pivot)
		{
			if (low_p == high) break;
		}
		while (pivot < S[--high_p]);
		if (low_p >= high_p)
			break;
		swapEle(S+low_p, S+high_p);
	}
	swapEle(S+low, S+high_p);
	quickSort(S, low, high_p-1);
	quickSort(S, high_p+1, high);
}

int threeSum(int* S, int len, int k)
{
    ////////////////////////////////////////////////////////
    // TODO: Finish your algorithm here                   //
    // Hint: pre-sorting the number array may be helpful  //
    ////////////////////////////////////////////////////////

	// sort the array.
	quickSort(S, 0, len - 1);

	int closest = 0;
	for (int i = 0; i < len; i ++)
	{
		int temp_k = k - S[i];
		int s_head = 0, s_tail = len - 1;
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
			}
			else
			{
				s_tail--;
			}
		}
		int temp_sum = S[s_head] + S[s_tail] + S[i];
		if (abs(temp_sum - k) < abs(closest-k))
		{
			closest = temp_sum;
		}
	}


    return closest;
}

#endif
