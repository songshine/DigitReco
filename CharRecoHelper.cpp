#include "StdAfx.h"
#include "CharRecoHelper.h"
#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

#define TEMPLATE_PATH "DigitTemplatesTrain"   //模版位置

#define DEFAULT_SAMPLE_WIDTH 24
#define DEFAULT_SAMPLE_HEIGHT 48
#define DEFAULT_POINT_NUM  66   //默认从字上面取的点数


CharRecoHelper::CharRecoHelper()
{
	templatePath = (char*)malloc(sizeof(char)*1024);
	strcpy(templatePath,TEMPLATE_PATH);
	sc_prob.nbins_theta = 12;
	sc_prob.nbins_r = 5;
	sc_prob.r_inner = 0.125;
	sc_prob.r_outer = 2.0; 
	nExtractPoints = DEFAULT_POINT_NUM;
}
IplImage* CharRecoHelper::Convert2Edge(const IplImage* img)
{
	IplImage* dst = cvCreateImage(cvSize(DEFAULT_SAMPLE_WIDTH,DEFAULT_SAMPLE_HEIGHT),
		IPL_DEPTH_8U,1);
	cvResize(img,dst);
	//中值滤波
	//cvSmooth(dst,dst,CV_MEDIAN);

	//求边缘图
	cvCanny(dst,dst,50,150,7);


	return dst;
}

char CharRecoHelper::ConvertFileToChar(char* fileName)
{
	char result = '#';
	if(fileName != NULL && strlen(fileName) > 2)
	{
		if(fileName[1] != '.')
		{
			if(fileName[0] >= 'a' && fileName[0] <= 'z')
			{
				result = fileName[0]-32;
			}
			else if(fileName[0] >= '0' && fileName[0] <= '9')
			{
				result = fileName[0]; 
			}
		}
		else
		{
			result = fileName[0];
		}
	}

	return result;
}