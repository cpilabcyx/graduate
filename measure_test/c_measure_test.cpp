#include"imgpro.h"
#include"res_c_r_w.h"
#include<ctime>
//#include"virtualcam.hpp"

//初始化相机
bool set_cam_w_h(VideoCapture &cam, int mono_or_stereo = 1, int wideth = 640, int highth = 480)
{
	wideth *= mono_or_stereo;
	cam.set(CV_CAP_PROP_FRAME_WIDTH, wideth);
	cam.set(CV_CAP_PROP_FRAME_HEIGHT, highth);
	return true;
}

//打开五次
bool open_cam(VideoCapture &cam_cv_mono, int which_cam = 0)
{
	int faile = 0;

	//打开相机
	bool res = cam_cv_mono.open(which_cam);
	while (faile<5 && (res == false))
	{
		if (cam_cv_mono.isOpened())
		{
			res = true;
			break;
		}
		else cam_cv_mono.open(which_cam);
		faile++;
		waitKey(100);
	}
	return res;
}

//抓取一帧
bool grab_frame(VideoCapture &cam_cv_stereo, Mat &frame_l, Mat &frame_r)
{
	Mat raw;
	cam_cv_stereo >> raw;

	frame_l = raw(cv::Rect(0, 0, raw.cols / 2, raw.rows));
	frame_r = raw(cv::Rect(raw.cols / 2, 0, raw.cols / 2, raw.rows));

	return (frame_l.data&&frame_r.data);
}

//抓取一帧,可视化
bool frame_and_show(Mat &frame_l, Mat &frame_r)
{
	if (frame_l.data&&frame_r.data)
	{
		imshow("frame_l", frame_l);
		imshow("frame_r", frame_r);
		waitKey(1);
	}
	return (frame_l.data&&frame_r.data);
}

//抓取一帧,可视化
bool grab_frame_and_show(VideoCapture &cam_cv_stereo, Mat &frame_l, Mat &frame_r)
{
	grab_frame(cam_cv_stereo, frame_l, frame_r);
	return frame_and_show(frame_l, frame_r);
}

//双目极线约束匹配，双目测量，精度验证
int main()
{
	//判断是否已经标定，提示加文件查找
	res_c_r_w res;
	Mat R, T, F;
	res.xml_read("extrinsics_1-3.yml", "R", R);
	res.xml_read("extrinsics_1-3.yml", "T", T);
	res.xml_read("extrinsics_1-3.yml", "F", F);
	cout << R << endl << T << endl << F;

	//打开相机
	VideoCapture mystero;

	open_cam(mystero);
	set_cam_w_h(mystero, 2);

	Mat frame_l, frame_r;

	imgpro mypro;
	mycircle cir_l, cir_r;

	clock_t start, finish;

	while (true)
	{
		start = clock();

		grab_frame(mystero, frame_l, frame_r);

		finish = clock();
		cout <<"grab_frame: "<<finish - start << "/" << CLOCKS_PER_SEC << " (s) " << endl;

		start = clock();
		//圆检测

		mypro.DecodeMark1(frame_l, cir_l);
		mypro.DecodeMark1(frame_r, cir_r);
		finish = clock();
		cout << "DecodeMark1: " << finish - start << "/" << CLOCKS_PER_SEC << " (s) " << endl;

		start = clock();

		cir_l.show_circle(frame_l);
		cir_r.show_circle(frame_r);

		frame_and_show(frame_l, frame_r);

		finish = clock();

		cout << "show_circle: " << finish - start << "/" << CLOCKS_PER_SEC << " (s) " << endl;

		//极线匹配


		//双目测量

		//精度验证
	}
	return 0;
}


