#include "res_c_r_w.h"

res_c_r_w::res_c_r_w()
{
}

//2d��ŷ�Ͼ���
float res_c_r_w::dis_comput_point(Point2f a, Point2f b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

//2d��������С����
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
			float dis = this->dis_comput_point(detected[i], idle[j]);//2d��ŷ�Ͼ���
			if (dis < min)min = dis;//������С��Ϊ���
		}
		err.push_back(min);
		num_match++;
	}
	return num_match;
}

//�յģ��պ���
int res_c_r_w::comput_err2d_random_order(const vector<RotatedRect> &detected, const vector<RotatedRect> &idle, vector<float> &err)
{
	return 0;
}

//��¼vector
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

//��ȡxml
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
