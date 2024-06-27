#pragma once
#ifdef _HAS_STD_BYTE
#undef _HAS_STD_BYTE
#endif
#define _HAS_STD_BYTE 0
#include<opencv.hpp>
#include <opencv2\imgproc\types_c.h>
using namespace std;
using namespace cv;


class Color
{
	Point lt;
	Point rb;
	Scalar grey;
public:
	Color() { grey[0] = grey[1] = grey[2] = grey[3] = -1; }
	void setLt(Point t) { lt = t; }
	void setRb(Point b) { rb = b; }
	Point getLt() { return lt; }
	Point getRb() { return rb; }
	void setGrey(Scalar grey) { this->grey = grey; }
	Scalar getGrey() { return grey; }
};