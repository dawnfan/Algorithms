#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int getMaxElement(vector<vector<vector<int> > > &subArray, int *MaxInd){
	// find the maximum element on a plane
	int maxInt = subArray[0][0][0];
	MaxInd[0] = 0;
	MaxInd[1] = 0;
	MaxInd[2] = 0;
	int m = subArray.size(), n = subArray[0].size(), k = subArray[0][0].size();
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			for (int h = 0; h < k; h++)
			{
				if (maxInt < subArray[i][j][h])
				{
					maxInt = subArray[i][j][h];
					MaxInd[0] = i;
					MaxInd[1] = j;
					MaxInd[2] = h;
				}
			}	
		}
	}
	return maxInt;
}
bool isNeighborBigger(vector<vector<vector<vector<int> > > > &curArray, int* maxInd, int neighborDim, int step){
	bool isBigger = false;
	int sizeLimit[4] = { curArray.size(), curArray[0].size(), curArray[0][0].size(), curArray[0][0][0].size() };
	int neighbor[4] = { maxInd[0], maxInd[1], maxInd[2], maxInd[3] };
	neighbor[neighborDim] += step;
	neighbor[neighborDim] = neighbor[neighborDim] < 0 ? 0 : neighbor[neighborDim];
	neighbor[neighborDim] = neighbor[neighborDim] < sizeLimit[neighborDim] ? neighbor[neighborDim] : sizeLimit[neighborDim]-1;
	int curInt = curArray[maxInd[0]][maxInd[1]][maxInd[2]][maxInd[3]];
	int neiInt = curArray[neighbor[0]][neighbor[1]][neighbor[2]][neighbor[3]];
	if (curInt < neiInt)
	{
		isBigger = true;
	}
	return isBigger;
}
void FindPeak(vector<vector<vector<vector<int> > > > curArray, int* peakInd){
	int sizeLimit[4] = { curArray.size(), curArray[0].size(), curArray[0][0].size(), curArray[0][0][0].size() };

	int checkPlane = sizeLimit[0] / 2;
	int maxInd[4];
	maxInd[0] = checkPlane;
	int tempMax = getMaxElement(curArray[checkPlane], maxInd + 1);
	int neighborDim = 0;
	int biggerInd[4] = { maxInd[0], maxInd[1], maxInd[2], maxInd[3] };
	// get the bigger neighbor
	if (isNeighborBigger(curArray, maxInd, neighborDim, 1))
	{
		biggerInd[neighborDim] += 1;
	}
	else if (isNeighborBigger(curArray, maxInd, neighborDim, -1))
	{
		biggerInd[neighborDim] -= 1;
	}
	else if (isNeighborBigger(curArray, maxInd, ++neighborDim, 1))
	{
		biggerInd[neighborDim] += 1;
	}
	else if (isNeighborBigger(curArray, maxInd, neighborDim, -1))
	{
		biggerInd[neighborDim] -= 1;
	}
	else if (isNeighborBigger(curArray, maxInd, ++neighborDim, 1))
	{
		biggerInd[neighborDim] += 1;
	}
	else if (isNeighborBigger(curArray, maxInd, neighborDim, -1))
	{
		biggerInd[neighborDim] -= 1;
	}
	else if (isNeighborBigger(curArray, maxInd, ++neighborDim, 1))
	{
		biggerInd[neighborDim] += 1;
	}
	else if (isNeighborBigger(curArray, maxInd, neighborDim, -1))
	{
		biggerInd[neighborDim] -= 1;
	}
	else{
		peakInd[0] = maxInd[0];
		peakInd[1] = maxInd[1];
		peakInd[2] = maxInd[2];
		peakInd[3] = maxInd[3];
		return;
	}
	// pluning
	if (biggerInd[0] > checkPlane)
	{
		curArray.erase(curArray.begin(), curArray.begin() + checkPlane / 2);
	}
	else{
		curArray.erase(curArray.begin() + (checkPlane / 2 - 1), curArray.begin() + checkPlane);
	}
	int limitLow = 0, limitHigh = 0;
	if (biggerInd[1] > sizeLimit[1] / 2)
	{
		limitHigh = sizeLimit[1] / 2;
	}
	else{
		limitLow = sizeLimit[1] / 2 - 1;
	}
	for (int i = 0; i < curArray.size(); i++)
	{
		curArray[i].erase(curArray[i].begin() + limitLow, curArray[i].erase(curArray[i].begin() + limitHigh));
	}
	if (biggerInd[2] > sizeLimit[2] / 2)
	{
		limitHigh = sizeLimit[2] / 2;
	}
	else{
		limitLow = sizeLimit[2] / 2 - 1;
	}
	for (int i = 0; i < curArray.size(); i++)
	{
		for (int j = 0; j < curArray[0].size(); j++)
		{
			curArray[i][j].erase(curArray[i][j].begin() + limitLow, curArray[i][j].erase(curArray[i][j].begin() + limitHigh));
		}
	}
	if (biggerInd[3] > sizeLimit[3] / 2)
	{
		limitHigh = sizeLimit[3] / 2;
	}
	else{
		limitLow = sizeLimit[3] / 2 - 1;
	}
	for (int i = 0; i < curArray.size(); i++)
	{
		for (int j = 0; j < curArray[0].size(); j++)
		{
			for (int h = 0; h < curArray[0][0].size(); h++)
			{
				curArray[i][j][h].erase(curArray[i][j][h].begin() + limitLow, curArray[i][j][h].erase(curArray[i][j][h].begin() + limitHigh));
			}
		}
	}
	FindPeak(curArray, peakInd);
	return;
}

int main()
{
	// length of each dimension
	int m = 0, n = 0, k = 0, l=0;
	// get the array using file stream
	ifstream arrFile("array.txt");
	if (!arrFile){
		cout << "Unable to open array-file";
		exit(1); // terminate with error
	}
	// get the length first
	arrFile >> m >> n >> k >> l;
	// initialize the 4D array
	vector<vector<vector<vector<int> > > >   arr4D(m, vector<vector<vector<int> > >(n, vector<vector<int> >(k, vector<int>(l))));
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			for (int h = 0; h < k; h++)
			{
				for (int g = 0; g < l; g++)
				{
					arrFile >> arr4D[i][j][h][g];
				}
			}
		}
	}
	// close file stream
	arrFile.close();

	int peakInd[4];
	FindPeak(arr4D, peakInd);
	cout << peakInd[0] + 1 << " " << peakInd[1] + 1 << " " << peakInd[2] + 1 << " " << peakInd[3] + 1 << endl;

	return 0;
}