#include"imgpro.h"
#include"mycircle.h"
#include"res_c_r_w.h"

//本程序测试椭圆拟合精度

int main()
{
	imgpro myimgpro;
	mycircle cir;

	//产生白底图片
	Mat img = Mat::Mat(960, 1280, CV_8UC3, Scalar(255, 255, 255));
	//Mat img = imread("7.png");//jpg  png

	//随机设置椭圆位置,长短轴等
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

	//画椭圆
	cir.genarate_ellipse(img, cir.cir_idle_ab);

	myimgpro.salt(img, 100);

	//检测椭圆并画成色
	myimgpro.DecodeMark1(img, cir);
	cir.show_circle(img);
	cir.genarate_ellipse(img, cir.cir_idle_ab_detect, Scalar(0,0,255),3);

    //计算误差
	res_c_r_w res;
	vector<float>err;
	res.comput_err2d_random_order(cir.cir_idle, cir.cir_vec, err);

	//保存数据
	imwrite("res.png", img);

	//记录理论位置
	string outfile_idle = "idle_position.txt";
	res.txt_record(outfile_idle, cir.cir_idle);

	//记录检测位置
	string outfile_detect = "detect_position.txt";
	res.txt_record(outfile_detect, cir.cir_vec);

	//记录误差
	string outfile_err = "err.txt";
	res.txt_record(outfile_err, err);

	for (int i = 0; i < err.size(); i++)
		cout << err[i]<<" ";

	//思路： 拟合误差统计，pixel，matlab可视化，圆，椭圆

	imshow("res",img);
	waitKey(0);
	return 0;
}
