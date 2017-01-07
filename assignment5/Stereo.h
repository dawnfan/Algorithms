#include <math.h>
#include <limits>
#include <iostream>
#include <fstream>
#include "graphcut/graph.h"

using namespace std;

#define MAX_DOUBLE_VALUE (static_cast<double>(10e10))

int** OutImageMat;
int ImageRawNum = 450;
int ImageColNum = 375;
int max_out = 0;
double lastE = MAX_DOUBLE_VALUE;

typedef Graph<double, double, double> GraphType;
GraphType *g;
GraphType::node_id node_id_map[1000][1000];

#define  OC 0.1

#define MAXD 64
#define K_POTTS 100

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

double potts_cost(int pos_1[2], int pos_2[2], int disp_1, int disp_2){
	// calculate the smoothness term- Potts-Model
	double cost_out = 0;

	//cost_out = pow(disp_in[pos_1[0]][pos_1[1]] - disp_in[pos_2[0]][pos_2[1]], 2);
	//cost_out = pow(disp_1 - disp_2, 2);
	cost_out = 100*abs(disp_1 - disp_2);
	cost_out = cost_out > K_POTTS ? K_POTTS : cost_out;

	return cost_out;
}

double energy_cost(int** disp_in, double*** dist_in)
{
	double energy = 0;
	for (int i = 0; i < ImageRawNum; i++)
	{
		for (int j = 0; j < ImageColNum; j++)
		{
			energy += dist_in[i][j][disp_in[i][j]];

			if (j + 1 < ImageColNum)
			{
				int pos_1[2] = { i, j };
				int pos_2[2] = { i, j + 1 };
				energy += potts_cost(pos_1, pos_2, disp_in[i][j], disp_in[i][j + 1]);
			}
		}
	}

	return energy;

}

