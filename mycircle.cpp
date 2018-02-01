#include "mycircle.h"

mycircle::mycircle()
{

}

//����⵽Բ�� circle(img, cir_vec[i], 3, Scalar(255, 255, 255), CV_FILLED);
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
		//cvPutText(&tstim, s, cir_vec[i], &font, CV_RGB(0, 0, 0));
		circle(img, cir_vec[i], 3, Scalar(255, 255, 255), CV_FILLED);//
	}
	return img;
}

//����⵽Բ˳��/id or (hsvû�з���)  sprintf(s, "%d", cir_id[i]);
Mat& mycircle::show_circle_id(Mat& img)
{
	if (!img.data) 
	{
		cout << "no data 1232\n";
		return img;
	}

	//û�з��ֱ࣬����ʾ hsv ��ֵ
	if (cir_id.size() != cir_hsv.size())
	{
		cir_id = cir_hsv;
	}

	//�����ܲ��ȣ�����û����
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

//����Բ,Ĭ�Ϻ�ɫcorlor��0,0,0����Ĭ�����thickness=-1
Mat& mycircle::genarate_ellipse(Mat& img, vector<RotatedRect> &center_ab_size, Scalar corlor, int thickness)
{
	int size = center_ab_size.size();
	for (size_t i = 0; i < size; i++)
	{
		//int thickness = -1;
		int lineType = 8;
		ellipse(img,
			Point(center_ab_size[i].center.x, center_ab_size[i].center.y),
			Size(center_ab_size[i].size.width / 2, center_ab_size[i].size.height / 2),
			center_ab_size[i].angle,
			0,
			360,
			corlor,
			thickness,
			lineType,
			0);
	}
	return img;
}

//ע�����group
Mat& mycircle::genarate_ellipse_maiker(Mat& img, vector<RotatedRect> &center_ab_size, int group, int thickness)
{
	Scalar corlor;
	int size = center_ab_size.size();
	for (size_t i = 0; i < size; i++)
	{
		corlor[0] = 180 / group * (rand() % group);
		corlor[1] = 255;
		corlor[2] = 255;

		//int thickness = -1;
		int lineType = 8;

		//�Ȼ����Բ
		ellipse(img,
			Point(center_ab_size[i].center.x, center_ab_size[i].center.y),
			Size(center_ab_size[i].size.width / 2, center_ab_size[i].size.height / 2),
			center_ab_size[i].angle,
			0,
			360,
			Scalar(0, 0, 0),
			thickness,
			lineType,
			0);

		//�ٻ�ͬ��Բ
		ellipse(img,
			Point(center_ab_size[i].center.x, center_ab_size[i].center.y),
			Size(center_ab_size[i].size.width / 4, center_ab_size[i].size.height / 4),
			center_ab_size[i].angle,
			0,
			360,
			corlor,
			thickness,
			lineType,
			0);
	}
	return img;
}

//��Բ
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
