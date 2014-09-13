#ifndef _CHARIMAGE2EDGE_H_
#define _CHARIMAGE2EDGE_H_

#include "IImage2Edge.h"
#include "shapecontext.h"
class CharRecoHelper:public IImage2Edge
{
public:
	CharRecoHelper();
	IplImage* Convert2Edge(const IplImage* img);
	char ConvertFileToChar(char* fileName);

public:
	char* GetTemplatePath(){return templatePath;}
	shape_context_prob GetShapeContextPara(){return sc_prob;}
	int GetExtractPoints(){return nExtractPoints;}

private:
	char* templatePath;
	shape_context_prob sc_prob;
	int nExtractPoints;
};
#endif