void initialize(double*** dist_in, int** &disp_out){
	// initialize the disparities
	disp_out = new int*[ImageRawNum];
	// #1: initialize out matrix
	for (int t_r = 0; t_r < ImageRawNum; t_r++)
	{
		disp_out[t_r] = new int[ImageColNum];
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

bool abSwap(int** &disp_in, double*** dist_in, int a_in, int b_in){
	int **new_disp;
	new_disp = new int*[ImageRawNum];
	for (int t_i = 0; t_i < ImageRawNum; t_i++)
	{
		new_disp[t_i] = new int[ImageColNum];
	}
	for (int i = 0; i < ImageRawNum; i++)
	{
		for (int j = 0; j < ImageColNum; j++)
		{
			new_disp[i][j] = disp_in[i][j];
		}
	}
	// # 1: create a graph
	g->reset();
	for (int i = 0; i < ImageRawNum; i++)
	{
		for (int j = 0; j < ImageColNum; j++)
		{
			node_id_map[i][j] = -1;
		}
	}
	for (int i = 1; i < ImageRawNum - 1; i++)
	{
		for (int j = 1; j < ImageColNum - 1; j++)
		{
			if (i != a_in && j != b_in)
			{
				continue;
			}

			bool brnode = (abs(disp_in[i][j + 1] - a_in) < 0.1 || abs(disp_in[i][j + 1] - b_in) < 0.1);
			bool bdnode = (abs(disp_in[i + 1][j] - a_in) < 0.1 || abs(disp_in[i + 1][j] - b_in) < 0.1);
			bool bunode = (abs(disp_in[i - 1][j] - a_in) < 0.1 || abs(disp_in[i - 1][j] - b_in) < 0.1);
			bool blnode = (abs(disp_in[i][j - 1] - a_in) < 0.1 || abs(disp_in[i][j - 1] - b_in) < 0.1);

			GraphType::node_id nid = node_id_map[i][j];
			if (nid == -1)
			{
				nid = g->add_node();
				node_id_map[i][j] = nid;
			}


			int pos_1[2] = { i, j };
			int pos_2[2] = { i, j + 1 };
			double t1 = dist_in[i][j][a_in];
			//double n_t1 = (brnode ? 0 : potts_cost(pos_1, pos_2, a_in, disp_in[i][j + 1]));
			//pos_2[1] = j - 1;
			//n_t1 += blnode ? 0 : potts_cost(pos_1, pos_2, a_in, disp_in[i][j - 1]);
			//pos_2[0] = i + 1;
			//pos_2[1] = j;
			//n_t1 += bunode ? 0 : potts_cost(pos_1, pos_2, a_in, disp_in[i + 1][j]);
			//pos_2[0] = i - 1;
			//n_t1 += bdnode ? 0 : potts_cost(pos_1, pos_2, a_in, disp_in[i - 1][j]);
			//t1 += n_t1 / 4;

			pos_2[0] = i;
			pos_2[1] = j + 1;
			double t2 = dist_in[i][j][a_in];
			//double n_t2 = (brnode ? 0 : potts_cost(pos_1, pos_2, b_in, disp_in[i][j + 1]));
			//pos_2[1] = j - 1;
			//n_t2 += blnode ? 0 : potts_cost(pos_1, pos_2, b_in, disp_in[i][j - 1]);
			//pos_2[0] = i + 1;
			//pos_2[1] = j;
			//n_t2 += bunode ? 0 : potts_cost(pos_1, pos_2, b_in, disp_in[i + 1][j]);
			//pos_2[0] = i - 1;
			//n_t2 += bdnode ? 0 : potts_cost(pos_1, pos_2, b_in, disp_in[i - 1][j]);
			//t2 += n_t2 / 4;

			g->add_tweights(nid, t1, t2);

			GraphType::node_id lnode = nid;
			GraphType::node_id rnode = node_id_map[i][j + 1];
			GraphType::node_id dnode = node_id_map[i + 1][j];
			if (rnode == -1 && brnode)
			{
				rnode = g->add_node();
				node_id_map[i][j + 1] = rnode;
			}

			if (dnode == -1 && bdnode)
			{
				dnode = g->add_node();
				node_id_map[i + 1][j] = dnode;
			}

			pos_2[0] = i;
			pos_2[1] = j + 1;
			if (brnode)
			{
				g->add_edge(lnode, rnode, potts_cost(pos_1, pos_2, a_in, b_in), 0);
			}

			if (bdnode)
			{
				pos_2[0] = i + 1;
				g->add_edge(lnode, dnode, potts_cost(pos_1, pos_2, a_in, b_in), 0);
			}
		}
	}

	// #2: calculate min cut
	double flow = g->maxflow();
	for (int i = 0; i < ImageRawNum; i++)
	{
		for (int j = 0; j < ImageColNum; j++)
		{
			GraphType::node_id nodeid = node_id_map[i][j];
			if (nodeid != -1)
			{
				if (g->what_segment(nodeid) == GraphType::SOURCE)
				{
					new_disp[i][j] = a_in;
				}
				else
				{
					new_disp[i][j] = b_in;
				}
			}
			else{
				new_disp[i][j] = disp_in[i][j];
			}
		}
	}
	//# 3: update the disparities using energy cost
	bool isSwaped = false;
	double newE = energy_cost(new_disp, dist_in);
	if (newE < lastE)
	{
		isSwaped = true;
		lastE = newE;
		for (int i = 0; i < ImageRawNum; i++)
		{
			for (int j = 0; j < ImageColNum; j++)
			{
				disp_in[i][j] = new_disp[i][j];
			}
		}
	}
	for (int t_i = 0; t_i < ImageRawNum; t_i++)
	{
		delete[] new_disp[t_i];
	}
	delete[] new_disp;

	return isSwaped;
}


void MatchStereo(double** left_mat, double** right_mat){
	OutImageMat = new int*[ImageRawNum];
	for (int t_i = 0; t_i < ImageRawNum; t_i++)
	{
		OutImageMat[t_i] = new int[ImageColNum];
	}
	int** disp;
	double*** dist;
	unary_cost(left_mat, right_mat, dist);
	initialize(dist, disp);
	// initialize the graph 
	g = new GraphType(ImageRawNum*ImageColNum * 2, ImageRawNum*ImageColNum * 8);

	// # test: initialization of disparities.
	//for (int t_r = 0; t_r < ImageRawNum; t_r++)
	//{
	//	for (int t_c = 0; t_c < ImageColNum; t_c++)
	//	{
	//		OutImageMat[t_r][t_c] = disp[t_r][t_c];
	//		if (max_out < OutImageMat[t_r][t_c])
	//		{
	//			max_out = OutImageMat[t_r][t_c];
	//		}
	//	}
	//}
	/////////////////////////////////////////////////////


	bool update_flag = true;
	int t_step = 0;
	while (update_flag || t_step< 10)
	{
		t_step++;
		update_flag = false;
		for (int i = 0; i < MAXD; i++)
		{
			for (int j = 0; j < MAXD; j++)
			{
				if (i == j)
				{
					continue;
				}
				update_flag = abSwap(disp, dist, i, j);
			}
		}
	}
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