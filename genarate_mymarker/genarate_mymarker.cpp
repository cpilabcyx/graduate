#include"mycircle.h"
#include"imgpro.h"

//http://blog.csdn.net/zdyueguanyun/article/details/50739374   各个颜色在hsv颜色区间分类  h[0 180] s[0 255] v[0 255]

int main()
{
	imgpro myimgpro;
	mycircle cir;

	//输入分组
	int group;
	cout << "please input num of groups\n";
	cin >> group;

	//产生白底图片
	Mat img = Mat::Mat(960, 1280, CV_8UC3, Scalar(255, 255, 255));

	//随机设置椭圆位置,长短轴等
	for (int i = 100; i < 1180; i += 200)
	{
		for (int j = 100; j < 860; j += 200)
		{
			RotatedRect temp;
			temp.center.x = i - 10 + rand() % 20;
			temp.center.y = j - 10 + rand() % 20;

			temp.size.height = 60 + rand() % 80;
			temp.size.width = temp.size.height;
			//temp.size.width = 60 + rand() % 80;

			temp.angle = rand() % 360;

			cir.cir_idle_ab.push_back(temp);
			cir.cir_idle.push_back(temp.center);
		}
	}
	cvtColor(img, img, CV_BGR2HSV);
	cir.genarate_ellipse_maiker(img, cir.cir_idle_ab, group);
	cvtColor(img, img, COLOR_HSV2BGR); //CV_BGR2RGB
	imwrite("marker.png", img);

	myimgpro.DecodeMark1(img, cir);
	myimgpro.identify_cir_hsv(img, cir.cir_vec,cir.cir_id);
	cir.show_circle_id(img);
	cir.show_circle(img);//画圆心

	cir.genarate_ellipse(img, cir.cir_idle_ab_detect, Scalar(120, 120, 255),3);

	imshow("res", img);
	//记录图像，显示理论hsv值
	imwrite("marker_hsv.png", img);

	waitKey(0);

}
