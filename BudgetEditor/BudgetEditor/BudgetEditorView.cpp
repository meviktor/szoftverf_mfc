
// BudgetEditorView.cpp : implementation of the CBudgetEditorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BudgetEditor.h"
#endif

#include "BudgetEditorDoc.h"
#include "BudgetEditorView.h"
#include "EditCategoryListDlg.h"
#include "AddOrEditBudgetSummaryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBudgetEditorView

IMPLEMENT_DYNCREATE(CBudgetEditorView, CListView)

BEGIN_MESSAGE_MAP(CBudgetEditorView, CListView)
	ON_COMMAND(ID_BUDGET_EDITCATEGORYLIST, &CBudgetEditorView::OnEditCategoryList)
	ON_COMMAND(ID_BUDGET_ADDNEWSUMMARY, &CBudgetEditorView::OnNewSummary)
	ON_COMMAND(ID_BUDGET_EDITSUMMARY, &CBudgetEditorView::OnEditSummary)
	ON_COMMAND(ID_BUDGET_DELETESUMMARY, &CBudgetEditorView::OnDeleteSummary)
END_MESSAGE_MAP()

// CBudgetEditorView construction/destruction

CBudgetEditorView::CBudgetEditorView()
{
}

CBudgetEditorView::~CBudgetEditorView()
{
}

BOOL CBudgetEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= (LVS_REPORT | LVS_SINGLESEL);
	return CListView::PreCreateWindow(cs);
}

void CBudgetEditorView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	ListView_SetExtendedListViewStyle(m_hWnd, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	CListCtrl& listControl = GetListCtrl();

	LVCOLUMN column;
	column.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	column.pszText = _T("Period");
	column.cx = 250;
	column.fmt = LVCFMT_LEFT;
	listControl.InsertColumn(0, &column);

	column.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	column.pszText = _T("Total income");
	column.cx = 200;
	column.fmt = LVCFMT_LEFT;
	listControl.InsertColumn(1, &column);

	column.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	column.pszText = _T("Outgoings");
	column.cx = 200;
	column.fmt = LVCFMT_LEFT;
	listControl.InsertColumn(2, &column);

	CBudgetEditorDoc *budgetDoc = GetDocument();
	int numberOfSummaries = budgetDoc->m_BudgetSummaries.GetSize();
	if (numberOfSummaries > 0)
	{
		for (int i = 0; i < numberOfSummaries; i++)
		{
			AddBudgetSummaryToGrid(listControl, (BudgetSummary *)(budgetDoc->m_BudgetSummaries[i]));
		}
	}
}

void CBudgetEditorView::AddBudgetSummaryToGrid(CListCtrl& listControl, BudgetSummary *budgetSummary, int positionToAdd) {
	int indexOfNewItem = (positionToAdd <= -1 || positionToAdd > listControl.GetItemCount()) ? listControl.GetItemCount() : positionToAdd;

	CString periodColumnValue = _T("");
	periodColumnValue.AppendFormat(_T("Year: %d, Month: %d"), budgetSummary->m_Period.m_Year, budgetSummary->m_Period.m_Month);
	CString totalIncomeColumnValue = _T("");
	totalIncomeColumnValue.AppendFormat(_T("%d HUF"), budgetSummary->m_Income);

	int outgoings = 0;
	int numberOfBudgetSummaryItems = budgetSummary->m_SummaryItems->GetCount();
	if (numberOfBudgetSummaryItems > 0)
	{
		CObject **arr_sumItems = budgetSummary->m_SummaryItems->GetData();
		for (int i = 0; i < numberOfBudgetSummaryItems; i++)
		{
			BudgetSummaryItem *item = (BudgetSummaryItem *)(arr_sumItems[i]);
			outgoings += item->m_Amount;
		}
	}
	CString outgoingsColumnValue = _T("");
	outgoingsColumnValue.AppendFormat(_T("%d HUF"), outgoings);

	listControl.InsertItem(LVIF_TEXT, indexOfNewItem, periodColumnValue, 0, 0, 0, NULL);
	listControl.SetItemText(indexOfNewItem, 1, totalIncomeColumnValue);
	listControl.SetItemText(indexOfNewItem, 2, outgoingsColumnValue);
}


// CBudgetEditorView diagnostics

#ifdef _DEBUG
void CBudgetEditorView::AssertValid() const
{
	CListView::AssertValid();
}

void CBudgetEditorView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

