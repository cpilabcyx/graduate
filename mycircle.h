#pragma once
#include<vector>
#include<opencv.hpp>
using namespace std;
using namespace cv;

class mycircle
{
public:

	vector<Point2f> cir_idle;

public:
	mycircle();

	//����Բ
	Mat& genarate_ellipse(Mat& img, vector<Point2f> center_idle, float angle = 0);

	//��Բ
	Mat& genarate_cir(Mat& img, vector<Point2f> center_idle, int radius);

	~mycircle();
};

