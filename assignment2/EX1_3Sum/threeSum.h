// Given an integer k and an array of integers S, find out 3 numbers in the array, which sum of them is clostest to k.
// return the clostest sum.

// Inputs:
//  S: an integer array
//  len: length of the number array
//  k: the target integer
#include <stdio.h>
#include <malloc.h>

#ifndef THREE_SUM_H_
#define THREE_SUM_H_

void insertSort(int* S, int len){
	int i, j;
	int temp;
	for (i = 1; i < len; i++)
	{
		temp = S[i];
		// numbers before i are sorted, compare from j-1 toward 0.
		for (j = i; j > 0 && S[j-1] > temp; j--)
		{
			// move backward.
			S[j] = S[j - 1];
		}
		S[j] = temp;
	}
}

void quickSort(int* S, int low, int high){
	if (low >= high){
		return;
	}
	// when the length of array is too big, quick sort makes stack overflow.
	// turn into insert sort.
	//if (high - low < 1200)
	//{
	//	insertSort(S + low, high - low + 1);
	//	return;
	//}
	int s_head = low;
	int s_tail = high;
	int pivot = S[low];
	// partition
	while (low < high)
	{
		// high move forward 
		while (low < high && S[high] >= pivot)
		{
			high--;
		}
		S[low] = S[high];
		// low move backward
		while (low < high && S[low] <= pivot)
		{
			low++;
		}
		S[high] = S[low];
		// now high and low exchange and turn next.
	}
	S[low] = pivot;
	// recurse
	quickSort(S, s_head, low-1);
	quickSort(S, low+1, s_tail);
}


int threeSum(int* S, int len, int k)
{
    ////////////////////////////////////////////////////////
    // TODO: Finish your algorithm here             //
    // Hint: pre-sorting the number array may be helpful  //
    ////////////////////////////////////////////////////////

	// sort the array.
	quickSort(S, 0, len - 1);


    return 0;
}

#endif
