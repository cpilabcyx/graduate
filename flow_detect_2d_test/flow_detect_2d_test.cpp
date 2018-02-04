#include"imgpro.h"
#include"mycircle.h"
#include"res_c_r_w.h"
#include <exception> 

//ԭ�����ϣ�x���� y��
bool move_contrue(int& mod, Point2i& position, int& step)
{
	if (mod == 2490368)//��
	{
		position.y -= step;
	}
	else if (mod == 2621440)//��
	{
		position.y += step;
	}
	else if (mod == 2424832)//��
	{
		position.x -= step;
	}
	else if (mod == 2555904)//��
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
	//��־������ɡ��˶�
	//�����׵�ͼƬ
	mycircle cir;
	imgpro pro;

	int highth_back = 1920*2, wideth_back = 2560*2;
	int step=40;

	Mat back = Mat::Mat(highth_back, wideth_back, CV_8UC3, Scalar(255, 255, 255));

	//���������Բλ��,�������
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
			//Խ��ָ�֮ǰλ��
			if (position.x<0 || position.y<0 || position.x > wideth_back / 4 * 3 || position.y > highth_back / 4 * 3)
				position = position_pre;

			//���µ�ǰ֡
			gray = Mat(back, Rect(position.x, position.y, wideth_back / 4, highth_back / 4)).clone();
			//cvtColor(gray, gray, COLOR_BGR2GRAY);
			//pro.salt(gray, 10000);

			//������׷��
			//pro.DecodeMark1(gray, cir,true);

			//pro.tack_flow();

			//ʹ��LK������������ѡ���ǵ㣺
			vector<uchar> status;
			vector<float> err;

			//����һ��ϡ���������Ĺ�����ʹ�ý������еĵ��� Lucas-Kanade ����
			calcOpticalFlowPyrLK(prevGray, gray, pre, now, status, err);
			size_t i, k;
			for (i = k = 0; i < now.size(); i++)
			{
			/*	if (!status[i])
					continue;*/
				now[k++] = now[i];//��¼׷�ٵ���
				circle(gray, now[i], 6, Scalar(0, 0, 255), -1, 4);
			}
			std::swap(now, pre);
			imshow("gray", gray);

			//������ϣ���ǰ֡��Ϊ��ʷ
			cv::swap(prevGray, gray);
			//prevGray = gray.clone();

			position_pre = position;

			
		}

		int mod=waitKey(100);
		update=move_contrue(mod, position, step);
	}

	waitKey(0);



	//��Բ�������



	//��֤�������ľ�����Ч��
	//����Ҷȱ仯���������ں���ʵ�飬����׷�������ֵ��С����ֵ��Χ��������������ֵ��Χ������Ч׷�ٵ�������������Լ����λ�ˣ����ȥ���õ㣬����˫Ŀ����


	return 0;
}