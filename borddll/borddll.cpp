#include <opencv2\opencv.hpp>
#include <iostream>
#include <Windows.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>
#include <deque>
#include <cmath>
#define _DLLExport __declspec(dllexport)  


//还没有将相机关闭集成

using namespace cv;
using namespace std;

Mat frame;
int bord_w, bord_h, talal_cornor, squareSize;
long long time;
Size bord_size;
Mat rvecs, tvecs;
VideoCapture camera0;
Mat cameraMatrix, distCoeffs;

int sign(float x)
{
	return x >= 0 ? 1 : -1;
}
float myMax(float x, float y)
{
	return x >y ? x : y;

}

//计算四元数
void QuaternionFromMatrix(const Mat& R, float quat[])
{
	// Adapted from: http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/index.htm
	/*quat[0] = (float)sqrt(myMax(0.0, 1 + R.ptr<float>(0)[0] + R.ptr<float>(1)[1] + R.ptr<float>(2)[2]))/2;

	quat[1] = (float)sqrt(myMax(0.0, 1 + R.ptr<float>(0)[0] - R.ptr<float>(1)[1] - R.ptr<float>(2)[2]))/2;
	quat[2] = (float)sqrt(myMax(0.0, 1 - R.ptr<float>(0)[0] + R.ptr<float>(1)[1] - R.ptr<float>(2)[2]))/2;
	quat[3] = (float)sqrt(myMax(0.0, 1 - R.ptr<float>(0)[0] - R.ptr<float>(1)[1] + R.ptr<float>(2)[2]))/2;

	quat[1] *= sign(R.ptr<float>(2)[1] - R.ptr<float>(1)[2]);
	quat[2] *= sign(R.ptr<float>(0)[2] - R.ptr<float>(2)[0]);
	quat[3] *= sign(R.ptr<float>(1)[0] - R.ptr<float>(0)[1]);*/
	quat[0] = (float)sqrt(myMax(0.0, 1 + R.at<float>(0, 0) + R.at<float>(1, 1) + R.at<float>(2, 2))) / 2;

	quat[1] = (float)sqrt(myMax(0.0, 1 + R.at<float>(0, 0) - R.at<float>(1, 1) - R.at<float>(2, 2))) / 2;
	quat[2] = (float)sqrt(myMax(0.0, 1 - R.at<float>(0, 0) + R.at<float>(1, 1) - R.at<float>(2, 2))) / 2;
	quat[3] = (float)sqrt(myMax(0.0, 1 - R.at<float>(0, 0) - R.at<float>(1, 1) + R.at<float>(2, 2))) / 2;

	quat[1] *= sign(R.at<float>(2, 1) - R.at<float>(1, 2));
	quat[2] *= sign(R.at<float>(0, 2) - R.at<float>(2, 0));
	quat[3] *= sign(R.at<float>(1, 0) - R.at<float>(0, 1));
}

Point2f pixel2cam(const Point2d& p, const Mat& K)
{
	return Point2f
		(
		(p.x - K.at<double>(0, 2)) / K.at<double>(0, 0),
		(p.y - K.at<double>(1, 2)) / K.at<double>(1, 1)
		);
}


//左右手坐标系转化,计算四元数
void CcalinitEctrinsicMat(float* m_fExtrinsic, Mat rr, Mat tt)
{
	if (false)//相机动
	{
		rr.convertTo(rr, CV_32F);
		tt.convertTo(tt, CV_32F);
		Mat R = Mat(3, 3, CV_32F), T = Mat(3, 1, CV_32F);
		T = tt;
		R = rr;
		////mrightEX = mrightEX.inv();
		//for (size_t i = 0; i < 3; i++)
		//{
		//	for (size_t j = 0; j < 3; j++)
		//	{
		//		R.at<float>(i, j) = mrightEX.at<float>(i, j);
		//		//T.at<float>(i, 0) = mrightEX.at<float>(i, 3);
		//	}
		//	T.at<float>(i, 0) = mrightEX.at<float>(i, 3);
		//}
		////左右手坐标系转化

		Mat R_CPP, R_inv, T_CPP;
		R_CPP = R;
		R_inv = R_CPP.inv();
		T_CPP = T;
		QuaternionFromMatrix(R_inv, m_fExtrinsic);//利用旋转矩阵求四元数，相机标定是世界坐标系在相机坐标系的表示，故取逆

		// We need to invert rotations on X and Z axis
		m_fExtrinsic[1] = -m_fExtrinsic[1];
		m_fExtrinsic[3] = -m_fExtrinsic[3];
		Mat Tt = -R_inv * T_CPP;   //相机标定求得世界坐标系原点在相机坐标系下位置，先将位置矢量表示为世界坐标系，再取反可求得相机在世界坐标系下位置
		m_fExtrinsic[4] = (float)Tt.at<float>(0);
		m_fExtrinsic[5] = -(float)Tt.at<float>(1);
		m_fExtrinsic[6] = (float)Tt.at<float>(2);
	}
	if (true)//相机不动
	{
		rr.convertTo(rr, CV_32F);
		tt.convertTo(tt, CV_32F);
		Mat R = Mat(3, 3, CV_32F), T = Mat(3, 1, CV_32F);
		T = tt;
		R = rr;
		////mrightEX = mrightEX.inv();
		//for (size_t i = 0; i < 3; i++)
		//{
		//	for (size_t j = 0; j < 3; j++)
		//	{
		//		R.at<float>(i, j) = mrightEX.at<float>(i, j);
		//		//T.at<float>(i, 0) = mrightEX.at<float>(i, 3);
		//	}
		//	T.at<float>(i, 0) = mrightEX.at<float>(i, 3);
		//}
		////左右手坐标系转化

		Mat R_CPP, T_CPP;
		R_CPP = R;
		//R_inv = R_CPP.inv();
		T_CPP = T;
		QuaternionFromMatrix(R_CPP, m_fExtrinsic);

		// We need to invert rotations on X and Z axis
		m_fExtrinsic[1] = -m_fExtrinsic[1];
		m_fExtrinsic[3] = -m_fExtrinsic[3];
		//Mat Tt = -R_inv * T_CPP;
		m_fExtrinsic[4] = (float)T_CPP.at<float>(0);
		m_fExtrinsic[5] = -(float)T_CPP.at<float>(1);
		m_fExtrinsic[6] = (float)T_CPP.at<float>(2);
	}
}

