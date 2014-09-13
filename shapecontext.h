#ifndef _SHAPECONTEXT_H_
#define _SHAPECONTEXT_H_

/*shape context mode
*/
#define HOR_VER 1
#define GRAVITY_CERTER 2
#define TANGENT 3
//平面上的一个点
typedef struct _point
{
	float x;
	float y;
}point;
typedef struct _shape_context
{
	int nbins_theta; 
	int nbins_r;  
	float r_inner;
	float r_outer;
	int mode;
}shape_context_prob;

float** async_sc_compute(point* total_points,int nl,float* mean_dist,
				   int nbins_theta,int nbins_r,float r_inner,float r_outer);
//返回所有的shapecontexts
float** sc_compute(point* total_points,int nl,float* mean_dist,
				 int nbins_theta,int nbins_r,float r_inner,float r_outer);
//计算任意两个shapecontext的cost
float** hist_cost(float** bh1,int n1,float** bh2,int n2,int K);
//保存cost值到文件
void save_cost(float** bh,int n1,int n2,char* file_name);

//保存shapecontext值到文件
void save_sc(int** bh,int n1,int n2,char* file_name);

/*从文件中读取数据,数据格式为:
total
x0 y0
x1 y1
...
*/
int read_data(char* file_name,point** pts);

#endif
