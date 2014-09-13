#include "stdafx.h"
#include "SCLibrary.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <io.h>

SCLibrary::SCLibrary(const char* libraryPath, SCExtract* pSCExtract)
{
	this->libraryPath = copy_str(libraryPath);
	this->pSCExtract = pSCExtract;

	scTemplateNumber = 0;
	pTemplateNodeHead = (PShapeContextTemplateNode)malloc(sizeof(ShapeContextTemplateNode));
	pTemplateNodeHead->next = null;
	pCurrentTemplateNode = null;
	isLoadLibrary = false;
}
void SCLibrary::InertSCTemplateNode(char* templateName, PShapeContextResult pShapeContextResult)
{
		if(null == pTemplateNodeHead)
		{
			pTemplateNodeHead = (PShapeContextTemplateNode)malloc(sizeof(ShapeContextTemplateNode));
			pTemplateNodeHead->next = null;
		}
		
		PShapeContextTemplateNode pTemplateNode = (PShapeContextTemplateNode)malloc(sizeof(ShapeContextTemplateNode));
		pTemplateNode->pShapeContextTemplate = (PShapeContextTemplate)malloc(sizeof(ShapeContextTemplate));
		pTemplateNode->pShapeContextTemplate->fileName = copy_str(templateName);
		pTemplateNode->pShapeContextTemplate->pShapeContextResult = pShapeContextResult;

		pTemplateNode->next = pTemplateNodeHead->next;
		pTemplateNodeHead->next = pTemplateNode;
		scTemplateNumber ++;
}
void SCLibrary::FreeShapeContextLibrary()
{
	PShapeContextTemplateNode node = pTemplateNodeHead->next;
	PShapeContextTemplateNode nextNode = node;

	while(node != null)
	{

		nextNode = node->next;
		pSCExtract->FreeShapeContextResult(node->pShapeContextTemplate->pShapeContextResult);
		free(node->pShapeContextTemplate->fileName);
		free(node->pShapeContextTemplate);
		free(node);
		node = nextNode;

	}
	free(pTemplateNodeHead);
	scTemplateNumber = 0;
	isLoadLibrary = false;
}
bool SCLibrary::UpdateSCExtract(SCExtract* pSCExtract)
{
	this->pSCExtract = pSCExtract;
	return true;
}
bool SCLibrary::UpdateLibrary(const char* libraryPath )
{
	if(libraryPath != null)
	{
		if(this->libraryPath != null)
		{
			free(this->libraryPath);			
		}

		this->libraryPath = copy_str(libraryPath);
	}
	if(isLoadLibrary)
	{
		FreeShapeContextLibrary();
	}

	return LoadShapeContextLib();
}
bool SCLibrary::LoadShapeContextLib()
{
	
	if(isLoadLibrary)
	{
		FreeShapeContextLibrary();
	}

	char pathWild[1024];

	strcpy(pathWild,libraryPath);
	strcat(pathWild,"\\*.jpg");

	struct _finddata_t c_file;
	long hFile;

	if( (hFile = _findfirst( LPCTSTR(pathWild), &c_file )) == -1L )
	{
		return false;
	}
	else
	{
		do {
			char temp_path[1024];
			strcpy(temp_path,libraryPath);
			strcat(temp_path,"\\");
			strcat(temp_path,c_file.name);
			PShapeContextResult pShpaeContextResult = pSCExtract->ConvertImage2ShapeContext(temp_path);
			if(pShpaeContextResult != null)
			{
				InertSCTemplateNode(c_file.name, pShpaeContextResult);
			}
			
		} while (_findnext(hFile, &c_file) == 0);

	}
	_findclose(hFile);

	isLoadLibrary = true;
	return true;
		
		
}

PShapeContextTemplate SCLibrary::FirstTemplate()
{
	assert(pTemplateNodeHead != null);	
	pCurrentTemplateNode = pTemplateNodeHead->next;
	if(pCurrentTemplateNode != null)
	{
		return pTemplateNodeHead->next->pShapeContextTemplate;
	}
	else
	{
		return null;
	}
	
}
PShapeContextTemplate SCLibrary::NextTemplate()
{
	PShapeContextTemplate pShapeContextTemplate = null;
	if(pCurrentTemplateNode != null)
	{
		pCurrentTemplateNode = pCurrentTemplateNode->next;
		if(pCurrentTemplateNode != null)
		{
			pShapeContextTemplate = pCurrentTemplateNode->pShapeContextTemplate;
		}
	}
	return pShapeContextTemplate;
	
}

PShapeContextTemplate SCLibrary::SpecialTemplate(int index)
{
	if(index >= scTemplateNumber) return null;
	pCurrentTemplateNode = pTemplateNodeHead->next;
	int iterIndex = 0;

	while(iterIndex++ < index)
	{
		pCurrentTemplateNode = pCurrentTemplateNode->next;
	}
	if(pCurrentTemplateNode != null)
	{
		return pCurrentTemplateNode->next->pShapeContextTemplate;
	}
	else
	{
		return null;
	}
}

PShapeContextTemplateNode SCLibrary::FirstTemplateNode()
{
	assert(pTemplateNodeHead != null);	
	return pTemplateNodeHead->next;
}
PShapeContextTemplateNode SCLibrary::SpecialTemplateNode(int index)
{
	if(index >= scTemplateNumber) return null;
	int interIndex = 0;
	PShapeContextTemplateNode templateNode = pTemplateNodeHead->next;
	while(interIndex++ < index)
	{
		templateNode = templateNode->next;
	}

	return templateNode;
}