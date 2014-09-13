#include "StdAfx.h"
#include "shapecontext.h"
#include "hungarian.h"


#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

/************************************************************************/
/* ��������                                                             */
/************************************************************************/
long knMe(const IplImage* img,int p,int q)
{
	long mpq = 0;
	int N = img->height;
	int M = img->width;
	int i,j;
	for(i=0;i<N;i++)
	{
		for(j=0;j<M;j++)
		{
			mpq += pow((float)i,p)*pow((float)j,q)*CV_IMAGE_ELEM(img,uchar,i,j)/255;
		}
	}
	
	return mpq;
}


long kmpq(const IplImage* img,int p,int q)
{
	int i,j;
	int ig,jg;
	long mpq=0;
	ig = knMe(img,1,0)/knMe(img,0,0);
	jg = knMe(img,0,1)/knMe(img,0,0);
	
	for(i=0;i<img->height;i++)
	{
		for(j=0;j<img->width;j++)
			mpq += pow((float)(i-ig),p)*pow((float)(j-jg),q)*CV_IMAGE_ELEM(img,uchar,i,j)/255;
		
	}
	return mpq;
}

long kmf(const IplImage* img)
{
	int i,j;
	int ig,jg;
	long mf=0;
	ig = knMe(img,1,0)/knMe(img,0,0);
	jg = knMe(img,0,1)/knMe(img,0,0);
	for(i=0;i<img->height;i++)
	{
		for(j=0;j<img->width;j++)
			mf += (pow((float)(i-ig),2)+pow((float)(j-jg),2))*CV_IMAGE_ELEM(img,uchar,i,j)/255;
		
	}
	return mf;
}

//���ͼ�������
void img_gravity_center(const IplImage* img,int* xg,int* yg)
{
	*xg = knMe(img,1,0)/knMe(img,0,0);
	*yg = knMe(img,0,1)/knMe(img,0,0);
}
//��һ����Եͼ���еķ�������ת����һ����ά����
int ConvertImage2XY(const IplImage* img_edge,point** p)
{
	int i,j,t = 0;
	int width = img_edge->width;
	int height = img_edge->height;
	int numpix = 0;	
	for(i=0;i<width;i++)
		for(j=0;j<height;j++)
		{
			if(CV_IMAGE_ELEM(img_edge,uchar,j,i) == 255)
				numpix ++;
		}
		(*p) = (point*)malloc(sizeof(point)*numpix);
		for(i=0;i<width;i++)
			for(j=0;j<height;j++)
			{
				if(CV_IMAGE_ELEM(img_edge,uchar,j,i) == 255)
				{
					(*p)[t].x = i;
					(*p)[t++].y = j;
				}
			}
			return numpix;
}


/************************************************************************/
/* ������ȡ                                                                     */
/************************************************************************/
typedef struct _pairDistance
{
    int i;
    int j;
    int distance;
}pairDistance;

/*
    ������contoursample 
    ���ܣ���������
    ������seq ------ ����������
     samplearry --- ���ڴ�ų�����
         samplearry ---- ��������
*/
void quick_sort(pairDistance* table,int low,int high)
{//��������
	int i = low;
	int j= high;
	int base = table[low].distance;
	if(i < j)
	{
		while(i<j)
		{
			while(i<j && base < table[j].distance) j--;
			if(i < j)
			{
				table[i] = table[j];
				i++;
			}
			while(i<j && base >= table[i].distance) i++;
			if(i < j)
			{
				table[j] = table[i];
				j--;
			}
		}
		table[i].distance = base;
		quick_sort(table,low,i-1);
		quick_sort(table,i+1,high);
	}
}
int contoursample(CvSeq * seq , CvPoint *samplearry, int samplenum);
point* GetContourPointRandom(const IplImage* img,int npoints)
{
	point* pts = NULL;
	IplImage* img_bw = cvCloneImage(img);
	// 2 . ������ȡ
	CvMemStorage * storage = cvCreateMemStorage();
	CvSeq * contour = NULL;
	/*cvShowImage("song",img_bw);
	cvWaitKey(0);*/
	int tatal = cvFindContours(img_bw,storage,&contour,sizeof(CvContour),
		CV_RETR_LIST,CV_CHAIN_APPROX_NONE );

	CvPoint *samplearray = (CvPoint *)malloc(npoints * sizeof(CvPoint));

	if(contoursample(contour,samplearray,npoints))
	{//��������ɳɹ�
		pts = (point *)malloc(npoints * sizeof(point));
		for(int i=0;i<npoints;i++)
		{
			pts[i].x = samplearray[i].x;
			pts[i].y = samplearray[i].y;
		}
	}



	cvReleaseMemStorage(&storage);
	free(samplearray);

	return pts;
}

