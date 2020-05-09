// EditCategoryListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetEditor.h"
#include "EditCategoryListDlg.h"
#include "afxdialogex.h"


// EditCategoryListDlg dialog

IMPLEMENT_DYNAMIC(EditCategoryListDlg, CDialogEx)

EditCategoryListDlg::EditCategoryListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EDITCATEGORYLIST, pParent)
{
}

EditCategoryListDlg::~EditCategoryListDlg()
{
}

BOOL EditCategoryListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_CategoryListBox = (CListBox*)GetDlgItem(IDC_CATEGORYLISTBOX);

	int numberOfCategories = m_Categories->GetCount();
	if (numberOfCategories > 0)
	{
		for (int i = 0; i < numberOfCategories; i++)
		{
			m_CategoryListBox->AddString((*m_Categories)[i]);
		}
	}

	return 0;
}

void EditCategoryListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

bool EditCategoryListDlg::IsListModified()
{
	return m_IsListModified;
}


BEGIN_MESSAGE_MAP(EditCategoryListDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADDNEWCATEGORY, &EditCategoryListDlg::OnBnClickedButtonAddnewcategory)
	ON_BN_CLICKED(IDC_BUTTON_DELETESELECTEDCATEGORY, &EditCategoryListDlg::OnBnClickedButtonDeleteselectedcategory)
END_MESSAGE_MAP()


// EditCategoryListDlg message handlers

void EditCategoryListDlg::OnBnClickedButtonAddnewcategory()
{
	CString newCategoryText;
	GetDlgItemText(IDC_EDIT_CATEGORYNAME, newCategoryText);
	bool isCategoryUnique = true;
	for (int i = 0; i < m_Categories->GetSize(); i++)
	{
		if ((*m_Categories)[i] == newCategoryText)
		{
			isCategoryUnique = false;
			break;
		}
	}
	bool isCategoryEmpty = newCategoryText == _T("");
	if (!isCategoryUnique || isCategoryEmpty)
	{
		MessageBox(
			_T("You provided an empty category name or this category is already exists!"),
			_T("Invalid category name"),
			MB_OK | MB_ICONWARNING | MB_APPLMODAL
		);
	}
	else
	{
		m_CategoryListBox->AddString(newCategoryText);
		m_Categories->Add(newCategoryText);
		m_IsListModified = true;
	}
}


void EditCategoryListDlg::OnBnClickedButtonDeleteselectedcategory()
{
	int selectedIndex = m_CategoryListBox->GetCurSel();
	if (selectedIndex != LB_ERR)
	{
		m_CategoryListBox->DeleteString(selectedIndex);
		m_Categories->RemoveAt(selectedIndex);
		m_IsListModified = true;
	}
}