bool CBudgetEditorView::IsSummaryPeriodUnique(BudgetSummary *budgetSummary)
{
	CBudgetEditorDoc *doc = GetDocument();
	for (int i = 0; i < doc->m_BudgetSummaries.GetSize(); i++)
	{
		BudgetSummary *sum = (BudgetSummary*)(doc->m_BudgetSummaries[i]);
		if (sum->m_Period.m_Year == budgetSummary->m_Period.m_Year && sum->m_Period.m_Month == budgetSummary->m_Period.m_Month)
		{
			return false;
		}
	}
	return true;
}

bool CBudgetEditorView::IsSummaryPeriodValid(BudgetSummary *budgetSummary)
{
	return (budgetSummary->m_Period.m_Year >= 1900 && budgetSummary->m_Period.m_Year <= 9999) && (budgetSummary->m_Period.m_Month >= 1 && budgetSummary->m_Period.m_Month <= 12);
}

CBudgetEditorDoc* CBudgetEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBudgetEditorDoc)));
	return (CBudgetEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CBudgetEditorView message handlers
void CBudgetEditorView::OnEditCategoryList()
{
	CBudgetEditorDoc *budgetDoc = GetDocument();

	EditCategoryListDlg editDlg;
	editDlg.m_Categories = &(budgetDoc->m_Categories);

	editDlg.DoModal();
	if (editDlg.m_Categories != NULL && editDlg.IsListModified())
	{
		budgetDoc->SetModifiedFlag();
	}
}

void CBudgetEditorView::OnNewSummary()
{
	AddOrEditBudgetSummaryDlg dialog;
	dialog.m_Categories = &(GetDocument()->m_Categories);
	dialog.m_BudgetSummary = new BudgetSummary();
	
	int dialogRes = dialog.DoModal();
	while (dialogRes == IDOK && (!IsSummaryPeriodUnique(dialog.m_BudgetSummary) || !IsSummaryPeriodValid(dialog.m_BudgetSummary)))
	{
		MessageBox(
			_T("Summary period is invalid (year, month)! A summary with this period is already exists or you provided invalid year or month value!"),
			_T("Invalid summary period"),
			MB_OK | MB_ICONWARNING | MB_APPLMODAL
		);
		dialogRes = dialog.DoModal();
	}
	if (dialogRes == IDOK) {
		CBudgetEditorDoc *doc = GetDocument();

		int indexOfNewElement = doc->m_BudgetSummaries.Add(dialog.m_BudgetSummary);
		doc->SetModifiedFlag();

		CListCtrl& listControl = GetListCtrl();
		AddBudgetSummaryToGrid(listControl, (BudgetSummary *)(doc->m_BudgetSummaries[indexOfNewElement]));
	}
	else {
		delete dialog.m_BudgetSummary;
	}
}

void CBudgetEditorView::OnEditSummary()
{
	CListCtrl& listControl = GetListCtrl();
	int selectedRow = listControl.GetSelectionMark();

	if (selectedRow != LB_ERR)
	{
		CBudgetEditorDoc *doc = GetDocument();

		AddOrEditBudgetSummaryDlg dialog(true);
		dialog.m_Categories = &(GetDocument()->m_Categories);
		dialog.m_BudgetSummary = (BudgetSummary *)(doc->m_BudgetSummaries[selectedRow]);

		dialog.DoModal();
		if (dialog.IsSummaryModified())
		{
			CListCtrl& listControl = GetListCtrl();
			listControl.DeleteItem(selectedRow);
			AddBudgetSummaryToGrid(listControl, dialog.m_BudgetSummary, selectedRow);

			doc->SetModifiedFlag();
		}
	}
	else {
		MessageBox(
			_T("You have to select an item from the list to edit it."),
			_T("Edit selected summary"),
			MB_OK | MB_ICONINFORMATION | MB_APPLMODAL
		);
	}
}

void CBudgetEditorView::OnDeleteSummary()
{
	CListCtrl& listControl = GetListCtrl();
	int selectedRow = listControl.GetSelectionMark();
	if (selectedRow != LB_ERR)
	{
		int askDlgRes = MessageBox(
			_T("Are you sure you want do delete the selected summary?"),
			_T("Delete selected summary"),
			MB_YESNO | MB_ICONWARNING | MB_APPLMODAL
		);
		if (askDlgRes == IDYES) {
			listControl.DeleteItem(selectedRow);

			CBudgetEditorDoc *doc = GetDocument();
			doc->m_BudgetSummaries.RemoveAt(selectedRow);
			doc->SetModifiedFlag();
		}
	}
	else {
		MessageBox(
			_T("You have to select one item from the list to delete it."),
			_T("Delete selected summary"),
			MB_OK | MB_ICONINFORMATION | MB_APPLMODAL
		);
	}
}
