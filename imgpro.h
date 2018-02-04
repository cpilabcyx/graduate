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

//������Բ��һ����, ��a��ʾ��Բ������ĳ�, b��ʾ��Բ�̰���ĳ�, ��a>b>0. ��Բ�ܳ���ʽ��L = 2��b + 4(a - b)
//��Բ�ܳ�������Բ���ܳ����ڸ���Բ�̰��᳤Ϊ�뾶��Բ�ܳ���2��b�������ı��ĸ���Բ�����᳤��a����̰��᳤��b���Ĳ�.��Բ�����ʽ�� S = ��ab

class imgpro
{
public:
	imgpro();

	//��Բɸѡ
	bool isEllipse(const vector<cv::Point2f> &contours, const RotatedRect &canEll, const double maxERR);

	//��Բ���
	void findEllipses(const vector< vector<cv::Point2f> >& contoursRaw, std::vector< int >& sublabEllipse, std::vector<RotatedRect>& rectEll);

	//��Բ���
	bool DecodeMark1(Mat &image, std::vector<cv::Point2f>& vpUV, bool if_gray=false);

	//��Բ���,��¼������
	bool DecodeMark1(Mat &image, mycircle &cir,bool if_gray = false);

	//��Բ����hsv
	void identify_cir_hsv(Mat &image, vector<cv::Point2f>& vpUV, vector<int>& hsv);

	//��Բ�ļ���hsv, ����Ϊgroup��
	void identify_cir_hsv(Mat &image, vector<cv::Point2f>& vpUV, vector<int>& hsv, vector<int>& id , int group=2);

	//�����
	void salt(Mat &image, int n);

	//����ƥ��
	bool matchEllipse(vector<Point2f>& cir_cev_l, vector<Point2f>& cir_cev_r, vector<Point2f>& cir_match_cev_l, vector<Point2f>& cir_match_cev_r, Mat& F_2_1);
	bool matchEllipse(mycircle &cir_l, mycircle &cir_r, Mat& F_2_1);

	//��˫Ŀ�������������������
	bool pointTo3D(vector<Point2f>& vec_uvLeft, vector<Point2f>& vec_uvRight, Mat& mLeftIntrinsic, Mat& mRightIntrinsic, Mat& mRightRT, vector<Point3f>& vec_3d);

	bool tack_flow(Mat& pregray, Mat& gray, vector<Point2f>& pre, vector<Point2f>& now, vector<uchar>& status,vector<float>& err);

	~imgpro();
};








