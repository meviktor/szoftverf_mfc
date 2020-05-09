
// BudgetEditorDoc.h : interface of the CBudgetEditorDoc class
//

#pragma once

struct BudgetPeriod {
	// Attributes
public:
	int m_Year;
	int m_Month;
};

class BudgetSummaryItem : public CObject
{
	DECLARE_SERIAL(BudgetSummaryItem)

	// Attributes
public:
	CString m_Category;
	int m_Amount;

	// Construction
public:
	BudgetSummaryItem();
	BudgetSummaryItem(CString &, int);

	// Methods
public:
	void Serialize(CArchive &);
};

class BudgetSummary : public CObject
{
	DECLARE_SERIAL(BudgetSummary)

	// Attributes
public:
	BudgetPeriod m_Period;
	int m_Income;
	CObArray *m_SummaryItems;

	// Construction
public:
	BudgetSummary();
	~BudgetSummary();

	// Methods
	void Seralize(CArchive &);
};

class CBudgetEditorDoc : public CDocument
{
protected: // create from serialization only
	CBudgetEditorDoc();
	DECLARE_DYNCREATE(CBudgetEditorDoc)

// Attributes
public:
	CStringArray m_Categories;
	CObArray m_BudgetSummaries;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CBudgetEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
