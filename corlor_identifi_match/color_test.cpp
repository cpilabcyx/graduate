#include"imgpro.h"
//�ó������ڲ��Է���

int main()
{
	imgpro myimgpro;//ͼ������
	mycircle cir;//��־����

	Mat img = imread("7.png");
	imshow("yuantu", img);
	waitKey(1);

	myimgpro.DecodeMark1(img, cir);

	myimgpro.identify_cir_hsv(img, cir.cir_vec, cir.cir_hsv,  cir.cir_id, 3);
	cir.show_circle_id(img);

	cir.show_circle(img);

	imshow("res", img);
	waitKey(0);
	//˫Ŀ�궨f

	//��־��ƥ��

	//����

	return 0;
}