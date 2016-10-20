#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int getMaxElement(vector<vector<int> > &subArray, int *MaxInd){
	// find the maximum element on a plane
	int maxInt = subArray[0][0];
	MaxInd[0] = 0;
	MaxInd[1] = 0; 
	int m = subArray.size(), n = subArray[0].size();
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (maxInt < subArray[i][j])
			{
				maxInt = subArray[i][j];
				MaxInd[0] = i;
				MaxInd[1] = j;
			}
		}
	}
	return maxInt;
}
bool isNeighborBigger(vector<vector<vector<int> > > &curArray, int* maxInd, int neighborDim, int step){
	bool isBigger = false;
	int sizeLimit[3] = { curArray.size(), curArray[0].size(), curArray[0][0].size() };
	int neighbor[3] = { maxInd[0], maxInd[1], maxInd[2] };
	neighbor[neighborDim] += step;
	neighbor[neighborDim] = neighbor[neighborDim] < 0 ? 0 : neighbor[neighborDim];
	neighbor[neighborDim] = neighbor[neighborDim] < sizeLimit[neighborDim] ? neighbor[neighborDim] : sizeLimit[neighborDim];
	int curInt = curArray[maxInd[0]][maxInd[1]][maxInd[2]];
	int neiInt = curArray[neighbor[0]][neighbor[1]][neighbor[2]];
	if (curInt < neiInt)
	{
		isBigger = true;
	}
	return isBigger;
}
void FindPeak(vector<vector<vector<int> > > curArray, int* peakInd){
	int sizeLimit[3] = { curArray.size(), curArray[0].size(), curArray[0][0].size() };

	int checkPlane = sizeLimit[0] / 2;
	int maxInd[3];
	maxInd[0] = checkPlane;
	int tempMax = getMaxElement(curArray[checkPlane], maxInd + 1);
	int neighborDim = 0;
	int biggerInd[3] = { maxInd[0], maxInd[1], maxInd[2] };
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
	else{
		peakInd[0] = maxInd[0];
		peakInd[1] = maxInd[1];
		peakInd[2] = maxInd[2];
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
	if (biggerInd[1] > sizeLimit[1]/2)
	{
		limitHigh = sizeLimit[1] / 2;
	}
	else{
		limitLow = sizeLimit[1] / 2 -1;
	}
	for (int i = 0; i < curArray.size(); i++)
	{
		curArray[i].erase(curArray[i].begin() + limitLow, curArray[i].begin() + limitHigh);
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
			curArray[i][j].erase(curArray[i][j].begin() + limitLow, curArray[i][j].begin() + limitHigh);
		}
	}
	FindPeak(curArray, peakInd);
	return;
}

int main()
{
	// length of each dimension
	int m = 0, n = 0, k = 0;
	// get the array using file stream
	ifstream arrFile("array.txt");
	if (!arrFile){
		cout << "Unable to open array-file";
		exit(1); // terminate with error
	}
	// get the length first
	arrFile >> m >> n >> k;
	// initialize the 3D array
	vector<vector<vector<int> > >   arr3D(m, vector<vector<int> >(n, vector<int>(k)));
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			for (int h = 0; h < k; h++)
			{
				arrFile >> arr3D[i][j][h];
			}
		}
	}
	// close file stream
	arrFile.close();

	int peakInd[3];
	FindPeak(arr3D, peakInd);
	cout << peakInd[0] + 1 << " " << peakInd[2] + 1 << " " << peakInd[1] + 1 << endl;

	return 0;
}