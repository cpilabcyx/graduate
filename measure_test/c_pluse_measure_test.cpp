#include"imgpro.h"
#include"res_c_r_w.h"
#include<ctime>
#include"virtualcam.h"

//˫Ŀ����Լ��ƥ�䣬˫Ŀ������������֤
int main()
{
	//�ж��Ƿ��Ѿ��궨����ʾ���ļ�����
	res_c_r_w res;
	Mat R, T, F;
	res.xml_read("extrinsics_1-3.yml", "R", R);
	res.xml_read("extrinsics_1-3.yml", "T", T);
	res.xml_read("extrinsics_1-3.yml", "F", F);
	cout << R << endl << T << endl << F;

	//�����
	cam_stereo mystero;
	mystero.open_cam();

	imgpro mypro;
	mycircle cir_l, cir_r;

	long start, finish;

	while (true)
	{
		start = clock();

		mystero.grab_frame();

		finish = clock();
		cout << "grab_frame: " << finish - start << "/" << CLOCKS_PER_SEC << " (s) " << endl;
		start = clock();

		//Բ���
		mypro.DecodeMark1(mystero.frame_l, cir_l);
		mypro.DecodeMark1(mystero.frame_r, cir_r);

		finish = clock();
		cout << "DecodeMark1: " << finish - start << "/" << CLOCKS_PER_SEC << " (s) " << endl;
		start = clock();

		cir_l.show_circle(mystero.frame_l);
		cir_r.show_circle(mystero.frame_r);

		mystero.show_stereo_fram();

		finish = clock();
		cout << "show_circle: " << finish - start << "/" << CLOCKS_PER_SEC << " (s) " << endl;
		//����ƥ��

		//˫Ŀ����

		//������֤
	}
	return 0;
}


