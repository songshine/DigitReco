// TestModeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DigitReco.h"
#include "TestModeDlg.h"
#include "DigitReco.h"
#include "DigitRecoDlg.h"
#include "shapecontext.h"
#include "utility.h"
#include "hungarian.h"
#include "img_sc.h"
#include "SCMatch.h"
#include "HandRecognition.h"
#include "TestModeDlg.h"
#include <io.h>

// CTestModeDlg �Ի���

IMPLEMENT_DYNCREATE(CTestModeDlg, CDHtmlDialog)

CTestModeDlg::CTestModeDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CTestModeDlg::IDD, CTestModeDlg::IDH, pParent)
{

	m_pCharRecoHelper = new CharRecoHelper();
	m_pCharMatch = new SCMatch(
		m_pCharRecoHelper->GetTemplatePath(),
		m_pCharRecoHelper,
		m_pCharRecoHelper->GetShapeContextPara(),
		m_pCharRecoHelper->GetExtractPoints());
}

CTestModeDlg::~CTestModeDlg()
{
	m_pCharMatch->FreeLibrary();
}

void CTestModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CTestModeDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	m_pCharMatch->LoadSCLibrary();

	CRect rect;
	GetDlgItem(IDC_STATIC_TEST_PIC)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_curTestImage = cvCreateImage(cvSize(rect.Width(),rect.Height()),IPL_DEPTH_8U,1);
	cvZero(m_curTestImage);

	GetDlgItem(IDC_STATIC_TRAIN_PIC)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_curTrainImage = cvCreateImage(cvSize(rect.Width(),rect.Height()),IPL_DEPTH_8U,1);
	cvZero(m_curTrainImage);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BEGIN_MESSAGE_MAP(CTestModeDlg, CDHtmlDialog)
	ON_BN_CLICKED(IDC_BTN_TRAIN_BOWSER, &CTestModeDlg::OnBnClickedBtnTrainBowser)
	ON_BN_CLICKED(IDC_BTN_TEST_BOWSER, &CTestModeDlg::OnBnClickedBtnTestBowser)
	ON_BN_CLICKED(IDC_BTN_START_OBSERVE, &CTestModeDlg::OnBnClickedBtnStartObserve)
	ON_BN_CLICKED(IDC_BTN_STATISTICS, &CTestModeDlg::OnBnClickedBtnStatistics)
	ON_BN_CLICKED(IDC_BTN_TRAIN_NEXT1, &CTestModeDlg::OnBnClickedBtnTrainNext1)
	ON_BN_CLICKED(IDC_BTN_TEST_NEXT, &CTestModeDlg::OnBnClickedBtnTestNext)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CTestModeDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CTestModeDlg ��Ϣ�������

HRESULT CTestModeDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;
}

HRESULT CTestModeDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;
}
void CTestModeDlg::DrawPicToHDC(IplImage *img, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;

	cimg.CopyOf(img,3);
	cimg.DrawToHDC(hDC,&rect);

	ReleaseDC(pDC);
}
void CTestModeDlg::OnBnClickedBtnTrainBowser()
{
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner =GetSafeHwnd();
	bInfo.lpszTitle = _T("��ѡģ��ͼƬ�����ļ��е�·��: ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS; 

	LPITEMIDLIST lpDlist; //�������淵����Ϣ��IDList
	lpDlist = SHBrowseForFolder(&bInfo) ; //��ʾѡ��Ի���
	if(lpDlist != NULL) //�û�����ȷ����ť
	{
		TCHAR chPath[MAX_PATH]; //�����洢·�����ַ���
		SHGetPathFromIDList(lpDlist, chPath);//����Ŀ��ʶ�б�ת�����ַ���
		m_strTrainPicFolderPath = chPath; //��TCHAR���͵��ַ���ת��ΪCString���͵��ַ���


		GetDlgItem(IDC_EDIT_TRAIN_LIB)->SetWindowText(m_strTrainPicFolderPath);
		UpdateData(FALSE);
	}
}

void CTestModeDlg::OnBnClickedBtnTestBowser()
{
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner =GetSafeHwnd();
	bInfo.lpszTitle = _T("��ѡ����ͼƬ�����ļ��е�·��: ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS; 

	LPITEMIDLIST lpDlist; //�������淵����Ϣ��IDList
	lpDlist = SHBrowseForFolder(&bInfo) ; //��ʾѡ��Ի���
	if(lpDlist != NULL) //�û�����ȷ����ť
	{
		TCHAR chPath[MAX_PATH]; //�����洢·�����ַ���
		SHGetPathFromIDList(lpDlist, chPath);//����Ŀ��ʶ�б�ת�����ַ���
		m_strTestPicFolderPath = chPath; //��TCHAR���͵��ַ���ת��ΪCString���͵��ַ���

		GetDlgItem(IDC_EDIT_TEST__LIB)->SetWindowText(m_strTestPicFolderPath);
		UpdateData(FALSE);
	}
}

void CTestModeDlg::OnBnClickedBtnStartObserve()
{

	CString pathWild = m_strTestPicFolderPath + "\\*.jpg";
	struct _finddata_t c_file;
	long hFile;

	if( (hFile = _findfirst( LPCTSTR(pathWild), &c_file )) == -1L )
	{
		return;
	}
	else
	{
		do {
			CString str = m_strTestPicFolderPath;
			str += "\\";
			str += c_file.name;
			m_curTestImage = cvLoadImage((LPSTR)(LPCTSTR)str);
			PMatchShapeContextResult pMatchShpaeContextResult = m_pCharMatch->Match(m_curTestImage);
			if(pMatchShpaeContextResult != null)
			{
				//InertSCTemplateNode(c_file.name, pShpaeContextResult);
				char trainPicPath[1024];
				strcpy(trainPicPath, m_pCharRecoHelper->GetTemplatePath());
				strcat(trainPicPath, pMatchShpaeContextResult->firstMatchResult);
				m_curTrainImage = cvLoadImage(trainPicPath);

				DrawPicToHDC(m_curTestImage, IDC_STATIC_TEST_PIC);
				DrawPicToHDC(m_curTrainImage, IDC_STATIC_TRAIN_PIC);
				Sleep(100);

				
				cvReleaseImage(&m_curTestImage);
				cvReleaseImage(&m_curTrainImage);
				
			}

		} while (_findnext(hFile, &c_file) == 0);

	}
	_findclose(hFile);
}

void CTestModeDlg::OnBnClickedBtnStatistics()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CTestModeDlg::OnBnClickedBtnTrainNext1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CTestModeDlg::OnBnClickedBtnTestNext()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
