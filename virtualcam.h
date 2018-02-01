#include <opencv2\opencv.hpp>

using namespace cv;

class virtualcam
{
public:
	int wide_frame, hight_frame;
	int which_cam;

public:
	virtualcam(){};
	 //打开相机，单目，双目
	virtual bool open_cam(int which_cam =0)=0;
	 //抓取一帧
	virtual bool grab_frame()=0;
	 //关闭相机
	virtual bool close_cam()=0;

	 ~virtualcam(){};
};

class cam_monocular :public virtualcam
{
private:
	VideoCapture cam_cv_mono;

public:
	Mat frame;

public:
	//初始化，设置相机参数
	cam_monocular(int wideth = 640, int highth = 480);

	//打开单目
	virtual bool open_cam(int which_cam = 0);

	//抓取一帧
	virtual bool grab_frame();

	//关闭相机
	virtual bool close_cam();

	~cam_monocular();
};

class cam_stereo :public virtualcam
{
private:
	VideoCapture cam_cv_stereo;

public:
	Mat frame_l, frame_r, raw;

public:
	//初始化，设置相机参数,默认宽高 int wideth=640, int highth=480
	cam_stereo(int wideth = 640, int highth = 480);

	//int which_cam = 0
	virtual bool open_cam(int which_cam = 0);

	//抓取一帧
	virtual bool grab_frame();

	//关闭相机
	virtual bool close_cam();

	//可视化一帧，左右图像
	bool show_stereo_fram();

	~cam_stereo();
};