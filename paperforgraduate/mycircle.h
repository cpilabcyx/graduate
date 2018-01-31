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

	vector<Point2f> cir_idle;//����Բ��
	vector<int> cir_id_idle;//����id
	vector<RotatedRect> cir_idle_ab;//����Բ��

	vector<Point2f> cir_vec;//��⵽Բ��
	vector<int> cir_id;//����id
	vector<RotatedRect> cir_idle_ab_detect;//�����Բ

public:
	mycircle();

	//����Բ��ʾ��ͼ����
	Mat& show_circle(Mat& img);

	//����Բid��ʾ��ͼ����
	Mat& show_circle_id(Mat& img);

	//����Բ
	Mat& genarate_ellipse(Mat& img, vector<RotatedRect> &center_ab_size, Scalar corlor = Scalar(0,0,0));

	//��Բ
	Mat& genarate_cir(Mat& img, vector<Point2f> center_idle, int radius);

	~mycircle();
};

