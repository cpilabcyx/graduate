#include"imgpro.h"
//该程序用于测试分类

int main()
{
	imgpro myimgpro;//图像处理类
	mycircle cir;//标志点类

	Mat img = imread("7.png");
	imshow("yuantu", img);
	waitKey(1);

	myimgpro.DecodeMark1(img, cir);

	myimgpro.identify_cir_hsv(img, cir.cir_vec, cir.cir_hsv,  cir.cir_id, 3);
	cir.show_circle_id(img);

	cir.show_circle(img);

	imshow("res", img);
	waitKey(0);
	//双目标定f

	//标志点匹配

	//测量

	return 0;
}