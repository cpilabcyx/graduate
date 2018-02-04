#include"imgpro.h"
#include"res_c_r_w.h"
#include<ctime>
#include"virtualcam.h"

//˫Ŀ����Լ��ƥ�䣬˫Ŀ������������֤
int main()
{
	//�ж��Ƿ��Ѿ��궨����ʾ���ļ�����
	//�궨������D:\project\track\campcplus   ��ȡͼƬ
	//D:\project\track\calibrationtwo    ˫Ŀ����

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

	//�����
	cam_stereo mystero;
	mystero.open_cam();

	imgpro mypro;
	mycircle cir_l, cir_r;

	while (true)
	{
		mystero.grab_frame();

		//Բ���
		mypro.DecodeMark1(mystero.frame_l, cir_l);
		mypro.DecodeMark1(mystero.frame_r, cir_r);

		cir_l.show_circle(mystero.frame_l);
		cir_r.show_circle(mystero.frame_r);

		mystero.show_stereo_fram();

		//����ƥ��
		mypro.matchEllipse(cir_l, cir_r, F);
		cir_l.show_match_order(mystero.frame_l);
		cir_r.show_match_order(mystero.frame_r);

		mystero.show_stereo_fram();

		vector<Point3f> vec_3d;

		//˫Ŀ����
		mypro.pointTo3D(cir_l.cir_match_vec, cir_r.cir_match_vec, M1, M2, mRightRT, vec_3d);

		imwrite("˫Ŀ�������ھ�����֤.png",mystero.frame_l);

		//������֤

		//˼·��˫Ŀ�������ھ�����֤.png ����Ļ���ӻ�����ӡ�ķ����ϸ���Ʊ�־��Ŀռ�λ��

		//����˫Ŀ���������������ֵ��Ƚϣ���֤����������ȣ�ͨ��matlab���ӻ��侫�ȣ������ֲ�ֱ��ͼ���ȣ����ص�

		//�Ľ���Բ����ֵ������߳��������㷨��ʵʱ��

		waitKey(0);
	}
	return 0;
}
