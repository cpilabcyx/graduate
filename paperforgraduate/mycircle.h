#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#ifndef PI
#define PI 3.141592653
#endif
using namespace cv;
using namespace std;

class mycircle
{
public:

	vector<Point2f> cir_idle;//理论圆心
	vector<int> cir_id_idle;//理论id
	vector<RotatedRect> cir_idle_ab;//理论圆心

	vector<Point2f> cir_vec;//检测到圆心
	vector<int> cir_id;//鉴别id
	vector<RotatedRect> cir_idle_ab_detect;//检测椭圆

public:
	mycircle();

	//将椭圆显示在图像上
	Mat& show_circle(Mat& img);

	//将椭圆id显示在图像上
	Mat& show_circle_id(Mat& img);

	//画椭圆
	Mat& genarate_ellipse(Mat& img, vector<RotatedRect> &center_ab_size, Scalar corlor = Scalar(0,0,0));

	//画圆
	Mat& genarate_cir(Mat& img, vector<Point2f> center_idle, int radius);

	~mycircle();
};

