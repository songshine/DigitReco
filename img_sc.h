#ifndef IMG_SC_H_
#define IMG_SC_H_

//��һ��ͼ���(p,q)�����ľ�
long knMe(const IplImage* img,int p,int q);
//��һ��ͼ��p+q���ľ�
long kmpq(const IplImage* img,int p,int q);
//��һ��ͼ�����Ĺ��Ծ�
long kmf(const IplImage* img);
//��ͼ�������
void img_gravity_center(const IplImage* img,int* xg,int* yg);

//��һ����Եͼ���еķ�������ת����һ����ά����
int ConvertImage2XY(const IplImage* img_edge,point** p);

//�����ȡnpoints���߽��,��ȡ�ɹ��������꣬���򷵻�NULL
point* GetContourPointRandom(const IplImage* img,int npoints);

#endif