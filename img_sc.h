#ifndef IMG_SC_H_
#define IMG_SC_H_

//求一幅图像的(p,q)阶中心矩
long knMe(const IplImage* img,int p,int q);
//求一幅图的p+q中心矩
long kmpq(const IplImage* img,int p,int q);
//求一幅图的中心惯性矩
long kmf(const IplImage* img);
//求图像的重心
void img_gravity_center(const IplImage* img,int* xg,int* yg);

//将一幅边缘图像中的非零坐标转换成一个二维矩阵
int ConvertImage2XY(const IplImage* img_edge,point** p);

//随机获取npoints个边界点,获取成功返回坐标，否则返回NULL
point* GetContourPointRandom(const IplImage* img,int npoints);

#endif