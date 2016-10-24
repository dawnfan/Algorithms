#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "threeSum.h"

#define LOOP 2000
#define LLEN 5000

int main()
{
    // A sample input and output
    int nums[] = {-1, 0, 1, -2, -1, -4};
    int resLen = 0;

    // Check correctnes, in this case the correct answer is
    // correct = 0

    int res = threeSum(nums, 6, 0);

    printf("Your Answer is %d\n", res);
   


    // Check complexity
    // Test with a random generated large integer array
    srand((unsigned int)time(NULL));
    int cnums[LLEN];
    for (int i = 0; i < LLEN; i++)
    {
        // random number from -128 - 127
        cnums[i] = rand() % 255 - 128;
    }

    clock_t begin = clock();
    for (int i = 0; i < LOOP; i++)
    {
        int test = threeSum(cnums, LLEN, 0);
    }
    clock_t end = clock();
    printf("Time consuming for %d times: %f ms\n", LOOP, (double)(end-begin)*1000/CLOCKS_PER_SEC);

    return 0;
}
