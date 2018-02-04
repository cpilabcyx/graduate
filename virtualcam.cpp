#include"virtualcam.h"

//初始化，设置相机参数int wideth = 640, int highth = 480
cam_monocular::cam_monocular(int wideth, int highth)
{
	hight_frame = highth;//默认640 X 480
	wide_frame = wideth;
}

//打开单目
bool cam_monocular::open_cam(int which_cam)
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

	cam_cv_mono.set(CV_CAP_PROP_FRAME_WIDTH, wide_frame);
	cam_cv_mono.set(CV_CAP_PROP_FRAME_HEIGHT, hight_frame);

	return res;
}

//抓取一帧
bool cam_monocular::grab_frame()
{
	cam_cv_mono >> frame;
	return frame.data;
}

//关闭相机
bool cam_monocular::close_cam()
{
	return false;
}

cam_monocular::~cam_monocular()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


//初始化，设置相机参数,默认宽高 int wideth=640, int highth=480
cam_stereo::cam_stereo(int wideth, int highth)
{
	hight_frame = highth;//默认640 X 480
	wide_frame = 2*wideth;
}

bool cam_stereo::open_cam(int which_cam)
{
	int faile = 0;
	//打开相机
	bool res = cam_cv_stereo.open(which_cam);
	while (faile<5 && (res == false))
	{
		if (cam_cv_stereo.isOpened())
		{
			res = true;
			break;
		}
		else cam_cv_stereo.open(which_cam);
		faile++;
		waitKey(100);
	}
	cam_cv_stereo.set(CV_CAP_PROP_FRAME_WIDTH, wide_frame);
	cam_cv_stereo.set(CV_CAP_PROP_FRAME_HEIGHT, hight_frame);

	return res;
}

//抓取一帧
bool cam_stereo::grab_frame()
{
	cam_cv_stereo >> raw;

	frame_l = raw(cv::Rect(0, 0, raw.cols / 2, raw.rows));
	frame_r = raw(cv::Rect(raw.cols / 2, 0, raw.cols / 2, raw.rows));

	return (frame_l.data&&frame_r.data);
}

//关闭相机
bool cam_stereo::close_cam()
{
	return false;
}

//可视化一帧，左右图像
bool cam_stereo::show_stereo_fram()
{
	if (frame_l.data&&frame_r.data)
	{
		imshow("frame_l", frame_l);
		imshow("frame_r", frame_r);
		waitKey(1);
	}
	return (frame_l.data&&frame_r.data);
}

cam_stereo::~cam_stereo()
{

}

