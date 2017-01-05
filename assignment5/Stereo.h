#include <math.h>
#include <limits>
#include <iostream>
#include <fstream>

using namespace std;

#define MAX_DOUBLE_VALUE (static_cast<double>(10e10))

int** OutImageMat;
int ImageRawNum = 450;
int ImageColNum = 375;
int max_out = 0;

#define  OC 0.1

#define MAXD 64
#define K_POTTS 50


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
			int temp_out = (int)(((double)imageMat[t_r][t_c] / max_out) * 255);
			arrFile << temp_out << ' ';
		}
		arrFile << '\n';
	}
	// close file stream
	arrFile.close();
}

void unary_cost(double** L_in, double** R_in, double*** &dist_out){
	// calculate the data term- unary cost
	dist_out = new double**[ImageRawNum];
	// #1: initialize out matrix
	for (int t_r = 0; t_r < ImageRawNum; t_r++)
	{
		dist_out[t_r] = new double*[ImageColNum];
		for (int t_c = 0; t_c < ImageColNum; t_c++)
		{
			dist_out[t_r][t_c] = new double[MAXD];
		}
	}
	// #2: calculate the cost
	for (int t_d = 0; t_d < MAXD; t_d++)
	{
		for (int t_r = 0; t_r < ImageRawNum; t_r++)
		{
			for (int t_c = 0; t_c < ImageColNum; t_c++)
			{
				if (t_c < t_d)
				{
					dist_out[t_r][t_c][t_d] = MAX_DOUBLE_VALUE;
				}
				else{
					dist_out[t_r][t_c][t_d] = pow((L_in[t_r][t_c] - R_in[t_r][t_c - t_d]), 2);
				}
			}
		}
	}
	return;
}

double potts_cost(int pos_1[2], int pos_2[2], double** disp_in){
	// calculate the smoothness term- Potts-Model
	double cost_out = 0;

	cost_out = pow(disp_in[pos_1[0]][pos_1[1]] - disp_in[pos_2[0]][pos_2[1]], 2);
	cost_out = cost_out > K_POTTS ? K_POTTS : cost_out;

	return cost_out;
}

void initialize(double*** dist_in, double** &disp_out){
	// initialize the disparities
	disp_out = new double*[ImageRawNum];
	// #1: initialize out matrix
	for (int t_r = 0; t_r < ImageRawNum; t_r++)
	{
		disp_out[t_r] = new double[ImageColNum];
	}
	// #2: initialize the disparity matrix -- find the minimum unary-cost for every pixel
	for (int t_r = 0; t_r < ImageRawNum; t_r++)
	{
		for (int t_c = 0; t_c < ImageColNum; t_c++)
		{
			double min_dist = MAX_DOUBLE_VALUE;
			for (int t_d = 0; t_d < MAXD; t_d++)
			{
				if (min_dist > dist_in[t_r][t_c][t_d])
				{
					min_dist = dist_in[t_r][t_c][t_d];
					disp_out[t_r][t_c] = t_d;
				}
			}
		}
	}

	return;
}

void MatchStereo(double** left_mat, double** right_mat){
	OutImageMat = new int*[ImageRawNum];
	for (int t_i = 0; t_i < ImageRawNum; t_i++)
	{
		OutImageMat[t_i] = new int[ImageColNum];
	}
	double** disp;
	double*** dist;
	unary_cost(left_mat, right_mat, dist);
	initialize(dist, disp);
	// # test: initialization of disparities.
	for (int t_r = 0; t_r < ImageRawNum; t_r++)
	{
		for (int t_c = 0; t_c < ImageColNum; t_c++)
		{
			OutImageMat[t_r][t_c] = disp[t_r][t_c];
			if (max_out < OutImageMat[t_r][t_c])
			{
				max_out = OutImageMat[t_r][t_c];
			}
		}
	}
	cout << "done." << endl;
	return;
}

///////////////////////////////////// StereoMatching with DP ////////////////////////////////////////////
//void MatchRaw(double* left_raw, double* right_raw, int raw_num){
//	double** C_mat;
//	double** M_mat;
//	C_mat = new double*[ImageColNum];
//	M_mat = new double*[ImageColNum];
//	for (int t_l = 0; t_l < ImageColNum; t_l++)
//	{
//		C_mat[t_l] = new double[ImageColNum];
//		M_mat[t_l] = new double[ImageColNum];
//	}
//	// initialize the cost
//	C_mat[0][0] = 0;
//	M_mat[0][0] = 2;
//	for (int t_l = 1; t_l < ImageColNum; t_l++)
//	{
//		C_mat[t_l][0] = OC * t_l;
//		C_mat[0][t_l] = OC * t_l;
//		M_mat[t_l][0] = 2;
//		M_mat[0][t_l] = 2;
//	}
//	// set cost and path-M
//	for (int t_l = 1; t_l < ImageColNum; t_l++)
//	{
//		for (int t_r = 1; t_r < ImageColNum; t_r++)
//		{
//			double cur_dis = pow((left_raw[t_l] - right_raw[t_r]), 2) + C_mat[t_l - 1][t_r - 1];
//			double t1 = OC + C_mat[t_l][t_r - 1];
//			double t2 = OC + C_mat[t_l - 1][t_r];
//			C_mat[t_l][t_r] = cur_dis > t1 ? t1 : cur_dis;
//			C_mat[t_l][t_r] = C_mat[t_l][t_r] > t2 ? t2 : C_mat[t_l][t_r];
//			// trace the path
//			if (C_mat[t_l][t_r] == cur_dis)
//			{
//				M_mat[t_l][t_r] = 1;
//			}
//			else if (C_mat[t_l][t_r] == t1)
//			{
//				M_mat[t_l][t_r] = 2;
//			}
//			else
//			{
//				M_mat[t_l][t_r] = 3;
//			}
//		}
//	}
//	//int max_out = 0;
//	for (int t_l = ImageColNum - 1; t_l > 0; t_l--)
//	{
//		for (int t_r = ImageColNum - 1; t_r > 0; t_r--)
//		{
//			if (M_mat[t_l][t_r] == 1)
//			{
//				OutImageMat[raw_num][t_l - 1] = t_l - t_r;
//				if (OutImageMat[raw_num][t_l - 1] > max_out)
//				{
//					max_out = OutImageMat[raw_num][t_l - 1];
//				}
//			}
//		}
//	}
//
//	for (int t_i = 0; t_i < ImageColNum; t_i++)
//	{
//		delete[]C_mat[t_i];
//		delete[]M_mat[t_i];
//	}
//	delete[]C_mat;
//	delete[]M_mat;
//
//}
//
//void MatchStereo(double** left_mat, double** right_mat){
//
//
//
//
//	OutImageMat = new int*[ImageRawNum];
//	for (int t_i = 0; t_i < ImageRawNum; t_i++)
//	{
//		OutImageMat[t_i] = new int[ImageColNum - 1];
//	}
//
//	for (int t_r = 0; t_r < ImageRawNum; t_r++)
//	{
//		for (int t_c = 0; t_c < ImageColNum; t_c++)
//		{
//			right_mat[t_r][t_c] /= 255;
//			left_mat[t_r][t_c] /= 255;
//		}
//	}
//
//	for (int t_i = 0; t_i < ImageRawNum; t_i++)
//	{
//		MatchRaw(left_mat[t_i], right_mat[t_i], t_i);
//	}
//
//	WriteTxtImage("out.txt", OutImageMat);
//
//	return;
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////