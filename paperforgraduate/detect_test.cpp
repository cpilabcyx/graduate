#include"imgpro.h"
#include"mycircle.h"
#include"res_c_r_w.h"

//�����������Բ��Ͼ���

int main()
{
	imgpro myimgpro;
	mycircle cir;

	//�����׵�ͼƬ
	Mat img = Mat::Mat(960, 1280, CV_8UC3, Scalar(255, 255, 255));
	//Mat img = imread("7.png");//jpg  png

	//���������Բλ��,�������
	for (int i = 100; i < 1180; i += 200)
	{
		for (int j = 100; j < 860; j += 200)
		{
			RotatedRect temp;
			temp.center.x = i - 10 + rand() % 20;
			temp.center.y = j - 10 + rand() % 20;

			temp.size.height = 60 + rand() % 80;
			//temp.size.width = temp.size.height;
			temp.size.width = 60 + rand() % 80;

			temp.angle = rand() % 360;

			cir.cir_idle_ab.push_back(temp);
			cir.cir_idle.push_back(temp.center);
		}
	}

	//����Բ
	cir.genarate_ellipse(img, cir.cir_idle_ab);

	myimgpro.salt(img, 100);

	//�����Բ������ɫ
	myimgpro.DecodeMark1(img, cir);
	cir.show_circle(img);
	cir.genarate_ellipse(img, cir.cir_idle_ab_detect, Scalar(0,0,255),3);

    //�������
	res_c_r_w res;
	vector<float>err;
	res.comput_err2d_random_order(cir.cir_idle, cir.cir_vec, err);

	//��������
	imwrite("res.png", img);

	//��¼����λ��
	string outfile_idle = "idle_position.txt";
	res.txt_record(outfile_idle, cir.cir_idle);

	//��¼���λ��
	string outfile_detect = "detect_position.txt";
	res.txt_record(outfile_detect, cir.cir_vec);

	//��¼���
	string outfile_err = "err.txt";
	res.txt_record(outfile_err, err);

	for (int i = 0; i < err.size(); i++)
		cout << err[i]<<" ";

	//˼·�� ������ͳ�ƣ�pixel��matlab���ӻ���Բ����Բ

	imshow("res",img);
	waitKey(0);
	return 0;
}
