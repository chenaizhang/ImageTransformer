#ifndef CONVERT_H
#define CONVERT_H

#include <QtCore/QDebug>
#include <QtGui/QImage>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

QImage cvMatToQImage(const cv::Mat& mat, bool clone = true, bool rb_swap = true);//类型转换:Mat转QImage

cv::Mat QImageTocvMat(QImage &image, bool clone = false, bool rb_swap = true);//类型转换：QImage转Mat

#endif // CONVERT_H
