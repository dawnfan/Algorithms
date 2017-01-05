#include "Stereo.h"

int main()
{
	double** LeftImageMat;
	double** RightImageMat;
	//LeftImageMat = NULL;
	//RightImageMat = NULL;
	ReadImageTxt("L.txt", LeftImageMat);
	ReadImageTxt("R.txt", RightImageMat);

	MatchStereo(LeftImageMat, RightImageMat);

	WriteTxtImage("out.txt", OutImageMat);


	// delete the mat data
	for (int t_i = 0; t_i < ImageRawNum; t_i++)
	{
		delete[]RightImageMat[t_i];
		delete[]LeftImageMat[t_i];
		delete[]OutImageMat[t_i];
	}
	delete[]RightImageMat;
	delete[]LeftImageMat;
	delete[]OutImageMat;

	return 0;
}