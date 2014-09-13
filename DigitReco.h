// DigitReco.h : main header file for the DIGITRECO application
//

#if !defined(AFX_DIGITRECO_H__08DC1C9A_DCB6_484D_8A36_40C730E63E8F__INCLUDED_)
#define AFX_DIGITRECO_H__08DC1C9A_DCB6_484D_8A36_40C730E63E8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDigitRecoApp:
// See DigitReco.cpp for the implementation of this class
//

class CDigitRecoApp : public CWinApp
{
public:
	CDigitRecoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDigitRecoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDigitRecoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIGITRECO_H__08DC1C9A_DCB6_484D_8A36_40C730E63E8F__INCLUDED_)
