#include "StdAfx.h"
//#include "utility.h"
//#include "shapecontext.h"
//#include "img_sc.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//
//
//int nbins_theta = 12;
//int nbins_r = 5;
//float r_inner = 0.125;
//float r_outner = 2.0;
//
////将图片转换成纹理图
//IplImage* convert_image_2_edge(const IplImage* src)
//{
//	IplImage* dst = cvCreateImage(cvSize(DEFAULT_SAMPLE_WIDTH,DEFAULT_SAMPLE_HEIGHT),
//		IPL_DEPTH_8U,1);
//	cvResize(src,dst);
//	//中值滤波
//	//cvSmooth(dst,dst,CV_MEDIAN);
//
//	//求边缘图
//	cvCanny(dst,dst,50,150,7);
//
//
//	return dst;
//}
//
//
//p_sc_result convert_img_2_sc(const IplImage* img)
//{
//	IplImage* img = NULL;
//	IplImage* img_edge = NULL;
//	p_sc_result sc_res = null;
//	float mean_dist = 0.0;
//	if(img == null) return null;
//	
//	img_edge = convert_image_2_edge(img);
//	
//	
//	point* pts = GetContourPointRandom(img,DEFAULT_POINT_NUM);
//
//	if(null == pts) return;
//
//	sc_res = (p_sc_result)malloc(sizeof(sc_result));
//	sc_res->sc_feature = sc_compute(pts,DEFAULT_POINT_NUM,&mean_dist,nbins_theta,nbins_r,r_inner,r_outner);
//	sc_res->nvectors = DEFAULT_POINT_NUM;
//	sc_res->ncol = nbins_r*nbins_theta;
//
//	cvReleaseImage(&img_edge);
//
//	return sc_res;
//}
//
//p_sc_result convert_img_2_sc(const char* img_path)
//{
//	IplImage* img = cvLoadImage(img_path,0);
//	p_sc_result sc_res = convert_img_2_sc(img);
//	
//
//	cvReleaseImage(&img);
//	return sc_res;
//}