#include "stdafx.h"
#include "HandRecognition.h"
#include "shapecontext.h"
#include "img_sc.h"

CvPoint* ReturnContourPoint(const IplImage* img,int npoints);


PGesturePara GrapGestureFromCamera(int npoints)
{
	
	CvCapture* caputure = NULL;
	IplImage* pFrame = NULL;
	int c;
	caputure = cvCreateCameraCapture(0);
	if(caputure == NULL)
	{
		ASSERT(caputure != NULL);
	}
	cvNamedWindow("handrecognition",CV_WINDOW_AUTOSIZE);
	while((pFrame = cvQueryFrame(caputure)) != NULL)
	{
		cvShowImage("handrecognition",pFrame);
		c = cvWaitKey(10);
		if(c == 27)
			break;
	}

	cvReleaseCapture(&caputure);
	cvDestroyAllWindows();

	return NULL;
}
PGesturePara GetGestureFromImage(const char* img_name,int npoint)
{

	PGesturePara pg = NULL;
	IplImage* img = cvLoadImage(img_name);
	IplImage* img_gray = NULL;
	if(img != NULL)
	{
		pg = (PGesturePara)malloc(sizeof(GesturePara));
		pg->img = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
		img_gray = cvCreateImage(cvGetSize(img),IPL_DEPTH_8U,1);
		cvCvtColor(img,img_gray,CV_BGR2GRAY);
		/*cvShowImage("img_song",img);
		cvWaitKey(0);*/
		cvAdaptiveThreshold(img_gray,pg->img,255,CV_ADAPTIVE_THRESH_MEAN_C,CV_THRESH_BINARY,5);
		//cvThreshold(img_gray,pg->img,150,255,CV_THRESH_BINARY);
		pg->points = ReturnContourPoint(pg->img,npoint);
	}
	cvReleaseImage(&img);
	cvReleaseImage(&img_gray);
	return pg;
}

int GestureRecognize(CvPoint* pts,int npoints)
{
	return 0;
}

CvPoint* ReturnContourPoint(const IplImage* img,int npoints)
{
	CvPoint* points = NULL;
	point* pts = GetContourPointRandom(img,npoints);
	if(pts != NULL)
	{
		points = (CvPoint*)malloc(sizeof(CvPoint)*npoints);
		for(int i=0;i<npoints;i++)
		{
			points[i].x = pts[i].x;
			points[i].y = pts[i].y;
		}

	}
	return points;
}