/*
    ������contoursample 
    ���ܣ���������
    ������seq ------ ����������
     samplearry --- ���ڴ�ų�����
         samplearry ---- ��������
*/
int contoursample(CvSeq * seq , CvPoint *samplearry, int samplenum)
{
    int num = 0 ; 
    for (CvSeq *s = seq ; s !=NULL;s=s->h_next)
        num +=s->total;

	if ( num < samplenum)
	{
		return 0; 
	}

    CvPoint *pointarray = (CvPoint *)malloc(num * sizeof(CvPoint));
    
    int accum = 0 ; 
    for (CvSeq *s =seq ; s!=NULL;s=s->h_next)
    {
        cvCvtSeqToArray( s, pointarray +accum);
        accum +=s->total;
        
    }
    
 
    // �������������
    CvRNG rng; 
    rng = cvRNG(cvGetTickCount());
    CvPoint pointtemp;
    int tagtemp = -1;
    for (int i = 0 ; i < num ; ++i)
    {
        int index = cvRandInt(&rng)%(num-i)+i;
        if(index !=i)
        {
            pointtemp = pointarray[index];
            pointarray[index] = pointarray[i];
            pointarray[i] = pointtemp;
            
        }
    }
    // ���*samplenum > num ��ȡ������ԶԶС�����������漴��ȡsamplenum�����ʡ����ʱ��
    if (num > 3 * samplenum)
    {
        
        CvPoint *pointarray2 = (CvPoint *)malloc(3*samplenum * sizeof(CvPoint));
        for (int i = 0;i < 3*samplenum;++i)
        {
            pointarray2[i] = pointarray[i];
        }
        free(pointarray);
        pointarray = pointarray2;
        num = 3 * samplenum;
    }
    // �����������������
	int beg = 0,nelement = 0;
	pairDistance* pd = (pairDistance*)malloc(sizeof(pairDistance)*((num-1)*num/2));
    for (int i = 0 ; i < num ; i++)
    {
        for (int j = i +1 ; j < num ; ++j)
        {
			pd[nelement].i = i;
			pd[nelement].j = j;
			pd[nelement++].distance = (pointarray[i].x -pointarray[j].x) * (pointarray[i].x -pointarray[j].x) + 
				(pointarray[i].y -pointarray[j].y) * (pointarray[i].y -pointarray[j].y);
        
        }
    }
	// ����
	quick_sort(pd,0,nelement-1);
    // ɾ����С�������е�����һ����ֱ������samplenum
    int nneedremove = num - samplenum;
    int *mask = (int *)malloc( num * sizeof(int));
    memset(mask,0,num * sizeof(int));
    //list<pairDistance>::iterator iter = list_pair.begin();
    //list<pairDistance>::iterator iter = list_pair.begin();
    while (nneedremove > 0)
    {
        int index0 = pd[beg].i;
        int index1 = pd[beg].j;
        if (mask[index0] == 0 && mask[index1] ==0)
        {
            mask[index1] = 1 ;
            nneedremove --;
        }
        beg++;
    }
    // ���������ŵ�samplearry��
    int nstartindex = 0 ;
    for (int i = 0 ; i < num ; ++i)
    {
        if (mask[i] ==0)
        {
            samplearry[nstartindex] = pointarray[i];
            nstartindex++;
        }
    }

    free(pointarray);
	free(pd);
	return 1;
}

