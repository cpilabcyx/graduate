#include"imgpro.h"
//�ó������ڲ��Է���

int main()
{
	imgpro myimgpro;//ͼ������
	mycircle cir;//��־����

	Mat img = imread("1.png");
	imshow("yuantu", img);
	waitKey(1);

	myimgpro.DecodeMark1(img, cir);

	myimgpro.identify_cir_hsv(img, cir.cir_vec, cir.cir_hsv,  cir.cir_id, 3);
	cir.show_circle_id(img);

	cir.show_circle(img);

	imshow("res", img);
	waitKey(0);

	//˼·�����ձ仯ʱ�㷨��³���� ��ʶ����ȷ��ͳ�ƣ��Ľ�������5 X 5 ͳ��ƽ��hsv���Ż�

	return 0;
}