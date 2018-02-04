#include "res_c_r_w.h"

res_c_r_w::res_c_r_w()
{
}

//2d点欧氏距离
float res_c_r_w::dis_comput_point(Point2f a, Point2f b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

//3d点欧氏距离
float res_c_r_w::dis_comput_point(Point3f a, Point3f b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2) + pow((a.z - b.z), 2));
}

//2d点乱序，最小距离
int res_c_r_w::comput_err2d_random_order(const vector<Point2f> &detected, const vector<Point2f> &idle, vector<float> &err)
{
	int num_match = 0;
	int seze_idle = idle.size();
	int seze_detected = detected.size();
	for (int i = 0; i < seze_detected; i++)
	{
		float min = 9999;
		for (int j = 0; j < seze_idle; j++)
		{
			float dis = this->dis_comput_point(detected[i], idle[j]);//2d点欧氏距离
			if (dis < min)min = dis;//距离最小点为误差
		}
		err.push_back(min);
		num_match++;
	}
	return num_match;
}

//空的，日后填
int res_c_r_w::comput_err2d_random_order(const vector<RotatedRect> &detected, const vector<RotatedRect> &idle, vector<float> &err)
{
	return 0;
}

//记录vector
int res_c_r_w::txt_record(string& filneme, vector<Point2f> &point2d)
{
	ofstream out(filneme);
	int size = point2d.size();
	for (int i = 0; i < size; i++)
		out << point2d[i].x << " " << point2d[i].y << endl;
	return size;
}
int res_c_r_w::txt_record(string& filneme, vector<float> &point)
{
	ofstream out(filneme);
	int size = point.size();
	for (int i = 0; i < size; i++)
		out << point[i] << endl;
	return size;
}

//读取xml
bool res_c_r_w::xml_read(char filneme[], char mat_name[], Mat& mat)
{
	FileStorage fs(filneme, FileStorage::READ);
	if (fs.isOpened())
	{
		fs[mat_name] >> mat;
		fs.release();
	}
	return mat.data;
}

int fps()
{
	/*
	#include<ctime>

	int main()
	{
	long start, finish;
	start = clock();

	cout << "HW .... " << endl;

	finish = clock();

	cout << finish - start << "/" << CLOCKS_PER_SEC << " (s) " << endl;

	return 0;
	}
	*/
	return 0;
}

res_c_r_w::~res_c_r_w()
{
}

Mat res_c_r_w::mergeRows(Mat& A, Mat& B)//行[A ; B]
{
	//CV_ASSERT(A.cols == B.cols&&A.type() == B.type());
	int totalRows = A.rows + B.rows;

	Mat mergedDescriptors(totalRows, A.cols, B.type());
	Mat submat = mergedDescriptors.rowRange(0, A.rows);
	A.copyTo(submat);
	submat = mergedDescriptors.rowRange(A.rows, totalRows);
	B.copyTo(submat);
	return mergedDescriptors;
}

Mat res_c_r_w::mergecols(Mat& A, Mat& B)//列[A   B]
{
	//CV_ASSERT(A.cols == B.cols&&A.type() == B.type());
	int totalCols = A.cols + B.cols;

	Mat mergedDescriptors(A.rows, totalCols, B.type());
	Mat submat = mergedDescriptors.colRange(0, A.cols);
	A.copyTo(submat);
	submat = mergedDescriptors.colRange(A.cols, totalCols);
	B.copyTo(submat);
	return mergedDescriptors;
}

//输出3d点
int res_c_r_w::cout_3d_point(const vector<Point3f> &point3d)
{
	int size = point3d.size();
	for (int i = 0; i < size; i++)
		cout << point3d[i].x << " " << point3d[i].y << " " << point3d[i].z <<endl;
	return size;
}