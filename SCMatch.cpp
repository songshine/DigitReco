#include "StdAfx.h"
#include "SCMatch.h"
#include "SCExtract.h"
#include "SCLibrary.h"
#include "IImage2Edge.h"
#include "hungarian.h"
#include "utility.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_THREAD_NUM 8


SCMatch::SCMatch(const char* lib_path,  IImage2Edge* pImg2Edge)
{
	assert(lib_path != null);
	assert(pImg2Edge != null);

	this->pscExtract = new SCExtract(pImg2Edge);
	this->pscLibrary = new SCLibrary(lib_path, this->pscExtract);
	fVpv = 0.15;
	isLoadLibrary = false;
}
SCMatch::SCMatch(const char* lib_path,  IImage2Edge* pImg2Edge, int nExractPoints)
{
	assert(lib_path != null);
	assert(pImg2Edge != null);
	assert(nExractPoints > 0);

	this->pscExtract = new SCExtract(pImg2Edge,nExractPoints);
	this->pscLibrary = new SCLibrary(lib_path, this->pscExtract);
	fVpv = 0.15;
	isLoadLibrary = false;
}
SCMatch::SCMatch(const char* lib_path, IImage2Edge* pImg2Edge, shape_context_prob sc_pram,int nExractPoints)
{
	assert(lib_path != null);
	assert(pImg2Edge != null);
	assert(nExractPoints > 0);

	this->pscExtract = new SCExtract(pImg2Edge, sc_pram,nExractPoints);
	this->pscLibrary = new SCLibrary(lib_path, this->pscExtract);
	fVpv = 0.15;
	isLoadLibrary = false;
}
void SCMatch::SetExtractPoints(int newExtractPoints, bool reloadLib)
{
	assert(newExtractPoints > 0);

	this->pscExtract->SetExtractPoints(newExtractPoints);
	if(reloadLib)
	{
		this->pscLibrary->LoadShapeContextLib();
	}
}
void SCMatch::SetImage2Edge(IImage2Edge* pImg2Edg, bool reloadLib)
{
	assert(pImg2Edg != null);
	this->pscExtract->SetImage2EdgeConvert(pImg2Edg);

	if(reloadLib)
	{
		this->pscLibrary->LoadShapeContextLib();
	}
}
void SCMatch::SetShapeContextParam(shape_context_prob sc_prob, bool reloadLib)
{
	this->pscExtract->SetShapeContextParam(sc_prob);
	if(reloadLib)
	{
		this->pscLibrary->LoadShapeContextLib();
		isLoadLibrary = true;
	}
}
bool SCMatch::LoadSCLibrary()
{
	bool result = false;
	if(this->pscLibrary->LoadShapeContextLib())
	{
		isLoadLibrary = true;
		result = true;
	}

	return false;
	
}
bool SCMatch::LoadSCLibrary(const char* lib_path)
{
	bool result = false;
	if(this->pscLibrary->UpdateLibrary(lib_path))
	{
		isLoadLibrary = true;
		result = true;
	}
	return result;
	
}

