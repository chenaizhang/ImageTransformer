//method.h
#pragma once

#include<opencv2/core/core.hpp>
#include<opencv2/opencv.hpp>

//
class method 
{
public:
	method();
	virtual void process() {}
	int methodId;//使用的方法
	cv::Mat origImg;//原图
	cv::Mat compressedImg;//压缩图
	cv::Mat segImg;//分割图
	int h;//图片高
	int w;//图片宽
	int xigema;//误差容许量
	double thresU_;
	double thresVar_ ;
	int num_;//最后一个参数

	double enTime;//编码所用时间
	double deTime;//解码所用时间
	double blocknum;//分块数
	double PSNR;
	double CR;
	double BPP;
};