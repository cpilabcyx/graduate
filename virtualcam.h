#include <opencv2\opencv.hpp>

using namespace cv;

class virtualcam
{
public:
	int wide_frame, hight_frame;
	int which_cam;

public:
	virtualcam(){};
	 //���������Ŀ��˫Ŀ
	virtual bool open_cam(int which_cam =0)=0;
	 //ץȡһ֡
	virtual bool grab_frame()=0;
	 //�ر����
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
	//��ʼ���������������
	cam_monocular(int wideth = 640, int highth = 480);

	//�򿪵�Ŀ
	virtual bool open_cam(int which_cam = 0);

	//ץȡһ֡
	virtual bool grab_frame();

	//�ر����
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
	//��ʼ���������������,Ĭ�Ͽ�� int wideth=640, int highth=480
	cam_stereo(int wideth = 640, int highth = 480);

	//int which_cam = 0
	virtual bool open_cam(int which_cam = 0);

	//ץȡһ֡
	virtual bool grab_frame();

	//�ر����
	virtual bool close_cam();

	//���ӻ�һ֡������ͼ��
	bool show_stereo_fram();

	~cam_stereo();
};