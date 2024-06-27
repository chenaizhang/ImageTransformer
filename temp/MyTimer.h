#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

class MyTimer {
private:
    std::chrono::high_resolution_clock::time_point _begin;
    std::chrono::high_resolution_clock::time_point _end;
public:
    long costTime; // 花费的时间(精确到毫秒)
public:
    MyTimer() : costTime(0) {}

    void Start() { // 开始计时
        _begin = std::chrono::high_resolution_clock::now();
    }

    void End() { // 结束计时
        _end = std::chrono::high_resolution_clock::now();
        costTime = std::chrono::duration_cast<std::chrono::microseconds>(_end - _begin).count()/1000;
    }

    void Reset() { // 计时清0
        costTime = 0;
    }
};
