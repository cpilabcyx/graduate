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
int res_c_r_w::comput_err2d_random_order(const vector<Point2f> detected, const vector<Point2f>idle, vector<float>err)
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

res_c_r_w::~res_c_r_w()
{
}
