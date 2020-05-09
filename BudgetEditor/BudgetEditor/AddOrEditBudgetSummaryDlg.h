#pragma once
#include "afxwin.h"
#include "BudgetEditorDoc.h"


// AddOrEditBudgetSummaryDlg dialog

class AddOrEditBudgetSummaryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AddOrEditBudgetSummaryDlg)

public:
	AddOrEditBudgetSummaryDlg(CWnd* pParent = NULL);   // standard constructor
	AddOrEditBudgetSummaryDlg(bool isEditDialog, CWnd* pParent = NULL);   // standard constructor
	virtual ~AddOrEditBudgetSummaryDlg();

public:
	virtual BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDOREDITBUDGETSUMMARYDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	BudgetSummary *m_BudgetSummary;
	CStringArray *m_Categories;
	bool IsSummaryModified();
private:
	CListBox *m_SummayItemListBox;
	CComboBox *m_CategoryComboBox;
	bool m_Editing;
	bool m_IsSummaryModified;
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDeleteselected();
	afx_msg void OnEnUpdateEditPeriodyear();
	afx_msg void OnEnUpdateEditPeriodmonth();
	afx_msg void OnEnUpdateEditIncome();
};
