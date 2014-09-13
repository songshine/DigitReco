#ifndef _SCLIBRARY_H_
#define _SCLIBRARY_H_
#include "SCExtract.h"

typedef struct
{
	char* fileName;
	PShapeContextResult pShapeContextResult;
}ShapeContextTemplate, *PShapeContextTemplate;


typedef struct _SC_lib_template_node
{
	PShapeContextTemplate pShapeContextTemplate;
	_SC_lib_template_node* next;
}ShapeContextTemplateNode, *PShapeContextTemplateNode;


class SCLibrary
{
public:
	SCLibrary(const char* libraryPath, SCExtract* pSCExtract);
public:
	bool UpdateLibrary(const char* libraryPath);
	bool UpdateSCExtract(SCExtract* pSCExtract);
	bool LoadShapeContextLib();
public:
	PShapeContextTemplate FirstTemplate();
	PShapeContextTemplate NextTemplate();
	PShapeContextTemplate SpecialTemplate(int index);
	int TemplateNumber(){return scTemplateNumber;}

public:
	PShapeContextTemplateNode FirstTemplateNode();
	PShapeContextTemplateNode SpecialTemplateNode(int index);
private:
	void InertSCTemplateNode(char* templateName,  PShapeContextResult pShapeContextResult);

public:
	void FreeShapeContextLibrary();
private:
	char* libraryPath;
	int scTemplateNumber;
	SCExtract* pSCExtract;
	PShapeContextTemplateNode pTemplateNodeHead;
	PShapeContextTemplateNode pCurrentTemplateNode;
	bool isLoadLibrary;
};
#endif