
// BudgetEditorView.h : interface of the CBudgetEditorView class
//

#pragma once

class CBudgetEditorView : public CListView
{
protected: // create from serialization only
	CBudgetEditorView();
	DECLARE_DYNCREATE(CBudgetEditorView)

// Attributes
public:
	CBudgetEditorDoc* GetDocument() const;

// Operations
private:
	void AddBudgetSummaryToGrid(CListCtrl&, BudgetSummary*, int = -1);
// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CBudgetEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

private:
	bool IsSummaryPeriodUnique(BudgetSummary*);
	bool IsSummaryPeriodValid(BudgetSummary*);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditCategoryList();
	afx_msg void OnNewSummary();
	afx_msg void OnEditSummary();
	afx_msg void OnDeleteSummary();
};

#ifndef _DEBUG  // debug version in BudgetEditorView.cpp
inline CBudgetEditorDoc* CBudgetEditorView::GetDocument() const
   { return reinterpret_cast<CBudgetEditorDoc*>(m_pDocument); }
#endif

