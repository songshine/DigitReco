// DigitRecoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DigitReco.h"
#include "DigitRecoDlg.h"
#include "shapecontext.h"
#include "utility.h"
#include "hungarian.h"
#include "img_sc.h"
#include "SCMatch.h"
#include "HandRecognition.h"
#include "TestModeDlg.h"
#define max(x,y)  ((x)>(y)?(x):(y))
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDigitRecoDlg dialog

CDigitRecoDlg::CDigitRecoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDigitRecoDlg::IDD, pParent)
	, m_strRec(_T(""))
{
	//{{AFX_DATA_INIT(CDigitRecoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_imgBk = NULL;
	m_sc_prob.nbins_theta = 12;
	m_sc_prob.nbins_r = 5;
	m_sc_prob.r_inner = 0.125;
	m_sc_prob.r_outer = 2.0;
	m_strRec = "";

	
	m_pCharRecoHelper = new CharRecoHelper();
	m_pCharMatch = new SCMatch(
		m_pCharRecoHelper->GetTemplatePath(),
		m_pCharRecoHelper,
		m_pCharRecoHelper->GetExtractPoints());
	
}
CDigitRecoDlg::~CDigitRecoDlg()
{
	if(m_imgBk != NULL)
		cvReleaseImage(&m_imgBk);
	m_pCharMatch->FreeLibrary();
	
}
void CDigitRecoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDigitRecoDlg)
	DDX_Control(pDX, IDC_PIC, m_pic);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT2, m_strRec);
	DDX_Control(pDX, IDC_EDIT2, m_editRec);
}

BEGIN_MESSAGE_MAP(CDigitRecoDlg, CDialog)
	//{{AFX_MSG_MAP(CDigitRecoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CDigitRecoDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_Start, &CDigitRecoDlg::OnBnClickedBtnStart)
	ON_COMMAND(ID_MENU_HR, &CDigitRecoDlg::OnMenuHr)
	ON_COMMAND(ID_SAVE, &CDigitRecoDlg::OnSave)
	ON_COMMAND(ID_TEST, &CDigitRecoDlg::OnTest)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDigitRecoDlg message handlers

BOOL CDigitRecoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CRect rect;
	m_pic.GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_imgBk = cvCreateImage(cvSize(rect.Width(),rect.Height()),IPL_DEPTH_8U,1);
	cvZero(m_imgBk);
	m_pCharMatch->LoadSCLibrary();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDigitRecoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDigitRecoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		
		//UpdateWindow(); 
		//ShowBackGround(rect.Width(),rect.Height(),IDC_PIC);
		DrawPicToHDC(m_imgBk,IDC_PIC);
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDigitRecoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CDigitRecoDlg::DrawPicToHDC(IplImage *img, UINT ID)
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
void CDigitRecoDlg::ShowBackGround(int width,int height,UINT id)
{
	if(m_imgBk != NULL)
		cvReleaseImage(&m_imgBk);

	m_imgBk = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
	cvZero(m_imgBk);
	DrawPicToHDC(m_imgBk,id);

}

void CDigitRecoDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	 
	CRect rect ;
	static CvPoint ptOld = cvPoint(0,0);	
	m_pic.GetWindowRect(&rect);
	ScreenToClient(&rect);
	if(nFlags == MK_LBUTTON && PtInRect(&rect,point))
	{
		point.x -= rect.left;
		point.y -= rect.top;

		if(ptOld.x == 0 && ptOld.y == 0)
		{
			ptOld.x = point.x;
			ptOld.y = point.y;
		}
		else
		{
			cvLine(m_imgBk,ptOld,cvPoint(point.x,point.y),CV_RGB(255,255,255),3);
			ptOld.x = point.x;
			ptOld.y = point.y;
		}
		//UpdateWindow();
		DrawPicToHDC(m_imgBk,IDC_PIC);

	}
	else
	{
		ptOld.x = 0;
		ptOld.y = 0;
	}


	CDialog::OnMouseMove(nFlags, point);
}

void CDigitRecoDlg::OnBnClickedBtnClear()
{//点击clear
	// TODO: 在此添加控件通知处理程序代码
	if(m_imgBk == NULL) return;
	cvZero(m_imgBk);
	UpdateWindow();
	DrawPicToHDC(m_imgBk,IDC_PIC);

}

void CDigitRecoDlg::OnBnClickedBtnStart()
{//点击开始
	// TODO: 在此添加控件通知处理程序代码

	//just for fun
	static int id = 0;
	char result[128];
	
	UpdateData(1);
	m_strRec = "";
	m_editRec.SetWindowText(m_strRec);
	GetDlgItem(IDC_STATIC_RESULT)->SetWindowText("");
	long timeStart = GetTickCount();
	if(m_imgBk != NULL)
	{
		
		PMatchShapeContextResult result = m_pCharMatch->AsyncMatch(m_imgBk);
		CharRecoHelper helper;
		if(result != NULL)
		{
			CString resultStr;
			resultStr.Format("%c %f\r\n%c %f\r\n%c %f\r\nTime:%f",
				helper.ConvertFileToChar(result->firstMatchResult),
				result->firstPay,
				helper.ConvertFileToChar(result->secondMatchResult),
				result->secondPay,
				helper.ConvertFileToChar(result->thirdMatchResult),
				result->thirdPay,
				(float)(GetTickCount()-timeStart)/1000);
			m_editRec.SetWindowText(resultStr);

			m_pCharMatch->FreeMatchResult(&result);
		}
		else
		{
			m_editRec.SetWindowText("Could not recongnize.");
		}

	}
	
	

}
void CDigitRecoDlg::OnMenuHr()
{
	// TODO: 在此添加命令处理程序代码
	PGesturePara pGp = GetGestureFromImage("F:\\.net program\\DigitReco\\gesture\\3.jpg",200);
	//GrapGestureFromCamera(10);
	if(pGp == NULL)
		return;
	IplImage* img = cvCreateImage(cvGetSize(pGp->img),IPL_DEPTH_8U,1);
	cvZero(img);
	for(int i=0;i<100;i++)
	{
		cvCircle(img,cvPoint(pGp->points[i].x,pGp->points[i].y),2,CV_RGB(255,255,255),1);
	}
	cvShowImage("img",img);
	cvWaitKey(0);
}

void CDigitRecoDlg::OnSave()
{
	CFileDialog fileDlg(FALSE,_T("*.jpg"),_T(""),
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("JPG Files(*.jpg)|*.jpg|"));
	fileDlg.m_ofn.lpstrTitle = "Save JPG";
	if(IDOK == fileDlg.DoModal())
	{
		
		CString strPath = fileDlg.GetPathName();
		cvSaveImage(strPath, m_imgBk);
	}
}

void CDigitRecoDlg::OnTest()
{
	CTestModeDlg dlg;
	dlg.DoModal();
}
