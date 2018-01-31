#include"imgpro.h"

double minArea = 30;//400--450之间
double maxArea = 40000;
double minLen = 10;
double maxLen = 20000;

TermCriteria termcrit(CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 30, 0.01);

bool SortByM1(const Point2f &v1, const Point2f &v2)//注意：本函数的参数的类型一定要与vector中元素的类型一致  
{
	return v1.x < v2.x;//升序排列  
}

imgpro::imgpro()
{
}

//椭圆筛选
bool imgpro::isEllipse(const vector<cv::Point2f> &contours, const RotatedRect &canEll, const double maxERR)
{
	bool isEll = true;
	double x0 = canEll.center.x;
	double y0 = canEll.center.y;
	double a = canEll.size.width > canEll.size.height ? canEll.size.width*0.5 : canEll.size.height*0.5;
	double a2 = a * 2;
	double b = canEll.size.width < canEll.size.height ? canEll.size.width*0.5 : canEll.size.height*0.5;//

	double err = 3;

	double angle = canEll.angle;
	double c = sqrt(a*a - b*b);
	Vec2f c1((float)cos(angle)*c + x0, (float)sin(angle)*c + y0);
	Vec2f c2(-(float)cos(angle)*c + x0, -(float)sin(angle)*c + y0);
	///Calculate max and min err
	for (size_t i = 0; i < contours.size(); i++)
	{
		double len1 = sqrt(pow(abs(c1[0] - contours[i].x), 2) + pow(abs(c1[1] - contours[i].y), 2));
		double len2 = sqrt(pow(abs(c2[0] - contours[i].x), 2) + pow(abs(c2[1] - contours[i].y), 2));
		double len3 = sqrt(pow(abs(x0 - contours[i].x), 2) + pow(abs(y0 - contours[i].y), 2));

		double errTmp = fabs(len1 + len2 - a2);
		//////////////////////////////////

		//第一次筛选
		if (errTmp > maxERR*a2)//两个筛选条件
			//if (errTmp >0.1* a2&&fabs(a - b)>err)
		{
			isEll = false;
		}
		//第二次（面积）筛选
		double trueArea = PI*a*b;
		double trueLen = 2 * PI*b + 4 * (a - b);
		/////////////////////////////////
		if (fabs(trueArea - abs(contourArea(contours)))>3)
		{
			isEll = false;
		}
		//第三次（周长）筛选
		//if (trueLen > abs(contourArea(contours)))
		//{
		//	isEll = false;
		//}
	}
	return isEll;
}

//椭圆检测
void imgpro::findEllipses(const vector< vector<cv::Point2f> >& contoursRaw, std::vector< int >& sublabEllipse, std::vector<RotatedRect>& rectEll)
{
	const size_t minContourPoint = 6;
	const double maxErrRate = 0.3;
	int itmps1 = contoursRaw.size();
	for (size_t i = 0; i < contoursRaw.size(); i++)
	{
		if (contoursRaw[i].size() < minContourPoint)
		{
			continue;
		}
		//临时拟合rect信息
		RotatedRect tmpRotatedRect;
		tmpRotatedRect = fitEllipse(contoursRaw[i]);

		///If the contour is a Ellipse 
		if (isEllipse(contoursRaw[i], tmpRotatedRect, maxErrRate))
		{
			sublabEllipse.push_back(i);
			rectEll.push_back(tmpRotatedRect);
			//aid_point.push_back(contoursRaw[i][0]);
		}
		else
		{
		}

	}

	return;
}

//椭圆检测,提取圆心到vpUV
bool imgpro::DecodeMark1(Mat &image, std::vector<cv::Point2f>& vpUV)
{
	bool found = false;
	mycircle temp;

	DecodeMark1(image, temp);
	
	int size=temp.cir_idle_ab_detect.size();
	for (size_t i = 0; i < size; i++)
	{
		vpUV.push_back(temp.cir_idle_ab_detect[i].center);
	}

	return found;
}

