#pragma once

#ifdef _WIN32_WCE
#error "Windows CE 不支持 CDHtmlDialog。"
#endif 

// CTestModeDlg 对话框
#include "CharRecoHelper.h"
#include "SCMatch.h"

class CTestModeDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CTestModeDlg)

public:
	CTestModeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestModeDlg();
// 重写
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// 对话框数据
	enum { IDD = IDD_DIALOG_TEST_MODE, IDH = IDR_HTML_TESTMODEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	void DrawPicToHDC(IplImage *img, UINT ID);
	afx_msg void OnBnClickedBtnTrainBowser();
	afx_msg void OnBnClickedBtnTestBowser();
	afx_msg void OnBnClickedBtnStartObserve();
	afx_msg void OnBnClickedBtnStatistics();
	afx_msg void OnBnClickedBtnTrainNext1();
	afx_msg void OnBnClickedBtnTestNext();

private:
	CString m_strTrainPicFolderPath;
	CString m_strTestPicFolderPath;
	IplImage* m_curTrainImage;
	IplImage* m_curTestImage;

	shape_context_prob m_sc_prob;
	SCMatch* m_pCharMatch;
	CharRecoHelper* m_pCharRecoHelper;
};
