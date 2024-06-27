#pragma once
#ifdef _HAS_STD_BYTE
#undef _HAS_STD_BYTE
#endif
#define _HAS_STD_BYTE 0
#include <opencv2/opencv.hpp> // 包含OpenCV 4的核心和大部分模块
#include <vector>
#include <chrono> // 替代 ctime 进行时间的测量
#include <iostream>
#include <cmath> // 使用 C++ 的cmath替代C语言的math.h
#include <iomanip>
#include <fstream>
#include "method.h"
#include "treeNode.h"
using namespace cv;


class stc :public method {
public:
	stc();
	stc(method*p);
	virtual void process();
};