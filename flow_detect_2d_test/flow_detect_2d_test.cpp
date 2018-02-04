#include"imgpro.h"
#include"mycircle.h"
#include"res_c_r_w.h"
#include <exception> 

//原点左上，x向右 y下
bool move_contrue(int& mod, Point2i& position, int& step)
{
	if (mod == 2490368)//上
	{
		position.y -= step;
	}
	else if (mod == 2621440)//下
	{
		position.y += step;
	}
	else if (mod == 2424832)//左
	{
		position.x -= step;
	}
	else if (mod == 2555904)//右
	{
		position.x += step;
	}
	else
	{
		return false;
	}

	return true;
}

int main()
{
	//标志点的生成、运动
	//产生白底图片
	mycircle cir;
	imgpro pro;

	int highth_back = 1920*2, wideth_back = 2560*2;
	int step=40;

	Mat back = Mat::Mat(highth_back, wideth_back, CV_8UC3, Scalar(255, 255, 255));

	//随机设置椭圆位置,长短轴等
	for (int i = 100; i < wideth_back - 100; i += 200)
	{
		for (int j = 100; j < highth_back - 100; j += 200)
		{
			RotatedRect temp;
			temp.center.x = i - 10 + rand() % 20;
			temp.center.y = j - 10 + rand() % 20;

			temp.size.height = 60 + rand() % 80;
			temp.size.width = temp.size.height;
			//temp.size.width = 60 + rand() % 80;

			temp.angle = rand() % 360;

			cir.cir_idle_ab.push_back(temp);
			cir.cir_idle.push_back(temp.center);
		}
	}

	cir.genarate_ellipse(back, cir.cir_idle_ab, Scalar(0, 0, 0), 20);

	Point2i position = { wideth_back / 8, highth_back / 8 };
	Point2i position_pre;

	Mat prevGray, gray;
	bool update=false;

	gray = Mat(back, Rect(position.x, position.y, wideth_back / 4, highth_back / 4)).clone();

	//cvtColor(gray, prevGray, COLOR_BGR2GRAY);
	prevGray = gray.clone();

	vector<Point2f> pre, now;

	pro.DecodeMark1(prevGray, cir);

	cir.genarate_ellipse(gray, cir.cir_idle_ab_detect, Scalar(0, 0, 255),5);
	pre = cir.cir_vec;

	imshow("gray", gray);

	while (true)
	{
		if (update)
		{
			//越界恢复之前位置
			if (position.x<0 || position.y<0 || position.x > wideth_back / 4 * 3 || position.y > highth_back / 4 * 3)
				position = position_pre;

			//更新当前帧
			gray = Mat(back, Rect(position.x, position.y, wideth_back / 4, highth_back / 4)).clone();
			//cvtColor(gray, gray, COLOR_BGR2GRAY);
			//pro.salt(gray, 10000);

			//光流法追踪
			//pro.DecodeMark1(gray, cir,true);

			//pro.tack_flow();

			//使用LK光流法跟踪已选定角点：
			vector<uchar> status;
			vector<float> err;

			//计算一个稀疏特征集的光流，使用金字塔中的迭代 Lucas-Kanade 方法
			calcOpticalFlowPyrLK(prevGray, gray, pre, now, status, err);
			size_t i, k;
			for (i = k = 0; i < now.size(); i++)
			{
			/*	if (!status[i])
					continue;*/
				now[k++] = now[i];//记录追踪到的
				circle(gray, now[i], 6, Scalar(0, 0, 255), -1, 4);
			}
			std::swap(now, pre);
			imshow("gray", gray);

			//计算完毕，当前帧变为历史
			cv::swap(prevGray, gray);
			//prevGray = gray.clone();

			position_pre = position;

			
		}

		int mod=waitKey(100);
		update=move_contrue(mod, position, step);
	}

	waitKey(0);



	//椭圆检测修正



	//验证光流法的距离有效性
	//加入灰度变化噪声，对于后续实验，设置追踪误差阈值，小于阈值范围的修正，大于阈值范围，若有效追踪点大于三个，可以计算出位姿，则可去掉该点，采用双目测量


	return 0;
}