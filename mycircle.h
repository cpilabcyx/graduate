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

	vector<int> cir_hsv;//����hsv
	vector<int> cir_id;//����id
	vector<RotatedRect> cir_idle_ab_detect;//�����Բ

public:
	mycircle();

	//����⵽Բ�� circle(img, cir_vec[i], 3, Scalar(255, 255, 255), CV_FILLED);
	Mat& show_circle(Mat& img);

	//����⵽Բ˳��/id  sprintf(s, "%d", cir_id[i]);
	Mat& show_circle_id(Mat& img);

	//����Բ,Ĭ����� ��ɫ thickness=-1
	Mat& genarate_ellipse(Mat& img, vector<RotatedRect> &center_ab_size, Scalar corlor = Scalar(0,0,0), int thickness=-1);

	//���黭��Բ  Ĭ����� ��ɫ thickness=-1
	Mat& genarate_ellipse_maiker(Mat& img, vector<RotatedRect> &center_ab_size, int group, int thickness = -1);

	//��Բ
	Mat& genarate_cir(Mat& img, vector<Point2f> center_idle, int radius);

	~mycircle();
};

