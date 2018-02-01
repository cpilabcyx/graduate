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

	vector<int> cir_hsv;//鉴别hsv
	vector<int> cir_id;//鉴别id
	vector<RotatedRect> cir_idle_ab_detect;//检测椭圆

public:
	mycircle();

	//画检测到圆心 circle(img, cir_vec[i], 3, Scalar(255, 255, 255), CV_FILLED);
	Mat& show_circle(Mat& img);

	//画检测到圆顺序/id  sprintf(s, "%d", cir_id[i]);
	Mat& show_circle_id(Mat& img);

	//画椭圆,默认填充 黑色 thickness=-1
	Mat& genarate_ellipse(Mat& img, vector<RotatedRect> &center_ab_size, Scalar corlor = Scalar(0,0,0), int thickness=-1);

	//分组画椭圆  默认填充 黑色 thickness=-1
	Mat& genarate_ellipse_maiker(Mat& img, vector<RotatedRect> &center_ab_size, int group, int thickness = -1);

	//画圆
	Mat& genarate_cir(Mat& img, vector<Point2f> center_idle, int radius);

	~mycircle();
};

