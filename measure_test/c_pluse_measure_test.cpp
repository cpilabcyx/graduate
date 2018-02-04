#include"imgpro.h"
#include"res_c_r_w.h"
#include<ctime>
#include"virtualcam.h"

//双目极线约束匹配，双目测量，精度验证
int main()
{
	//判断是否已经标定，提示加文件查找
	//标定程序是D:\project\track\campcplus   获取图片
	//D:\project\track\calibrationtwo    双目标性

	res_c_r_w res;
	Mat R, T, F;
	res.xml_read("extrinsics_1-3.yml", "R", R);
	res.xml_read("extrinsics_1-3.yml", "T", T);
	res.xml_read("extrinsics_1-3.yml", "F", F);
	cout << R << endl << T << endl << F << endl;

	Mat M1, M2;
	res.xml_read("intrinsics_1-3.yml", "M1", M1);
	res.xml_read("intrinsics_1-3.yml", "M2", M2);
	//res.xml_read("intrinsics_1-3.yml", "D1", D1);
	//res.xml_read("intrinsics_1-3.yml", "D2", D2);
	cout << M1 << endl << M2 << endl;

	Mat mRightRT = res.mergecols(R, T);
	//cout << mRightRT;

	//打开相机
	cam_stereo mystero;
	mystero.open_cam();

	imgpro mypro;
	mycircle cir_l, cir_r;

	while (true)
	{
		mystero.grab_frame();

		//圆检测
		mypro.DecodeMark1(mystero.frame_l, cir_l);
		mypro.DecodeMark1(mystero.frame_r, cir_r);

		cir_l.show_circle(mystero.frame_l);
		cir_r.show_circle(mystero.frame_r);

		mystero.show_stereo_fram();

		//极线匹配
		mypro.matchEllipse(cir_l, cir_r, F);
		cir_l.show_match_order(mystero.frame_l);
		cir_r.show_match_order(mystero.frame_r);

		mystero.show_stereo_fram();

		vector<Point3f> vec_3d;

		//双目测量
		mypro.pointTo3D(cir_l.cir_match_vec, cir_r.cir_match_vec, M1, M2, mRightRT, vec_3d);

		imwrite("双目测量用于精度验证.png",mystero.frame_l);

		//精度验证

		//思路，双目测量用于精度验证.png 用屏幕可视化、打印的方法严格控制标志点的空间位置

		//在用双目测量其距离与理论值相比较，验证单点测量精度，通过matlab可视化其精度，如误差分布直方图，等，非重点

		//改进，圆检测二值化或多线程来增加算法的实时性

		waitKey(0);
	}
	return 0;
}
