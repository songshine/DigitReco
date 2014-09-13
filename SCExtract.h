#ifndef _SCEXTRACT_H_
#define _SCEXTRACT_H_
#include "IImage2Edge.h"
#include "shapecontext.h"
#include <cv.h>

typedef struct _sc_result
{
	int nvectors; //The number of vecrors
	int ncol;     // The number of col.
	float** sc_feature; // The Shape context features.
}ShapeContextResult, *PShapeContextResult;

class SCExtract
{
public:
	SCExtract(IImage2Edge* pImage2Edge);
	SCExtract(IImage2Edge* pImage2Edge,int nExtractPoints);
	SCExtract(IImage2Edge* pImage2Edge, shape_context_prob sc_prob, int nExtractPoints);
public:
	/*Updates parameters*/
	void SetImage2EdgeConvert(IImage2Edge* pImage2Edge);
	void SetShapeContextParam(shape_context_prob sc_prob);
	void SetExtractPoints(int nExtractPoints);
	void SetParam(shape_context_prob sc_prob, int nExtractPoints);
	void SetParam(IImage2Edge* pImage2Edge, shape_context_prob sc_prob, int nExtractPoints);
public:
	PShapeContextResult ConvertImage2ShapeContext(const IplImage* img);
	PShapeContextResult ConvertImage2ShapeContext(const char* img_path);
public:
	bool FreeShapeContextResult(PShapeContextResult pShapeContextResult);
private:
	IImage2Edge* pImage2Edge;
	shape_context_prob scProb;
	int nExtractPoints;

	static const int defaultBins_r = 5;
	static const int defaultBins_theta =  12;
	static const float defaultR_inner;
	static const float defaultR_outer;
	static const int defaultExtractPoints = 60;
};
#endif