PMatchShapeContextResult SCMatch::MergeMatchResult(MatchShapeContextResult** pMatchResult, int startIndex, int endIndex)
{
	PMatchShapeContextResult pResult = 
		(PMatchShapeContextResult)malloc(sizeof(MatchShapeContextResult));
	pResult->firstMatchResult = null;
	pResult->secondMatchResult = null;
	pResult->thirdMatchResult = null;
	if(startIndex >= endIndex) 
	{	
		if(pMatchResult[startIndex]->firstMatchResult != null)
		{
			pResult->firstMatchResult = copy_str(pMatchResult[startIndex]->firstMatchResult);
			pResult->firstPay = pMatchResult[startIndex]->firstPay;

		}
		if(pMatchResult[startIndex]->secondMatchResult != null)
		{
			pResult->secondMatchResult = copy_str(pMatchResult[startIndex]->secondMatchResult);
			pResult->secondPay = pMatchResult[startIndex]->secondPay;

		}
		if(pMatchResult[startIndex]->thirdMatchResult != null)
		{
			pResult->thirdMatchResult = copy_str(pMatchResult[startIndex]->thirdMatchResult);
			pResult->thirdPay = pMatchResult[startIndex]->thirdPay;

		}
		
		return pResult;
		
	}
	else if(startIndex < endIndex)
	{
		int midIndex = (startIndex + endIndex)/2;
		PMatchShapeContextResult pResult1 = MergeMatchResult(pMatchResult,startIndex, midIndex);
		PMatchShapeContextResult pResult2 = MergeMatchResult(pMatchResult,midIndex+1, endIndex);

		if(pResult1->firstPay < pResult2->firstPay)
		{
			pResult->firstPay = pResult1->firstPay;
			pResult->firstMatchResult = copy_str(pResult1->firstMatchResult);

			if(pResult1->secondPay < pResult2->firstPay)
			{
				pResult->secondPay = pResult1->secondPay;
				pResult->secondMatchResult = copy_str(pResult2->secondMatchResult);
				if(pResult1->thirdPay < pResult2->thirdPay)
				{
					pResult->thirdPay = pResult1->thirdPay;
					pResult->thirdMatchResult = copy_str(pResult1->thirdMatchResult);
				}
				else
				{
					pResult->thirdPay = pResult2->firstPay;
					pResult->thirdMatchResult = copy_str(pResult2->firstMatchResult);
				}
			}
			else
			{
				pResult->secondPay = pResult2->firstPay;
				pResult->secondMatchResult = copy_str(pResult2->firstMatchResult);
				if(pResult1->secondPay < pResult2->secondPay)
				{
					pResult->thirdPay = pResult1->secondPay;
					pResult->thirdMatchResult = copy_str(pResult1->secondMatchResult);
				}
				else
				{
					pResult->thirdPay = pResult2->secondPay;
					pResult->thirdMatchResult = copy_str(pResult2->secondMatchResult);
				}
			}
		}
		else
		{
			PMatchShapeContextResult pTempResult = pResult1;
			pResult1 = pResult2;
			pResult2 = pTempResult;

			pResult->firstPay = pResult1->firstPay;
			pResult->firstMatchResult = copy_str(pResult1->firstMatchResult);

			if(pResult1->secondPay < pResult2->firstPay)
			{
				pResult->secondPay = pResult1->secondPay;
				pResult->secondMatchResult = copy_str(pResult2->secondMatchResult);
				if(pResult1->thirdPay < pResult2->thirdPay)
				{
					pResult->thirdPay = pResult1->thirdPay;
					pResult->thirdMatchResult = copy_str(pResult1->thirdMatchResult);
				}
				else
				{
					pResult->thirdPay = pResult2->firstPay;
					pResult->thirdMatchResult = copy_str(pResult2->firstMatchResult);
				}
			}
			else
			{
				pResult->secondPay = pResult2->firstPay;
				pResult->secondMatchResult = copy_str(pResult2->firstMatchResult);
				if(pResult1->secondPay < pResult2->secondPay)
				{
					pResult->thirdPay = pResult1->secondPay;
					pResult->thirdMatchResult = copy_str(pResult1->secondMatchResult);
				}
				else
				{
					pResult->thirdPay = pResult2->secondPay;
					pResult->thirdMatchResult = copy_str(pResult2->secondMatchResult);
				}
			}

		}
		
		FreeMatchResult(&pResult1);
		FreeMatchResult(&pResult2);
		
		
	}

	return pResult;
	
}
PMatchShapeContextResult SCMatch::AsyncMatch(IplImage* img)
{
	PMatchShapeContextResult pMatchReslut = null;
	int templatePerThread = pscLibrary->TemplateNumber() / MAX_THREAD_NUM;
	int restTemplate = pscLibrary->TemplateNumber() % MAX_THREAD_NUM;
	if(templatePerThread < 3 )
	{
		return Match(img);
	}
	else
	{
		int startIndex = 0;
		int endIndex = 0;

		PMatchShapeContextResult pMatchShapeContextResult[MAX_THREAD_NUM];
		HANDLE hThread[MAX_THREAD_NUM];

		PShapeContextResult pShapeContextResult = pscExtract->ConvertImage2ShapeContext(img);
		if(pShapeContextResult == null) return null;

		for(int i=0;i<MAX_THREAD_NUM;i++)
		{
			
			if(i < restTemplate)
			{
				endIndex = startIndex + templatePerThread + 1;
			}
			else
			{
				endIndex = startIndex + templatePerThread;
			}

			pMatchShapeContextResult[i] = 
				(PMatchShapeContextResult)malloc(sizeof(MatchShapeContextResult));
			hThread[i] = AsyncMatchWork(
				pscLibrary->SpecialTemplateNode(startIndex),
				endIndex - startIndex,
				pShapeContextResult,
				&(pMatchShapeContextResult[i])
				);

			startIndex = endIndex;
		}

		WaitForMultipleObjects(MAX_THREAD_NUM, hThread, TRUE, INFINITE);

		pMatchReslut = MergeMatchResult(pMatchShapeContextResult, 0 , MAX_THREAD_NUM-1);	

		for(int i=0;i<MAX_THREAD_NUM;i++)
		{
			free(pMatchShapeContextResult[i]);
			CloseHandle(hThread[i]);
		}

	}

	return pMatchReslut;

}
PMatchShapeContextResult SCMatch::Match(IplImage* img)
{
	PShapeContextTemplate firstTemplate = null;
	float firstPay = 99999.0;

	PShapeContextTemplate secondTemplate = null;
	float secondPay = 99999.0;

	PShapeContextTemplate thirdTemplate = null;
	float thirdPay = 99999.0;

	PShapeContextTemplate shapeContextTemplateIter = null;
	PShapeContextResult pShapeContextResult = pscExtract->ConvertImage2ShapeContext(img);
	if(pShapeContextResult == null) return null;
	for(shapeContextTemplateIter = pscLibrary->FirstTemplate();shapeContextTemplateIter != null; shapeContextTemplateIter = pscLibrary->NextTemplate())
	{
		float pay = MatchShapeContext(pShapeContextResult, shapeContextTemplateIter->pShapeContextResult);
		if(pay < firstPay)
		{
			thirdTemplate = secondTemplate;
			thirdPay = secondPay;

			secondTemplate = firstTemplate;
			secondPay = firstPay;

			firstTemplate = shapeContextTemplateIter;
			firstPay = pay;
		}
		else if(pay < secondPay)
		{
			thirdTemplate = secondTemplate;
			thirdPay = secondPay;

			secondTemplate = shapeContextTemplateIter;
			secondPay = pay;
		}
		else if(pay < thirdPay)
		{
			thirdTemplate = shapeContextTemplateIter;
			thirdPay = pay;
		}
	}

	PMatchShapeContextResult matchSCResult = null;

	if(!(firstTemplate == null && secondTemplate == null && thirdTemplate == null))
	{
		matchSCResult = (PMatchShapeContextResult)malloc(sizeof(MatchShapeContextResult));
		if(firstTemplate != null)
		{
			matchSCResult->firstMatchResult = copy_str(firstTemplate->fileName);
			matchSCResult->firstPay = firstPay;
		}
		else
		{
			matchSCResult->firstMatchResult = null;
		}
		if(secondTemplate != null)
		{
			matchSCResult->secondMatchResult = copy_str(secondTemplate->fileName);
			matchSCResult->secondPay = secondPay;
		}
		else
		{
			matchSCResult->secondMatchResult = null;
		}
		if(thirdTemplate != null)
		{
			matchSCResult->thirdMatchResult = copy_str(thirdTemplate->fileName);
			matchSCResult->thirdPay = thirdPay;
		}
		else
		{
			matchSCResult->thirdMatchResult = null;
		}

		free(pShapeContextResult);
	}	
	
	return matchSCResult;
}
PMatchShapeContextResult SCMatch::Match2(const char* img_path)
{
	PMatchShapeContextResult result = null;
	if(img_path != null)
	{
		IplImage* img = cvLoadImage(img_path, 0);
		result = Match(img);

		cvReleaseImage(&img);
	}
	return result;

}
void SCMatch::FreeLibrary()
{
	pscLibrary->FreeShapeContextLibrary();
}
void SCMatch::FreeMatchResult(MatchShapeContextResult** pMatchResult)
{
	if(*pMatchResult != NULL)
	{
		if((*pMatchResult)->firstMatchResult != NULL)
		{
			free((*pMatchResult)->firstMatchResult);
			(*pMatchResult)->firstMatchResult = NULL;
		}

		if((*pMatchResult)->secondMatchResult != NULL)
		{
			free((*pMatchResult)->secondMatchResult);
			(*pMatchResult)->secondMatchResult = NULL;
		}

		if((*pMatchResult)->thirdMatchResult != NULL)
		{
			free((*pMatchResult)->thirdMatchResult);
			(*pMatchResult)->thirdMatchResult = NULL;
		}

		free(*pMatchResult);
		*pMatchResult = NULL;
	}
}
float SCMatch::MatchShapeContext(PShapeContextResult pResult1, PShapeContextResult pResult2)
{
	float pay = 100000.0;
	float** cost = null;
	int* r = null;
	int ndum = max(pResult1->nvectors, pResult2->nvectors);
	cost = hist_cost(
		pResult1->sc_feature,
		pResult1->nvectors,
		pResult2->sc_feature,
		pResult2->nvectors,
		pResult1->ncol
		);
	r = (int*)malloc(sizeof(int)*ndum*ndum);
	for(int t=0;t<ndum;t++)
	{
		for(int s=0;s<ndum;s++)
		{
			if(t < pResult1->nvectors && s < pResult2->nvectors)
				r[t*ndum+s] = (int)(cost[t][s]*1000);
			else
				r[t*ndum+s] = (int)(0.15*1000);
		}
	}

	hungarian_t prob;
	hungarian_init(&prob,r,ndum,ndum,HUNGARIAN_MIN);
	hungarian_solve(&prob);		
	if(hungarian_check_feasibility(&prob))
	{
		pay = (float)hungarian_benefit(&prob)/1000;
	}
	hungarian_fini(&prob);
	free(r);
	free_2((void**)cost, pResult1->nvectors);

	return pay;
}

