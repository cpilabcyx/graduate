#include "mycircle.h"

mycircle::mycircle()
{
}

//void DrawEllipse(Mat img, double angle)
//{
//	int thickness = 2;
//	int lineType = 8;
//	ellipse(img,
//		Point(WINDOW_SIZE / 2.0, WINDOW_SIZE / 2.0),
//		Size(WINDOW_SIZE / 4.0, WINDOW_SIZE / 16.0),
//		angle,
//		0,
//		360,
//		Scalar(255, 0, 255),
//		thickness,
//		lineType);
//}

//ª≠Õ÷‘≤
Mat& mycircle::genarate_ellipse(Mat& img, vector<Point2f> center_idle, float angle)
{
	int size = center_idle.size();
	for (size_t i = 0; i < size; i++)
	{
		int thickness = 2;
		int lineType = 8;
		ellipse(img,
			Point(center_idle[i].x, center_idle[i].y),
			Size(20, 80),
			angle,
			0,
			360,
			Scalar(255, 0, 255),
			thickness,
			lineType);
	}
	return img;
}

//ª≠‘≤
Mat& mycircle::genarate_cir(Mat& img, vector<Point2f> center_idle, int radius)
{
	int size = center_idle.size();
	for (size_t i = 0; i < size; i++)
	{
		int thickness = 2;
		int lineType = 8;
		circle(img,
			Point(center_idle[i].x, center_idle[i].y),
			radius,
			Scalar(255, 0, 255),
			thickness,
			lineType);
	}
	return img;
}

mycircle::~mycircle()
{
}
