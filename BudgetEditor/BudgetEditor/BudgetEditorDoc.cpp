
// BudgetEditorDoc.cpp : implementation of the CBudgetEditorDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "BudgetEditor.h"
#endif

#include "BudgetEditorDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CBudgetEditorDoc

IMPLEMENT_DYNCREATE(CBudgetEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CBudgetEditorDoc, CDocument)
END_MESSAGE_MAP()


// CBudgetEditorDoc construction/destruction

CBudgetEditorDoc::CBudgetEditorDoc()
{
}

CBudgetEditorDoc::~CBudgetEditorDoc()
{
	for (int i = 0; i < m_BudgetSummaries.GetSize(); ++i)
	{
		delete m_BudgetSummaries[i];
	}
	m_BudgetSummaries.RemoveAll();
}

BOOL CBudgetEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CBudgetEditorDoc serialization

void CBudgetEditorDoc::Serialize(CArchive& ar)
{
	m_Categories.Serialize(ar);
	m_BudgetSummaries.Serialize(ar);
	for (int i = 0; i < m_BudgetSummaries.GetSize(); i++)
	{
		((BudgetSummary*)m_BudgetSummaries[i])->Seralize(ar);
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CBudgetEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CBudgetEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CBudgetEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CBudgetEditorDoc diagnostics

#ifdef _DEBUG
void CBudgetEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBudgetEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBudgetEditorDoc commands

// BudgetSummary
IMPLEMENT_SERIAL(BudgetSummary, CObject, 1)

BudgetSummary::BudgetSummary()
{
	m_Income = 0;
	m_SummaryItems = new CObArray();
	m_Period = BudgetPeriod();
}

BudgetSummary::~BudgetSummary()
{
	for (int i = 0; i < m_SummaryItems->GetSize(); i++)
	{
		delete (*m_SummaryItems)[i];
	}
	m_SummaryItems->RemoveAll();
	delete m_SummaryItems;
}

void BudgetSummary::Seralize(CArchive &ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar.Write(&m_Period, sizeof(BudgetPeriod));
		ar << m_Income;
	}
	else {
		ar.Read(&m_Period, sizeof(BudgetPeriod));
		ar >> m_Income;
	}
	m_SummaryItems->Serialize(ar);
	for (int i = 0; i < m_SummaryItems->GetSize(); i++)
	{
		((BudgetSummaryItem*)((*m_SummaryItems)[i]))->Serialize(ar);
	}
}

// BudgetSummaryItem
IMPLEMENT_SERIAL(BudgetSummaryItem, CObject, 1)

BudgetSummaryItem::BudgetSummaryItem()
{
}

BudgetSummaryItem::BudgetSummaryItem(CString &category, int amount)
{
	m_Category = category;
	m_Amount = amount;
}

void BudgetSummaryItem::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << m_Category << m_Amount;
	}
	else {
		ar >> m_Category >> m_Amount;
	}
}