HANDLE SCMatch::AsyncMatchWork(
										PShapeContextTemplateNode startTemplateNode,
										int templateNum,
										PShapeContextResult pShapeContextResult,
										MatchShapeContextResult** pMatchShapeContextReslut)
{
	struct MatchThreadPara* pMatchThreadPara = (struct MatchThreadPara*)malloc(sizeof(struct MatchThreadPara));

	pMatchThreadPara->startTemplateNode = startTemplateNode;
	pMatchThreadPara->templateNum = templateNum;
	pMatchThreadPara->pShapeContextResult = pShapeContextResult;
	pMatchThreadPara->pMatchResult = pMatchShapeContextReslut;

	HANDLE hResult = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ShapeContextMatchThread, pMatchThreadPara, 0, NULL);

	return hResult;

}

DWORD WINAPI SCMatch::ShapeContextMatchThread(LPDWORD lpdwParam)
{

	struct MatchThreadPara* pMatchThreadPara = (struct MatchThreadPara*)lpdwParam;
	PShapeContextTemplate firstTemplate = null;
	float firstPay = 99999.0;

	PShapeContextTemplate secondTemplate = null;
	float secondPay = 99999.0;

	PShapeContextTemplate thirdTemplate = null;
	float thirdPay = 99999.0;

	PShapeContextTemplateNode pShapeTemplateNode = pMatchThreadPara->startTemplateNode;
	PShapeContextTemplate shapeContextTemplateIter = null;

	for(int i=0;i<pMatchThreadPara->templateNum;i++)
	{
		shapeContextTemplateIter = pShapeTemplateNode->pShapeContextTemplate;
		float pay = MatchShapeContext(pMatchThreadPara->pShapeContextResult, shapeContextTemplateIter->pShapeContextResult);
		if(pay < firstPay)
		{
			thirdTemplate = secondTemplate;
			thirdPay = secondPay;

			secondTemplate = firstTemplate;
			secondPay = firstPay;

			firstTemplate = shapeContextTemplateIter;
			firstPay = pay;
		}
		else if(pay < secondPay)
		{
			thirdTemplate = secondTemplate;
			thirdPay = secondPay;

			secondTemplate = shapeContextTemplateIter;
			secondPay = pay;
		}
		else if(pay < thirdPay)
		{
			thirdTemplate = shapeContextTemplateIter;
			thirdPay = pay;
		}

		pShapeTemplateNode = pShapeTemplateNode->next;
	}
	if(!(firstTemplate == null && secondTemplate == null && thirdTemplate == null))
	{
		if(firstTemplate != null)
		{
			(*(pMatchThreadPara->pMatchResult))->firstMatchResult = copy_str(firstTemplate->fileName);
			(*(pMatchThreadPara->pMatchResult))->firstPay = firstPay;
		}
		else
		{
			(*(pMatchThreadPara->pMatchResult))->firstMatchResult = null;
		}
		if(secondTemplate != null)
		{
			(*(pMatchThreadPara->pMatchResult))->secondMatchResult = copy_str(secondTemplate->fileName);
			(*(pMatchThreadPara->pMatchResult))->secondPay = secondPay;
		}
		else
		{
			(*(pMatchThreadPara->pMatchResult))->secondMatchResult = null;
		}
		if(thirdTemplate != null)
		{
			(*(pMatchThreadPara->pMatchResult))->thirdMatchResult = copy_str(thirdTemplate->fileName);
			(*(pMatchThreadPara->pMatchResult))->thirdPay = thirdPay;
		}
		else
		{
			(*(pMatchThreadPara->pMatchResult))->thirdMatchResult = null;
		}

	}	

	return 0;
}