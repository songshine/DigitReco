#ifndef HANDRECOGNITION
#define HANDRECOGNITION

typedef struct _Gesture
{
	int id;  //编号
	char* description;   //手势描述
	CvPoint* points ;  //关键点
	IplImage* img;

}GesturePara,*PGesturePara;


//获取手势上的点
PGesturePara GrapGestureFromCamera(int npoints);
PGesturePara GetGestureFromImage(const char* img_name,int npoint);

int GestureRecognize(CvPoint* pts,int npoints);

#endif