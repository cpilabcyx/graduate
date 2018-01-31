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
	bool DecodeMark1(Mat &image, std::vector<cv::Point2f>& vpUV);

	//椭圆检测,记录长短轴
	bool DecodeMark1(Mat &image, mycircle &cir);

	//椭圆鉴别hsv
	void identify_cir(Mat &image, std::vector<cv::Point2f>& vpUV, std::vector<int>& id);

	//加噪点
	void salt(Mat &image, int n);

	~imgpro();
};








