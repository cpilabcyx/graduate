             #include"imgpro.h"

double minArea = 130;//400--450之间
double maxArea = 30000;
double minLen = 40;
double maxLen = 6000;

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
bool imgpro::DecodeMark1(Mat &image, std::vector<cv::Point2f>& vpUV, bool if_gray)
{
	vpUV.clear();

	bool found = false;
	mycircle temp;

	DecodeMark1(image, temp, if_gray);

	int size = temp.cir_idle_ab_detect.size();
	for (size_t i = 0; i < size; i++)
	{
		vpUV.push_back(temp.cir_idle_ab_detect[i].center);
	}

	return found;
}

//椭圆检测,记录长短轴
bool imgpro::DecodeMark1(Mat &image, mycircle &cir,bool if_gray)
{
	bool found = false;
	const double resizeFactor = 1.0;

	//////////////////////////////////////////////////////////////////////////
	//图像处理
	//灰度+8U+边缘  用于轮廓检测
	//image.convertTo(image, CV_8UC1);
	Mat imGary;
	if (!if_gray)
	cvtColor(image, imGary, CV_BGR2GRAY);
	else
	{
		imGary = image;
	}
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
	cir.cir_vec.clear();

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
void imgpro::identify_cir_hsv(Mat &image, vector<cv::Point2f>& vpUV, vector<int>& hsv)
{
	if (hsv.size()>0)hsv.clear();

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

		hsv.push_back(bgr[0]);
	}
}