int init(int w = 9, int h = 6, int s = 25)
{
	bord_w = w;
	bord_h = h;
	squareSize = s;
	talal_cornor = w*h;
	bord_size = Size(w, h);
	time = 0;

	FileStorage ff("camera.yml", FileStorage::READ);
	if (ff.isOpened())
	{
		ff["camera_matrix"] >> cameraMatrix;
		ff["distortion_coefficients"] >> distCoeffs;
		ff.release();
	}
	else
	{
		return -1;
	}
	return 0;
}

extern "C" _DLLExport
int opencam()
{
	int ret = 0;
	ret = init(9, 6, 2.5);
	//打开相机
	camera0.open(0);
	camera0.set(CV_CAP_PROP_FRAME_WIDTH, 640);
	camera0.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

	while (true)
	{
		if (camera0.isOpened()) break;
		else camera0.open(0);
	}
	return ret;
}

extern "C" _DLLExport
bool computperframe(float* m_fExtrinsic, void* text)
{
	bool found = false;
	vector<Point2f>corners;
	vector<vector<Point2f> >corners_t;

	//grab and retrieve each frames of the video sequentially 
	camera0 >> frame;
	/*imshow("frame", frame);
	waitKey(1);*/
	
	
	//find cornor
	found = findChessboardCorners(
		frame,
		bord_size,
		corners
		);

	//conver to gray
	Mat gray;
	cvtColor(frame, gray, CV_BGR2GRAY);
	if (found)
	{
		cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
		drawChessboardCorners(frame, bord_size, corners, found);
		/*imshow("frame__cc", frame);
		waitKey(1);*/

		vector<Point3f>object_points;

		for (int i = 0; i < bord_h; i++)
			for (int j = 0; j < bord_w; j++)
				object_points.push_back(Point3f(float(j*squareSize), float(i*squareSize), 0));

		Mat r;


		/*vector<vector<Point3f> > objectPoints = { object_points };
		corners_t = { corners };*/

		//double rms = calibrateCamera(objectPoints, corners_t, frame.size(), cameraMatrix, distCoeffs, rvecs, tvecs);
		cv::solvePnP(object_points, corners, cameraMatrix, distCoeffs, rvecs, tvecs);
		//cvFindExtrinsicCameraParams2();
		//! converts rotation vector to rotation matrix or vice versa using Rodrigues transformation
		//CV_EXPORTS_W void Rodrigues(InputArray src, OutputArray dst, OutputArray jacobian = noArray());
		Rodrigues(rvecs, r);
		CcalinitEctrinsicMat(m_fExtrinsic,r, tvecs);
	}
	flip(frame, frame, 1);
	cvtColor(frame, frame, CV_BGR2RGB);
	memcpy(text, frame.data, frame.cols*frame.rows*frame.channels()*sizeof(uchar));
	return found;
}

extern "C" _DLLExport
bool computbyorb(float* m_fExtrinsic, void* text)
{
	bool found = false;
	vector<Point2f>corners;
	vector<vector<Point2f> >corners_t;

	//grab and retrieve each frames of the video sequentially 
	camera0 >> frame;
	/*imshow("frame", frame);
	waitKey(1);*/


	//find cornor
	found = findChessboardCorners(
		frame,
		bord_size,
		corners
		);

	//conver to gray
	Mat gray;
	cvtColor(frame, gray, CV_BGR2GRAY);
	if (found)
	{
		cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1), cvTermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
		drawChessboardCorners(frame, bord_size, corners, found);
		/*imshow("frame__cc", frame);
		waitKey(1);*/

		vector<Point3f>object_points;

		for (int i = 0; i < bord_h; i++)
			for (int j = 0; j < bord_w; j++)
				object_points.push_back(Point3f(float(j*squareSize), float(i*squareSize), 0));

		Mat r;


		/*vector<vector<Point3f> > objectPoints = { object_points };
		corners_t = { corners };*/

		//double rms = calibrateCamera(objectPoints, corners_t, frame.size(), cameraMatrix, distCoeffs, rvecs, tvecs);
		cv::solvePnP(object_points, corners, cameraMatrix, distCoeffs, rvecs, tvecs);
		//cvFindExtrinsicCameraParams2();
		//! converts rotation vector to rotation matrix or vice versa using Rodrigues transformation
		//CV_EXPORTS_W void Rodrigues(InputArray src, OutputArray dst, OutputArray jacobian = noArray());
		Rodrigues(rvecs, r);
		CcalinitEctrinsicMat(m_fExtrinsic, r, tvecs);
	}
	flip(frame, frame, 1);
	cvtColor(frame, frame, CV_BGR2RGB);
	memcpy(text, frame.data, frame.cols*frame.rows*frame.channels()*sizeof(uchar));
	return found;
	return true;
}

extern "C" _DLLExport
bool quit()
{
//
	if (camera0.isOpened())
	{
		camera0.release();
		return true;
	}
	return false;
}