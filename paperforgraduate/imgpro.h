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
	bool DecodeMark1(Mat &image, std::vector<cv::Point2f>& vpUV);

	//��Բ���,��¼������
	bool DecodeMark1(Mat &image, mycircle &cir);

	//��Բ����hsv
	void identify_cir(Mat &image, std::vector<cv::Point2f>& vpUV, std::vector<int>& id);

	//�����
	void salt(Mat &image, int n);

	~imgpro();
};