//椭圆心鉴别hsv, 并分为group类
void imgpro::identify_cir_hsv(Mat &image, vector<cv::Point2f>& vpUV, vector<int>& hsv, vector<int>& id, int group)
{
	this->identify_cir_hsv(image, vpUV, hsv);

	int size = hsv.size();

	int step = 180 / group / 3;

	for (size_t i = 0; i < size; i++)
	{

		int not_sure = -10;

		int num = hsv[i] / step + 1;

		int k = num / 3;
		if (k == group)k = 0;

		if (num % 3 != 2)not_sure = k;
		id.push_back(not_sure);
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

//椭圆的匹配
bool imgpro::matchEllipse(vector<Point2f>& cir_cev_l, vector<Point2f>& cir_cev_r, vector<Point2f>& cir_match_cev_l, vector<Point2f>& cir_match_cev_r, Mat& F_2_1)
{
	std::vector<cv::Vec<float, 3>> line1, line2, line3;
	cir_match_cev_l.clear();
	cir_match_cev_r.clear();

	//vpUVcenter3.clear();
	//先检测到椭圆

	//在图2上画极线
	if (cir_cev_l.size()>0 && cir_cev_r.size()>0 && F_2_1.data)
	{
		computeCorrespondEpilines(cir_cev_l, 1, F_2_1, line1);//计算对应点的外极线line1是一个三元组(a,b,c)，表示点在另一视图中对应的外极线ax+by+c=0;  

		//cv::circle(image1, cir_cev_l[9], 4, Scalar(0, 0, 255), -1);//在视图1中把关键点用圆圈画出来，然后再绘制在对应点处的外极线 
		//cv::line(image2, cv::Point(0, -line1[9][2] / line1[9][1]), cv::Point(image2.cols, -(line1[9][2] + line1[9][0] * image2.cols) / line1[9][1]), Scalar(0, 255, 0));


		for (size_t n = 0; n < cir_cev_l.size(); n++)
		{
			//
			double a = 0;
			double	b = 0;
			double	c = 0;
			double dis = 0;
			vector<Point2f> Epilines1_2;
			a = line1[n][0];
			b = line1[n][1];
			c = line1[n][2];

			double stddis = 2.00;
			//float stdSredis = 80;
			// 距离公式为d = |A*x0 + B*y0 + C|/√(A^2 + B^2)
			////////////////////////第一次筛选
			//在图2极线上筛选椭圆
			vector<int> numR;
			int m = 0;
			for (size_t i = 0; i < cir_cev_r.size(); i++)
			{

				dis = abs(a *cir_cev_r[i].x + b * cir_cev_r[i].y + c) / sqrt(a * a + b * b);
				float absx = abs(cir_cev_r[i].x - cir_cev_l[n].x);
				float absy = abs(cir_cev_r[i].y - cir_cev_l[n].y);
				if (dis < stddis /*&& absx<stdSredis && absy<stdSredis*/)//增加像素区域约束和极线误差约束
				{

					//if (cir_cev_l[n].x>cir_cev_r[i].x)//在合理夹角下，对于左图的点来说，右图对应的点X坐标值一定大于它的X坐标值
					//{
					//	break;
					//}
					Epilines1_2.push_back(cir_cev_r[i]);
					numR.push_back(i);
					m = i;
					break;
				}
			}
#if 0
			////////////////画出第n个点的反算极线
			if (n == 0)
			{
				cv::circle(image1, cir_cev_l[n], 4, Scalar(0, 0, 255), -1);//在视图1中把第0个关键点用圆圈画出来，然后再绘制在对应点处的外极线 
				cv::line(image2, cv::Point(0, -line1[n][2] / line1[n][1]), cv::Point(image2.cols, -(line1[n][2] + line1[n][0] * image2.cols) / line1[n][1]), Scalar(0, 255, 0), 3, 8);

				if (Epilines1_2.size()>0)
				{
					std::vector<cv::Vec<float, 3>>  linen;
					computeCorrespondEpilines(Epilines1_2, 2, F_2_1, linen);
					if (Epilines1_2.size() == linen.size())
					{
						for (size_t i = 0; i < Epilines1_2.size(); i++)
						{
							cv::line(image1, cv::Point(0, -linen[i][2] / linen[i][1]), cv::Point(image1.cols, -(linen[i][2] + linen[i][0] * image1.cols) / linen[i][1]), Scalar(255, 0, 0), 1, 8);
						}

					}
				}
			}
			//////////////////////////
#endif
			////////////////////////第二次筛选
			if (Epilines1_2.size() == 0 && numR.size() == 0)//极线约束没找到对应点
			{
				continue;
			}
			else if (Epilines1_2.size() == 1)//只找到一个
			{
				cir_match_cev_l.push_back(cir_cev_l[n]);
				cir_match_cev_r.push_back(Epilines1_2[0]);

				if (cir_cev_r.size() > 1)//如果极限约束匹配点为一个，将其从右相机待匹配点剔除
				{
					vector <Point2f>::iterator Iter = cir_cev_r.begin();
					cir_cev_r.erase(Iter + m);

				}
				//circle(image2, cir_match_cev_r[n], 4, Scalar(0, 255, 255), -1);
			}
			//else if (Epilines1_2.size()>1)//找到两个以上
			//{
			//	computeCorrespondEpilines(Epilines1_2, 1, F_2_1, line2);

			//	if (Epilines1_2.size() == line2.size())
			//	{
			//		for (size_t i = 0; i < Epilines1_2.size(); i++)
			//		{
			//			bool isOK = false;
			//			double a = 0;
			//			double	b = 0;
			//			double	c = 0;


			//			a = line2[i][0];
			//			b = line2[i][1];
			//			c = line2[i][2];

			//			dis = abs(a *cir_cev_l[n].x + b * cir_cev_l[n].y + c) / sqrt(a * a + b * b);

			//			if (dis < stddis)
			//			{
			//				cir_match_cev_l.push_back(cir_cev_l[n]);
			//				cir_match_cev_r.push_back(Epilines1_2[i]);
			//				isOK = true;
			//				break;
			//			}


			//		}
			//	}


			//}


		}
		return true;
	}
	return false;

}

bool imgpro::matchEllipse(mycircle &cir_l, mycircle &cir_r, Mat& F_2_1)
{
	return matchEllipse(cir_l.cir_vec, cir_r.cir_vec, cir_l.cir_match_vec, cir_r.cir_match_vec, F_2_1);
}

//由双目像素坐标计算世界坐标
bool imgpro::pointTo3D(vector<Point2f>& vec_uvLeft, vector<Point2f>& vec_uvRight, Mat& mLeftIntrinsic, Mat& mRightIntrinsic, Mat& mRightRT, vector<Point3f>& vec_3d)
{
	//  [u1]      |X|                     [u2]      |X|  
	//Z*|v1| = Ml*|Y|                   Z*|v2| = Mr*|Y|  
	//  [ 1]      |Z|                     [ 1]      |Z|  
	//            |1|                               |1|  

	if (vec_uvLeft.size() != vec_uvRight.size())return false;
	vec_3d.clear();

	Mat mLeftRT = (Mat_<float>(3, 4) <<
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0);
	mLeftRT.convertTo(mLeftRT, CV_32F);
	mRightRT.convertTo(mRightRT, CV_32F);

	mLeftIntrinsic.convertTo(mLeftIntrinsic, CV_32F);
	mRightIntrinsic.convertTo(mRightIntrinsic, CV_32F);


	Mat mLeftM = Mat(3, 4, CV_32F);
	Mat mRightM = Mat(3, 4, CV_32F);
	mLeftM = mLeftIntrinsic * mLeftRT;
	mRightM = mRightIntrinsic * mRightRT;

	int size = vec_uvLeft.size();
	for (size_t i = 0; i < size; i++)
	{
		Point2f uvLeft = vec_uvLeft[i];
		Point2f uvRight = vec_uvRight[i];
		//最小二乘法A矩阵  
		Mat A = Mat(4, 3, CV_32F);
		A.at<float>(0, 0) = uvLeft.x * mLeftM.at<float>(2, 0) - mLeftM.at<float>(0, 0);
		A.at<float>(0, 1) = uvLeft.x * mLeftM.at<float>(2, 1) - mLeftM.at<float>(0, 1);
		A.at<float>(0, 2) = uvLeft.x * mLeftM.at<float>(2, 2) - mLeftM.at<float>(0, 2);

		A.at<float>(1, 0) = uvLeft.y * mLeftM.at<float>(2, 0) - mLeftM.at<float>(1, 0);
		A.at<float>(1, 1) = uvLeft.y * mLeftM.at<float>(2, 1) - mLeftM.at<float>(1, 1);
		A.at<float>(1, 2) = uvLeft.y * mLeftM.at<float>(2, 2) - mLeftM.at<float>(1, 2);

		A.at<float>(2, 0) = uvRight.x * mRightM.at<float>(2, 0) - mRightM.at<float>(0, 0);
		A.at<float>(2, 1) = uvRight.x * mRightM.at<float>(2, 1) - mRightM.at<float>(0, 1);
		A.at<float>(2, 2) = uvRight.x * mRightM.at<float>(2, 2) - mRightM.at<float>(0, 2);

		A.at<float>(3, 0) = uvRight.y * mRightM.at<float>(2, 0) - mRightM.at<float>(1, 0);
		A.at<float>(3, 1) = uvRight.y * mRightM.at<float>(2, 1) - mRightM.at<float>(1, 1);
		A.at<float>(3, 2) = uvRight.y * mRightM.at<float>(2, 2) - mRightM.at<float>(1, 2);

		//最小二乘法B矩阵  
		Mat B = Mat(4, 1, CV_32F);
		B.at<float>(0, 0) = mLeftM.at<float>(0, 3) - uvLeft.x * mLeftM.at<float>(2, 3);
		B.at<float>(1, 0) = mLeftM.at<float>(1, 3) - uvLeft.y * mLeftM.at<float>(2, 3);
		B.at<float>(2, 0) = mRightM.at<float>(0, 3) - uvRight.x * mRightM.at<float>(2, 3);
		B.at<float>(3, 0) = mRightM.at<float>(1, 3) - uvRight.y * mRightM.at<float>(2, 3);

		Mat XYZ = Mat(3, 1, CV_32F);
		//采用SVD最小二乘法求解XYZ  
		solve(A, B, XYZ, DECOMP_SVD);

		//cout<<"空间坐标为 = "<<endl<<XYZ<<endl;  

		//世界坐标系中坐标  
		Point3f world;
		world.x = XYZ.at<float>(0, 0);
		world.y = XYZ.at<float>(1, 0);
		world.z = XYZ.at<float>(2, 0);
		vec_3d.push_back(world);
	}
	return true;
}

bool imgpro::tack_flow(Mat& pregray, Mat& gray, vector<Point2f>& pre, vector<Point2f>& now, vector<uchar>& status, vector<float>& err)
{
	calcOpticalFlowPyrLK(pregray, gray, pre, now, status, err);
	return true;
}

imgpro::~imgpro()
{
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 