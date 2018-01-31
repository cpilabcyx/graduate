#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include <fstream>
#include <string>
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

	//2d点欧氏距离
	float dis_comput_point(Point2f a, Point2f b);

	//2d点乱序，最小距离
	int comput_err2d_random_order(const vector<Point2f> &detected, const vector<Point2f> &idle, vector<float> &err);

	//2d点乱序，最小距离，//空的，日后填
	int comput_err2d_random_order(const vector<RotatedRect> &detected, const vector<RotatedRect> &idle, vector<float> &err);

	//记录vector
	int txt_record(string& filneme, vector<Point2f> &point2d);
	int txt_record(string& filneme, vector<float> &point);


	~res_c_r_w();
};

