// DigitRecoDlg.h : header file
//

#if !defined(AFX_DIGITRECODLG_H__08F7CDB1_94DE_444A_8D17_90F75E91D015__INCLUDED_)
#define AFX_DIGITRECODLG_H__08F7CDB1_94DE_444A_8D17_90F75E91D015__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDigitRecoDlg dialog
#include "CharRecoHelper.h"
#include "SCMatch.h"
#include "afxwin.h"
class CDigitRecoDlg : public CDialog
{
// Construction
public:
	CDigitRecoDlg(CWnd* pParent = NULL);	// standard constructor
	~CDigitRecoDlg();
// Dialog Data
	//{{AFX_DATA(CDigitRecoDlg)
	enum { IDD = IDD_DIGITRECO_DIALOG };
	CStatic	m_pic;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDigitRecoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDigitRecoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	IplImage* m_imgBk;
	void DrawPicToHDC(IplImage *img, UINT ID);
	void ShowBackGround(int width,int height,UINT id);
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnStart();
private:
	int m_nbins_theta;
	int m_nbins_r;
	float m_r_inner;
	float m_r_outner;
	
public:
	void ReleaseAray(int** arr,int n)
	{
		for(int i=0;i<n;i++)
		{
			free(arr[i]);
			arr[i] = NULL;
		}
		free(arr);
		arr = NULL;
	}
	void ReleaseAray(float** arr,int n)
	{
		for(int i=0;i<n;i++)
		{
			free(arr[i]);
			arr[i] = NULL;
		}
		free(arr);
		arr = NULL;
	}
	//将图片转换成纹理图
	IplImage* ConvertImage2Edge(const IplImage* src);
	//从一个图片文件中读取所有点
	int LoadTemplatePoints(char* img_name,point** pts);
	//加载模版
	void LoadTemplates();
	
	//开始识别
	int StartReco();
	CString m_strRec;
	CEdit m_editRec;
	afx_msg void OnMenuHr();
	afx_msg void OnSave();

	shape_context_prob m_sc_prob;
	SCMatch* m_pCharMatch;
	CharRecoHelper* m_pCharRecoHelper;
	afx_msg void OnTest();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGITRECODLG_H__08F7CDB1_94DE_444A_8D17_90F75E91D015__INCLUDED_)
