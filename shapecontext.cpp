#include "StdAfx.h"
#include "shapecontext.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#define  pi 3.1415926
#define maxNumThreads 8

struct ComputeSCThreadPara 
{
	// Thread parameter.
	point* totalPoints;
	int numPoints;
	int startIndex;
	int endIndex;

	// Shape Context parameter.
	int nbins_theta;
	int nbins_r;
	float mean_d;
	float* log_r_array;
	
	// Out put parameter.
	float*** sc;   //out
};

/************All Handels**********/
HANDLE hAverageDistance;


/************************************************************************/
//返回两个点之间的距离
float distance(point p1,point p2);

//求两个shapecontext的距离
float x2_dis(float* sc1,float* sc2,int K);
//返回对数极坐标logr范围
float* logspace(double d1,double d2,int n);
//归一化
float*  normaliz_shape_context(int* sc,int K);
 
//计算某个点的shape_context
float* shape_context(point p0,point* total_points,int nl,
				   int nbins_theta,int nbins_r,float mean_d,float* log_r_array);



				/*函数定义如下*/
/************************************************************************/

HANDLE async_shape_context(point* total_points, 
						 int num_points, 
						 int start_index, 
						 int end_index,
						 int nbins_theta,
						 int nbins_r,
						 float mean_d,
						 float* log_r_array,
						 float*** sc);

DWORD WINAPI shape_context_thread(LPDWORD lpdwParam);



//返回所有点的shapecontext
//返回所有点的shapecontext
float** sc_compute(point* total_points,int nl,float* mean_dist,
				 int nbins_theta,int nbins_r,float r_inner,float r_outer)
{

	assert(nl > maxNumThreads);
	int i,j;
	float** sc;	
	//求任意两点的平均距离
	
	//r边界
	float* log_array  = logspace(log10(r_inner),log10(r_outer),nbins_r);

	if(*mean_dist <= 0.0)
	{
		for(i=0;i<nl;i++)
		for(j=i;j<nl;j++)
			*mean_dist += distance(total_points[i],total_points[j]);

		*mean_dist /= ((nl+1)*nl/2);
	}
	

	sc = (float**)malloc(sizeof(float*)*nl);
	for(i=0;i<nl;i++)
	{
		sc[i] = shape_context(total_points[i],total_points,nl,nbins_theta,nbins_r,*mean_dist,log_array);
	}

	return sc;
}

float** async_sc_compute(point* total_points,int nl,float* mean_dist,
						 int nbins_theta,int nbins_r,float r_inner,float r_outer)
{
	int i,j;
	
	int numScPerThread = nl / maxNumThreads;
	int restScNum = nl % maxNumThreads;

	if(numScPerThread == 0)
	{
		return sc_compute(total_points,nl,mean_dist,nbins_theta,nbins_r,r_inner,r_outer);
	}
	//r边界
	float* log_array  = logspace(log10(r_inner),log10(r_outer),nbins_r);

	//求任意两点的平均距离
	if(*mean_dist <= 0.0)
	{
		for(i=0;i<nl;i++)
		for(j=i;j<nl;j++)
			*mean_dist += distance(total_points[i],total_points[j]);

		*mean_dist /= ((nl+1)*nl/2);
	}
	

	float** sc = (float**)malloc(sizeof(float*)*nl);
	
	int startIndex = 0;
	int endIndex = numScPerThread-1;
	HANDLE hThread[maxNumThreads];

	for(i = 0 ;i <maxNumThreads;i++)
	{
		if(i < restScNum)
		{
			endIndex = startIndex + numScPerThread + 1;
		}
		else
		{
			endIndex = startIndex + numScPerThread;
		}

		hThread[i] = async_shape_context(
			total_points, 
			nl, 
			startIndex, 
			endIndex, 
			nbins_theta, 
			nbins_r, 
			*mean_dist , 
			log_array,
			&sc);
		startIndex = endIndex;
	}
	WaitForMultipleObjects(maxNumThreads, hThread,TRUE,INFINITE);

	for(i = 0 ;i <maxNumThreads;i++)
	{
		CloseHandle(hThread[i]);
	}
	free(log_array);
	return sc;
}
//计算任意两点的cost
float** hist_cost(float** bh1,int n1,float** bh2,int n2,int K)
{
	int i,j;
	float** cost = NULL;
	cost = (float**)malloc(sizeof(float*)*n1);
	
	for(i=0;i<n1;i++)
	{
		cost[i] = (float*)malloc(sizeof(float)*n2);
		for(j=0;j<n2;j++)
		{
			cost[i][j] = x2_dis(bh1[i],bh2[j],K);
		}
	}
	return cost;
}

//保存所有的cost到文件里面
void save_cost(float** bh,int n1,int n2,char* file_name)
{
	FILE* file = fopen(file_name,"w");
	if(file == NULL)
		return;
	for(int i=0;i<n1;i++)
	{
		for(int j=0;j<n2;j++)
			fprintf(file,"%f ",bh[i][j]);
		fprintf(file,"\n");
	}
	
	fclose(file);
	
}
//保存shapecontext值到文件
void save_sc(int** bh,int n1,int n2,char* file_name)
{
	FILE* file = fopen(file_name,"w");
	if(file == NULL)
		return;
	for(int i=0;i<n1;i++)
	{
		for(int j=0;j<n2;j++)
			fprintf(file,"%d ",bh[i][j]);
		fprintf(file,"\n");
	}
	
	fclose(file);
}
/*从文件中读取数据,数据格式为:
total
x0 y0
x1 y1
...
*/
int read_data(char* file_name,point** pts)
{
	int num = 0;
	FILE* file = fopen(file_name,"r");
	if(file == NULL)
		return 0;
	fscanf(file,"%d\n",&num);
	if(num > 0)
		*pts = (point*)malloc(sizeof(point)*num);

	for(int i=0;i<num;i++)
		fscanf(file,"%f %f\n",&((*pts)[i].x),&((*pts)[i].y));

	fclose(file);
	return num;
}
/************************************************************************/

