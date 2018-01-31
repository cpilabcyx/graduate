#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#ifndef PI
#define PI 3.141592653
#endif

using namespace cv;
using namespace std;

class res_c_r_w
{
public:


public:
	res_c_r_w();

	//2dµãÅ·ÊÏ¾àÀë
	float dis_comput_point(Point2f a, Point2f b);

	//2dµãÂÒĞò£¬×îĞ¡¾àÀë
	int comput_err2d_random_order(const vector<Point2f> detected, const vector<Point2f>idle, vector<float>err);

	~res_c_r_w();
};

