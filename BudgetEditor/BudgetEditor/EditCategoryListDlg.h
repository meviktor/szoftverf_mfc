#pragma once
#include "afxwin.h"


// EditCategoryListDlg dialog

class EditCategoryListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EditCategoryListDlg)

public:
	EditCategoryListDlg(CWnd* pParent = NULL);
	virtual ~EditCategoryListDlg();

public:
	virtual BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITCATEGORYLIST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	CStringArray *m_Categories;
	bool IsListModified();
private:
	CListBox  *m_CategoryListBox;
	bool m_IsListModified;
public:
	afx_msg void OnBnClickedButtonAddnewcategory();
	afx_msg void OnBnClickedButtonDeleteselectedcategory();
};
