// IncomesOutgoingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BudgetEditor.h"
#include "IncomesOutgoingsDlg.h"
#include "afxdialogex.h"
#include "ChartLineSerie.h"
#include "ChartBarSerie.h"

using namespace std;
// IncomesOutgoingsDlg dialog

IMPLEMENT_DYNAMIC(IncomesOutgoingsDlg, CDialogEx)

IncomesOutgoingsDlg::IncomesOutgoingsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DIAGRAM, pParent)
{
}

IncomesOutgoingsDlg::~IncomesOutgoingsDlg()
{
	delete m_Years;
	delete m_incomesDiagramPoints;
	delete m_outgoingsDiagramPoints;
	free(m_XVals_years);
	free(m_YVals_incomes);
	free(m_YVals_outgoings);
}

IncomesOutGoingsDialogData* IncomesOutgoingsDlg::GenerateIncomesDiagramPoints()
{
	IncomesOutGoingsDialogData *arr = new IncomesOutGoingsDialogData[m_nYears];
	for (int i = 0; i < m_nYears; i++)
	{
		IncomesOutGoingsDialogData point;
		point.year_xValue = m_Years[i];
		point.amount_yValue = 0;
		for (int j = 0; j < m_BudgetSummaries->GetSize(); j++)
		{
			BudgetSummary* summary = ((BudgetSummary*)(*m_BudgetSummaries)[j]);
			if (summary->m_Period.m_Year == point.year_xValue)
			{
				point.amount_yValue += summary->m_Income;
			}
		}
		arr[i] = point;
	}
	return arr;
}

IncomesOutGoingsDialogData* IncomesOutgoingsDlg::GenerateOutgoingsDiagramPoints()
{
	IncomesOutGoingsDialogData *arr = new IncomesOutGoingsDialogData[m_nYears];
	for (int i = 0; i < m_nYears; i++)
	{
		IncomesOutGoingsDialogData point;
		point.year_xValue = m_Years[i];
		point.amount_yValue = 0;
		for (int j = 0; j < m_BudgetSummaries->GetSize(); j++)
		{
			BudgetSummary* summary = ((BudgetSummary*)(*m_BudgetSummaries)[j]);
			CObArray *summaryItems = summary->m_SummaryItems;
			for (int k = 0; k < summaryItems->GetSize(); k++)
			{
				if (summary->m_Period.m_Year == point.year_xValue)
				{
					point.amount_yValue += ((BudgetSummaryItem*)((*summaryItems)[k]))->m_Amount;
				}
			}
		}
		arr[i] = point;
	}
	return arr;
}

void IncomesOutgoingsDlg::InitSummaryYears()
{
	m_nYears = 0;
	m_Years = new int[m_BudgetSummaries->GetSize()];

	for (int i = 0; i < m_BudgetSummaries->GetSize(); i++)
	{
		int summaryYear = ((BudgetSummary*)(m_BudgetSummaries->ElementAt(i)))->m_Period.m_Year;
		bool addToSet = true;
		for (int j = 0; j < m_nYears; j++)
		{
			if (m_Years[j] == summaryYear)
			{
				addToSet = false;
				break;
			}
		}
		if (addToSet)
		{
			m_Years[m_nYears++] = summaryYear;
		}
	}
}

void IncomesOutgoingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHARTCTRL, m_ChartCtrl);
}

BOOL IncomesOutgoingsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	InitSummaryYears();
	m_incomesDiagramPoints = GenerateIncomesDiagramPoints();
	m_outgoingsDiagramPoints = GenerateOutgoingsDiagramPoints();

	m_pBottomAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::BottomAxis);
	m_pLeftAxis = m_ChartCtrl.CreateStandardAxis(CChartCtrl::LeftAxis);
	m_incomesLineSerie = m_ChartCtrl.CreateBarSerie();
	m_outgoingsLineSerie = m_ChartCtrl.CreateBarSerie();

	m_XVals_years = (double*) malloc(m_nYears * sizeof(double));
	m_YVals_incomes = (double*) malloc(m_nYears * sizeof(double));
	m_YVals_outgoings = (double*) malloc(m_nYears * sizeof(double));

	for(int i = 0; i < m_nYears; i++)
	{
		m_XVals_years[i] = m_incomesDiagramPoints[i].year_xValue; // lehetne m_outgoingsDiagramPoints[i] is, mindegy
		m_YVals_incomes[i] = m_incomesDiagramPoints[i].amount_yValue;
		m_YVals_outgoings[i] = m_outgoingsDiagramPoints[i].amount_yValue;
	}

	int minYear = *min_element(m_XVals_years, m_XVals_years + m_nYears);
	int maxYear = *max_element(m_XVals_years, m_XVals_years + m_nYears);
	int maxIncome = *max_element(m_YVals_incomes, m_YVals_incomes + m_nYears);
	int maxOutgoings = *max_element(m_YVals_outgoings, m_YVals_outgoings + m_nYears);

	m_pBottomAxis->SetMinMax(minYear, maxYear + 1);
	m_pBottomAxis->SetTickIncrement(false, 1);
	m_pBottomAxis->SetDiscrete(true);
	m_pBottomAxis->EnableScrollBar(true);

	m_pLeftAxis->SetMinMax(0, maxIncome >= maxOutgoings ? maxIncome * 1.05 : maxOutgoings * 1.05);

	m_incomesLineSerie->SetPoints(m_XVals_years, m_YVals_incomes, m_nYears);
	m_incomesLineSerie->SetColor(RGB(0, 255, 0));
	m_incomesLineSerie->SetBarWidth(15);
	m_incomesLineSerie->SetGroupId(1);

	m_outgoingsLineSerie->SetPoints(m_XVals_years, m_YVals_outgoings, m_nYears);
	m_outgoingsLineSerie->SetColor(RGB(255, 0, 0));
	m_outgoingsLineSerie->SetBarWidth(15);
	m_outgoingsLineSerie->SetGroupId(1);

	return TRUE;
}

BEGIN_MESSAGE_MAP(IncomesOutgoingsDlg, CDialogEx)
END_MESSAGE_MAP()


// IncomesOutgoingsDlg message handlers
