#include"imgpro.h"
//该程序用于测试分类

int main()
{
	imgpro myimgpro;//图像处理类
	mycircle cir;//标志点类

	Mat img = imread("1.png");
	imshow("yuantu", img);
	waitKey(1);

	myimgpro.DecodeMark1(img, cir);

	myimgpro.identify_cir_hsv(img, cir.cir_vec, cir.cir_hsv,  cir.cir_id, 3);
	cir.show_circle_id(img);

	cir.show_circle(img);

	imshow("res", img);
	waitKey(0);

	//思路，光照变化时算法的鲁棒性 即识别正确率统计，改进，中心5 X 5 统计平均hsv需优化

	return 0;
}