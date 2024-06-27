//rnam.h
#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include "method.h"
#include "Color.h"
#include "MyTimer.h"
using namespace std;
using namespace cv;
class rnam :public method
{
public:
	rnam();
	rnam(method* p);
	virtual void process();
	friend bool isSameBlock(Mat f, Mat mark, Point lt, Point rb, int xgm);
	friend void mark(Mat& mark, Point lt, Point rb);
};

double psnr(Mat f, Mat R, int M, int N);
double bbp(vector<Color> p, int M, int N, vector<char> q);
void EnCode(Mat& R, int height, int width, vector<char>& Q);
void Decode(Mat& R, int height, int width, const vector<char>& Q);
void RNAMCDecoding(Mat& R, int M, int N, vector<Color> p, vector<char> q);
void segmentDisplay(Mat& display, vector<Color> p);
void RNAMCEncoding(Mat f, Mat& R, Mat& markMatrix, int M, int N, vector<Color>& p, vector<char>& q, int xgm, int methodId);
Point diagonalPriority(Mat f, Mat mark, int M, int N, Point start, int xgm);
Point diagonalPriority_x(Mat f, Mat mark, int M, int N, Point start, int xgm);
Point diagonalPriority_y(Mat f, Mat mark, int M, int N, Point start, int xgm);
Point hirizonalverticalPriority(Mat f, Mat mark, int M, int N, Point start, int xgm);
Point hirizonalPriority(Mat f, Mat mark, int M, int N, Point start, int xgm);
Point verticalPriority(Mat f, Mat mark, int M, int N, Point start, int xgm);
Point sizePriority(Mat f, Mat mark, int M, int N, Point start, int xgm);
void RDHelper(Mat& R, Color p1);