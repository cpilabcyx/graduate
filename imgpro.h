#pragma once
#include "mycircle.h"
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include <opencv2\opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <cstdio>
#include<algorithm>
#include<Windows.h>
#include<deque>
#ifndef PI
#define PI 3.141592653
#endif

using namespace cv;
using namespace std;

//根据椭圆第一定义, 用a表示椭圆长半轴的长, b表示椭圆短半轴的长, 且a>b>0. 椭圆周长公式：L = 2πb + 4(a - b)
//椭圆周长定理：椭圆的周长等于该椭圆短半轴长为半径的圆周长（2πb）加上四倍的该椭圆长半轴长（a）与短半轴长（b）的差.椭圆面积公式： S = πab

class imgpro
{
public:
	imgpro();

	//椭圆筛选
	bool isEllipse(const vector<cv::Point2f> &contours, const RotatedRect &canEll, const double maxERR);

	//椭圆检测
	void findEllipses(const vector< vector<cv::Point2f> >& contoursRaw, std::vector< int >& sublabEllipse, std::vector<RotatedRect>& rectEll);

	//椭圆检测
	bool DecodeMark1(Mat &image, std::vector<cv::Point2f>& vpUV, bool if_gray=false);

	//椭圆检测,记录长短轴
	bool DecodeMark1(Mat &image, mycircle &cir,bool if_gray = false);

	//椭圆鉴别hsv
	void identify_cir_hsv(Mat &image, vector<cv::Point2f>& vpUV, vector<int>& hsv);

	//椭圆心鉴别hsv, 并分为group类
	void identify_cir_hsv(Mat &image, vector<cv::Point2f>& vpUV, vector<int>& hsv, vector<int>& id , int group=2);

	//加噪点
	void salt(Mat &image, int n);

	//极线匹配
	bool matchEllipse(vector<Point2f>& cir_cev_l, vector<Point2f>& cir_cev_r, vector<Point2f>& cir_match_cev_l, vector<Point2f>& cir_match_cev_r, Mat& F_2_1);
	bool matchEllipse(mycircle &cir_l, mycircle &cir_r, Mat& F_2_1);

	//由双目像素坐标计算世界坐标
	bool pointTo3D(vector<Point2f>& vec_uvLeft, vector<Point2f>& vec_uvRight, Mat& mLeftIntrinsic, Mat& mRightIntrinsic, Mat& mRightRT, vector<Point3f>& vec_3d);

	bool tack_flow(Mat& pregray, Mat& gray, vector<Point2f>& pre, vector<Point2f>& now, vector<uchar>& status,vector<float>& err);

	~imgpro();
};








