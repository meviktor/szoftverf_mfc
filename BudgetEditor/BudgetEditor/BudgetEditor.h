
// BudgetEditor.h : main header file for the BudgetEditor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CBudgetEditorApp:
// See BudgetEditor.cpp for the implementation of this class
//

class CBudgetEditorApp : public CWinApp
{
public:
	CBudgetEditorApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBudgetEditorApp theApp;
