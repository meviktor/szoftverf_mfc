#include "ChartCtrl.h"
#include "BudgetEditorDoc.h"

#pragma once

// IncomesOutgoingsDlg dialog

struct IncomesOutGoingsDialogData {
public:
	double year_xValue;
	double amount_yValue;
};

class IncomesOutgoingsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(IncomesOutgoingsDlg)

public:
	IncomesOutgoingsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~IncomesOutgoingsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DIAGRAM };
#endif
public:
	CObArray* m_BudgetSummaries;
	CChartCtrl m_ChartCtrl;
	CChartStandardAxis  *m_pBottomAxis;
	CChartStandardAxis *m_pLeftAxis;
	CChartBarSerie *m_incomesLineSerie;
	CChartBarSerie *m_outgoingsLineSerie;
private:
	IncomesOutGoingsDialogData* GenerateIncomesDiagramPoints();
	IncomesOutGoingsDialogData* GenerateOutgoingsDiagramPoints();
	void InitSummaryYears();
	int m_nYears;
	int* m_Years;
	IncomesOutGoingsDialogData* m_incomesDiagramPoints;
	IncomesOutGoingsDialogData* m_outgoingsDiagramPoints;
	double *m_XVals_years;
	double *m_YVals_incomes;
	double *m_YVals_outgoings;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()	
};
