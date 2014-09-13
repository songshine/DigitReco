#include "StdAfx.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


void free_2(void** p, int n)
{
	if(p != null)
	{
		int i = 0;
		for (;i<n;i++)
		{
			if(p[i] != null)
			{
				free(p[i]);
				p[i]  = null;
			}			
			 
		}

		free(p);
		p = null;
	}
}

char* copy_str(const char* src)
{
	int len = strlen(src);
	char* des = (char*)malloc(sizeof(char)*(len+1));
	strcpy(des,src);
	return des;
}