#include "mycircle.h"

mycircle::mycircle()
{

}

//»­¼ì²âµ½Ô²ÐÄ
Mat& mycircle::show_circle(Mat& img)
{
	if (!img.data)
	{
		cout << "no data 1232";
		return img;
	}
	IplImage tstim = img;
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN | CV_FONT_ITALIC, 2.0, 2.0, 1, 1);
	for (size_t i = 0; i < cir_vec.size(); i++)
	{
		char s[100];
		sprintf(s, "%d", i);
		cvPutText(&tstim, s, cir_vec[i], &font, CV_RGB(0, 0, 0));
		//circle(img, cir_vec[i], 3, Scalar(255, 255, 255), CV_FILLED);//
	}
	return img;
}

//»­¼ì²âµ½Ô²Ë³Ðò
Mat& mycircle::show_circle_id(Mat& img)
{
	if (!img.data) 
	{
		cout << "no data 1232\n";
		return img;
	}
	if (cir_id.size() != cir_vec.size())
	{
		cout << "cir_vec.size!=cir_id.size\n";
		return img;
	}
	IplImage tstim = img;
	CvFont font;
	cvInitFont(&font, CV_FONT_HERSHEY_PLAIN | CV_FONT_ITALIC, 2.0, 2.0, 1, 1);
	for (size_t i = 0; i < cir_vec.size(); i++)
	{
		char s[100];
		sprintf(s, "%d", cir_id[i]);
		cvPutText(&tstim, s, cir_vec[i], &font, CV_RGB(0, 0, 0));
	}
	return img;
}

//»­ÍÖÔ²
Mat& mycircle::genarate_ellipse(Mat& img, vector<RotatedRect> &center_ab_size, Scalar corlor)
{
	int size = center_ab_size.size();
	for (size_t i = 0; i < size; i++)
	{
		int thickness = 15;
		int lineType = 8;
		ellipse(img,
			Point(center_ab_size[i].center.x, center_ab_size[i].center.y),
			Size(center_ab_size[i].size.width / 2, center_ab_size[i].size.height / 2),
			center_ab_size[i].angle,
			0,
			360,
			corlor,
			thickness,
			lineType);
	}
	return img;
}

//»­Ô²
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