//计算两个点的欧式距离
float distance(point p1,point p2)
{
	return (float)sqrt(pow((float)(p1.x-p2.x),2)+pow((float)(p1.y-p2.y),2));
}

//返回对数极坐标logr范围
float* logspace(double d1,double d2,int n)
{
	float* logs = (float*)malloc(sizeof(float)*n);
	for(int i=0;i<n;i++)
	{
		logs[i] = (float)pow(10,(float)i*(d2-d1)/(n-1)+d1);
	}
	return logs;
}

//求两个shapecontext的距离
float x2_dis(float* sc1,float* sc2,int K)
{
	float ret = 0;
	int sum = 0;
	//float* scf1 = normaliz_shape_context(sc1,K);
	//float* scf2 = normaliz_shape_context(sc2,K);
	for(int i=0;i<K;i++)
	{
		if(sc1[i] == sc2[i])
			continue;

		ret += (sc1[i]-sc2[i])*(sc1[i]-sc2[i])/(sc1[i]+sc2[i]);
	}
	
	return ret/2;

}

//归一化
float*  normaliz_shape_context(int* sc,int K)
{
	int i;
	int sum = 0;
	float* scf = (float*)malloc(sizeof(float)*K);
	for(i=0;i<K;i++)
		sum += sc[i];

	for(i=0;i<K;i++)
		scf[i] = (float)sc[i]/sum;

	return scf;

}
//求出该点所在bin的序号,返回-1表示忽略该点
int get_log_space_bin_index(const point p0, 
						const point des_point, 
						const int nbins_theta, 
						const int nbins_r, 
						const float mean_dis, 
						const float* log_r_array)
{
	int i;
	float avg_dis = 0.0;
	float theta = 0.0;
	int nlogr = 0, ntheta = 0;

	//归一化logr,并求角度
	avg_dis = distance(des_point,p0)/mean_dis;
	theta = (float)atan2((float)des_point.y-p0.y,des_point.x-p0.x);
	theta = (float)(theta>0?theta:theta+2*pi);

	if(avg_dis > log_r_array[nbins_r-1])
		return -1;

	for(i=0;i< nbins_r;i++)
	{
		if(avg_dis <= log_r_array[i])
		{
			nlogr = i;
			break;				
		}
	}
	ntheta = (int)floor(theta/(2*pi/nbins_theta));

	return (nbins_theta * nlogr + ntheta);

}


HANDLE async_shape_context(point* total_points, 
						 int num_points, 
						 int start_index, 
						 int end_index,
						 int nbins_theta,
						 int nbins_r,
						 float mean_d,
						 float* log_r_array,
						 float*** sc)
{
	assert(end_index >= start_index);

	ComputeSCThreadPara * p = (ComputeSCThreadPara*)malloc(sizeof(ComputeSCThreadPara));
	p->totalPoints = total_points;
	p->numPoints = num_points;
	p->startIndex = start_index;
	p->endIndex = end_index;
	p->nbins_theta = nbins_theta;
	p->nbins_r = nbins_r;
	p->mean_d = mean_d;
	p->log_r_array = log_r_array;
	p->sc = sc;

	HANDLE hResultHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)shape_context_thread,p,0,NULL);
	return hResultHandle;
	
}

DWORD WINAPI shape_context_thread(LPDWORD lpdwParam)
{
	ComputeSCThreadPara * p = (ComputeSCThreadPara*)lpdwParam;
	ComputeSCThreadPara threadPara = *p;

	for(int i=threadPara.startIndex; i<threadPara.endIndex;i++)
	{
		(*(threadPara.sc))[i] = shape_context(
			threadPara.totalPoints[i],
			threadPara.totalPoints,
			threadPara.numPoints,
			threadPara.nbins_theta,
			threadPara.nbins_r,
			threadPara.mean_d,
			threadPara.log_r_array
			);
	}
	return 0;
}
//计算某个点的shape_context
float* shape_context(point p0,point* total_points,int nl,
				   int nbins_theta,int nbins_r,float mean_d,float* log_r_array)
{
	int i;	
	int npoints = nl;
	float* bh = (float*)malloc(sizeof(float)*(nbins_r*nbins_theta));

	for(i=0;i<nbins_r*nbins_theta;i++)
		bh[i] = 0.0;

	for(i=0;i<nl;i++)
	{
		int index = 
			get_log_space_bin_index(
			p0,total_points[i],
			nbins_theta,
			nbins_r,
			mean_d,
			log_r_array
			);
		if(-1 == index)
		{
			npoints -- ;
			continue;
		}
		else
		{
			bh[index] += 1.0;
		}
	}
	for(i=0;i<nbins_r*nbins_theta;i++)
	{
		bh[i] /= npoints;
	}
	return bh;
}



