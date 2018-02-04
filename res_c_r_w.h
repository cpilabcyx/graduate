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

	//3d点欧氏距离
	float dis_comput_point(Point3f a, Point3f b);

	//输出3d点
	int cout_3d_point(const vector<Point3f> &point3d);

	//2d点乱序，最小距离
	int comput_err2d_random_order(const vector<Point2f> &detected, const vector<Point2f> &idle, vector<float> &err);

	//2d点乱序，最小距离，//空的，日后填
	int comput_err2d_random_order(const vector<RotatedRect> &detected, const vector<RotatedRect> &idle, vector<float> &err);

	//记录vector
	int txt_record(string& filneme, vector<Point2f> &point2d);
	int txt_record(string& filneme, vector<float> &point);

	//读取xml
	bool xml_read(char filneme[], char mat_name[], Mat& mat);

	//融合两个矩阵
	Mat mergeRows(Mat& A, Mat& B);//行[A ; B]
	Mat mergecols(Mat& A, Mat& B);//列[A   B]

	~res_c_r_w();
};

