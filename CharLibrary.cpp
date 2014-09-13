#include "StdAfx.h"
//
//#include "SCEngine.h"
//#include "shapecontext.h"
//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//
//
//// inserts char template into the char template list.
//void insert_char_template(p_char_template head, p_char_template)
//{
//	if(null == p_char_template) return;
//
//	if(null == head)
//	{
//		head = (p_char_template)malloc(sizeof(char_template));
//	}
//	
//	p_char_template->next = head->next;
//	head->next = p_char_template;
//}
//
////sc_features: Shape Context features.
////sc_paramter: The parameter of shape context.
////return: The list of char template.
//p_char_template load_char_lib()
//{
//
//	
//	p_char_template head = (p_char_template)malloc(sizeof(char_template));
//	
//	char pathWild[1024];
//
//	strcpy(pathWild,TEMPLATE_PATH);
//	strcat(pathWild,"\\*.jpg");
//
//	struct _finddata_t c_file;
//	long hFile;
//
//	head->next = null;
//	if( (hFile = _findfirst( LPCTSTR(pathWild), &c_file )) == -1L )
//	{
//		return null;
//	}
//	else
//	{
//		do {
//			char temp_path[1024];
//			strcpy(pathWild,TEMPLATE_PATH);
//			strcat(temp_path,"\\");
//			strcat(temp_path,c_file.name);
//			p_char_template ct = (p_char_template)malloc(sizeof(char_template));
//			ct->sc_res = convert_img_2_sc(temp_path);
//			insert_char_template(head,ct);
//		} while (_findnext(hFile, &c_file) == 0);
//	
//	}
//	_findclose(hFile);
//	return head;
//}
//
