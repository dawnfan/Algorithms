#include <math.h>
#include <limits>
#include <iostream>
#include <fstream>

using namespace std;


int** OutImageMat;
int ImageRawNum = 450;
int ImageColNum = 375;
int max_out = 0;

#define  OC 0.1


void ReadImageTxt(char* imagename, double** &imageMat){
	//int ImageRawNum = 0, ImageColNum = 0;
	// read the image-text file
	ifstream arrFile(imagename);
	if (!arrFile){
		cout << "Unable to open array-file";
		exit(1); // terminate with error
	}
	// get image size first
	//int tr, tc;
	//arrFile >> tr >> tc;
	arrFile >> ImageRawNum >> ImageColNum;
	imageMat = new double*[ImageRawNum];
	for (int t_i = 0; t_i < ImageRawNum; t_i++)
	{
		imageMat[t_i] = new double[ImageColNum];
	}
	// read the pixel
	for (int t_r = 0; t_r < ImageRawNum; t_r++)
	{
		for (int t_c = 0; t_c < ImageColNum; t_c++)
		{
			arrFile >> imageMat[t_r][t_c];
		}
	}
	// close file stream
	arrFile.close();
}

void WriteTxtImage(char* txtname, int** imageMat){
	ofstream arrFile(txtname);
	if (!arrFile){
		cout << "Unable to open array-file";
		exit(1); // terminate with error
	}
	// write the pixel
	for (int t_r = 0; t_r < ImageRawNum; t_r++)
	{
		for (int t_c = 0; t_c < ImageColNum; t_c++)
		{
			arrFile << (int)(((double)imageMat[t_r][t_c]/max_out) * 255)<<' ';
		}
		arrFile << '\n';
	}
	// close file stream
	arrFile.close();
}


void MatchRaw(double* left_raw, double* right_raw, int raw_num){
	double** C_mat;
	double** M_mat;
	C_mat = new double*[ImageColNum];
	M_mat = new double*[ImageColNum];
	for (int t_l = 0; t_l < ImageColNum; t_l++)
	{
		C_mat[t_l] = new double[ImageColNum];
		M_mat[t_l] = new double[ImageColNum];
	}
	// initialize the cost
	C_mat[0][0] = 0;
	M_mat[0][0] = 2;
	for (int t_l = 1; t_l < ImageColNum; t_l++)
	{
		C_mat[t_l][0] = OC * t_l;
		C_mat[0][t_l] = OC * t_l;
		M_mat[t_l][0] = 2;
		M_mat[0][t_l] = 2;
	}
	// set cost and path-M
	for (int t_l = 1; t_l < ImageColNum; t_l++)
	{
		for (int t_r = 1; t_r < ImageColNum; t_r++)
		{
			double cur_dis = pow((left_raw[t_l] - right_raw[t_r]), 2) + C_mat[t_l - 1][t_r - 1];
			double t1 = OC + C_mat[t_l][t_r - 1];
			double t2 = OC + C_mat[t_l - 1][t_r];
			C_mat[t_l][t_r] = cur_dis > t1 ? t1 : cur_dis;
			C_mat[t_l][t_r] = C_mat[t_l][t_r] > t2 ? t2 : C_mat[t_l][t_r];
			// trace the path
			if (C_mat[t_l][t_r] == cur_dis)
			{
				M_mat[t_l][t_r] = 1;
			}
			else if (C_mat[t_l][t_r] == t1)
			{
				M_mat[t_l][t_r] = 2;
			}
			else
			{
				M_mat[t_l][t_r] = 3;
			}
		}
	}
	//int max_out = 0;
	for (int t_l = ImageColNum-1; t_l > 0; t_l--)
	{
		for (int t_r = ImageColNum-1; t_r > 0; t_r--)
		{
			if (M_mat[t_l][t_r] == 1)
			{
				OutImageMat[raw_num][t_l-1] = t_l - t_r;
				if (OutImageMat[raw_num][t_l - 1] > max_out)
				{
					max_out = OutImageMat[raw_num][t_l - 1];
				}
			}
		}
	}

	for (int t_i = 0; t_i < ImageColNum; t_i++)
	{
		delete[]C_mat[t_i];
		delete[]M_mat[t_i];
	}
	delete[]C_mat;
	delete[]M_mat;

}

void MatchStereo(double** left_mat, double** right_mat){




	OutImageMat = new int*[ImageRawNum];
	for (int t_i = 0; t_i < ImageRawNum; t_i++)
	{
		OutImageMat[t_i] = new int[ImageColNum-1];
	}

	for (int t_r = 0; t_r < ImageRawNum; t_r++)
	{
		for (int t_c = 0; t_c < ImageColNum; t_c++)
		{
			right_mat[t_r][t_c] /= 255;
			left_mat[t_r][t_c] /= 255;
		}
	}

	for (int t_i = 0; t_i < ImageRawNum; t_i++)
	{
		MatchRaw(left_mat[t_i], right_mat[t_i], t_i);
	}

	WriteTxtImage("out.txt", OutImageMat);

	return;
}