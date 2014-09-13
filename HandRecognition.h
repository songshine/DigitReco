#ifndef HANDRECOGNITION
#define HANDRECOGNITION

typedef struct _Gesture
{
	int id;  //���
	char* description;   //��������
	CvPoint* points ;  //�ؼ���
	IplImage* img;

}GesturePara,*PGesturePara;


//��ȡ�����ϵĵ�
PGesturePara GrapGestureFromCamera(int npoints);
PGesturePara GetGestureFromImage(const char* img_name,int npoint);

int GestureRecognize(CvPoint* pts,int npoints);

#endif