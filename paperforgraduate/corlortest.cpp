#include"imgpro.h"
#include"mycircle.h"

int main()
{
	imgpro myimgpro;
	mycircle cir;

	//�����׵�ͼƬ
	Mat img = Mat::Mat(960, 1280, CV_8UC3, Scalar(255, 255, 255));
	//Mat img = imread("6.png");//jpg  png

	//������Բλ��
	for (int i = 100; i < 1180; i += 200)
	{
		for (int j = 100; j < 860; j += 200)
		{
			RotatedRect temp;
			temp.center.x = i - 10 + rand() % 20;
			temp.center.y = j - 10 + rand() % 20;

			temp.size.height = 80 + rand() % 40;
			temp.size.width  = 80 + rand() % 40;

			temp.angle = rand() % 360;

			cir.cir_idle_ab.push_back(temp);
		}
	}

	//����Բ
	cir.genarate_ellipse(img, cir.cir_idle_ab);

	//�����Բ������ɫ
	myimgpro.DecodeMark1(img, cir);
	cir.genarate_ellipse(img, cir.cir_idle_ab_detect, Scalar(0,0,255));

    //�������


	//bool myimgpro::DecodeMark1(Mat &image, std::vector<cv::Point2f>& vpUV)
	//myimgpro.DecodeMark1(img, cir);

	//myimgpro.identify_cir(img, cir.cir_vec, cir.cir_id);


	//cir.show_circle(img);
	//cir.show_circle_id(img);

	imshow("res",img);
	waitKey(0);
	return 0;
}
