#include "res_c_r_w.h"

res_c_r_w::res_c_r_w()
{
}

//2d点欧氏距离
float res_c_r_w::dis_comput_point(Point2f a, Point2f b)
{
	return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

//2d点乱序，最小距离
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
			float dis = this->dis_comput_point(detected[i], idle[j]);//2d点欧氏距离
			if (dis < min)min = dis;//距离最小点为误差
		}
		err.push_back(min);
		num_match++;
	}
	return num_match;
}

res_c_r_w::~res_c_r_w()
{
}
