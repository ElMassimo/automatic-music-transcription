// GAParametersDialog.cpp : implementation file
//

#include "stdafx.h"
#include "gademo.h"
#include "GAParametersDialog.h"
#include "gademoDoc.h"
#include "gademoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGAParametersDialog dialog


CGAParametersDialog::CGAParametersDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGAParametersDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGAParametersDialog)
	m_pcross = 0.0f;
	m_pmut = 0.0f;
	m_popsize = 0;
	m_ngen = 0;
	//}}AFX_DATA_INIT

	m_pParent = pParent;
//	SetDefID(IDC_APPLY);
}

BOOL CGAParametersDialog::Create(){
	return CDialog::Create(CGAParametersDialog::IDD, m_pParent);
}


void CGAParametersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGAParametersDialog)
	DDX_Text(pDX, IDC_PCROSS, m_pcross);
	DDV_MinMaxFloat(pDX, m_pcross, 0.f, 1.f);
	DDX_Text(pDX, IDC_PMUT, m_pmut);
	DDV_MinMaxFloat(pDX, m_pmut, 0.f, 1.f);
	DDX_Text(pDX, IDC_POPSIZE, m_popsize);
	DDV_MinMaxInt(pDX, m_popsize, 10, 1000);
	DDX_Text(pDX, IDC_NGEN, m_ngen);
	DDV_MinMaxInt(pDX, m_ngen, 0, 10000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGAParametersDialog, CDialog)
	//{{AFX_MSG_MAP(CGAParametersDialog)
	ON_BN_CLICKED(IDC_REVERT, OnRevert)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_EN_CHANGE(IDC_PMUT, OnChangePmut)
	ON_EN_CHANGE(IDC_PCROSS, OnChangePcross)
	ON_EN_CHANGE(IDC_NGEN, OnChangeNgen)
	ON_EN_CHANGE(IDC_POPSIZE, OnChangePopsize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGAParametersDialog message handlers

void CGAParametersDialog::OnRevert() {
	CGademoView& parent = ((CGademoView&)(*m_pParent));
	m_pcross = parent.pCrossover();
	m_pmut = parent.pMutation();
	m_ngen = parent.nGenerations();
	m_popsize = parent.populationSize();
	UpdateData(FALSE);
	GetDlgItem(IDC_REVERT)->EnableWindow(FALSE);
	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
}

void CGAParametersDialog::OnApply() {
	CGademoView& parent = ((CGademoView&)(*m_pParent));
	UpdateData(TRUE);
	GetDlgItem(IDC_REVERT)->EnableWindow(FALSE);
	GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);
	parent.pCrossover(m_pcross);
	parent.pMutation(m_pmut);
	parent.nGenerations(m_ngen);
	parent.populationSize(m_popsize);
}

void CGAParametersDialog::OnClose() {
	EndDialog(TRUE);
	m_pParent->SetFocus();
}

void CGAParametersDialog::OnChangePmut() {	
	GetDlgItem(IDC_REVERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);	
}

void CGAParametersDialog::OnChangePcross() {	
	GetDlgItem(IDC_REVERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);	
}

void CGAParametersDialog::OnChangeNgen() {	
	GetDlgItem(IDC_REVERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);	
}

void CGAParametersDialog::OnChangePopsize() {	
	GetDlgItem(IDC_REVERT)->EnableWindow(TRUE);
	GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);		
}
