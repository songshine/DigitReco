#ifndef _SCMATCH_H_
#define _SCMATCH_H_
#include "IImage2Edge.h"
#include "shapecontext.h"
#include "SCExtract.h"
#include "SCLibrary.h"

typedef struct  
{
	char* firstMatchResult;
	float firstPay;

	char* secondMatchResult;
	float secondPay;

	char* thirdMatchResult;
	float thirdPay;
}MatchShapeContextResult, *PMatchShapeContextResult;

struct MatchThreadPara
{
	PShapeContextTemplateNode startTemplateNode;
	int templateNum;
	PShapeContextResult pShapeContextResult;
	MatchShapeContextResult** pMatchResult;
};
class SCMatch
{
public:
	/*Contruct methods*/
	SCMatch(const char* lib_path,  IImage2Edge* pImg2Edge);
	SCMatch(const char* lib_path,  IImage2Edge* pImg2Edge, int nExractPoints);
	SCMatch(const char* lib_path, IImage2Edge* pImg2Edge, shape_context_prob sc_pram, int nExractPoints);
public:
	/*Inits those important parameter*/
	//Loads the shape constext template.(Only support Jpeg)
	void SetExtractPoints(int newExtractPoints, bool reloadLib);
	void SetImage2Edge(IImage2Edge* pImg2Edg, bool reloadLib);
	void SetShapeContextParam(shape_context_prob sc_prob, bool reloadLib);
public:
	//Loads the shape constext template.(Only support Jpeg)
	bool LoadSCLibrary();
	bool LoadSCLibrary(const char* lib_path);

public:
	PMatchShapeContextResult AsyncMatch(IplImage* img);
public:

	PMatchShapeContextResult Match(IplImage* img);
	PMatchShapeContextResult Match2(const char* img_path);
	void FreeLibrary();
	void FreeMatchResult(MatchShapeContextResult** pMatchResult);
private:
	static float MatchShapeContext(PShapeContextResult pResult1, PShapeContextResult pResult2);

	HANDLE AsyncMatchWork(
		PShapeContextTemplateNode startTemplateNode,
		int templateNum,
		PShapeContextResult pShapeContextResult,
		MatchShapeContextResult** pMatchShapeContextReslut);

	PMatchShapeContextResult MergeMatchResult(MatchShapeContextResult** pMatchResult, int startIndex, int endIndex);
public:
	static DWORD WINAPI ShapeContextMatchThread(LPDWORD lpdwParam);


private:
	float fVpv;
	bool isLoadLibrary;
	SCExtract* pscExtract;
	SCLibrary* pscLibrary;
};
#endif