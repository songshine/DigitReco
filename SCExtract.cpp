#include "StdAfx.h"
#include "SCExtract.h"
#include "utility.h"
#include "shapecontext.h"
#include "IImage2Edge.h"
#include "img_sc.h"


const float SCExtract::defaultR_inner = 0.125;
const float SCExtract::defaultR_outer = 2.0;

SCExtract::SCExtract(IImage2Edge* pImage2Edge)
{
	shape_context_prob defultSc_prob;
	defultSc_prob.nbins_r = defaultBins_r;
	defultSc_prob.nbins_theta = defaultBins_theta;
	defultSc_prob.r_inner = defaultR_inner;
	defultSc_prob.r_outer = defaultR_outer;
	
	SetParam(pImage2Edge, defultSc_prob, defaultExtractPoints);
}
SCExtract::SCExtract(IImage2Edge* pImage2Edge,int nExtractPoints)
{
	shape_context_prob defultSc_prob;
	defultSc_prob.nbins_r = defaultBins_r;
	defultSc_prob.nbins_theta = defaultBins_theta;
	defultSc_prob.r_inner = defaultR_inner;
	defultSc_prob.r_outer = defaultR_outer;

	SetParam(pImage2Edge, defultSc_prob, nExtractPoints);
}
SCExtract::SCExtract(IImage2Edge* pImage2Edge, shape_context_prob sc_prob, int nExtractPoints)
{
	SetParam(pImage2Edge, sc_prob, nExtractPoints);	
}

void SCExtract::SetImage2EdgeConvert(IImage2Edge* pImage2Edge)
{
	this->pImage2Edge = pImage2Edge;
}
void SCExtract::SetShapeContextParam(shape_context_prob sc_prob)
{
	this->scProb.mode = sc_prob.mode;
	this->scProb.nbins_r = sc_prob.nbins_r;
	this->scProb.nbins_theta = sc_prob.nbins_theta;
	this->scProb.r_inner = sc_prob.r_inner;
	this->scProb.r_outer = sc_prob.r_outer;
}
void SCExtract::SetExtractPoints(int nExtractPoints)
{
	this->nExtractPoints = nExtractPoints;
}
void SCExtract::SetParam(shape_context_prob sc_prob, int nExtractPoints)
{
	this->scProb.mode = sc_prob.mode;
	this->scProb.nbins_r = sc_prob.nbins_r;
	this->scProb.nbins_theta = sc_prob.nbins_theta;
	this->scProb.r_inner = sc_prob.r_inner;
	this->scProb.r_outer = sc_prob.r_outer;

	this->nExtractPoints = nExtractPoints;
}
void SCExtract::SetParam(IImage2Edge* pImage2Edge, shape_context_prob sc_prob, int nExtractPoints)
{
	this->pImage2Edge = pImage2Edge;

	this->scProb.mode = sc_prob.mode;
	this->scProb.nbins_r = sc_prob.nbins_r;
	this->scProb.nbins_theta = sc_prob.nbins_theta;
	this->scProb.r_inner = sc_prob.r_inner;
	this->scProb.r_outer = sc_prob.r_outer;

	this->nExtractPoints = nExtractPoints;
}

bool SCExtract::FreeShapeContextResult(PShapeContextResult pShapeContextResult)
{
	free_2((void**)(pShapeContextResult->sc_feature),pShapeContextResult->nvectors);
	free(pShapeContextResult);
	return true;
}

PShapeContextResult SCExtract::ConvertImage2ShapeContext(const IplImage* img)
{
		IplImage* img_edge = NULL;
		PShapeContextResult psc_result = null;
		float mean_dist = 0.0;
		if(img == null) return null;
		
		img_edge = pImage2Edge->Convert2Edge(img);
		
		
		point* pts = GetContourPointRandom(img,nExtractPoints);
	
		if(null == pts) return null;
	
		psc_result = (PShapeContextResult)malloc(sizeof(ShapeContextResult));
		psc_result->sc_feature = async_sc_compute(
			pts,nExtractPoints,
			&mean_dist,
			this->scProb.nbins_theta,
			this->scProb.nbins_r,
			this->scProb.r_inner,
			this->scProb.r_outer);
		psc_result->nvectors = nExtractPoints;
		psc_result->ncol = this->scProb.nbins_r*this->scProb.nbins_theta;
	
		cvReleaseImage(&img_edge);
		free(pts);
		return psc_result;
}
PShapeContextResult SCExtract::ConvertImage2ShapeContext(const char* img_path)
{
		IplImage* img = cvLoadImage(img_path,0);

		PShapeContextResult psc_result = ConvertImage2ShapeContext(img);	
	
		cvReleaseImage(&img);
		return psc_result;
}