// AddOrEditBudgetSummaryDlg.cpp : implementation file

#include "stdafx.h"
#include "BudgetEditor.h"
#include "AddOrEditBudgetSummaryDlg.h"
#include "afxdialogex.h"

// AddOrEditBudgetSummaryDlg dialog

IMPLEMENT_DYNAMIC(AddOrEditBudgetSummaryDlg, CDialogEx)

AddOrEditBudgetSummaryDlg::AddOrEditBudgetSummaryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDOREDITBUDGETSUMMARYDLG, pParent)
{
	m_Editing = false;
	m_IsSummaryModified = false;
}

AddOrEditBudgetSummaryDlg::AddOrEditBudgetSummaryDlg(bool isEditDialog, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDOREDITBUDGETSUMMARYDLG, pParent)
{
	m_Editing = isEditDialog;
	m_IsSummaryModified = false;
}

AddOrEditBudgetSummaryDlg::~AddOrEditBudgetSummaryDlg()
{	
}

BOOL AddOrEditBudgetSummaryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	if (m_Editing) {
		SetWindowText(_T("Edit budget summary"));
		ModifyStyle(0, CS_NOCLOSE);
		((CEdit*)GetDlgItem(IDC_EDIT_PERIODYEAR))->SetReadOnly();
		((CEdit*)GetDlgItem(IDC_EDIT_PERIODMONTH))->SetReadOnly();
		((CButton*)GetDlgItem(IDOK))->ModifyStyle(WS_VISIBLE, 0);
		SetDlgItemTextW(IDCANCEL, _T("Close"));
	}
	else {
		SetWindowText(_T("Add new budget summary"));
	}

	m_SummayItemListBox = (CListBox*)GetDlgItem(IDC_LISTBOX_OUTGOINGS);
	m_CategoryComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_CATEGORY);

	int numberOfCategories = m_Categories->GetCount();
	if (numberOfCategories > 0)
	{
		for (int i = 0; i < numberOfCategories; i++)
		{
			m_CategoryComboBox->AddString((*m_Categories)[i]);
		}
	}

	int numberOfSummaryItems = m_BudgetSummary->m_SummaryItems->GetCount();
	int sumOfSummaryItems = 0;
	if (numberOfSummaryItems > 0)
	{
		for (int i = 0; i < numberOfSummaryItems; i++)
		{
			BudgetSummaryItem *summaryItem = (BudgetSummaryItem*)(*(m_BudgetSummary->m_SummaryItems))[i];
			CString summaryItemValue = _T("");
			summaryItemValue.AppendFormat(_T("Category: %s, Amount: %d HUF"), summaryItem->m_Category, summaryItem->m_Amount);
			m_SummayItemListBox->AddString(summaryItemValue);
			sumOfSummaryItems += summaryItem->m_Amount;
		}
	}

	SetDlgItemInt(IDC_EDIT_PERIODYEAR, m_BudgetSummary->m_Period.m_Year);
	SetDlgItemInt(IDC_EDIT_PERIODMONTH, m_BudgetSummary->m_Period.m_Month);
	SetDlgItemInt(IDC_EDIT_INCOME, m_BudgetSummary->m_Income);
	SetDlgItemInt(IDC_EDIT_AMOUNT, 0);
	SetDlgItemInt(IDC_EDIT_TOTAL, sumOfSummaryItems);

	return 0;
}

void AddOrEditBudgetSummaryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

bool AddOrEditBudgetSummaryDlg::IsSummaryModified()
{
	return m_IsSummaryModified;
}

BEGIN_MESSAGE_MAP(AddOrEditBudgetSummaryDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &AddOrEditBudgetSummaryDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETESELECTED, &AddOrEditBudgetSummaryDlg::OnBnClickedButtonDeleteselected)
	ON_EN_UPDATE(IDC_EDIT_PERIODYEAR, &AddOrEditBudgetSummaryDlg::OnEnUpdateEditPeriodyear)
	ON_EN_UPDATE(IDC_EDIT_PERIODMONTH, &AddOrEditBudgetSummaryDlg::OnEnUpdateEditPeriodmonth)
	ON_EN_UPDATE(IDC_EDIT_INCOME, &AddOrEditBudgetSummaryDlg::OnEnUpdateEditIncome)
END_MESSAGE_MAP()

// AddOrEditBudgetSummaryDlg message handlers

void AddOrEditBudgetSummaryDlg::OnBnClickedButtonAdd()
{
	CString selectedCategory;
	m_CategoryComboBox->GetLBText(m_CategoryComboBox->GetCurSel(), selectedCategory);
	
	CString summaryItemValue = _T("");
	summaryItemValue.AppendFormat(_T("Category: %s, Amount: %d HUF"), selectedCategory, GetDlgItemInt(IDC_EDIT_AMOUNT));
	m_SummayItemListBox->AddString(summaryItemValue);
	SetDlgItemInt(IDC_EDIT_TOTAL, GetDlgItemInt(IDC_EDIT_TOTAL) + GetDlgItemInt(IDC_EDIT_AMOUNT));

	m_BudgetSummary->m_SummaryItems->Add(new BudgetSummaryItem(selectedCategory, GetDlgItemInt(IDC_EDIT_AMOUNT)));
	m_IsSummaryModified = true;
}


void AddOrEditBudgetSummaryDlg::OnBnClickedButtonDeleteselected()
{
	int selectedIndex = m_SummayItemListBox->GetCurSel();
	if (selectedIndex != LB_ERR) {
		m_SummayItemListBox->DeleteString(selectedIndex);
		int amountOfDeletedItem = ((BudgetSummaryItem*)(*(m_BudgetSummary->m_SummaryItems))[selectedIndex])->m_Amount;
		SetDlgItemInt(IDC_EDIT_TOTAL, GetDlgItemInt(IDC_EDIT_TOTAL) - amountOfDeletedItem);
		m_BudgetSummary->m_SummaryItems->RemoveAt(selectedIndex);
		m_IsSummaryModified = true;
	}
}

void AddOrEditBudgetSummaryDlg::OnEnUpdateEditPeriodyear()
{
	m_BudgetSummary->m_Period.m_Year = GetDlgItemInt(IDC_EDIT_PERIODYEAR);
	m_IsSummaryModified = true;
}

void AddOrEditBudgetSummaryDlg::OnEnUpdateEditPeriodmonth()
{
	m_BudgetSummary->m_Period.m_Month = GetDlgItemInt(IDC_EDIT_PERIODMONTH);
	m_IsSummaryModified = true;
}

void AddOrEditBudgetSummaryDlg::OnEnUpdateEditIncome()
{
	m_BudgetSummary->m_Income = GetDlgItemInt(IDC_EDIT_INCOME);
	m_IsSummaryModified = true;
}