//椭圆检测,记录长短轴
bool imgpro::DecodeMark1(Mat &image, mycircle &cir)
{
	bool found = false;
	const double resizeFactor = 1.0;

	//////////////////////////////////////////////////////////////////////////
	//图像处理
	//灰度+8U+边缘  用于轮廓检测
	//image.convertTo(image, CV_8UC1);
	Mat imGary;
	cvtColor(image, imGary, CV_BGR2GRAY);
	//cv::GaussianBlur(imGary, imBlur, Size(5,5), 1, 1);

	Mat imCanny;
	Canny(imGary, imCanny, 30, 50, 3);
	//imshow("图像处理①Canny", imCanny);
	//waitKey(0);

	//////////////////////////////////////////////////////////////////////////
	//检测轮廓饭
	vector<vector<cv::Point2f> > contours;
	vector<vector<cv::Point>> contour1;

	//只检测外轮廓. CV_CHAIN_APPROX_SIMPLE压缩水平方向，垂直方向，对角线方向的元素，只保留该方向的终点坐标
	//CV_CHAIN_APPROX_NONE储存所有点
	//检测完轮廓 imcanny已经改变

	//cv::findContours(imCanny, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	cv::findContours(imCanny, contour1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

	for (size_t i = 0; i < contour1.size(); i++)
	{
		double a = abs(contourArea(contour1[i]));
		double b = abs(arcLength(contour1[i], true));

		if (a >= minArea&&a <= maxArea&&b >= minLen&&b <= maxLen)
		{
			//亚像素角点检测
			vector<Point2f> tmp;
			for (size_t j = 0; j < contour1[i].size(); j++)
			{
				tmp.push_back(contour1[i][j]);
			}
			/*cornerSubPix(imCanny, tmp, Size(5, 5),
			Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));*/
			contours.push_back(tmp);
		}
		else
		{
			continue;
		}

	}

	if (!contours.empty())
	{
		found = true;
		//drawContours(image, contours,)
	}
	else
	{
		found = false;
		return found;
	}
	Mat contoursIm;
	image.copyTo(contoursIm);

	//////////////////////////////////////////////////////////////////////////
	//筛选轮廓
	//轮廓中是通过了椭圆筛选的序号
	std::vector<int > EllipseContour;
	//轮廓拟合rect信息
	//std::vector<RotatedRect> EllipseRect;
	//筛选下椭圆
	cir.cir_idle_ab_detect.clear();
	findEllipses(contours, EllipseContour, cir.cir_idle_ab_detect);

	int size = cir.cir_idle_ab_detect.size();
	for (size_t i = 0; i < size; i++)
	{
		cir.cir_vec.push_back(cir.cir_idle_ab_detect[i].center);
	}
	//////////////////////////////////////////////////////////////////////////
	return found;
}

//椭圆鉴别hsv
void imgpro::identify_cir(Mat &image, std::vector<cv::Point2f>& vpUV, std::vector<int>& id)
{
	if (id.size()>0)id.clear();

	int size = vpUV.size();
	for (size_t i = 0; i < size; i++)
	{
		Mat temm(image, Rect(vpUV[i].x - 2, vpUV[i].y - 2, 5, 5));
		Mat temmp;
		cvtColor(temm, temmp, CV_BGR2HSV);

		/* 注意 Mat::at 函数是个模板函数, 需要指明参数类型, 因为这张图是具有红蓝绿三通道的图,
		所以它的参数类型可以传递一个 Vec3b, 这是一个存放 3 个 uchar 数据的 Vec(向量). 这里
		提供了索引重载, [2]表示的是返回第三个通道, 在这里是 Red 通道, 第一个通道(Blue)用[0]返回 */
		int bgr[3] = { 0, 0, 0 };

		for (int k = 0; k < 5; k++)
			for (int l = 0; l < 5; l++)
			{
				bgr[2] += (int)temmp.at<Vec3b>(k, l)[2];
				bgr[1] += (int)temmp.at<Vec3b>(k, l)[1];
				bgr[0] += (int)temmp.at<Vec3b>(k, l)[0];
			}
		bgr[2] /= 25;
		bgr[1] /= 25;
		bgr[0] /= 25;

		id.push_back(bgr[0]);
	}
}

//加噪点
void imgpro::salt(Mat &image, int n)
{
	int i, j;
	for (int k = 0; k<n; k++)
	{
		// rand()是随机数生成器  
		i = rand() % image.cols;
		j = rand() % image.rows;
		if (image.type() == CV_8UC1)
		{ // 灰度图像  
			image.at<uchar>(j, i) = 255;
		}
		else if (image.type() == CV_8UC3)
		{ // 彩色图像  
			image.at<cv::Vec3b>(j, i)[0] = 255;
			image.at<cv::Vec3b>(j, i)[1] = 255;
			image.at<cv::Vec3b>(j, i)[2] = 255;
		}
	}
}

imgpro::~imgpro()
{
}


