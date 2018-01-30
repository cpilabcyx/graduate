#include<opencv.hpp>
#include<cmath>  
#include"mycircle.h"
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

//�����
void salt(Mat image, int n)
{
	int i, j;
	for (int k = 0; k<n; k++)
	{
		// rand()�������������  
		i = rand() % image.cols;
		j = rand() % image.rows;
		if (image.type() == CV_8UC1)
		{ // �Ҷ�ͼ��  
			image.at<uchar>(j, i) = 255;
		}
		else if (image.type() == CV_8UC3)
		{ // ��ɫͼ��  
			image.at<cv::Vec3b>(j, i)[0] = 255;
			image.at<cv::Vec3b>(j, i)[1] = 255;
			image.at<cv::Vec3b>(j, i)[2] = 255;
		}
	}
}

//2d��ŷ�Ͼ���
float dis_comput_point(Point2f a, Point2f b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

//2d��������С����
int comput_err2d_random_order(const vector<Point2f> detected, const vector<Point2f>idle, vector<float>err)
{
	int num_match=0;
	int seze_idle = idle.size();
	int seze_detected = detected.size();
	for (int i = 0; i < seze_detected; i++)
	{
		float min = 9999;
		for (int j = 0; j < seze_idle; j++)
		{
			float dis=dis_comput_point(detected[i], idle[j]);//2d��ŷ�Ͼ���
			if (dis < min)min = dis;//������С��Ϊ���
		}
		err.push_back(min);
		num_match++;
	}
	return num_match;
}

int main()
{
	Mat a = imread("1.jpg");

	mycircle cir;
	cir.cir_idle.push_back({ 100, 100 });
	cir.cir_idle.push_back({ 200, 300 });

	//cir.genarate_ellipse(a, cir.cir_idle, 45);
	cir.genarate_cir(a, cir.cir_idle, 45);

	salt(a, 5000);
	
	Mat _blur;
	blur(a, _blur, Size(2, 2), Point(-1, -1));
	imshow("_blur", _blur);

	//Mat gass;

	//GaussianBlur(a, gass, Size(10, 10), 2, 2);
	//imshow("gass", gass);
	//
	
	/*Mat media;
	medianBlur(a, media, 10);
	imshow("media", media);*/

	imshow("1.jpg", a);
	waitKey();
	return 0;
}